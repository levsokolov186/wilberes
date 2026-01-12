#include "adminpanel.h"
#include "producteditor.h"
#include "datamanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFrame>

AdminPanel::AdminPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    refreshProducts();
    refreshUsers();
}

void AdminPanel::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Заголовок
    QWidget *header = new QWidget;
    header->setStyleSheet("background: linear-gradient(135deg, #cb11ab, #7b1fa2);");
    header->setFixedHeight(80);

    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    QLabel *title = new QLabel("🛠 Панель администратора");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    mainLayout->addWidget(header);

    // Вкладки
    m_tabWidget = new QTabWidget;
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { border: none; background: white; }"
        "QTabBar::tab { padding: 12px 25px; font-size: 14px; }"
        "QTabBar::tab:selected { background: #cb11ab; color: white; }"
        "QTabBar::tab:!selected { background: #f0f0f0; }");

    // Создаём вкладки
    QWidget *dashboardWidget = new QWidget;
    QWidget *productsWidget = new QWidget;
    QWidget *usersWidget = new QWidget;

    createDashboard();
    createProductsTab();
    createUsersTab();

    m_tabWidget->addTab(findChild<QWidget*>("dashboard"), "📊 Дашборд");
    m_tabWidget->addTab(findChild<QWidget*>("products"), "📦 Товары");
    m_tabWidget->addTab(findChild<QWidget*>("users"), "👥 Пользователи");

    mainLayout->addWidget(m_tabWidget, 1);
}

void AdminPanel::createDashboard()
{
    QWidget *dashboard = new QWidget;
    dashboard->setObjectName("dashboard");
    dashboard->setParent(m_tabWidget);

    QVBoxLayout *layout = new QVBoxLayout(dashboard);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(20);

    QLabel *welcomeLabel = new QLabel("Добро пожаловать в панель управления!");
    welcomeLabel->setStyleSheet("font-size: 18px; color: #333;");
    layout->addWidget(welcomeLabel);

    // Статистика
    QHBoxLayout *statsLayout = new QHBoxLayout;
    statsLayout->setSpacing(20);

    auto createStatCard = [](const QString &title, const QString &icon, QLabel *&valueLabel) {
        QFrame *card = new QFrame;
        card->setStyleSheet(
            "QFrame { background: white; border: 1px solid #eee; "
            "border-radius: 12px; padding: 20px; }");
        card->setMinimumSize(200, 120);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);

        QLabel *iconLabel = new QLabel(icon);
        iconLabel->setStyleSheet("font-size: 32px;");
        cardLayout->addWidget(iconLabel);

        valueLabel = new QLabel("0");
        valueLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #cb11ab;");
        cardLayout->addWidget(valueLabel);

        QLabel *titleLabel = new QLabel(title);
        titleLabel->setStyleSheet("font-size: 14px; color: #666;");
        cardLayout->addWidget(titleLabel);

        return card;
    };

    statsLayout->addWidget(createStatCard("Товаров", "📦", m_totalProductsLabel));
    statsLayout->addWidget(createStatCard("Пользователей", "👥", m_totalUsersLabel));
    statsLayout->addWidget(createStatCard("Заказов", "🛒", m_totalOrdersLabel));
    statsLayout->addWidget(createStatCard("Выручка", "💰", m_totalRevenueLabel));
    statsLayout->addStretch();

    layout->addLayout(statsLayout);

    // Быстрые действия
    QLabel *actionsLabel = new QLabel("Быстрые действия");
    actionsLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333; margin-top: 20px;");
    layout->addWidget(actionsLabel);

    QHBoxLayout *actionsLayout = new QHBoxLayout;

    QPushButton *addProductBtn = new QPushButton("➕ Добавить товар");
    addProductBtn->setStyleSheet(
        "QPushButton { background: #4CAF50; color: white; border: none; "
        "padding: 15px 25px; border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #43A047; }");
    connect(addProductBtn, &QPushButton::clicked, this, &AdminPanel::addProduct);
    actionsLayout->addWidget(addProductBtn);

    QPushButton *refreshBtn = new QPushButton("🔄 Обновить данные");
    refreshBtn->setStyleSheet(
        "QPushButton { background: #2196F3; color: white; border: none; "
        "padding: 15px 25px; border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #1E88E5; }");
    connect(refreshBtn, &QPushButton::clicked, [this]() {
        refreshProducts();
        refreshUsers();
        updateStats();
    });
    actionsLayout->addWidget(refreshBtn);

    actionsLayout->addStretch();
    layout->addLayout(actionsLayout);

    layout->addStretch();

    updateStats();
}

