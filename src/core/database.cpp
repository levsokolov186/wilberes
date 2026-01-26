#include "database.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <cctype>

const std::string Database::ADMIN_SECRET_CODE = "ADMIN2024SECRET";

Database::Database(const std::string& dataPath)
    : m_dataPath(dataPath)
    , m_nextUserId(1)
    , m_nextProductId(1)
    , m_dirty(false)
{
    // Создание директории данных, если она не существует
    std::filesystem::create_directories(dataPath);

    loadUsers();
    loadProducts();

    if (m_products.empty()) {
        initDefaultProducts();
    }
}

Database::~Database() {
    flush();
}

Database::Database(Database&& other) noexcept
    : m_users(std::move(other.m_users))
    , m_products(std::move(other.m_products))
    , m_userIdIndex(std::move(other.m_userIdIndex))
    , m_usernameIndex(std::move(other.m_usernameIndex))
    , m_productIdIndex(std::move(other.m_productIdIndex))
    , m_emails(std::move(other.m_emails))
    , m_dataPath(std::move(other.m_dataPath))
    , m_nextUserId(other.m_nextUserId)
    , m_nextProductId(other.m_nextProductId)
    , m_dirty(other.m_dirty)
{
    other.m_nextUserId = 1;
    other.m_nextProductId = 1;
    other.m_dirty = false;
}

Database& Database::operator=(Database&& other) noexcept {
    if (this != &other) {
        flush(); // Сохранение текущего состояния перед перемещением
        
        m_users = std::move(other.m_users);
        m_products = std::move(other.m_products);
        m_userIdIndex = std::move(other.m_userIdIndex);
        m_usernameIndex = std::move(other.m_usernameIndex);
        m_productIdIndex = std::move(other.m_productIdIndex);
        m_emails = std::move(other.m_emails);
        m_dataPath = std::move(other.m_dataPath);
        m_nextUserId = other.m_nextUserId;
        m_nextProductId = other.m_nextProductId;
        m_dirty = other.m_dirty;
        
        other.m_nextUserId = 1;
        other.m_nextProductId = 1;
        other.m_dirty = false;
    }
    return *this;
}

void Database::rebuildUserIndices() {
    m_userIdIndex.clear();
    m_usernameIndex.clear();
    m_emails.clear();
    
    m_userIdIndex.reserve(m_users.size());
    m_usernameIndex.reserve(m_users.size());
    m_emails.reserve(m_users.size());
    
    for (std::size_t i = 0; i < m_users.size(); ++i) {
        const auto& user = m_users[i];
        m_userIdIndex[user.getId()] = i;
        m_usernameIndex[user.getUsername()] = i;
        m_emails.insert(user.getEmail());
    }
}

void Database::rebuildProductIndices() {
    m_productIdIndex.clear();
    m_productIdIndex.reserve(m_products.size());
    
    for (std::size_t i = 0; i < m_products.size(); ++i) {
        m_productIdIndex[m_products[i].getId()] = i;
    }
}

bool Database::loadUsers() {
    std::ifstream file(m_dataPath + "users.txt");
    if (!file.is_open()) {
        return false;
    }

    m_users.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            User user = User::deserialize(line);
            if (user.getId() > 0) {
                if (user.getId() >= m_nextUserId) {
                    m_nextUserId = user.getId() + 1;
                }
                m_users.push_back(std::move(user));
            }
        }
    }
    
    file.close();
    rebuildUserIndices();
    return true;
}

bool Database::saveUsers() {
    std::ofstream file(m_dataPath + "users.txt");
    if (!file.is_open()) {
        return false;
    }

    for (const auto& user : m_users) {
        file << user.serialize() << '\n';
    }
    
    file.close();
    return true;
}

bool Database::loadProducts() {
    std::ifstream file(m_dataPath + "products.txt");
    if (!file.is_open()) {
        return false;
    }

    m_products.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Product product = Product::deserialize(line);
            if (product.getId() > 0) {
                if (product.getId() >= m_nextProductId) {
                    m_nextProductId = product.getId() + 1;
                }
                m_products.push_back(std::move(product));
            }
        }
    }
    
    file.close();
    rebuildProductIndices();
    return true;
}

