#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>

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
    Product();
    Product(int id, const std::string& name, const std::string& description,
            const std::string& category, double price, int stock);

    // Getters
    int getId() const { return m_id; }
    std::string getName() const { return m_name; }
    std::string getDescription() const { return m_description; }
    std::string getCategory() const { return m_category; }
    std::string getBrand() const { return m_brand; }
    double getPrice() const { return m_price; }
    double getOldPrice() const { return m_oldPrice; }
    int getDiscount() const { return m_discount; }
    int getStock() const { return m_stock; }
    double getRating() const { return m_rating; }
    int getReviewCount() const { return m_reviewCount; }
    std::string getImageUrl() const { return m_imageUrl; }
    bool isAvailable() const { return m_isAvailable && m_stock > 0; }

    // Setters
    void setId(int id) { m_id = id; }
    void setName(const std::string& name) { m_name = name; }
    void setDescription(const std::string& desc) { m_description = desc; }
    void setCategory(const std::string& cat) { m_category = cat; }
    void setBrand(const std::string& brand) { m_brand = brand; }
    void setPrice(double price);
    void setOldPrice(double price) { m_oldPrice = price; }
    void setDiscount(int discount);
    void setStock(int stock);
    void setRating(double rating) { m_rating = rating; }
    void setReviewCount(int count) { m_reviewCount = count; }
    void setImageUrl(const std::string& url) { m_imageUrl = url; }
    void setAvailable(bool available) { m_isAvailable = available; }

    // Methods
    bool decreaseStock(int amount = 1);
    void increaseStock(int amount = 1);
    double getFinalPrice() const;

    // Сериализация
    std::string serialize() const;
    static Product deserialize(const std::string& data);
};

#endif // PRODUCT_H
