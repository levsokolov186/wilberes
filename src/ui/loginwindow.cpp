#include "loginwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>

LoginWindow::LoginWindow(Database* db, QWidget *parent)
    : QWidget(parent)
    , m_database(db) 
{
    setupUI();
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
    QLabel* logoLabel = new QLabel("MarketPlace");
    logoLabel->setObjectName("logoLabel");
    logoLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(logoLabel);

    // Заголовок
    QLabel* titleLabel = new QLabel("Sign In");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);

    // Поле имени пользователя
    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText("Username or email");
    m_usernameEdit->setObjectName("inputField");
    formLayout->addWidget(m_usernameEdit);

    // Поле пароля
    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setPlaceholderText("Password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setObjectName("inputField");
    formLayout->addWidget(m_passwordEdit);

    // Метка ошибки
    m_errorLabel = new QLabel;
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->hide();
    formLayout->addWidget(m_errorLabel);

    // Кнопка входа
    m_loginButton = new QPushButton("Sign In");
    m_loginButton->setObjectName("primaryButton");
    m_loginButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_loginButton);

    // Разделитель
    QLabel* dividerLabel = new QLabel("or");
    dividerLabel->setAlignment(Qt::AlignCenter);
    dividerLabel->setObjectName("subtitleLabel");
    formLayout->addWidget(dividerLabel);

    // Кнопка регистрации
    m_registerButton = new QPushButton("Create Account");
    m_registerButton->setObjectName("secondaryButton");
    m_registerButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_registerButton);

    // Эффект тени для контейнера
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 10);
    formContainer->setGraphicsEffect(shadow);

    mainLayout->addWidget(formContainer);

    // Соединение сигналов
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWindow::registerClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLoginClicked);
    connect(m_usernameEdit, &QLineEdit::returnPressed, [this]() {
        m_passwordEdit->setFocus();
    });
}

void LoginWindow::applyStyles() {
    // Стили теперь применяются глобально через ThemeManager
}

void LoginWindow::onLoginClicked() {
    const QString username = m_usernameEdit->text().trimmed();
    const QString password = m_passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("Please fill in all fields");
        m_errorLabel->show();
        return;
    }

    User* user = m_database->authenticateUser(
        username.toStdString(),
        password.toStdString()
    );

    if (user) {
        m_errorLabel->hide();
        m_usernameEdit->clear();
        m_passwordEdit->clear();
        emit loginSuccess(user);
    } else {
        m_errorLabel->setText("Invalid username or password");
        m_errorLabel->show();
        m_passwordEdit->clear();
        m_passwordEdit->setFocus();
    }
}