#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

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
    User();
    User(int id, const std::string& username, const std::string& password,
         const std::string& email, UserRole role = UserRole::Customer);

    // Getters
    int getId() const { return m_id; }
    std::string getUsername() const { return m_username; }
    std::string getPassword() const { return m_password; }
    std::string getEmail() const { return m_email; }
    std::string getPhone() const { return m_phone; }
    std::string getAddress() const { return m_address; }
    double getBalance() const { return m_balance; }
    UserRole getRole() const { return m_role; }
    bool isAdmin() const { return m_role == UserRole::Admin; }

    // Setters
    void setId(int id) { m_id = id; }
    void setUsername(const std::string& username) { m_username = username; }
    void setPassword(const std::string& password) { m_password = password; }
    void setEmail(const std::string& email) { m_email = email; }
    void setPhone(const std::string& phone) { m_phone = phone; }
    void setAddress(const std::string& address) { m_address = address; }
    void setBalance(double balance) { m_balance = balance; }
    void setRole(UserRole role) { m_role = role; }

    void addBalance(double amount);
    bool withdrawBalance(double amount);

    // Сериализация
    std::string serialize() const;
    static User deserialize(const std::string& data);
};

#endif // USER_H
