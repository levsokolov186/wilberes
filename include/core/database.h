#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "user.h"
#include "product.h"

class Database {
private:
    std::vector<User> m_users;
    std::vector<Product> m_products;
    
    // Карты индексов для быстрого поиска
    std::unordered_map<int, std::size_t> m_userIdIndex;
    std::unordered_map<std::string, std::size_t> m_usernameIndex;
    std::unordered_map<int, std::size_t> m_productIdIndex;
    std::unordered_set<std::string> m_emails;
    
    std::string m_dataPath;
    int m_nextUserId;
    int m_nextProductId;
    bool m_dirty;

    static const std::string ADMIN_SECRET_CODE;
    
    // Вспомогательные внутренние методы
    void rebuildUserIndices();
    void rebuildProductIndices();

public:
    explicit Database(const std::string& dataPath = "data/");
    ~Database();
    
    // Запрет копирования
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
    // Разрешение перемещения
    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;

    // Операции загрузки/сохранения
    bool loadUsers();
    bool saveUsers();
    bool loadProducts();
    bool saveProducts();
    void initDefaultProducts();
    
    // Принудительное сохранение, если данные изменены
    bool flush();

    // Операции с пользователями
    User* authenticateUser(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password,
                      const std::string& email, const std::string& adminCode = "");
    bool usernameExists(const std::string& username) const noexcept;
    bool emailExists(const std::string& email) const noexcept;
    User* getUserById(int id) noexcept;
    const User* getUserById(int id) const noexcept;
    User* getUserByUsername(const std::string& username) noexcept;
    const User* getUserByUsername(const std::string& username) const noexcept;
    bool updateUser(const User& user);
    const std::vector<User>& getAllUsers() const noexcept { return m_users; }
    std::size_t getUserCount() const noexcept { return m_users.size(); }

    // Операции с товарами
    const std::vector<Product>& getAllProducts() const noexcept { return m_products; }
    std::vector<Product> getProductsByCategory(const std::string& category) const;
    std::vector<Product> searchProducts(const std::string& query) const;
    Product* getProductById(int id) noexcept;
    const Product* getProductById(int id) const noexcept;
    bool addProduct(const Product& product);
    bool addProduct(Product&& product);
    bool updateProduct(const Product& product);
    bool deleteProduct(int id);
    std::vector<std::string> getCategories() const;
    std::size_t getProductCount() const noexcept { return m_products.size(); }

    // Валидация администраторского кода
    static bool isValidAdminCode(const std::string& code) noexcept;
    
    // Доступ к пути к данным
    const std::string& getDataPath() const noexcept { return m_dataPath; }
};

#endif // DATABASE_H