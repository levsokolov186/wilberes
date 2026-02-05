#include "mainwindow.h"
#include "productcard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QScrollBar>
#include <QFrame>
#include <QGroupBox>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_database(nullptr)
    , m_currentUser(nullptr)
    , m_cart(nullptr)
    , m_stackedWidget(nullptr)
    , m_loginWindow(nullptr)
    , m_registerWindow(nullptr)
    , m_mainPage(nullptr)
    , m_cartWidget(nullptr)
    , m_profileWidget(nullptr)
    , m_adminPanel(nullptr)
    , m_settingsPage(nullptr)
    , m_cartPageWrapper(nullptr)
    , m_profilePageWrapper(nullptr)
    , m_settingsPageWrapper(nullptr)
    , m_adminPageWrapper(nullptr)
    , m_header(nullptr)
    , m_searchEdit(nullptr)
    , m_categoryList(nullptr)
    , m_productsGrid(nullptr)
    , m_productsScrollArea(nullptr)
    , m_cartButton(nullptr)
    , m_cartBadge(nullptr)
    , m_profileButton(nullptr)
    , m_adminButton(nullptr)
    , m_settingsButton(nullptr)
    , m_userLabel(nullptr)
    , m_sidebarCatalogBtn(nullptr)
    , m_sidebarCartBtn(nullptr)
    , m_sidebarProfileBtn(nullptr)
    , m_sidebarSettingsBtn(nullptr)
    , m_sidebarAdminBtn(nullptr)
    , m_sidebarButtonGroup(nullptr)
    , m_themeComboBox(nullptr)
{
    m_database = new Database("data/");

    // Удаление стандартного меню и строки состояния
    setMenuBar(nullptr);
    setStatusBar(nullptr);

    setupUI();
    setWindowTitle("MarketPlace");
    setMinimumSize(1200, 800);
    showMaximized();
}

MainWindow::~MainWindow() {
    delete m_database;
    delete m_cart;
}

void MainWindow::setupUI() {
    m_stackedWidget = new QStackedWidget;
    setCentralWidget(m_stackedWidget);

    // Страница входа
    m_loginWindow = new LoginWindow(m_database);
    m_stackedWidget->addWidget(m_loginWindow);

    // Страница регистрации
    m_registerWindow = new RegisterWindow(m_database);
    m_stackedWidget->addWidget(m_registerWindow);

    // Соединение сигналов аутентификации
    connect(m_loginWindow, &LoginWindow::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(m_loginWindow, &LoginWindow::registerClicked, this, &MainWindow::showRegister);
    connect(m_registerWindow, &RegisterWindow::registerSuccess, this, &MainWindow::onRegisterSuccess);
    connect(m_registerWindow, &RegisterWindow::backToLogin, this, &MainWindow::showLogin);

    // Соединение сигнала смены темы
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &MainWindow::onThemeApplied);
}

