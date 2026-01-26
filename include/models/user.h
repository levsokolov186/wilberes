#ifndef USER_H
#define USER_H

#include <string>
#include <utility>

enum class UserRole {
    Customer,
    Admin
};

class User {
private:
    int m_id;
    std::string m_username;
    std::string m_password;
    std::string m_email;
    std::string m_phone;
    std::string m_address;
    double m_balance;
    UserRole m_role;

public:
    User() noexcept;
    User(int id, const std::string& username, const std::string& password,
         const std::string& email, UserRole role = UserRole::Customer);
    
    // Конструктор перемещения и оператор присваивания
    User(User&& other) noexcept;
    User& operator=(User&& other) noexcept;
    
    // Конструктор копирования и оператор присваивания (по умолчанию)
    User(const User&) = default;
    User& operator=(const User&) = default;
    
    ~User() = default;

    // Геттеры - возвращают константные ссылки для строк
    int getId() const noexcept { return m_id; }
    const std::string& getUsername() const noexcept { return m_username; }
    const std::string& getPassword() const noexcept { return m_password; }
    const std::string& getEmail() const noexcept { return m_email; }
    const std::string& getPhone() const noexcept { return m_phone; }
    const std::string& getAddress() const noexcept { return m_address; }
    double getBalance() const noexcept { return m_balance; }
    UserRole getRole() const noexcept { return m_role; }
    bool isAdmin() const noexcept { return m_role == UserRole::Admin; }

    // Сеттеры - поддерживают семантику перемещения
    void setId(int id) noexcept { m_id = id; }
    
    void setUsername(const std::string& username) { m_username = username; }
    void setUsername(std::string&& username) noexcept { m_username = std::move(username); }
    
    void setPassword(const std::string& password) { m_password = password; }
    void setPassword(std::string&& password) noexcept { m_password = std::move(password); }
    
    void setEmail(const std::string& email) { m_email = email; }
    void setEmail(std::string&& email) noexcept { m_email = std::move(email); }
    
    void setPhone(const std::string& phone) { m_phone = phone; }
    void setPhone(std::string&& phone) noexcept { m_phone = std::move(phone); }
    
    void setAddress(const std::string& address) { m_address = address; }
    void setAddress(std::string&& address) noexcept { m_address = std::move(address); }
    
    void setBalance(double balance) noexcept { m_balance = balance; }
    void setRole(UserRole role) noexcept { m_role = role; }

    // Операции с балансом
    void addBalance(double amount) noexcept;
    bool withdrawBalance(double amount) noexcept;

    // Сериализация
    std::string serialize() const;
    static User deserialize(const std::string& data);
};

#endif // USER_H