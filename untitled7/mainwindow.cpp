#include "mainwindow.h"
#include "productcard.h"
#include "productdetaildialog.h"
#include "logindialog.h"
#include "datamanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("WildBerries Local");
    setMinimumSize(1200, 800);

    m_cartDialog = new CartDialog(this);
    connect(m_cartDialog, &CartDialog::cartChanged, [this]() {
        int count = m_cartDialog->totalItems();
        m_cartBadge->setText(QString::number(count));
        m_cartBadge->setVisible(count > 0);
    });

    // Подписываемся на изменение пользователя
    connect(DataManager::instance(), &DataManager::userChanged,
            this, &MainWindow::updateUserUI);
    connect(DataManager::instance(), &DataManager::productsChanged,
            this, &MainWindow::filterProducts);

    setupUI();

    // Показываем диалог входа при запуске
    showLoginDialog();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showLoginDialog()
{
    LoginDialog loginDialog(this);
    if (loginDialog.exec() == QDialog::Accepted) {
        updateUserUI();
        filterProducts();
    } else {
        // Если пользователь закрыл окно без входа - выходим
        QApplication::quit();
    }
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    createHeader();

    // Главный стек (магазин / админка)
    m_mainStack = new QStackedWidget;

    // Создаём магазин
    m_shopWidget = new QWidget;
    QHBoxLayout *shopLayout = new QHBoxLayout(m_shopWidget);
    shopLayout->setContentsMargins(20, 20, 20, 20);
    shopLayout->setSpacing(20);

    createSidebar();
    shopLayout->addWidget(findChild<QWidget*>("sidebar"));

    createProductGrid();
    shopLayout->addWidget(m_scrollArea, 1);

    m_mainStack->addWidget(m_shopWidget);

    // Создаём админ-панель
    m_adminPanel = new AdminPanel;
    connect(m_adminPanel, &AdminPanel::productChanged, this, &MainWindow::filterProducts);
    m_mainStack->addWidget(m_adminPanel);

    QWidget *headerWidget = findChild<QWidget*>("header");
    mainLayout->addWidget(headerWidget);
    mainLayout->addWidget(m_mainStack, 1);

    // Стиль приложения
    qApp->setStyleSheet(
        "QMainWindow { background-color: #f5f5f5; }"
        "QScrollArea { border: none; background-color: transparent; }"
        );
}

