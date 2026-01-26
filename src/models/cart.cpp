#include "cart.h"

Cart::Cart(int userId) : m_userId(userId) {}

void Cart::addItem(const Product& product, int quantity) {
    if (quantity <= 0) return;

    auto it = m_items.find(product.getId());
    if (it != m_items.end()) {
        it->second.quantity += quantity;
    } else {
        m_items.emplace(product.getId(), CartItem(product, quantity));
    }
}

void Cart::removeItem(int productId) {
    m_items.erase(productId);
}

void Cart::updateQuantity(int productId, int quantity) {
    if (quantity <= 0) {
        removeItem(productId);
        return;
    }

    auto it = m_items.find(productId);
    if (it != m_items.end()) {
        it->second.quantity = quantity;
    }
}

void Cart::clear() {
    m_items.clear();
}

std::vector<CartItem> Cart::getItems() const {
    std::vector<CartItem> items;
    for (const auto& pair : m_items) {
        items.push_back(pair.second);
    }
    return items;
}

int Cart::getItemCount() const {
    return static_cast<int>(m_items.size());
}

int Cart::getTotalItemsCount() const {
    int total = 0;
    for (const auto& pair : m_items) {
        total += pair.second.quantity;
    }
    return total;
}

double Cart::getTotalPrice() const {
    double total = 0.0;
    for (const auto& pair : m_items) {
        total += pair.second.getTotalPrice();
    }
    return total;
}

double Cart::getTotalDiscount() const {
    double discount = 0.0;
    for (const auto& pair : m_items) {
        const auto& item = pair.second;
        if (item.product.getDiscount() > 0) {
            discount += (item.product.getOldPrice() - item.product.getPrice()) * item.quantity;
        }
    }
    return discount;
}

bool Cart::hasProduct(int productId) const {
    return m_items.find(productId) != m_items.end();
}

int Cart::getProductQuantity(int productId) const {
    auto it = m_items.find(productId);
    return (it != m_items.end()) ? it->second.quantity : 0;
}