void AdminPanel::createProductsTab()
{
    QWidget *products = new QWidget;
    products->setObjectName("products");
    products->setParent(m_tabWidget);

    QVBoxLayout *layout = new QVBoxLayout(products);
    layout->setContentsMargins(20, 20, 20, 20);

    // Панель инструментов
    QHBoxLayout *toolbar = new QHBoxLayout;

    QPushButton *addBtn = new QPushButton("➕ Добавить товар");
    addBtn->setStyleSheet(
        "QPushButton { background: #4CAF50; color: white; border: none; "
        "padding: 10px 20px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #43A047; }");
    connect(addBtn, &QPushButton::clicked, this, &AdminPanel::addProduct);
    toolbar->addWidget(addBtn);

    QPushButton *editBtn = new QPushButton("✏️ Редактировать");
    editBtn->setStyleSheet(
        "QPushButton { background: #FF9800; color: white; border: none; "
        "padding: 10px 20px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #F57C00; }");
    connect(editBtn, &QPushButton::clicked, this, &AdminPanel::editProduct);
    toolbar->addWidget(editBtn);

    QPushButton *deleteBtn = new QPushButton("🗑️ Удалить");
    deleteBtn->setStyleSheet(
        "QPushButton { background: #f44336; color: white; border: none; "
        "padding: 10px 20px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #D32F2F; }");
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::deleteProduct);
    toolbar->addWidget(deleteBtn);

    QPushButton *refreshBtn = new QPushButton("🔄");
    refreshBtn->setStyleSheet(
        "QPushButton { background: #f0f0f0; border: none; "
        "padding: 10px 15px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #e0e0e0; }");
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPanel::refreshProducts);
    toolbar->addWidget(refreshBtn);

    toolbar->addStretch();
    layout->addLayout(toolbar);

    // Таблица товаров
    m_productsTable = new QTableWidget;
    m_productsTable->setColumnCount(8);
    m_productsTable->setHorizontalHeaderLabels({
        "ID", "Название", "Цена", "Старая цена", "Категория",
        "Бренд", "Рейтинг", "Отзывы"
    });
    m_productsTable->horizontalHeader()->setStretchLastSection(true);
    m_productsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_productsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_productsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_productsTable->setAlternatingRowColors(true);
    m_productsTable->setStyleSheet(
        "QTableWidget { border: 1px solid #ddd; border-radius: 8px; }"
        "QTableWidget::item { padding: 8px; }"
        "QHeaderView::section { background: #f5f5f5; padding: 10px; "
        "border: none; font-weight: bold; }");

    layout->addWidget(m_productsTable, 1);
}

void AdminPanel::createUsersTab()
{
    QWidget *users = new QWidget;
    users->setObjectName("users");
    users->setParent(m_tabWidget);

    QVBoxLayout *layout = new QVBoxLayout(users);
    layout->setContentsMargins(20, 20, 20, 20);

    // Панель инструментов
    QHBoxLayout *toolbar = new QHBoxLayout;

    QPushButton *deleteBtn = new QPushButton("🗑️ Удалить пользователя");
    deleteBtn->setStyleSheet(
        "QPushButton { background: #f44336; color: white; border: none; "
        "padding: 10px 20px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #D32F2F; }");
    connect(deleteBtn, &QPushButton::clicked, this, &AdminPanel::deleteUser);
    toolbar->addWidget(deleteBtn);

    QPushButton *refreshBtn = new QPushButton("🔄 Обновить");
    refreshBtn->setStyleSheet(
        "QPushButton { background: #f0f0f0; border: none; "
        "padding: 10px 15px; border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #e0e0e0; }");
    connect(refreshBtn, &QPushButton::clicked, this, &AdminPanel::refreshUsers);
    toolbar->addWidget(refreshBtn);

    toolbar->addStretch();
    layout->addLayout(toolbar);

    // Таблица пользователей
    m_usersTable = new QTableWidget;
    m_usersTable->setColumnCount(5);
    m_usersTable->setHorizontalHeaderLabels({
        "ID", "Логин", "Email", "Роль", "Баланс"
    });
    m_usersTable->horizontalHeader()->setStretchLastSection(true);
    m_usersTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_usersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_usersTable->setAlternatingRowColors(true);
    m_usersTable->setStyleSheet(
        "QTableWidget { border: 1px solid #ddd; border-radius: 8px; }"
        "QTableWidget::item { padding: 8px; }"
        "QHeaderView::section { background: #f5f5f5; padding: 10px; "
        "border: none; font-weight: bold; }");

    layout->addWidget(m_usersTable, 1);
}

