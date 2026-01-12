#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "user.h"
#include "product.h"

class DataManager : public QObject
{
    Q_OBJECT

public:
    static DataManager* instance();

    // Пользователи
    bool registerUser(const QString &username, const QString &password,
                      const QString &email, UserRole role = UserRole::Customer);
    User* login(const QString &username, const QString &password);
    void logout();
    User* currentUser() { return m_currentUser; }
    bool isLoggedIn() const { return m_currentUser != nullptr; }
    bool isAdmin() const { return m_currentUser && m_currentUser->isAdmin(); }
    QList<User> allUsers() const { return m_users.values(); }
    bool deleteUser(int userId);

    // Товары
    QList<Product>& products() { return m_products; }
    void addProduct(const Product &product);
    void updateProduct(const Product &product);
    void deleteProduct(int productId);
    Product* findProduct(int productId);
    int nextProductId() { return ++m_nextProductId; }

    // Сохранение/загрузка
    void saveData();
    void loadData();

    // Статистика
    int totalProducts() const { return m_products.size(); }
    int totalUsers() const { return m_users.size(); }
    int totalOrders() const;
    double totalRevenue() const;

signals:
    void userChanged();
    void productsChanged();

private:
    DataManager();
    static DataManager *m_instance;

    void initDefaultData();

    QMap<int, User> m_users;
    QList<Product> m_products;
    User *m_currentUser;
    int m_nextUserId;
    int m_nextProductId;
};

#endif // DATAMANAGER_H