void MainWindow::setupMainUI() {
    m_mainPage = new QWidget;
    m_mainPage->setObjectName("mainPage");

    QHBoxLayout* rootLayout = new QHBoxLayout(m_mainPage);
    rootLayout->setSpacing(0);
    rootLayout->setContentsMargins(0, 0, 0, 0);

    // =====================
    // === LEFT SIDEBAR ===
    // =====================
    QWidget* sidebarContainer = new QWidget;
    sidebarContainer->setObjectName("sidebarContainer");
    sidebarContainer->setFixedWidth(280);
    
    QVBoxLayout* sidebarContainerLayout = new QVBoxLayout(sidebarContainer);
    sidebarContainerLayout->setContentsMargins(12, 12, 0, 12);
    sidebarContainerLayout->setSpacing(0);
    
    QWidget* sidebar = new QWidget;
    sidebar->setObjectName("sidebar");

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setSpacing(4);
    sidebarLayout->setContentsMargins(16, 24, 16, 24);

    // Логотип
    QLabel* logoLabel = new QLabel("MarketPlace");
    logoLabel->setObjectName("sidebarLogo");
    sidebarLayout->addWidget(logoLabel);

    // Информация о пользователе
    m_userLabel = new QLabel;
    m_userLabel->setObjectName("userInfoLabel");
    sidebarLayout->addWidget(m_userLabel);

    sidebarLayout->addSpacing(20);

    // Разделитель
    QFrame* sep1 = new QFrame;
    sep1->setObjectName("sidebarSeparator");
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFixedHeight(1);
    sidebarLayout->addWidget(sep1);

    sidebarLayout->addSpacing(20);

    // Группа навигационных кнопок
    m_sidebarButtonGroup = new QButtonGroup(this);
    m_sidebarButtonGroup->setExclusive(true);

    // Кнопка каталога
    m_sidebarCatalogBtn = new QPushButton("  Catalog");
    m_sidebarCatalogBtn->setObjectName("sidebarButton");
    m_sidebarCatalogBtn->setCheckable(true);
    m_sidebarCatalogBtn->setChecked(true);
    m_sidebarCatalogBtn->setCursor(Qt::PointingHandCursor);
    m_sidebarButtonGroup->addButton(m_sidebarCatalogBtn, 0);
    sidebarLayout->addWidget(m_sidebarCatalogBtn);
    connect(m_sidebarCatalogBtn, &QPushButton::clicked, this, &MainWindow::showCatalog);

    // Кнопка корзины
    m_sidebarCartBtn = new QPushButton("  Cart");
    m_sidebarCartBtn->setObjectName("sidebarButton");
    m_sidebarCartBtn->setCheckable(true);
    m_sidebarCartBtn->setCursor(Qt::PointingHandCursor);
    m_sidebarButtonGroup->addButton(m_sidebarCartBtn, 1);
    sidebarLayout->addWidget(m_sidebarCartBtn);
    connect(m_sidebarCartBtn, &QPushButton::clicked, this, &MainWindow::showCart);

    // Кнопка профиля
    m_sidebarProfileBtn = new QPushButton("  Profile");
    m_sidebarProfileBtn->setObjectName("sidebarButton");
    m_sidebarProfileBtn->setCheckable(true);
    m_sidebarProfileBtn->setCursor(Qt::PointingHandCursor);
    m_sidebarButtonGroup->addButton(m_sidebarProfileBtn, 2);
    sidebarLayout->addWidget(m_sidebarProfileBtn);
    connect(m_sidebarProfileBtn, &QPushButton::clicked, this, &MainWindow::showProfile);

    // Кнопка настроек
    m_sidebarSettingsBtn = new QPushButton("  Settings");
    m_sidebarSettingsBtn->setObjectName("sidebarButton");
    m_sidebarSettingsBtn->setCheckable(true);
    m_sidebarSettingsBtn->setCursor(Qt::PointingHandCursor);
    m_sidebarButtonGroup->addButton(m_sidebarSettingsBtn, 3);
    sidebarLayout->addWidget(m_sidebarSettingsBtn);
    connect(m_sidebarSettingsBtn, &QPushButton::clicked, this, &MainWindow::showSettings);

    // Кнопка панели администратора (скрыта для обычных пользователей)
    m_sidebarAdminBtn = new QPushButton("  Admin Panel");
    m_sidebarAdminBtn->setObjectName("sidebarButton");
    m_sidebarAdminBtn->setCheckable(true);
    m_sidebarAdminBtn->setCursor(Qt::PointingHandCursor);
    m_sidebarButtonGroup->addButton(m_sidebarAdminBtn, 4);
    sidebarLayout->addWidget(m_sidebarAdminBtn);
    connect(m_sidebarAdminBtn, &QPushButton::clicked, this, &MainWindow::showAdmin);

    sidebarLayout->addStretch();

    // Разделитель перед выходом
    QFrame* sep2 = new QFrame;
    sep2->setObjectName("sidebarSeparator");
    sep2->setFrameShape(QFrame::HLine);
    sep2->setFixedHeight(1);
    sidebarLayout->addWidget(sep2);

    sidebarLayout->addSpacing(16);

    // Кнопка выхода
    QPushButton* logoutBtn = new QPushButton("  Sign Out");
    logoutBtn->setObjectName("sidebarLogoutButton");
    logoutBtn->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(logoutBtn);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::logout);

    sidebarContainerLayout->addWidget(sidebar);
    rootLayout->addWidget(sidebarContainer);

    // ========================
    // === RIGHT CONTENT ===
    // ========================
    QWidget* contentWrapper = new QWidget;
    contentWrapper->setObjectName("contentWrapper");

    QVBoxLayout* contentWrapperLayout = new QVBoxLayout(contentWrapper);
    contentWrapperLayout->setSpacing(0);
    contentWrapperLayout->setContentsMargins(0, 0, 0, 0);

    // === ШАПКА ===
    m_header = new QWidget;
    m_header->setObjectName("headerBar");
    m_header->setFixedHeight(70);

    QHBoxLayout* headerLayout = new QHBoxLayout(m_header);
    headerLayout->setContentsMargins(24, 0, 24, 0);
    headerLayout->setSpacing(16);

    // Поле поиска
    m_searchEdit = new QLineEdit;
    m_searchEdit->setObjectName("searchField");
    m_searchEdit->setPlaceholderText("Search products...");
    m_searchEdit->setMinimumWidth(300);
    m_searchEdit->setMaximumWidth(500);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearch);
    headerLayout->addWidget(m_searchEdit);

    headerLayout->addStretch();

    // Иконка баланса в шапке
    QLabel* balanceIcon = new QLabel;
    balanceIcon->setObjectName("headerBalanceIcon");
    balanceIcon->setText("$");
    headerLayout->addWidget(balanceIcon);

    // Корзина с индикатором
    QWidget* cartContainer = new QWidget;
    cartContainer->setObjectName("cartButtonContainer");
    QHBoxLayout* cartLayout = new QHBoxLayout(cartContainer);
    cartLayout->setContentsMargins(0, 0, 0, 0);
    cartLayout->setSpacing(0);

    m_cartButton = new QPushButton("Cart");
    m_cartButton->setObjectName("headerCartButton");
    m_cartButton->setCursor(Qt::PointingHandCursor);
    connect(m_cartButton, &QPushButton::clicked, this, &MainWindow::showCart);
    cartLayout->addWidget(m_cartButton);

    m_cartBadge = new QLabel("0");
    m_cartBadge->setObjectName("cartBadge");
    m_cartBadge->setFixedSize(22, 22);
    m_cartBadge->setAlignment(Qt::AlignCenter);
    m_cartBadge->hide();
    cartLayout->addWidget(m_cartBadge);

    headerLayout->addWidget(cartContainer);

    // Профиль
    m_profileButton = new QPushButton("Profile");
    m_profileButton->setObjectName("headerButton");
    m_profileButton->setCursor(Qt::PointingHandCursor);
    connect(m_profileButton, &QPushButton::clicked, this, &MainWindow::showProfile);
    headerLayout->addWidget(m_profileButton);

    // Админ
    m_adminButton = new QPushButton("Admin");
    m_adminButton->setObjectName("headerAdminButton");
    m_adminButton->setCursor(Qt::PointingHandCursor);
    connect(m_adminButton, &QPushButton::clicked, this, &MainWindow::showAdmin);
    headerLayout->addWidget(m_adminButton);

    // Настройки
    m_settingsButton = new QPushButton();
    m_settingsButton->setObjectName("headerSettingsButton");
    m_settingsButton->setText("*");
    m_settingsButton->setFixedSize(40, 40);
    m_settingsButton->setCursor(Qt::PointingHandCursor);
    m_settingsButton->setToolTip("Settings");
    connect(m_settingsButton, &QPushButton::clicked, this, &MainWindow::showSettings);
    headerLayout->addWidget(m_settingsButton);

    contentWrapperLayout->addWidget(m_header);

    // === ОСНОВНАЯ ОБЛАСТЬ КОНТЕНТА ===
    QWidget* mainContentArea = new QWidget;
    mainContentArea->setObjectName("mainContentArea");

    QHBoxLayout* mainContentLayout = new QHBoxLayout(mainContentArea);
    mainContentLayout->setSpacing(0);
    mainContentLayout->setContentsMargins(0, 0, 0, 0);

    // Боковая панель категорий
    QWidget* categorySidebar = new QWidget;
    categorySidebar->setObjectName("categorySidebar");
    categorySidebar->setFixedWidth(240);

    QVBoxLayout* categoryLayout = new QVBoxLayout(categorySidebar);
    categoryLayout->setContentsMargins(0, 0, 0, 0);
    categoryLayout->setSpacing(0);

    QLabel* categoryTitle = new QLabel("Categories");
    categoryTitle->setObjectName("categoryTitle");
    categoryLayout->addWidget(categoryTitle);

    m_categoryList = new QListWidget;
    m_categoryList->setObjectName("categoryList");
    m_categoryList->setFrameShape(QFrame::NoFrame);

    // Добавление категорий
    m_categoryList->addItem("All Products");
    for (const auto& cat : m_database->getCategories()) {
        m_categoryList->addItem(QString::fromStdString(cat));
    }
    m_categoryList->setCurrentRow(0);

    connect(m_categoryList, &QListWidget::currentTextChanged,
            this, &MainWindow::onCategorySelected);
    categoryLayout->addWidget(m_categoryList);

    mainContentLayout->addWidget(categorySidebar);

    // Область товаров
    m_productsScrollArea = new QScrollArea;
    m_productsScrollArea->setObjectName("productsScrollArea");
    m_productsScrollArea->setWidgetResizable(true);
    m_productsScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_productsScrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* productsContainer = new QWidget;
    productsContainer->setObjectName("productsContainer");
    m_productsGrid = new QGridLayout(productsContainer);
    m_productsGrid->setSpacing(24);
    m_productsGrid->setContentsMargins(24, 24, 24, 24);
    m_productsGrid->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_productsScrollArea->setWidget(productsContainer);
    mainContentLayout->addWidget(m_productsScrollArea, 1);

    contentWrapperLayout->addWidget(mainContentArea, 1);

    rootLayout->addWidget(contentWrapper, 1);

    // Добавление главной страницы в стек
    m_stackedWidget->addWidget(m_mainPage);

    // === Создание дополнительных страниц ===

    // Обертка страницы корзины
    m_cartPageWrapper = new QWidget;
    m_cartPageWrapper->setObjectName("pageWrapper");
    QVBoxLayout* cartPageLayout = new QVBoxLayout(m_cartPageWrapper);
    cartPageLayout->setContentsMargins(0, 0, 0, 0);
    cartPageLayout->setSpacing(0);
    
    QWidget* cartHeader = new QWidget;
    cartHeader->setObjectName("pageHeader");
    cartHeader->setFixedHeight(50);
    QHBoxLayout* cartHeaderLayout = new QHBoxLayout(cartHeader);
    cartHeaderLayout->setContentsMargins(20, 0, 20, 0);
    QPushButton* cartBackBtn = new QPushButton("<- Back to Catalog");
    cartBackBtn->setObjectName("backButton");
    cartBackBtn->setCursor(Qt::PointingHandCursor);
    connect(cartBackBtn, &QPushButton::clicked, this, &MainWindow::showCatalog);
    cartHeaderLayout->addWidget(cartBackBtn);
    cartHeaderLayout->addStretch();
    cartPageLayout->addWidget(cartHeader);
    
    m_cartWidget = new CartWidget(m_cart);
    connect(m_cartWidget, &CartWidget::checkoutClicked, this, &MainWindow::onCheckout);
    connect(m_cartWidget, &CartWidget::cartUpdated, this, &MainWindow::updateCartBadge);
    cartPageLayout->addWidget(m_cartWidget, 1);
    m_stackedWidget->addWidget(m_cartPageWrapper);

    // Обертка страницы профиля
    m_profilePageWrapper = new QWidget;
    m_profilePageWrapper->setObjectName("pageWrapper");
    QVBoxLayout* profilePageLayout = new QVBoxLayout(m_profilePageWrapper);
    profilePageLayout->setContentsMargins(0, 0, 0, 0);
    profilePageLayout->setSpacing(0);
    
    QWidget* profileHeader = new QWidget;
    profileHeader->setObjectName("pageHeader");
    profileHeader->setFixedHeight(50);
    QHBoxLayout* profileHeaderLayout = new QHBoxLayout(profileHeader);
    profileHeaderLayout->setContentsMargins(20, 0, 20, 0);
    QPushButton* profileBackBtn = new QPushButton("<- Back to Catalog");
    profileBackBtn->setObjectName("backButton");
    profileBackBtn->setCursor(Qt::PointingHandCursor);
    connect(profileBackBtn, &QPushButton::clicked, this, &MainWindow::showCatalog);
    profileHeaderLayout->addWidget(profileBackBtn);
    profileHeaderLayout->addStretch();
    profilePageLayout->addWidget(profileHeader);
    
    m_profileWidget = new ProfileWidget(m_currentUser, m_database);
    connect(m_profileWidget, &ProfileWidget::logoutClicked, this, &MainWindow::logout);
    connect(m_profileWidget, &ProfileWidget::balanceChanged, this, [this]() {
        if (m_currentUser) {
            m_userLabel->setText(QString("Balance: %1 $").arg(m_currentUser->getBalance(), 0, 'f', 0));
        }
    });
    profilePageLayout->addWidget(m_profileWidget, 1);
    m_stackedWidget->addWidget(m_profilePageWrapper);

    // Обертка страницы настроек
    m_settingsPageWrapper = new QWidget;
    m_settingsPageWrapper->setObjectName("pageWrapper");
    QVBoxLayout* settingsWrapperLayout = new QVBoxLayout(m_settingsPageWrapper);
    settingsWrapperLayout->setContentsMargins(0, 0, 0, 0);
    settingsWrapperLayout->setSpacing(0);
    
    QWidget* settingsHeader = new QWidget;
    settingsHeader->setObjectName("pageHeader");
    settingsHeader->setFixedHeight(50);
    QHBoxLayout* settingsHeaderLayout = new QHBoxLayout(settingsHeader);
    settingsHeaderLayout->setContentsMargins(20, 0, 20, 0);
    QPushButton* settingsBackBtn = new QPushButton("<- Back to Catalog");
    settingsBackBtn->setObjectName("backButton");
    settingsBackBtn->setCursor(Qt::PointingHandCursor);
    connect(settingsBackBtn, &QPushButton::clicked, this, &MainWindow::showCatalog);
    settingsHeaderLayout->addWidget(settingsBackBtn);
    settingsHeaderLayout->addStretch();
    settingsWrapperLayout->addWidget(settingsHeader);
    
    setupSettingsPage();
    settingsWrapperLayout->addWidget(m_settingsPage, 1);
    m_stackedWidget->addWidget(m_settingsPageWrapper);

    // Панель администратора (только для админов)
    m_adminPageWrapper = nullptr;
    if (m_currentUser && m_currentUser->isAdmin()) {
        m_adminPageWrapper = new QWidget;
        m_adminPageWrapper->setObjectName("pageWrapper");
        QVBoxLayout* adminPageLayout = new QVBoxLayout(m_adminPageWrapper);
        adminPageLayout->setContentsMargins(0, 0, 0, 0);
        adminPageLayout->setSpacing(0);
        
        QWidget* adminHeader = new QWidget;
        adminHeader->setObjectName("pageHeader");
        adminHeader->setFixedHeight(50);
        QHBoxLayout* adminHeaderLayout = new QHBoxLayout(adminHeader);
        adminHeaderLayout->setContentsMargins(20, 0, 20, 0);
        QPushButton* adminBackBtn = new QPushButton("<- Back to Catalog");
        adminBackBtn->setObjectName("backButton");
        adminBackBtn->setCursor(Qt::PointingHandCursor);
        connect(adminBackBtn, &QPushButton::clicked, this, &MainWindow::showCatalog);
        adminHeaderLayout->addWidget(adminBackBtn);
        adminHeaderLayout->addStretch();
        adminPageLayout->addWidget(adminHeader);
        
        m_adminPanel = new AdminPanel(m_database);
        connect(m_adminPanel, &AdminPanel::productsChanged, this, &MainWindow::refreshCatalog);
        adminPageLayout->addWidget(m_adminPanel, 1);
        m_stackedWidget->addWidget(m_adminPageWrapper);
        m_adminButton->setVisible(true);
        m_sidebarAdminBtn->setVisible(true);
    } else {
        m_adminButton->setVisible(false);
        m_sidebarAdminBtn->setVisible(false);
    }

    // Отображение товаров
    displayProducts(m_database->getAllProducts());
}

