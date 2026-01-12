#include "user.h"
#include <QStringList>

User::User()
    : m_id(0), m_role(UserRole::Guest), m_balance(0)
{
}

User::User(int id, const QString &username, const QString &password,
           const QString &email, UserRole role)
    : m_id(id), m_username(username), m_password(password),
    m_email(email), m_role(role), m_balance(0)
{
}

QString User::roleString() const
{
    switch (m_role) {
    case UserRole::Admin: return "Администратор";
    case UserRole::Customer: return "Покупатель";
    default: return "Гость";
    }
}

QString User::toString() const
{
    return QString("%1|%2|%3|%4|%5|%6|%7|%8|%9")
    .arg(m_id)
        .arg(m_username)
        .arg(m_password)
        .arg(m_email)
        .arg(static_cast<int>(m_role))
        .arg(m_fullName)
        .arg(m_phone)
        .arg(m_address)
        .arg(m_balance);
}

User User::fromString(const QString &str)
{
    QStringList parts = str.split("|");
    if (parts.size() < 9) return User();

    User user(parts[0].toInt(), parts[1], parts[2], parts[3],
              static_cast<UserRole>(parts[4].toInt()));
    user.setFullName(parts[5]);
    user.setPhone(parts[6]);
    user.setAddress(parts[7]);
    user.setBalance(parts[8].toDouble());

    return user;
}