bool Database::saveProducts() {
    std::ofstream file(m_dataPath + "products.txt");
    if (!file.is_open()) {
        return false;
    }

    for (const auto& product : m_products) {
        file << product.serialize() << '\n';
    }
    
    file.close();
    return true;
}

bool Database::flush() {
    if (m_dirty) {
        bool success = saveUsers() && saveProducts();
        if (success) {
            m_dirty = false;
        }
        return success;
    }
    return true;
}

void Database::initDefaultProducts() {
    // Электроника
    {
        Product p(m_nextProductId++, "iPhone 15 Pro Max 256GB",
                  "Flagship Apple smartphone with A17 Pro chip", "Electronics", 129990, 50);
        p.setBrand("Apple");
        p.setDiscount(10);
        p.setRating(4.9);
        p.setReviewCount(1523);
        m_products.push_back(std::move(p));
    }
    
    {
        Product p(m_nextProductId++, "Samsung Galaxy S24 Ultra",
                  "Premium smartphone with AI features", "Electronics", 119990, 35);
        p.setBrand("Samsung");
        p.setDiscount(15);
        p.setRating(4.8);
        p.setReviewCount(892);
        m_products.push_back(std::move(p));
    }
    
    {
        Product p(m_nextProductId++, "MacBook Air M3 15\"",
                  "Apple laptop with M3 chip, 8GB RAM, 256GB SSD", "Electronics", 149990, 25);
        p.setBrand("Apple");
        p.setRating(4.9);
        p.setReviewCount(456);
        m_products.push_back(std::move(p));
    }
    
    {
        Product p(m_nextProductId++, "Sony WH-1000XM5",
                  "Wireless headphones with noise cancellation", "Electronics", 32990, 100);
        p.setBrand("Sony");
        p.setDiscount(20);
        p.setRating(4.7);
        p.setReviewCount(2341);
        m_products.push_back(std::move(p));
    }

    // Одежда
    {
        Product p(m_nextProductId++, "Men's Winter Jacket",
                  "Warm winter jacket with hood", "Clothing", 8990, 200);
        p.setBrand("Zara");
        p.setDiscount(30);
        p.setRating(4.5);
        p.setReviewCount(567);
        m_products.push_back(std::move(p));
    }
    
    {
        Product p(m_nextProductId++, "Women's Summer Dress",
                  "Light cotton summer dress", "Clothing", 2990, 150);
        p.setBrand("H&M");
        p.setDiscount(25);
        p.setRating(4.3);
        p.setReviewCount(234);
        m_products.push_back(std::move(p));
    }
    
    {
        Product p(m_nextProductId++, "Nike Air Max Sneakers",
                  "Running sports sneakers", "Footwear", 12990, 80);
        p.setBrand("Nike");
        p.setDiscount(15);
        p.setRating(4.8);
        p.setReviewCount(1892);
        m_products.push_back(std::move(p));
    }

    // Дом и сад
    {
        Product p(m_nextProductId++, "Xiaomi Robot Vacuum",
                  "Smart robot vacuum with wet cleaning", "Home & Garden", 24990, 60);
        p.setBrand("Xiaomi");
        p.setDiscount(10);
        p.setRating(4.6);
        p.setReviewCount(3421);
        m_products.push_back(std::move(p));
    }
    
    {
        Product p(m_nextProductId++, "DeLonghi Coffee Machine",
                  "Automatic espresso coffee machine", "Home & Garden", 45990, 30);
        p.setBrand("DeLonghi");
        p.setRating(4.7);
        p.setReviewCount(789);
        m_products.push_back(std::move(p));
    }

    // Красота
    {
        Product p(m_nextProductId++, "L'Oreal Cosmetics Set",
                  "Gift set of decorative cosmetics", "Beauty", 3990, 100);
        p.setBrand("L'Oreal");
        p.setDiscount(40);
        p.setRating(4.4);
        p.setReviewCount(456);
        m_products.push_back(std::move(p));
    }
    
    {
        Product p(m_nextProductId++, "Chanel No 5 Perfume",
                  "Classic women's fragrance", "Beauty", 8990, 45);
        p.setBrand("Chanel");
        p.setRating(4.9);
        p.setReviewCount(2103);
        m_products.push_back(std::move(p));
    }

    // Дети
    {
        Product p(m_nextProductId++, "LEGO Star Wars",
                  "Millennium Falcon Constructor Set", "Kids", 14990, 40);
        p.setBrand("LEGO");
        p.setDiscount(5);
        p.setRating(4.9);
        p.setReviewCount(678);
        m_products.push_back(std::move(p));
    }

    rebuildProductIndices();
    m_dirty = true;
    saveProducts();
}