void MainWindow::setupSettingsPage() {
    m_settingsPage = new QWidget;
    m_settingsPage->setObjectName("settingsPage");

    QVBoxLayout* mainLayout = new QVBoxLayout(m_settingsPage);
    mainLayout->setSpacing(24);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Заголовок
    QLabel* titleLabel = new QLabel("Settings");
    titleLabel->setObjectName("pageTitle");
    mainLayout->addWidget(titleLabel);

    // Карточка настроек темы
    QWidget* themeCard = new QWidget;
    themeCard->setObjectName("settingsCard");
    themeCard->setMaximumWidth(600);

    QVBoxLayout* themeCardLayout = new QVBoxLayout(themeCard);
    themeCardLayout->setSpacing(20);
    themeCardLayout->setContentsMargins(24, 24, 24, 24);

    QLabel* themeTitle = new QLabel("Appearance");
    themeTitle->setObjectName("settingsCardTitle");
    themeCardLayout->addWidget(themeTitle);

    // Выбор темы
    QHBoxLayout* themeSelectLayout = new QHBoxLayout;
    themeSelectLayout->setSpacing(16);

    QLabel* themeLabel = new QLabel("Theme:");
    themeLabel->setObjectName("settingsLabel");
    themeSelectLayout->addWidget(themeLabel);

    m_themeComboBox = new QComboBox;
    m_themeComboBox->setObjectName("themeComboBox");
    m_themeComboBox->setMinimumWidth(200);
    m_themeComboBox->addItem("Light", static_cast<int>(ThemeType::Light));
    m_themeComboBox->addItem("Dark", static_cast<int>(ThemeType::Dark));
    m_themeComboBox->addItem("Wildberries", static_cast<int>(ThemeType::Wildberries));

    syncThemeComboBox();

    connect(m_themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onThemeChanged);

    themeSelectLayout->addWidget(m_themeComboBox);
    themeSelectLayout->addStretch();

    themeCardLayout->addLayout(themeSelectLayout);

    // Описания тем
    QLabel* themeDesc = new QLabel(
        "- Light: Classic light theme\n"
        "- Dark: Dark mode for comfortable work\n"
        "- Wildberries: Marketplace brand style"
    );
    themeDesc->setObjectName("settingsDescription");
    themeDesc->setWordWrap(true);
    themeCardLayout->addWidget(themeDesc);

    mainLayout->addWidget(themeCard);

    // Карточка "О программе"
    QWidget* aboutCard = new QWidget;
    aboutCard->setObjectName("settingsCard");
    aboutCard->setMaximumWidth(600);

    QVBoxLayout* aboutLayout = new QVBoxLayout(aboutCard);
    aboutLayout->setSpacing(12);
    aboutLayout->setContentsMargins(24, 24, 24, 24);

    QLabel* aboutTitle = new QLabel("About");
    aboutTitle->setObjectName("settingsCardTitle");
    aboutLayout->addWidget(aboutTitle);

    QLabel* aboutText = new QLabel(
        "MarketPlace v1.0\n"
        "Modern marketplace built with Qt Widgets\n"
        "C++17"
    );
    aboutText->setObjectName("settingsDescription");
    aboutLayout->addWidget(aboutText);

    mainLayout->addWidget(aboutCard);

    mainLayout->addStretch();
}

