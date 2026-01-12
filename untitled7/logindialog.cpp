#include "logindialog.h"
#include "datamanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), m_loggedInUser(nullptr), m_isGuest(false)
{
    setWindowTitle("Вход в WildBerries");
    setFixedSize(450, 550);
    setStyleSheet("background-color: #f5f5f5;");
    setupUI();
}

void LoginDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // Логотип
    QLabel *logo = new QLabel("🛍 WildBerries");
    logo->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #cb11ab; "
        "padding: 20px; background: white; border-radius: 10px;");
    logo->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(logo);

    mainLayout->addSpacing(20);

    // Стек страниц
    m_stackedWidget = new QStackedWidget;
    m_stackedWidget->setStyleSheet("background: white; border-radius: 15px;");

    // Создаём страницы
    QWidget *loginPage = new QWidget;
    QWidget *registerPage = new QWidget;

    createLoginPage();
    createRegisterPage();

    m_stackedWidget->addWidget(m_stackedWidget->findChild<QWidget*>("loginPage"));
    m_stackedWidget->addWidget(m_stackedWidget->findChild<QWidget*>("registerPage"));

    mainLayout->addWidget(m_stackedWidget, 1);
}

void LoginDialog::createLoginPage()
{
    QWidget *page = new QWidget;
    page->setObjectName("loginPage");
    page->setParent(m_stackedWidget);

    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(15);

    QLabel *title = new QLabel("Вход в аккаунт");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #333;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    layout->addSpacing(10);

    // Поле логина
    QLabel *userLabel = new QLabel("Имя пользователя");
    userLabel->setStyleSheet("font-size: 14px; color: #666;");
    layout->addWidget(userLabel);

    m_loginUsername = new QLineEdit;
    m_loginUsername->setPlaceholderText("Введите логин");
    m_loginUsername->setStyleSheet(
        "QLineEdit { padding: 12px; border: 2px solid #ddd; border-radius: 8px; "
        "font-size: 14px; }"
        "QLineEdit:focus { border-color: #cb11ab; }");
    layout->addWidget(m_loginUsername);

    // Поле пароля
    QLabel *passLabel = new QLabel("Пароль");
    passLabel->setStyleSheet("font-size: 14px; color: #666;");
    layout->addWidget(passLabel);

    m_loginPassword = new QLineEdit;
    m_loginPassword->setPlaceholderText("Введите пароль");
    m_loginPassword->setEchoMode(QLineEdit::Password);
    m_loginPassword->setStyleSheet(
        "QLineEdit { padding: 12px; border: 2px solid #ddd; border-radius: 8px; "
        "font-size: 14px; }"
        "QLineEdit:focus { border-color: #cb11ab; }");
    layout->addWidget(m_loginPassword);

    // Ошибка
    m_loginError = new QLabel;
    m_loginError->setStyleSheet("color: #f44336; font-size: 12px;");
    m_loginError->setWordWrap(true);
    m_loginError->hide();
    layout->addWidget(m_loginError);

    layout->addSpacing(10);

    // Кнопка входа
    QPushButton *loginBtn = new QPushButton("Войти");
    loginBtn->setStyleSheet(
        "QPushButton { background-color: #cb11ab; color: white; "
        "border: none; padding: 14px; border-radius: 8px; "
        "font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #a00e8a; }"
        "QPushButton:pressed { background-color: #8a0c75; }");
    connect(loginBtn, &QPushButton::clicked, this, &LoginDialog::onLogin);
    layout->addWidget(loginBtn);

    // Кнопка гостя
    QPushButton *guestBtn = new QPushButton("Войти как гость");
    guestBtn->setStyleSheet(
        "QPushButton { background-color: #f0f0f0; color: #333; "
        "border: none; padding: 12px; border-radius: 8px; "
        "font-size: 14px; }"
        "QPushButton:hover { background-color: #e0e0e0; }");
    connect(guestBtn, &QPushButton::clicked, this, &LoginDialog::onGuestLogin);
    layout->addWidget(guestBtn);

    layout->addStretch();

    // Переход на регистрацию
    QHBoxLayout *switchLayout = new QHBoxLayout;
    QLabel *switchLabel = new QLabel("Нет аккаунта?");
    switchLabel->setStyleSheet("color: #666;");
    switchLayout->addWidget(switchLabel);

    QPushButton *switchBtn = new QPushButton("Зарегистрироваться");
    switchBtn->setStyleSheet(
        "QPushButton { background: transparent; color: #cb11ab; "
        "border: none; font-weight: bold; text-decoration: underline; }"
        "QPushButton:hover { color: #a00e8a; }");
    connect(switchBtn, &QPushButton::clicked, this, &LoginDialog::switchToRegister);
    switchLayout->addWidget(switchBtn);
    switchLayout->addStretch();

    layout->addLayout(switchLayout);

    // Подсказка
    QLabel *hint = new QLabel("👤 admin / admin123\n👤 user / user123");
    hint->setStyleSheet("color: #999; font-size: 11px; padding: 10px; "
                        "background: #f9f9f9; border-radius: 5px;");
    hint->setAlignment(Qt::AlignCenter);
    layout->addWidget(hint);
}

