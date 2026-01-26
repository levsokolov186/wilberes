#include "product.h"
#include <sstream>
#include <algorithm>
#include <vector>

Product::Product() noexcept
    : m_id(0)
    , m_price(0.0)
    , m_oldPrice(0.0)
    , m_discount(0)
    , m_stock(0)
    , m_rating(0.0)
    , m_reviewCount(0)
    , m_isAvailable(true) 
{}

Product::Product(int id, const std::string& name, const std::string& description,
                 const std::string& category, double price, int stock)
    : m_id(id)
    , m_name(name)
    , m_description(description)
    , m_category(category)
    , m_price(price)
    , m_oldPrice(price)
    , m_discount(0)
    , m_stock(stock)
    , m_rating(0.0)
    , m_reviewCount(0)
    , m_isAvailable(true) 
{}

Product::Product(Product&& other) noexcept
    : m_id(other.m_id)
    , m_name(std::move(other.m_name))
    , m_description(std::move(other.m_description))
    , m_category(std::move(other.m_category))
    , m_brand(std::move(other.m_brand))
    , m_price(other.m_price)
    , m_oldPrice(other.m_oldPrice)
    , m_discount(other.m_discount)
    , m_stock(other.m_stock)
    , m_rating(other.m_rating)
    , m_reviewCount(other.m_reviewCount)
    , m_imageUrl(std::move(other.m_imageUrl))
    , m_isAvailable(other.m_isAvailable)
{
    other.m_id = 0;
    other.m_price = 0.0;
    other.m_oldPrice = 0.0;
    other.m_discount = 0;
    other.m_stock = 0;
    other.m_rating = 0.0;
    other.m_reviewCount = 0;
    other.m_isAvailable = false;
}

Product& Product::operator=(Product&& other) noexcept {
    if (this != &other) {
        m_id = other.m_id;
        m_name = std::move(other.m_name);
        m_description = std::move(other.m_description);
        m_category = std::move(other.m_category);
        m_brand = std::move(other.m_brand);
        m_price = other.m_price;
        m_oldPrice = other.m_oldPrice;
        m_discount = other.m_discount;
        m_stock = other.m_stock;
        m_rating = other.m_rating;
        m_reviewCount = other.m_reviewCount;
        m_imageUrl = std::move(other.m_imageUrl);
        m_isAvailable = other.m_isAvailable;
        
        other.m_id = 0;
        other.m_price = 0.0;
        other.m_oldPrice = 0.0;
        other.m_discount = 0;
        other.m_stock = 0;
        other.m_rating = 0.0;
        other.m_reviewCount = 0;
        other.m_isAvailable = false;
    }
    return *this;
}

void Product::setPrice(double price) noexcept {
    if (price >= 0) {
        m_price = price;
    }
}

void Product::setDiscount(int discount) noexcept {
    m_discount = std::clamp(discount, 0, 99);
    if (m_discount > 0 && m_oldPrice <= m_price) {
        m_oldPrice = m_price;
        m_price = m_oldPrice * (100 - m_discount) / 100.0;
    }
}

void Product::setStock(int stock) noexcept {
    m_stock = std::max(0, stock);
}

bool Product::decreaseStock(int amount) noexcept {
    if (amount > 0 && m_stock >= amount) {
        m_stock -= amount;
        return true;
    }
    return false;
}

void Product::increaseStock(int amount) noexcept {
    if (amount > 0) {
        m_stock += amount;
    }
}

double Product::getFinalPrice() const noexcept {
    return m_price;
}

std::string Product::serialize() const {
    std::ostringstream oss;
    oss << m_id << '|'
        << m_name << '|'
        << m_description << '|'
        << m_category << '|'
        << m_brand << '|'
        << m_price << '|'
        << m_oldPrice << '|'
        << m_discount << '|'
        << m_stock << '|'
        << m_rating << '|'
        << m_reviewCount << '|'
        << m_imageUrl << '|'
        << (m_isAvailable ? 1 : 0);
    return oss.str();
}

Product Product::deserialize(const std::string& data) {
    Product product;
    
    if (data.empty()) {
        return product;
    }
    
    std::vector<std::string> tokens;
    tokens.reserve(13);
    
    std::size_t start = 0;
    std::size_t end = 0;
    
    while ((end = data.find('|', start)) != std::string::npos) {
        tokens.emplace_back(data.substr(start, end - start));
        start = end + 1;
    }
    tokens.emplace_back(data.substr(start));
    
    if (tokens.size() >= 13) {
        try {
            product.m_id = std::stoi(tokens[0]);
            product.m_name = std::move(tokens[1]);
            product.m_description = std::move(tokens[2]);
            product.m_category = std::move(tokens[3]);
            product.m_brand = std::move(tokens[4]);
            product.m_price = std::stod(tokens[5]);
            product.m_oldPrice = std::stod(tokens[6]);
            product.m_discount = std::stoi(tokens[7]);
            product.m_stock = std::stoi(tokens[8]);
            product.m_rating = std::stod(tokens[9]);
            product.m_reviewCount = std::stoi(tokens[10]);
            product.m_imageUrl = std::move(tokens[11]);
            product.m_isAvailable = (tokens[12] == "1");
        } catch (const std::exception&) {
            // Возврат товара по умолчанию в случае ошибки парсинга
            return Product();
        }
    }
    
    return product;
}