void MainWindow::createHeader()
{
    QWidget *header = new QWidget;
    header->setObjectName("header");
    header->setStyleSheet("QWidget#header { background-color: #cb11ab; }");
    header->setFixedHeight(70);

    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(30, 10, 30, 10);

    // Логотип
    QLabel *logo = new QLabel("🛍 WildBerries");
    logo->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    logo->setCursor(Qt::PointingHandCursor);
    headerLayout->addWidget(logo);

    // Кнопка "Магазин"
    QPushButton *shopBtn = new QPushButton("🏪 Магазин");
    shopBtn->setStyleSheet(
        "QPushButton { background: rgba(255,255,255,0.2); color: white; "
        "border: none; padding: 8px 15px; border-radius: 15px; font-size: 13px; }"
        "QPushButton:hover { background: rgba(255,255,255,0.3); }");
    connect(shopBtn, &QPushButton::clicked, this, &MainWindow::switchToShop);
    headerLayout->addWidget(shopBtn);

    // Кнопка "Админ-панель" (только для админов)
    m_adminButton = new QPushButton("⚙️ Админ-панель");
    m_adminButton->setStyleSheet(
        "QPushButton { background: rgba(255,255,255,0.2); color: white; "
        "border: none; padding: 8px 15px; border-radius: 15px; font-size: 13px; }"
        "QPushButton:hover { background: rgba(255,255,255,0.3); }");
    connect(m_adminButton, &QPushButton::clicked, this, &MainWindow::switchToAdmin);
    m_adminButton->hide();
    headerLayout->addWidget(m_adminButton);

    headerLayout->addSpacing(30);

    // Поиск
    m_searchEdit = new QLineEdit;
    m_searchEdit->setPlaceholderText("Искать товары...");
    m_searchEdit->setStyleSheet(
        "QLineEdit { padding: 12px 20px; border-radius: 20px; "
        "border: none; font-size: 14px; min-width: 350px; }"
        "QLineEdit:focus { outline: none; }");
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
    headerLayout->addWidget(m_searchEdit);

    QPushButton *searchBtn = new QPushButton("🔍");
    searchBtn->setStyleSheet(
        "QPushButton { background: white; border: none; padding: 12px 20px; "
        "border-radius: 20px; font-size: 16px; }"
        "QPushButton:hover { background: #f0f0f0; }");
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearch);
    headerLayout->addWidget(searchBtn);

    headerLayout->addStretch();

    // Информация о пользователе
    m_userLabel = new QLabel;
    m_userLabel->setStyleSheet("color: white; font-size: 13px;");
    headerLayout->addWidget(m_userLabel);

    // Корзина
    QWidget *cartWidget = new QWidget;
    QHBoxLayout *cartLayout = new QHBoxLayout(cartWidget);
    cartLayout->setContentsMargins(0, 0, 0, 0);

    m_cartButton = new QPushButton("🛒 Корзина");
    m_cartButton->setStyleSheet(
        "QPushButton { background: white; color: #cb11ab; "
        "border: none; padding: 10px 20px; border-radius: 20px; "
        "font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background: #f0f0f0; }");
    connect(m_cartButton, &QPushButton::clicked, this, &MainWindow::showCart);
    cartLayout->addWidget(m_cartButton);

    m_cartBadge = new QLabel("0");
    m_cartBadge->setStyleSheet(
        "background: #ff4444; color: white; border-radius: 10px; "
        "padding: 2px 6px; font-size: 12px; font-weight: bold;");
    m_cartBadge->setVisible(false);
    cartLayout->addWidget(m_cartBadge);

    headerLayout->addWidget(cartWidget);

    headerLayout->addSpacing(10);

    // Кнопка входа/выхода
    m_userButton = new QPushButton("👤 Войти");
    m_userButton->setStyleSheet(
        "QPushButton { background: transparent; color: white; "
        "border: 2px solid white; padding: 10px 20px; border-radius: 20px; "
        "font-size: 14px; }"
        "QPushButton:hover { background: rgba(255,255,255,0.2); }");
    connect(m_userButton, &QPushButton::clicked, this, &MainWindow::onLoginLogout);
    headerLayout->addWidget(m_userButton);
}

