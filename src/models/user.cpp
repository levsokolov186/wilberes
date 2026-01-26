#include "user.h"
#include <sstream>
#include <vector>

User::User() noexcept
    : m_id(0)
    , m_balance(0.0)
    , m_role(UserRole::Customer) 
{}

User::User(int id, const std::string& username, const std::string& password,
           const std::string& email, UserRole role)
    : m_id(id)
    , m_username(username)
    , m_password(password)
    , m_email(email)
    , m_balance(0.0)
    , m_role(role) 
{}

User::User(User&& other) noexcept
    : m_id(other.m_id)
    , m_username(std::move(other.m_username))
    , m_password(std::move(other.m_password))
    , m_email(std::move(other.m_email))
    , m_phone(std::move(other.m_phone))
    , m_address(std::move(other.m_address))
    , m_balance(other.m_balance)
    , m_role(other.m_role)
{
    other.m_id = 0;
    other.m_balance = 0.0;
    other.m_role = UserRole::Customer;
}

User& User::operator=(User&& other) noexcept {
    if (this != &other) {
        m_id = other.m_id;
        m_username = std::move(other.m_username);
        m_password = std::move(other.m_password);
        m_email = std::move(other.m_email);
        m_phone = std::move(other.m_phone);
        m_address = std::move(other.m_address);
        m_balance = other.m_balance;
        m_role = other.m_role;
        
        other.m_id = 0;
        other.m_balance = 0.0;
        other.m_role = UserRole::Customer;
    }
    return *this;
}

void User::addBalance(double amount) noexcept {
    if (amount > 0) {
        m_balance += amount;
    }
}

bool User::withdrawBalance(double amount) noexcept {
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
    
    if (data.empty()) {
        return user;
    }
    
    std::vector<std::string> tokens;
    tokens.reserve(8);
    
    std::size_t start = 0;
    std::size_t end = 0;
    
    while ((end = data.find('|', start)) != std::string::npos) {
        tokens.emplace_back(data.substr(start, end - start));
        start = end + 1;
    }
    tokens.emplace_back(data.substr(start));
    
    if (tokens.size() >= 8) {
        try {
            user.m_id = std::stoi(tokens[0]);
            user.m_username = std::move(tokens[1]);
            user.m_password = std::move(tokens[2]);
            user.m_email = std::move(tokens[3]);
            user.m_phone = std::move(tokens[4]);
            user.m_address = std::move(tokens[5]);
            user.m_balance = std::stod(tokens[6]);
            user.m_role = static_cast<UserRole>(std::stoi(tokens[7]));
        } catch (const std::exception&) {
            // Возврат пользователя по умолчанию в случае ошибки парсинга
            return User();
        }
    }
    
    return user;
}