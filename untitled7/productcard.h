#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "product.h"

class ProductCard : public QWidget
{
    Q_OBJECT

public:
    explicit ProductCard(const Product &product, QWidget *parent = nullptr);
    Product product() const { return m_product; }

signals:
    void addToCart(const Product &product);
    void viewDetails(const Product &product);
    void addToFavorites(const Product &product);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void setupUI();
    void updateStyle(bool hovered);
    QString createRatingStars(double rating);

    Product m_product;
    QLabel *m_imageLabel;
    QLabel *m_priceLabel;
    QLabel *m_oldPriceLabel;
    QLabel *m_discountLabel;
    QLabel *m_nameLabel;
    QLabel *m_ratingLabel;
    QLabel *m_brandLabel;
    QPushButton *m_addToCartBtn;
    QPushButton *m_favoriteBtn;
};

#endif // PRODUCTCARD_H
