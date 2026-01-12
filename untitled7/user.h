#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QDateTime>

enum class UserRole {
    Guest,
    Customer,
    Admin
};

struct Order {
    int id;
    QDateTime date;
    double total;
    int itemsCount;
    QString status; // "Оформлен", "В пути", "Доставлен"
};

class User
{
public:
    User();
    User(int id, const QString &username, const QString &password,
         const QString &email, UserRole role = UserRole::Customer);

    int id() const { return m_id; }
    QString username() const { return m_username; }
    QString password() const { return m_password; }
    QString email() const { return m_email; }
    UserRole role() const { return m_role; }
    QString fullName() const { return m_fullName; }
    QString phone() const { return m_phone; }
    QString address() const { return m_address; }
    double balance() const { return m_balance; }
    QList<Order> orders() const { return m_orders; }

    void setFullName(const QString &name) { m_fullName = name; }
    void setPhone(const QString &phone) { m_phone = phone; }
    void setAddress(const QString &address) { m_address = address; }
    void setBalance(double balance) { m_balance = balance; }
    void addOrder(const Order &order) { m_orders.append(order); }

    bool isAdmin() const { return m_role == UserRole::Admin; }
    bool isGuest() const { return m_role == UserRole::Guest; }

    QString roleString() const;

    // Сериализация
    QString toString() const;
    static User fromString(const QString &str);

private:
    int m_id;
    QString m_username;
    QString m_password;
    QString m_email;
    UserRole m_role;
    QString m_fullName;
    QString m_phone;
    QString m_address;
    double m_balance;
    QList<Order> m_orders;
};

#endif // USER_H
