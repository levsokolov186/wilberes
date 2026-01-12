#include "datamanager.h"
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>

DataManager* DataManager::m_instance = nullptr;

DataManager* DataManager::instance()
{
    if (!m_instance) {
        m_instance = new DataManager();
    }
    return m_instance;
}

DataManager::DataManager()
    : m_currentUser(nullptr), m_nextUserId(1), m_nextProductId(1)
{
    loadData();
    if (m_users.isEmpty() && m_products.isEmpty()) {
        initDefaultData();
    }
}

void DataManager::initDefaultData()
{
    // Создаём администратора по умолчанию
    registerUser("admin", "admin123", "admin@wb.local", UserRole::Admin);

    // Создаём тестового пользователя
    registerUser("user", "user123", "user@example.com", UserRole::Customer);

    // Создаём тестовые товары
    QStringList categories = {"Электроника", "Одежда", "Обувь",
                              "Дом и сад", "Красота", "Спорт"};
    QStringList brands = {"Samsung", "Apple", "Nike", "Adidas", "Zara",
                          "H&M", "IKEA", "L'Oreal", "Sony", "LG"};
    QStringList productNames = {
        "Смартфон Galaxy S23", "iPhone 15 Pro", "Кроссовки Air Max",
        "Футболка классическая", "Джинсы slim fit", "Платье летнее",
        "Кресло офисное", "Лампа настольная", "Крем для лица",
        "Наушники беспроводные", "Телевизор 55\"", "Куртка зимняя",
        "Ботинки кожаные", "Сумка женская", "Часы умные",
        "Пылесос робот", "Блендер", "Гантели 5 кг",
        "Коврик для йоги", "Рюкзак городской", "Шампунь восстанавливающий",
        "Планшет 10\"", "Клавиатура игровая", "Мышь беспроводная"
    };

    for (int i = 0; i < productNames.size(); i++) {
        double price = 500 + (rand() % 50000);
        double oldPrice = (rand() % 3 == 0) ? price * 1.2 + rand() % 1000 : price;

        Product p(
            nextProductId(),
            productNames[i],
            "Отличный товар высокого качества. Идеально подходит для повседневного использования. "
            "Современный дизайн и надёжность. Гарантия качества от производителя.",
            price,
            oldPrice,
            categories[rand() % categories.size()],
            brands[rand() % brands.size()],
            3.5 + (rand() % 15) / 10.0,
            rand() % 5000
            );
        m_products.append(p);
    }

    saveData();
}

bool DataManager::registerUser(const QString &username, const QString &password,
                               const QString &email, UserRole role)
{
    // Проверяем, что пользователь не существует
    for (const User &u : m_users) {
        if (u.username() == username || u.email() == email) {
            return false;
        }
    }

    User newUser(m_nextUserId++, username, password, email, role);
    m_users[newUser.id()] = newUser;
    saveData();
    return true;
}

User* DataManager::login(const QString &username, const QString &password)
{
    for (auto &user : m_users) {
        if (user.username() == username && user.password() == password) {
            m_currentUser = &m_users[user.id()];
            emit userChanged();
            return m_currentUser;
        }
    }
    return nullptr;
}

void DataManager::logout()
{
    m_currentUser = nullptr;
    emit userChanged();
}

bool DataManager::deleteUser(int userId)
{
    if (m_users.contains(userId)) {
        m_users.remove(userId);
        saveData();
        return true;
    }
    return false;
}

void DataManager::addProduct(const Product &product)
{
    m_products.append(product);
    saveData();
    emit productsChanged();
}

void DataManager::updateProduct(const Product &product)
{
    for (int i = 0; i < m_products.size(); i++) {
        if (m_products[i].id() == product.id()) {
            m_products[i] = product;
            saveData();
            emit productsChanged();
            return;
        }
    }
}

void DataManager::deleteProduct(int productId)
{
    for (int i = 0; i < m_products.size(); i++) {
        if (m_products[i].id() == productId) {
            m_products.removeAt(i);
            saveData();
            emit productsChanged();
            return;
        }
    }
}

Product* DataManager::findProduct(int productId)
{
    for (int i = 0; i < m_products.size(); i++) {
        if (m_products[i].id() == productId) {
            return &m_products[i];
        }
    }
    return nullptr;
}

int DataManager::totalOrders() const
{
    int count = 0;
    for (const User &u : m_users) {
        count += u.orders().size();
    }
    return count;
}

double DataManager::totalRevenue() const
{
    double total = 0;
    for (const User &u : m_users) {
        for (const Order &o : u.orders()) {
            total += o.total;
        }
    }
    return total;
}

void DataManager::saveData()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);

    // Сохраняем пользователей
    QFile userFile(path + "/users.dat");
    if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&userFile);
        out << m_nextUserId << "\n";
        for (const User &u : m_users) {
            out << u.toString() << "\n";
        }
        userFile.close();
    }

    // Сохраняем товары
    QFile productFile(path + "/products.dat");
    if (productFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&productFile);
        out << m_nextProductId << "\n";
        for (const Product &p : m_products) {
            out << p.toString() << "\n";
        }
        productFile.close();
    }
}

void DataManager::loadData()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // Загружаем пользователей
    QFile userFile(path + "/users.dat");
    if (userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&userFile);
        m_nextUserId = in.readLine().toInt();
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                User u = User::fromString(line);
                if (u.id() > 0) {
                    m_users[u.id()] = u;
                }
            }
        }
        userFile.close();
    }

    // Загружаем товары
    QFile productFile(path + "/products.dat");
    if (productFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&productFile);
        m_nextProductId = in.readLine().toInt();
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (!line.isEmpty()) {
                Product p = Product::fromString(line);
                if (p.id() > 0) {
                    m_products.append(p);
                }
            }
        }
        productFile.close();
    }
}