void MainWindow::createSidebar()
{
    QWidget *sidebar = new QWidget;
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(250);
    sidebar->setStyleSheet("QWidget#sidebar { background: white; border-radius: 12px; }");

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(15, 20, 15, 20);
    sidebarLayout->setSpacing(15);

    // Заголовок
    QLabel *filterTitle = new QLabel("Фильтры");
    filterTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    sidebarLayout->addWidget(filterTitle);

    // Категории
    QLabel *catLabel = new QLabel("Категория");
    catLabel->setStyleSheet("font-size: 14px; color: #666;");
    sidebarLayout->addWidget(catLabel);

    m_categoryCombo = new QComboBox;
    m_categoryCombo->addItems({"Все категории", "Электроника", "Одежда",
                               "Обувь", "Дом и сад", "Красота", "Спорт"});
    m_categoryCombo->setStyleSheet(
        "QComboBox { padding: 10px; border: 1px solid #ddd; border-radius: 8px; }"
        "QComboBox:hover { border-color: #cb11ab; }");
    connect(m_categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCategoryChanged);
    sidebarLayout->addWidget(m_categoryCombo);

    // Сортировка
    QLabel *sortLabel = new QLabel("Сортировка");
    sortLabel->setStyleSheet("font-size: 14px; color: #666;");
    sidebarLayout->addWidget(sortLabel);

    m_sortCombo = new QComboBox;
    m_sortCombo->addItems({"По популярности", "Сначала дешёвые",
                           "Сначала дорогие", "По рейтингу", "По скидке"});
    m_sortCombo->setStyleSheet(
        "QComboBox { padding: 10px; border: 1px solid #ddd; border-radius: 8px; }"
        "QComboBox:hover { border-color: #cb11ab; }");
    connect(m_sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSortChanged);
    sidebarLayout->addWidget(m_sortCombo);

    // Цена
    QLabel *priceLabel = new QLabel("Цена");
    priceLabel->setStyleSheet("font-size: 14px; color: #666;");
    sidebarLayout->addWidget(priceLabel);

    m_priceRangeLabel = new QLabel("0 ₽ — 100 000 ₽");
    m_priceRangeLabel->setStyleSheet("font-size: 13px; color: #333;");
    sidebarLayout->addWidget(m_priceRangeLabel);

    m_minPriceSlider = new QSlider(Qt::Horizontal);
    m_minPriceSlider->setRange(0, 100000);
    m_minPriceSlider->setValue(0);
    m_minPriceSlider->setStyleSheet(
        "QSlider::groove:horizontal { height: 6px; background: #ddd; border-radius: 3px; }"
        "QSlider::handle:horizontal { width: 16px; height: 16px; "
        "background: #cb11ab; border-radius: 8px; margin: -5px 0; }");
    connect(m_minPriceSlider, &QSlider::valueChanged, this, &MainWindow::onPriceFilterChanged);
    sidebarLayout->addWidget(m_minPriceSlider);

    m_maxPriceSlider = new QSlider(Qt::Horizontal);
    m_maxPriceSlider->setRange(0, 100000);
    m_maxPriceSlider->setValue(100000);
    m_maxPriceSlider->setStyleSheet(
        "QSlider::groove:horizontal { height: 6px; background: #ddd; border-radius: 3px; }"
        "QSlider::handle:horizontal { width: 16px; height: 16px; "
        "background: #cb11ab; border-radius: 8px; margin: -5px 0; }");
    connect(m_maxPriceSlider, &QSlider::valueChanged, this, &MainWindow::onPriceFilterChanged);
    sidebarLayout->addWidget(m_maxPriceSlider);

    // Разделитель
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #eee;");
    sidebarLayout->addWidget(line);

    // Кнопка сброса
    QPushButton *resetBtn = new QPushButton("Сбросить фильтры");
    resetBtn->setStyleSheet(
        "QPushButton { background: #f5f5f5; border: none; padding: 12px; "
        "border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #eee; }");
    connect(resetBtn, &QPushButton::clicked, [this]() {
        m_categoryCombo->setCurrentIndex(0);
        m_sortCombo->setCurrentIndex(0);
        m_minPriceSlider->setValue(0);
        m_maxPriceSlider->setValue(100000);
        m_searchEdit->clear();
        filterProducts();
    });
    sidebarLayout->addWidget(resetBtn);

    sidebarLayout->addStretch();
}

void MainWindow::createProductGrid()
{
    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_productContainer = new QWidget;
    m_productContainer->setStyleSheet("background-color: transparent;");

    m_productGrid = new QGridLayout(m_productContainer);
    m_productGrid->setSpacing(20);
    m_productGrid->setContentsMargins(0, 0, 0, 0);

    m_scrollArea->setWidget(m_productContainer);
}

void MainWindow::filterProducts()
{
    m_filteredProducts.clear();

    QString searchText = m_searchEdit->text().toLower();
    QString category = m_categoryCombo->currentText();
    int minPrice = m_minPriceSlider->value();
    int maxPrice = m_maxPriceSlider->value();

    for (const Product &p : DataManager::instance()->products()) {
        // Фильтр по поиску
        if (!searchText.isEmpty() &&
            !p.name().toLower().contains(searchText) &&
            !p.brand().toLower().contains(searchText)) {
            continue;
        }

        // Фильтр по категории
        if (category != "Все категории" && p.category() != category) {
            continue;
        }

        // Фильтр по цене
        if (p.price() < minPrice || p.price() > maxPrice) {
            continue;
        }

        m_filteredProducts.append(p);
    }

    // Сортировка
    int sortIndex = m_sortCombo->currentIndex();
    std::sort(m_filteredProducts.begin(), m_filteredProducts.end(),
              [sortIndex](const Product &a, const Product &b) {
                  switch (sortIndex) {
                  case 1: return a.price() < b.price();
                  case 2: return a.price() > b.price();
                  case 3: return a.rating() > b.rating();
                  case 4: return a.discount() > b.discount();
                  default: return a.reviews() > b.reviews();
                  }
              });

    displayProducts(m_filteredProducts);
}