void LoginDialog::createRegisterPage()
{
    QWidget *page = new QWidget;
    page->setObjectName("registerPage");
    page->setParent(m_stackedWidget);

    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(10);

    QLabel *title = new QLabel("Регистрация");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #333;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // Имя пользователя
    m_regUsername = new QLineEdit;
    m_regUsername->setPlaceholderText("Имя пользователя");
    m_regUsername->setStyleSheet(
        "QLineEdit { padding: 10px; border: 2px solid #ddd; border-radius: 8px; }"
        "QLineEdit:focus { border-color: #cb11ab; }");
    layout->addWidget(m_regUsername);

    // Email
    m_regEmail = new QLineEdit;
    m_regEmail->setPlaceholderText("Email");
    m_regEmail->setStyleSheet(
        "QLineEdit { padding: 10px; border: 2px solid #ddd; border-radius: 8px; }"
        "QLineEdit:focus { border-color: #cb11ab; }");
    layout->addWidget(m_regEmail);

    // Пароль
    m_regPassword = new QLineEdit;
    m_regPassword->setPlaceholderText("Пароль");
    m_regPassword->setEchoMode(QLineEdit::Password);
    m_regPassword->setStyleSheet(
        "QLineEdit { padding: 10px; border: 2px solid #ddd; border-radius: 8px; }"
        "QLineEdit:focus { border-color: #cb11ab; }");
    layout->addWidget(m_regPassword);

    // Подтверждение пароля
    m_regPasswordConfirm = new QLineEdit;
    m_regPasswordConfirm->setPlaceholderText("Подтвердите пароль");
    m_regPasswordConfirm->setEchoMode(QLineEdit::Password);
    m_regPasswordConfirm->setStyleSheet(
        "QLineEdit { padding: 10px; border: 2px solid #ddd; border-radius: 8px; }"
        "QLineEdit:focus { border-color: #cb11ab; }");
    layout->addWidget(m_regPasswordConfirm);

    // Чекбокс администратора
    m_regAdminCheck = new QCheckBox("Регистрация как администратор");
    m_regAdminCheck->setStyleSheet("color: #666;");
    connect(m_regAdminCheck, &QCheckBox::toggled, [this](bool checked) {
        m_regAdminCode->setVisible(checked);
    });
    layout->addWidget(m_regAdminCheck);

    // Код администратора
    m_regAdminCode = new QLineEdit;
    m_regAdminCode->setPlaceholderText("Код администратора");
    m_regAdminCode->setEchoMode(QLineEdit::Password);
    m_regAdminCode->setStyleSheet(
        "QLineEdit { padding: 10px; border: 2px solid #ddd; border-radius: 8px; }"
        "QLineEdit:focus { border-color: #cb11ab; }");
    m_regAdminCode->hide();
    layout->addWidget(m_regAdminCode);

    // Ошибка
    m_regError = new QLabel;
    m_regError->setStyleSheet("color: #f44336; font-size: 12px;");
    m_regError->setWordWrap(true);
    m_regError->hide();
    layout->addWidget(m_regError);

    // Кнопка регистрации
    QPushButton *registerBtn = new QPushButton("Зарегистрироваться");
    registerBtn->setStyleSheet(
        "QPushButton { background-color: #cb11ab; color: white; "
        "border: none; padding: 14px; border-radius: 8px; "
        "font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #a00e8a; }");
    connect(registerBtn, &QPushButton::clicked, this, &LoginDialog::onRegister);
    layout->addWidget(registerBtn);

    layout->addStretch();

    // Переход на вход
    QHBoxLayout *switchLayout = new QHBoxLayout;
    QLabel *switchLabel = new QLabel("Уже есть аккаунт?");
    switchLabel->setStyleSheet("color: #666;");
    switchLayout->addWidget(switchLabel);

    QPushButton *switchBtn = new QPushButton("Войти");
    switchBtn->setStyleSheet(
        "QPushButton { background: transparent; color: #cb11ab; "
        "border: none; font-weight: bold; text-decoration: underline; }");
    connect(switchBtn, &QPushButton::clicked, this, &LoginDialog::switchToLogin);
    switchLayout->addWidget(switchBtn);
    switchLayout->addStretch();

    layout->addLayout(switchLayout);

    // Подсказка для кода админа
    QLabel *adminHint = new QLabel("🔐 Код админа: ADMIN2024");
    adminHint->setStyleSheet("color: #999; font-size: 11px;");
    layout->addWidget(adminHint);
}

