#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <utility>

class Product {
private:
    int m_id;
    std::string m_name;
    std::string m_description;
    std::string m_category;
    std::string m_brand;
    double m_price;
    double m_oldPrice;
    int m_discount;
    int m_stock;
    double m_rating;
    int m_reviewCount;
    std::string m_imageUrl;
    bool m_isAvailable;

public:
    Product() noexcept;
    Product(int id, const std::string& name, const std::string& description,
            const std::string& category, double price, int stock);
    
    // Конструктор перемещения и оператор присваивания
    Product(Product&& other) noexcept;
    Product& operator=(Product&& other) noexcept;
    
    // Конструктор копирования и оператор присваивания (по умолчанию)
    Product(const Product&) = default;
    Product& operator=(const Product&) = default;
    
    ~Product() = default;

    // Геттеры - возвращают константные ссылки для строк
    int getId() const noexcept { return m_id; }
    const std::string& getName() const noexcept { return m_name; }
    const std::string& getDescription() const noexcept { return m_description; }
    const std::string& getCategory() const noexcept { return m_category; }
    const std::string& getBrand() const noexcept { return m_brand; }
    double getPrice() const noexcept { return m_price; }
    double getOldPrice() const noexcept { return m_oldPrice; }
    int getDiscount() const noexcept { return m_discount; }
    int getStock() const noexcept { return m_stock; }
    double getRating() const noexcept { return m_rating; }
    int getReviewCount() const noexcept { return m_reviewCount; }
    const std::string& getImageUrl() const noexcept { return m_imageUrl; }
    bool isAvailable() const noexcept { return m_isAvailable && m_stock > 0; }

    // Сеттеры - поддерживают семантику перемещения
    void setId(int id) noexcept { m_id = id; }
    
    void setName(const std::string& name) { m_name = name; }
    void setName(std::string&& name) noexcept { m_name = std::move(name); }
    
    void setDescription(const std::string& desc) { m_description = desc; }
    void setDescription(std::string&& desc) noexcept { m_description = std::move(desc); }
    
    void setCategory(const std::string& cat) { m_category = cat; }
    void setCategory(std::string&& cat) noexcept { m_category = std::move(cat); }
    
    void setBrand(const std::string& brand) { m_brand = brand; }
    void setBrand(std::string&& brand) noexcept { m_brand = std::move(brand); }
    
    void setPrice(double price) noexcept;
    void setOldPrice(double price) noexcept { m_oldPrice = price; }
    void setDiscount(int discount) noexcept;
    void setStock(int stock) noexcept;
    void setRating(double rating) noexcept { m_rating = rating; }
    void setReviewCount(int count) noexcept { m_reviewCount = count; }
    
    void setImageUrl(const std::string& url) { m_imageUrl = url; }
    void setImageUrl(std::string&& url) noexcept { m_imageUrl = std::move(url); }
    
    void setAvailable(bool available) noexcept { m_isAvailable = available; }

    // Операции с запасами
    bool decreaseStock(int amount = 1) noexcept;
    void increaseStock(int amount = 1) noexcept;
    
    // Расчет цены
    double getFinalPrice() const noexcept;

    // Сериализация
    std::string serialize() const;
    static Product deserialize(const std::string& data);
};

#endif // PRODUCT_H