void MainWindow::displayProducts(const QList<Product> &products)
{
    // Очистка текущих карточек
    QLayoutItem *item;
    while ((item = m_productGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Отображение товаров
    int col = 0, row = 0;
    int columns = 4;

    for (const Product &p : products) {
        ProductCard *card = new ProductCard(p);
        connect(card, &ProductCard::addToCart, this, &MainWindow::addToCart);
        connect(card, &ProductCard::viewDetails, this, &MainWindow::showProductDetails);

        m_productGrid->addWidget(card, row, col);

        col++;
        if (col >= columns) {
            col = 0;
            row++;
        }
    }

    m_productGrid->setRowStretch(row + 1, 1);
    m_productGrid->setColumnStretch(columns, 1);
}

void MainWindow::onSearch()
{
    filterProducts();
}

void MainWindow::onCategoryChanged(int index)
{
    Q_UNUSED(index);
    filterProducts();
}

void MainWindow::onSortChanged(int index)
{
    Q_UNUSED(index);
    filterProducts();
}

void MainWindow::onPriceFilterChanged()
{
    int minVal = m_minPriceSlider->value();
    int maxVal = m_maxPriceSlider->value();

    if (minVal > maxVal) {
        m_minPriceSlider->setValue(maxVal);
        return;
    }

    m_priceRangeLabel->setText(QString("%1 ₽ — %2 ₽").arg(minVal).arg(maxVal));
    filterProducts();
}

void MainWindow::addToCart(const Product &product)
{
    if (DataManager::instance()->currentUser() == nullptr) {
        QMessageBox::information(this, "Вход", "Войдите, чтобы добавить товары в корзину");
        return;
    }

    m_cartDialog->addProduct(product);

    m_cartButton->setStyleSheet(
        "QPushButton { background: #4CAF50; color: white; "
        "border: none; padding: 10px 20px; border-radius: 20px; "
        "font-size: 14px; font-weight: bold; }");

    QTimer::singleShot(300, [this]() {
        m_cartButton->setStyleSheet(
            "QPushButton { background: white; color: #cb11ab; "
            "border: none; padding: 10px 20px; border-radius: 20px; "
            "font-size: 14px; font-weight: bold; }"
            "QPushButton:hover { background: #f0f0f0; }");
    });
}

void MainWindow::showProductDetails(const Product &product)
{
    ProductDetailDialog *dialog = new ProductDetailDialog(product, this);
    connect(dialog, &ProductDetailDialog::addToCart, this, &MainWindow::addToCart);
    dialog->exec();
    delete dialog;
}

void MainWindow::showCart()
{
    if (DataManager::instance()->currentUser() == nullptr) {
        QMessageBox::information(this, "Вход", "Войдите, чтобы открыть корзину");
        return;
    }
    m_cartDialog->exec();
}

void MainWindow::onLoginLogout()
{
    if (DataManager::instance()->isLoggedIn()) {
        DataManager::instance()->logout();
        m_cartDialog->clear();
        QMessageBox::information(this, "Выход", "Вы вышли из аккаунта");
    } else {
        showLoginDialog();
    }
}

void MainWindow::switchToShop()
{
    m_mainStack->setCurrentIndex(0);
}

void MainWindow::switchToAdmin()
{
    if (!DataManager::instance()->isAdmin()) {
        QMessageBox::warning(this, "Доступ запрещён", "У вас нет прав администратора");
        return;
    }
    m_mainStack->setCurrentIndex(1);
}

void MainWindow::updateUserUI()
{
    User *user = DataManager::instance()->currentUser();

    if (user) {
        m_userLabel->setText(QString("👤 %1 (%2)")
                                 .arg(user->username())
                                 .arg(user->roleString()));
        m_userButton->setText("🚪 Выйти");
        m_adminButton->setVisible(user->isAdmin());
        m_cartButton->setEnabled(true);
    } else {
        m_userLabel->setText("Гость");
        m_userButton->setText("👤 Войти");
        m_adminButton->hide();
        m_cartButton->setEnabled(false);
    }

    // Переключаемся на магазин при смене пользователя
    m_mainStack->setCurrentIndex(0);
}