void LoginDialog::onLogin()
{
    QString username = m_loginUsername->text().trimmed();
    QString password = m_loginPassword->text();

    if (username.isEmpty() || password.isEmpty()) {
        m_loginError->setText("Заполните все поля");
        m_loginError->show();
        return;
    }

    User *user = DataManager::instance()->login(username, password);
    if (user) {
        m_loggedInUser = user;
        accept();
    } else {
        m_loginError->setText("Неверный логин или пароль");
        m_loginError->show();
    }
}

void LoginDialog::onRegister()
{
    QString username = m_regUsername->text().trimmed();
    QString email = m_regEmail->text().trimmed();
    QString password = m_regPassword->text();
    QString passwordConfirm = m_regPasswordConfirm->text();

    // Валидация
    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        m_regError->setText("Заполните все обязательные поля");
        m_regError->show();
        return;
    }

    if (username.length() < 3) {
        m_regError->setText("Имя пользователя должно быть не менее 3 символов");
        m_regError->show();
        return;
    }

    if (password.length() < 6) {
        m_regError->setText("Пароль должен быть не менее 6 символов");
        m_regError->show();
        return;
    }

    if (password != passwordConfirm) {
        m_regError->setText("Пароли не совпадают");
        m_regError->show();
        return;
    }

    if (!email.contains("@")) {
        m_regError->setText("Введите корректный email");
        m_regError->show();
        return;
    }

    UserRole role = UserRole::Customer;
    if (m_regAdminCheck->isChecked()) {
        if (m_regAdminCode->text() != "ADMIN2024") {
            m_regError->setText("Неверный код администратора");
            m_regError->show();
            return;
        }
        role = UserRole::Admin;
    }

    if (DataManager::instance()->registerUser(username, password, email, role)) {
        User *user = DataManager::instance()->login(username, password);
        if (user) {
            m_loggedInUser = user;
            accept();
        }
    } else {
        m_regError->setText("Пользователь с таким именем или email уже существует");
        m_regError->show();
    }
}

void LoginDialog::onGuestLogin()
{
    m_isGuest = true;
    accept();
}

void LoginDialog::switchToLogin()
{
    m_stackedWidget->setCurrentIndex(0);
    m_regError->hide();
}

void LoginDialog::switchToRegister()
{
    m_stackedWidget->setCurrentIndex(1);
    m_loginError->hide();
}

void LoginDialog::showError(const QString &message)
{
    if (m_stackedWidget->currentIndex() == 0) {
        m_loginError->setText(message);
        m_loginError->show();
    } else {
        m_regError->setText(message);
        m_regError->show();
    }
}

void LoginDialog::showSuccess(const QString &message)
{
    QMessageBox::information(this, "Успех", message);
}
