#include "user.h"
#include <sstream>

User::User()
    : m_id(0), m_balance(0.0), m_role(UserRole::Customer) {}

User::User(int id, const std::string& username, const std::string& password,
           const std::string& email, UserRole role)
    : m_id(id), m_username(username), m_password(password),
    m_email(email), m_balance(0.0), m_role(role) {}

void User::addBalance(double amount) {
    if (amount > 0) {
        m_balance += amount;
    }
}

bool User::withdrawBalance(double amount) {
    if (amount > 0 && m_balance >= amount) {
        m_balance -= amount;
        return true;
    }
    return false;
}

std::string User::serialize() const {
    std::ostringstream oss;
    oss << m_id << "|"
        << m_username << "|"
        << m_password << "|"
        << m_email << "|"
        << m_phone << "|"
        << m_address << "|"
        << m_balance << "|"
        << static_cast<int>(m_role);
    return oss.str();
}

User User::deserialize(const std::string& data) {
    User user;
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 8) {
        user.m_id = std::stoi(tokens[0]);
        user.m_username = tokens[1];
        user.m_password = tokens[2];
        user.m_email = tokens[3];
        user.m_phone = tokens[4];
        user.m_address = tokens[5];
        user.m_balance = std::stod(tokens[6]);
        user.m_role = static_cast<UserRole>(std::stoi(tokens[7]));
    }

    return user;
}