User* Database::authenticateUser(const std::string& username, const std::string& password) {
    auto it = m_usernameIndex.find(username);
    if (it != m_usernameIndex.end()) {
        User& user = m_users[it->second];
        if (user.getPassword() == password) {
            return &user;
        }
    }
    return nullptr;
}

bool Database::registerUser(const std::string& username, const std::string& password,
                            const std::string& email, const std::string& adminCode) {
    if (usernameExists(username) || emailExists(email)) {
        return false;
    }

    UserRole role = UserRole::Customer;
    if (!adminCode.empty() && isValidAdminCode(adminCode)) {
        role = UserRole::Admin;
    }

    User newUser(m_nextUserId++, username, password, email, role);
    newUser.setBalance(1000.0); // Начальный баланс
    
    // Обновление индексов
    std::size_t index = m_users.size();
    m_userIdIndex[newUser.getId()] = index;
    m_usernameIndex[username] = index;
    m_emails.insert(email);
    
    m_users.push_back(std::move(newUser));
    m_dirty = true;
    
    return saveUsers();
}

bool Database::usernameExists(const std::string& username) const noexcept {
    return m_usernameIndex.find(username) != m_usernameIndex.end();
}

bool Database::emailExists(const std::string& email) const noexcept {
    return m_emails.find(email) != m_emails.end();
}

User* Database::getUserById(int id) noexcept {
    auto it = m_userIdIndex.find(id);
    if (it != m_userIdIndex.end() && it->second < m_users.size()) {
        return &m_users[it->second];
    }
    return nullptr;
}

const User* Database::getUserById(int id) const noexcept {
    auto it = m_userIdIndex.find(id);
    if (it != m_userIdIndex.end() && it->second < m_users.size()) {
        return &m_users[it->second];
    }
    return nullptr;
}

User* Database::getUserByUsername(const std::string& username) noexcept {
    auto it = m_usernameIndex.find(username);
    if (it != m_usernameIndex.end() && it->second < m_users.size()) {
        return &m_users[it->second];
    }
    return nullptr;
}

const User* Database::getUserByUsername(const std::string& username) const noexcept {
    auto it = m_usernameIndex.find(username);
    if (it != m_usernameIndex.end() && it->second < m_users.size()) {
        return &m_users[it->second];
    }
    return nullptr;
}

bool Database::updateUser(const User& user) {
    auto it = m_userIdIndex.find(user.getId());
    if (it != m_userIdIndex.end() && it->second < m_users.size()) {
        User& existingUser = m_users[it->second];
        
        // Обновление индекса email, если email изменился
        if (existingUser.getEmail() != user.getEmail()) {
            m_emails.erase(existingUser.getEmail());
            m_emails.insert(user.getEmail());
        }
        
        // Обновление индекса имени пользователя, если оно изменилось
        if (existingUser.getUsername() != user.getUsername()) {
            m_usernameIndex.erase(existingUser.getUsername());
            m_usernameIndex[user.getUsername()] = it->second;
        }
        
        existingUser = user;
        m_dirty = true;
        return saveUsers();
    }
    return false;
}