void MainWindow::syncThemeComboBox() {
    if (!m_themeComboBox) {
        return;
    }

    const ThemeType current = ThemeManager::instance().currentTheme();
    const int index = m_themeComboBox->findData(static_cast<int>(current));
    
    if (index >= 0) {
        m_themeComboBox->blockSignals(true);
        m_themeComboBox->setCurrentIndex(index);
        m_themeComboBox->blockSignals(false);
    }
}

void MainWindow::onThemeChanged(int index) {
    if (!m_themeComboBox) {
        return;
    }

    const int themeValue = m_themeComboBox->itemData(index).toInt();
    const ThemeType theme = static_cast<ThemeType>(themeValue);
    ThemeManager::instance().applyTheme(theme);
}

void MainWindow::onThemeApplied(ThemeType) {
    syncThemeComboBox();
}

void MainWindow::updateSidebarActiveButton(QPushButton* activeBtn) {
    if (m_sidebarButtonGroup && activeBtn) {
        activeBtn->setChecked(true);
    }
}

void MainWindow::onLoginSuccess(User* user) {
    m_currentUser = user;
    m_cart = new Cart(user->getId());

    setupMainUI();

    // Обновление виджетов новыми указателями на пользователя/корзину
    if (m_cartWidget) {
        m_cartWidget->setCart(m_cart);
    }
    
    if (m_profileWidget) {
        m_profileWidget->setUser(m_currentUser);
    }

    if (m_currentUser) {
        m_userLabel->setText(QString("Balance: %1 $").arg(user->getBalance(), 0, 'f', 0));
    }

    showCatalog();
}

