#include "product.h"
#include <sstream>
#include <algorithm>

Product::Product()
    : m_id(0), m_price(0.0), m_oldPrice(0.0), m_discount(0),
    m_stock(0), m_rating(0.0), m_reviewCount(0), m_isAvailable(true) {}

Product::Product(int id, const std::string& name, const std::string& description,
                 const std::string& category, double price, int stock)
    : m_id(id), m_name(name), m_description(description), m_category(category),
    m_price(price), m_oldPrice(price), m_discount(0), m_stock(stock),
    m_rating(0.0), m_reviewCount(0), m_isAvailable(true) {}

void Product::setPrice(double price) {
    if (price >= 0) {
        m_price = price;
    }
}

void Product::setDiscount(int discount) {
    m_discount = std::clamp(discount, 0, 99);
    if (m_discount > 0 && m_oldPrice <= m_price) {
        m_oldPrice = m_price;
        m_price = m_oldPrice * (100 - m_discount) / 100.0;
    }
}

void Product::setStock(int stock) {
    m_stock = std::max(0, stock);
}

bool Product::decreaseStock(int amount) {
    if (m_stock >= amount) {
        m_stock -= amount;
        return true;
    }
    return false;
}

void Product::increaseStock(int amount) {
    if (amount > 0) {
        m_stock += amount;
    }
}

double Product::getFinalPrice() const {
    return m_price;
}

std::string Product::serialize() const {
    std::ostringstream oss;
    oss << m_id << "|"
        << m_name << "|"
        << m_description << "|"
        << m_category << "|"
        << m_brand << "|"
        << m_price << "|"
        << m_oldPrice << "|"
        << m_discount << "|"
        << m_stock << "|"
        << m_rating << "|"
        << m_reviewCount << "|"
        << m_imageUrl << "|"
        << (m_isAvailable ? 1 : 0);
    return oss.str();
}

Product Product::deserialize(const std::string& data) {
    Product product;
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 13) {
        product.m_id = std::stoi(tokens[0]);
        product.m_name = tokens[1];
        product.m_description = tokens[2];
        product.m_category = tokens[3];
        product.m_brand = tokens[4];
        product.m_price = std::stod(tokens[5]);
        product.m_oldPrice = std::stod(tokens[6]);
        product.m_discount = std::stoi(tokens[7]);
        product.m_stock = std::stoi(tokens[8]);
        product.m_rating = std::stod(tokens[9]);
        product.m_reviewCount = std::stoi(tokens[10]);
        product.m_imageUrl = tokens[11];
        product.m_isAvailable = (tokens[12] == "1");
    }

    return product;
}
