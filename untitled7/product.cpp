#include "product.h"
#include <QPainter>
#include <QRandomGenerator>
#include <QStringList>

Product::Product()
    : m_id(0), m_price(0), m_oldPrice(0), m_rating(0), m_reviews(0)
{
}

Product::Product(int id, const QString &name, const QString &description,
                 double price, double oldPrice, const QString &category,
                 const QString &brand, double rating, int reviews,
                 const QString &imagePath)
    : m_id(id), m_name(name), m_description(description),
    m_price(price), m_oldPrice(oldPrice), m_category(category),
    m_brand(brand), m_rating(rating), m_reviews(reviews),
    m_imagePath(imagePath)
{
}

int Product::discount() const
{
    if (m_oldPrice <= m_price || m_oldPrice == 0)
        return 0;
    return static_cast<int>((1.0 - m_price / m_oldPrice) * 100);
}

QPixmap Product::getImage(int width, int height) const
{
    QPixmap pixmap(width, height);

    // Генерируем цвет на основе ID товара
    QRandomGenerator gen(m_id);
    QColor color = QColor::fromHsl(gen.bounded(360), 150, 200);

    pixmap.fill(color);

    QPainter painter(&pixmap);
    painter.setPen(Qt::darkGray);
    painter.setFont(QFont("Arial", 12));
    painter.drawRect(0, 0, width - 1, height - 1);

    // Рисуем название товара
    painter.drawText(pixmap.rect(), Qt::AlignCenter | Qt::TextWordWrap,
                     m_name.left(30));

    return pixmap;
}

QString Product::toString() const
{
    // Заменяем | на другой символ в текстовых полях
    QString safeName = m_name;
    safeName.replace("|", "&#124;");
    QString safeDesc = m_description;
    safeDesc.replace("|", "&#124;");
    QString safeBrand = m_brand;
    safeBrand.replace("|", "&#124;");

    return QString("%1|%2|%3|%4|%5|%6|%7|%8|%9|%10")
        .arg(m_id)
        .arg(safeName)
        .arg(safeDesc)
        .arg(m_price)
        .arg(m_oldPrice)
        .arg(m_category)
        .arg(safeBrand)
        .arg(m_rating)
        .arg(m_reviews)
        .arg(m_imagePath);
}

Product Product::fromString(const QString &str)
{
    QStringList parts = str.split("|");
    if (parts.size() < 10) return Product();

    // Восстанавливаем | в текстовых полях
    QString name = parts[1];
    name.replace("&#124;", "|");
    QString desc = parts[2];
    desc.replace("&#124;", "|");
    QString brand = parts[6];
    brand.replace("&#124;", "|");

    return Product(
        parts[0].toInt(),
        name,
        desc,
        parts[3].toDouble(),
        parts[4].toDouble(),
        parts[5],
        brand,
        parts[7].toDouble(),
        parts[8].toInt(),
        parts[9]
        );
}