void MainWindow::onRegisterSuccess() {
    QMessageBox::information(this, "Success", "Registration complete! You can now sign in.");
    showLogin();
}

void MainWindow::showLogin() {
    m_stackedWidget->setCurrentWidget(m_loginWindow);
}

void MainWindow::showRegister() {
    m_stackedWidget->setCurrentWidget(m_registerWindow);
}

void MainWindow::showCatalog() {
    m_stackedWidget->setCurrentWidget(m_mainPage);
    updateSidebarActiveButton(m_sidebarCatalogBtn);
}

void MainWindow::showCart() {
    if (m_cartWidget && m_cartPageWrapper) {
        m_cartWidget->updateCart();
        m_stackedWidget->setCurrentWidget(m_cartPageWrapper);
    }
    updateSidebarActiveButton(m_sidebarCartBtn);
}

void MainWindow::showProfile() {
    if (m_profileWidget && m_profilePageWrapper) {
        m_profileWidget->updateProfile();
        m_stackedWidget->setCurrentWidget(m_profilePageWrapper);
    }
    updateSidebarActiveButton(m_sidebarProfileBtn);
}

void MainWindow::showSettings() {
    syncThemeComboBox();
    if (m_settingsPageWrapper) {
        m_stackedWidget->setCurrentWidget(m_settingsPageWrapper);
    }
    updateSidebarActiveButton(m_sidebarSettingsBtn);
}