void AdminPanel::updateStats()
{
    m_totalProductsLabel->setText(QString::number(DataManager::instance()->totalProducts()));
    m_totalUsersLabel->setText(QString::number(DataManager::instance()->totalUsers()));
    m_totalOrdersLabel->setText(QString::number(DataManager::instance()->totalOrders()));
    m_totalRevenueLabel->setText(QString("%1 ₽").arg(DataManager::instance()->totalRevenue(), 0, 'f', 0));
}

void AdminPanel::refreshProducts()
{
    m_productsTable->setRowCount(0);

    for (const Product &p : DataManager::instance()->products()) {
        int row = m_productsTable->rowCount();
        m_productsTable->insertRow(row);

        m_productsTable->setItem(row, 0, new QTableWidgetItem(QString::number(p.id())));
        m_productsTable->setItem(row, 1, new QTableWidgetItem(p.name()));
        m_productsTable->setItem(row, 2, new QTableWidgetItem(QString("%1 ₽").arg(p.price(), 0, 'f', 0)));
        m_productsTable->setItem(row, 3, new QTableWidgetItem(QString("%1 ₽").arg(p.oldPrice(), 0, 'f', 0)));
        m_productsTable->setItem(row, 4, new QTableWidgetItem(p.category()));
        m_productsTable->setItem(row, 5, new QTableWidgetItem(p.brand()));
        m_productsTable->setItem(row, 6, new QTableWidgetItem(QString::number(p.rating(), 'f', 1)));
        m_productsTable->setItem(row, 7, new QTableWidgetItem(QString::number(p.reviews())));
    }

    updateStats();
}

void AdminPanel::refreshUsers()
{
    m_usersTable->setRowCount(0);

    for (const User &u : DataManager::instance()->allUsers()) {
        int row = m_usersTable->rowCount();
        m_usersTable->insertRow(row);

        m_usersTable->setItem(row, 0, new QTableWidgetItem(QString::number(u.id())));
        m_usersTable->setItem(row, 1, new QTableWidgetItem(u.username()));
        m_usersTable->setItem(row, 2, new QTableWidgetItem(u.email()));
        m_usersTable->setItem(row, 3, new QTableWidgetItem(u.roleString()));
        m_usersTable->setItem(row, 4, new QTableWidgetItem(QString("%1 ₽").arg(u.balance(), 0, 'f', 0)));

        // Подсветка админов
        if (u.isAdmin()) {
            for (int col = 0; col < 5; col++) {
                m_usersTable->item(row, col)->setBackground(QColor("#fff3e0"));
            }
        }
    }

    updateStats();
}

void AdminPanel::addProduct()
{
    ProductEditor editor(this);
    if (editor.exec() == QDialog::Accepted) {
        DataManager::instance()->addProduct(editor.getProduct());
        refreshProducts();
        emit productChanged();
        QMessageBox::information(this, "Успех", "Товар успешно добавлен!");
    }
}

void AdminPanel::editProduct()
{
    int row = m_productsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для редактирования");
        return;
    }

    int productId = m_productsTable->item(row, 0)->text().toInt();
    Product *product = DataManager::instance()->findProduct(productId);

    if (!product) {
        QMessageBox::warning(this, "Ошибка", "Товар не найден");
        return;
    }

    ProductEditor editor(*product, this);
    if (editor.exec() == QDialog::Accepted) {
        DataManager::instance()->updateProduct(editor.getProduct());
        refreshProducts();
        emit productChanged();
        QMessageBox::information(this, "Успех", "Товар успешно обновлён!");
    }
}

void AdminPanel::deleteProduct()
{
    int row = m_productsTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите товар для удаления");
        return;
    }

    int productId = m_productsTable->item(row, 0)->text().toInt();
    QString productName = m_productsTable->item(row, 1)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Подтверждение",
        QString("Вы уверены, что хотите удалить товар \"%1\"?").arg(productName),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        DataManager::instance()->deleteProduct(productId);
        refreshProducts();
        emit productChanged();
        QMessageBox::information(this, "Успех", "Товар удалён!");
    }
}

void AdminPanel::deleteUser()
{
    int row = m_usersTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите пользователя для удаления");
        return;
    }

    int userId = m_usersTable->item(row, 0)->text().toInt();
    QString username = m_usersTable->item(row, 1)->text();

    // Нельзя удалить текущего пользователя
    if (DataManager::instance()->currentUser() &&
        DataManager::instance()->currentUser()->id() == userId) {
        QMessageBox::warning(this, "Ошибка", "Нельзя удалить текущего пользователя");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Подтверждение",
        QString("Вы уверены, что хотите удалить пользователя \"%1\"?").arg(username),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        DataManager::instance()->deleteUser(userId);
        refreshUsers();
        QMessageBox::information(this, "Успех", "Пользователь удалён!");
    }
}
