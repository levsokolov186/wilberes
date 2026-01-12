#ifndef PRODUCTDETAILDIALOG_H
#define PRODUCTDETAILDIALOG_H

#include <QDialog>
#include "product.h"

class ProductDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDetailDialog(const Product &product, QWidget *parent = nullptr);

signals:
    void addToCart(const Product &product);

private:
    void setupUI();
    Product m_product;
};

#endif // PRODUCTDETAILDIALOG_H