void MainWindow::showAdmin() {
    if (m_currentUser && m_currentUser->isAdmin() && m_adminPageWrapper) {
        m_stackedWidget->setCurrentWidget(m_adminPageWrapper);
        updateSidebarActiveButton(m_sidebarAdminBtn);
    }
}

void MainWindow::onCategorySelected(const QString& category) {
    if (category == "All Products" || category.isEmpty()) {
        displayProducts(m_database->getAllProducts());
    } else {
        displayProducts(m_database->getProductsByCategory(category.toStdString()));
    }
}

void MainWindow::onSearch() {
    const QString query = m_searchEdit->text().trimmed();
    if (query.isEmpty()) {
        displayProducts(m_database->getAllProducts());
    } else {
        displayProducts(m_database->searchProducts(query.toStdString()));
    }
}

void MainWindow::onAddToCart(const Product& product) {
    if (!m_cart) {
        return;
    }

    m_cart->addItem(product, 1);
    updateCartBadge();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Added");
    msgBox.setText(QString("\"%1\" added to cart").arg(QString::fromStdString(product.getName())));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

void MainWindow::onCheckout() {
    if (!m_cart || !m_currentUser) {
        return;
    }

    const double total = m_cart->getTotalPrice();

    if (m_currentUser->getBalance() < total) {
        QMessageBox::warning(this, "Insufficient Funds",
            QString("You don't have enough funds.\n\n"
                    "Required: %1 $\nYour balance: %2 $\n\n"
                    "Top up your balance in Profile.")
                .arg(total, 0, 'f', 0)
                .arg(m_currentUser->getBalance(), 0, 'f', 0));
        return;
    }

    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Checkout",
        QString("Total: %1 $\n\nDeduct from balance?").arg(total, 0, 'f', 0),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        m_currentUser->withdrawBalance(total);
        m_database->updateUser(*m_currentUser);

        const auto items = m_cart->getItems();
        for (const auto& item : items) {
            Product* p = m_database->getProductById(item.product.getId());
            if (p) {
                p->decreaseStock(item.quantity);
                m_database->updateProduct(*p);
            }
        }

        m_cart->clear();
        updateCartBadge();
        m_userLabel->setText(QString("Balance: %1 $").arg(m_currentUser->getBalance(), 0, 'f', 0));

        QMessageBox::information(this, "Order Placed",
            "Your order has been placed successfully!\nExpect delivery soon.");

        showCatalog();
        refreshCatalog();
    }
}

