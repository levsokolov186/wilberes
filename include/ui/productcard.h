#ifndef PRODUCTCARD_H
#define PRODUCTCARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "product.h"

class ProductCard : public QWidget {
    Q_OBJECT

public:
    explicit ProductCard(const Product& product, QWidget *parent = nullptr);
    void updateProduct(const Product& product);

signals:
    void addToCartClicked(const Product& product);
    void productClicked(const Product& product);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void setupUI();

    Product m_product;
    QLabel* m_imageLabel;
    QLabel* m_discountLabel;
    QLabel* m_nameLabel;
    QLabel* m_brandLabel;
    QLabel* m_priceLabel;
    QLabel* m_oldPriceLabel;
    QLabel* m_ratingLabel;
    QPushButton* m_addButton;
};

#endif // PRODUCTCARD_H
