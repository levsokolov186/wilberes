#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>

LoginWindow::LoginWindow(Database* db, QWidget *parent)
    : QWidget(parent), m_database(db) {
    setupUI();
    applyStyles();
}

void LoginWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Контейнер формы
    QWidget* formContainer = new QWidget;
    formContainer->setObjectName("formContainer");
    formContainer->setFixedWidth(400);

    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setSpacing(20);
    formLayout->setContentsMargins(40, 40, 40, 40);

    // Логотип
    QLabel* logoLabel = new QLabel("🛒 MarketPlace");
    logoLabel->setObjectName("logoLabel");
    logoLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(logoLabel);

    // Заголовок
    QLabel* titleLabel = new QLabel("Вход в аккаунт");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);

    // Поле логина
    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText("Логин или email");
    m_usernameEdit->setObjectName("inputField");
    formLayout->addWidget(m_usernameEdit);

    // Поле пароля
    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setPlaceholderText("Пароль");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setObjectName("inputField");
    formLayout->addWidget(m_passwordEdit);

    // Ошибка
    m_errorLabel = new QLabel;
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->hide();
    formLayout->addWidget(m_errorLabel);

    // Кнопка входа
    m_loginButton = new QPushButton("Войти");
    m_loginButton->setObjectName("primaryButton");
    m_loginButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_loginButton);

    // Разделитель
    QLabel* dividerLabel = new QLabel("или");
    dividerLabel->setAlignment(Qt::AlignCenter);
    dividerLabel->setObjectName("subtitleLabel");
    formLayout->addWidget(dividerLabel);

    // Кнопка регистрации
    m_registerButton = new QPushButton("Создать аккаунт");
    m_registerButton->setObjectName("secondaryButton");
    m_registerButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_registerButton);

    // Тень для контейнера
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 10);
    formContainer->setGraphicsEffect(shadow);

    mainLayout->addWidget(formContainer);

    // Подключение сигналов
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::registerClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
}

void LoginWindow::applyStyles() {
    // Стили теперь применяются глобально через ThemeManager
    // Здесь можно оставить только минимальные локальные стили при необходимости
}

void LoginWindow::onLoginClicked() {
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("Заполните все поля");
        m_errorLabel->show();
        return;
    }

    User* user = m_database->authenticateUser(
        username.toStdString(),
        password.toStdString()
        );

    if (user) {
        m_errorLabel->hide();
        emit loginSuccess(user);
    } else {
        m_errorLabel->setText("Неверный логин или пароль");
        m_errorLabel->show();
        m_passwordEdit->clear();
    }
}