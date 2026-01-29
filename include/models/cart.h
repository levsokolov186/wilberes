#ifndef CART_H
#define CART_H

#include <vector>
#include <map>
#include "product.h"

struct CartItem {
    Product product;
    int quantity;

    explicit CartItem(const Product& p, int qty = 1) 
        : product(p)
        , quantity(qty) 
    {}
    
    CartItem(Product&& p, int qty = 1) noexcept
        : product(std::move(p))
        , quantity(qty) 
    {}

    double getTotalPrice() const noexcept {
        return product.getFinalPrice() * quantity;
    }
    
    double getOriginalPrice() const noexcept {
        return product.getOldPrice() * quantity;
    }
    
    double getDiscount() const noexcept {
        if (product.getDiscount() > 0) {
            return (product.getOldPrice() - product.getPrice()) * quantity;
        }
        return 0.0;
    }
};

class Cart {
private:
    std::map<int, CartItem> m_items; // productId -> элемент корзины
    int m_userId;

public:
    explicit Cart(int userId = 0) noexcept;
    
    // Запрет копирования (корзина должна быть уникальной для сессии пользователя)
    Cart(const Cart&) = delete;
    Cart& operator=(const Cart&) = delete;
    
    // Разрешение перемещения
    Cart(Cart&& other) noexcept;
    Cart& operator=(Cart&& other) noexcept;
    
    ~Cart() = default;

    // Операции с элементами
    void addItem(const Product& product, int quantity = 1);
    void removeItem(int productId) noexcept;
    void updateQuantity(int productId, int quantity);
    void clear() noexcept;

    // Геттеры (методы доступа)
    std::vector<CartItem> getItems() const;
    int getItemCount() const noexcept;
    int getTotalItemsCount() const noexcept;
    double getTotalPrice() const noexcept;
    double getTotalDiscount() const noexcept;
    int getUserId() const noexcept { return m_userId; }
    
    // Проверка состояния
    bool isEmpty() const noexcept { return m_items.empty(); }
    bool hasProduct(int productId) const noexcept;
    int getProductQuantity(int productId) const noexcept;
    
    // Получение элемента по ID товара (возвращает nullptr, если не найден)
    const CartItem* getItem(int productId) const noexcept;
};

#endif // CART_H