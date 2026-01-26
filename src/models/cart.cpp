#include "cart.h"
#include <algorithm>
#include <numeric>

Cart::Cart(int userId) noexcept 
    : m_userId(userId) 
{}

Cart::Cart(Cart&& other) noexcept
    : m_items(std::move(other.m_items))
    , m_userId(other.m_userId)
{
    other.m_userId = 0;
}

Cart& Cart::operator=(Cart&& other) noexcept {
    if (this != &other) {
        m_items = std::move(other.m_items);
        m_userId = other.m_userId;
        other.m_userId = 0;
    }
    return *this;
}

void Cart::addItem(const Product& product, int quantity) {
    if (quantity <= 0) {
        return;
    }

    const int productId = product.getId();
    auto it = m_items.find(productId);
    
    if (it != m_items.end()) {
        // Товар уже в корзине, увеличиваем количество
        it->second.quantity += quantity;
    } else {
        // Новый товар, добавляем в корзину
        m_items.emplace(productId, CartItem(product, quantity));
    }
}

void Cart::removeItem(int productId) noexcept {
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

void Cart::clear() noexcept {
    m_items.clear();
}

std::vector<CartItem> Cart::getItems() const {
    std::vector<CartItem> items;
    items.reserve(m_items.size());
    
    for (const auto& [id, item] : m_items) {
        items.push_back(item);
    }
    
    return items;
}

int Cart::getItemCount() const noexcept {
    return static_cast<int>(m_items.size());
}

int Cart::getTotalItemsCount() const noexcept {
    int total = 0;
    for (const auto& [id, item] : m_items) {
        total += item.quantity;
    }
    return total;
}

double Cart::getTotalPrice() const noexcept {
    double total = 0.0;
    for (const auto& [id, item] : m_items) {
        total += item.getTotalPrice();
    }
    return total;
}

double Cart::getTotalDiscount() const noexcept {
    double discount = 0.0;
    for (const auto& [id, item] : m_items) {
        discount += item.getDiscount();
    }
    return discount;
}

bool Cart::hasProduct(int productId) const noexcept {
    return m_items.find(productId) != m_items.end();
}

int Cart::getProductQuantity(int productId) const noexcept {
    auto it = m_items.find(productId);
    return (it != m_items.end()) ? it->second.quantity : 0;
}

const CartItem* Cart::getItem(int productId) const noexcept {
    auto it = m_items.find(productId);
    return (it != m_items.end()) ? &(it->second) : nullptr;
}