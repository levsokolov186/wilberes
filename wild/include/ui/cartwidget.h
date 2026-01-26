#ifndef CARTWIDGET_H
#define CARTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include "cart.h"

class CartWidget : public QWidget {
    Q_OBJECT

public:
    explicit CartWidget(Cart* cart, QWidget *parent = nullptr);
    void updateCart();

signals:
    void checkoutClicked();
    void cartUpdated();

private:
    void setupUI();
    QWidget* createCartItemWidget(const CartItem& item);

    Cart* m_cart;
    QVBoxLayout* m_itemsLayout;
    QLabel* m_totalLabel;
    QLabel* m_discountLabel;
    QLabel* m_emptyLabel;
    QPushButton* m_checkoutButton;
    QScrollArea* m_scrollArea;
};

#endif // CARTWIDGET_H
