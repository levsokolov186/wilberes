#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QPixmap>

class Product
{
public:
    Product();
    Product(int id, const QString &name, const QString &description,
            double price, double oldPrice, const QString &category,
            const QString &brand, double rating, int reviews,
            const QString &imagePath = "");

    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString description() const { return m_description; }
    double price() const { return m_price; }
    double oldPrice() const { return m_oldPrice; }
    QString category() const { return m_category; }
    QString brand() const { return m_brand; }
    double rating() const { return m_rating; }
    int reviews() const { return m_reviews; }
    QString imagePath() const { return m_imagePath; }

    int discount() const;
    bool hasDiscount() const { return m_oldPrice > m_price; }
    QPixmap getImage(int width = 200, int height = 200) const;

    // Сериализация для сохранения в файл
    QString toString() const;
    static Product fromString(const QString &str);

private:
    int m_id;
    QString m_name;
    QString m_description;
    double m_price;
    double m_oldPrice;
    QString m_category;
    QString m_brand;
    double m_rating;
    int m_reviews;
    QString m_imagePath;
};

#endif // PRODUCT_H