std::vector<Product> Database::getProductsByCategory(const std::string& category) const {
    std::vector<Product> result;
    result.reserve(m_products.size() / 4); // Оценка (резервирование памяти)
    
    for (const auto& product : m_products) {
        if (product.getCategory() == category && product.isAvailable()) {
            result.push_back(product);
        }
    }
    
    return result;
}

namespace {
    // Вспомогательная функция для преобразования строки в нижний регистр
    std::string toLower(const std::string& str) {
        std::string result;
        result.reserve(str.size());
        for (char c : str) {
            result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
        return result;
    }
    
    // Поиск подстроки без учета регистра
    bool containsIgnoreCase(const std::string& haystack, const std::string& needle) {
        if (needle.empty()) return true;
        if (haystack.size() < needle.size()) return false;
        
        std::string lowerHaystack = toLower(haystack);
        return lowerHaystack.find(needle) != std::string::npos;
    }
}

std::vector<Product> Database::searchProducts(const std::string& query) const {
    std::vector<Product> result;
    
    if (query.empty()) {
        return result;
    }
    
    std::string lowerQuery = toLower(query);
    result.reserve(m_products.size() / 4); // Оценка (резервирование памяти)
    
    for (const auto& product : m_products) {
        if (containsIgnoreCase(product.getName(), lowerQuery) ||
            containsIgnoreCase(product.getDescription(), lowerQuery) ||
            containsIgnoreCase(product.getBrand(), lowerQuery)) {
            result.push_back(product);
        }
    }
    
    return result;
}

Product* Database::getProductById(int id) noexcept {
    auto it = m_productIdIndex.find(id);
    if (it != m_productIdIndex.end() && it->second < m_products.size()) {
        return &m_products[it->second];
    }
    return nullptr;
}

const Product* Database::getProductById(int id) const noexcept {
    auto it = m_productIdIndex.find(id);
    if (it != m_productIdIndex.end() && it->second < m_products.size()) {
        return &m_products[it->second];
    }
    return nullptr;
}

bool Database::addProduct(const Product& product) {
    Product newProduct = product;
    newProduct.setId(m_nextProductId++);
    
    std::size_t index = m_products.size();
    m_productIdIndex[newProduct.getId()] = index;
    m_products.push_back(std::move(newProduct));
    
    m_dirty = true;
    return saveProducts();
}

bool Database::addProduct(Product&& product) {
    product.setId(m_nextProductId++);
    
    std::size_t index = m_products.size();
    m_productIdIndex[product.getId()] = index;
    m_products.push_back(std::move(product));
    
    m_dirty = true;
    return saveProducts();
}

bool Database::updateProduct(const Product& product) {
    auto it = m_productIdIndex.find(product.getId());
    if (it != m_productIdIndex.end() && it->second < m_products.size()) {
        m_products[it->second] = product;
        m_dirty = true;
        return saveProducts();
    }
    return false;
}

bool Database::deleteProduct(int id) {
    auto it = m_productIdIndex.find(id);
    if (it != m_productIdIndex.end() && it->second < m_products.size()) {
        std::size_t index = it->second;
        
        // Удаление из вектора
        m_products.erase(m_products.begin() + static_cast<std::ptrdiff_t>(index));
        
        // Перестройка индексов (необходимо, так как индексы смещаются после удаления)
        rebuildProductIndices();
        
        m_dirty = true;
        return saveProducts();
    }
    return false;
}

std::vector<std::string> Database::getCategories() const {
    std::unordered_set<std::string> categorySet;
    categorySet.reserve(10);
    
    for (const auto& product : m_products) {
        categorySet.insert(product.getCategory());
    }
    
    std::vector<std::string> categories(categorySet.begin(), categorySet.end());
    std::sort(categories.begin(), categories.end());
    
    return categories;
}

bool Database::isValidAdminCode(const std::string& code) noexcept {
    return code == ADMIN_SECRET_CODE;
}