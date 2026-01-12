#ifndef CARTDIALOG_H
#define CARTDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QMap>
#include "product.h"

struct CartItem {
    Product product;
    int quantity;
};

class CartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CartDialog(QWidget *parent = nullptr);

    void addProduct(const Product &product);
    void clear();
    int totalItems() const;
    double totalPrice() const;

signals:
    void cartChanged();

private:
    void setupUI();
    void updateCartList();
    void updateTotals();
    void removeItem(int productId);
    void changeQuantity(int productId, int delta);

    QListWidget *m_cartList;
    QLabel *m_totalLabel;
    QLabel *m_itemsCountLabel;
    QLabel *m_discountLabel;
    QPushButton *m_checkoutBtn;
    QPushButton *m_clearBtn;

    QMap<int, CartItem> m_items; // productId -> CartItem
};

#endif // CARTDIALOG_H
