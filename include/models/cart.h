#ifndef CART_H
#define CART_H

#include <vector>
#include <map>
#include "product.h"

struct CartItem {
    Product product;
    int quantity;

    CartItem(const Product& p, int qty = 1) : product(p), quantity(qty) {}

    double getTotalPrice() const {
        return product.getFinalPrice() * quantity;
    }
};

class Cart {
private:
    std::map<int, CartItem> m_items; // productId -> CartItem
    int m_userId;

public:
    Cart(int userId = 0);

    void addItem(const Product& product, int quantity = 1);
    void removeItem(int productId);
    void updateQuantity(int productId, int quantity);
    void clear();

    std::vector<CartItem> getItems() const;
    int getItemCount() const;
    int getTotalItemsCount() const;
    double getTotalPrice() const;
    double getTotalDiscount() const;
    bool isEmpty() const { return m_items.empty(); }

    bool hasProduct(int productId) const;
    int getProductQuantity(int productId) const;
};

#endif // CART_H