void MainWindow::updateCartBadge() {
    if (!m_cart || !m_cartBadge) {
        return;
    }

    const int count = m_cart->getTotalItemsCount();
    m_cartBadge->setText(QString::number(count));
    m_cartBadge->setVisible(count > 0);
}

void MainWindow::displayProducts(const std::vector<Product>& products) {
    // Очистка сетки
    QLayoutItem* item;
    while ((item = m_productsGrid->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    if (products.empty()) {
        QLabel* emptyLabel = new QLabel("No products found");
        emptyLabel->setObjectName("emptyProductsLabel");
        emptyLabel->setAlignment(Qt::AlignCenter);
        m_productsGrid->addWidget(emptyLabel, 0, 0);
        return;
    }

    int col = 0;
    int row = 0;
    const int maxCols = 4;

    for (const auto& product : products) {
        ProductCard* card = new ProductCard(product);
        connect(card, &ProductCard::addToCartClicked, this, &MainWindow::onAddToCart);

        m_productsGrid->addWidget(card, row, col);

        ++col;
        if (col >= maxCols) {
            col = 0;
            ++row;
        }
    }
}

void MainWindow::refreshCatalog() {
    QString currentCategory;
    if (m_categoryList && m_categoryList->currentItem()) {
        currentCategory = m_categoryList->currentItem()->text();
    }
    onCategorySelected(currentCategory.isEmpty() ? "All Products" : currentCategory);
}

void MainWindow::logout() {
    m_currentUser = nullptr;

    if (m_cart) {
        delete m_cart;
        m_cart = nullptr;
    }

    // Удаление всех страниц, кроме входа и регистрации
    while (m_stackedWidget->count() > 2) {
        QWidget* w = m_stackedWidget->widget(2);
        m_stackedWidget->removeWidget(w);
        w->deleteLater();
    }

    m_mainPage = nullptr;
    m_cartWidget = nullptr;
    m_profileWidget = nullptr;
    m_settingsPage = nullptr;
    m_adminPanel = nullptr;
    m_sidebarButtonGroup = nullptr;
    m_themeComboBox = nullptr;
    m_cartPageWrapper = nullptr;
    m_profilePageWrapper = nullptr;
    m_settingsPageWrapper = nullptr;
    m_adminPageWrapper = nullptr;

    showLogin();
}