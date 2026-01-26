#include "registerwindow.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QRegularExpression>

RegisterWindow::RegisterWindow(Database* db, QWidget *parent)
    : QWidget(parent), m_database(db) {
    setupUI();
    applyStyles();
}

void RegisterWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget* formContainer = new QWidget;
    formContainer->setObjectName("formContainer");
    formContainer->setFixedWidth(450);

    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setSpacing(15);
    formLayout->setContentsMargins(40, 30, 40, 30);

    // Логотип
    QLabel* logoLabel = new QLabel("🛒 MarketPlace");
    logoLabel->setObjectName("logoLabel");
    logoLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(logoLabel);

    // Заголовок
    QLabel* titleLabel = new QLabel("Регистрация");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);

    // Логин
    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText("Придумайте логин");
    m_usernameEdit->setObjectName("inputField");
    formLayout->addWidget(m_usernameEdit);

    // Email
    m_emailEdit = new QLineEdit;
    m_emailEdit->setPlaceholderText("Email");
    m_emailEdit->setObjectName("inputField");
    formLayout->addWidget(m_emailEdit);

    // Пароль
    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setPlaceholderText("Придумайте пароль");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setObjectName("inputField");
    formLayout->addWidget(m_passwordEdit);

    // Подтверждение пароля
    m_confirmPasswordEdit = new QLineEdit;
    m_confirmPasswordEdit->setPlaceholderText("Повторите пароль");
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setObjectName("inputField");
    formLayout->addWidget(m_confirmPasswordEdit);

    // Чекбокс для админа
    m_adminCheckBox = new QCheckBox("У меня есть код администратора");
    m_adminCheckBox->setObjectName("adminCheckBox");
    formLayout->addWidget(m_adminCheckBox);

    // Поле для кода админа (скрыто по умолчанию)
    m_adminCodeEdit = new QLineEdit;
    m_adminCodeEdit->setPlaceholderText("Введите секретный код");
    m_adminCodeEdit->setObjectName("inputField");
    m_adminCodeEdit->setEchoMode(QLineEdit::Password);
    m_adminCodeEdit->hide();
    formLayout->addWidget(m_adminCodeEdit);

    // Ошибка
    m_errorLabel = new QLabel;
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->hide();
    formLayout->addWidget(m_errorLabel);

    // Кнопка регистрации
    m_registerButton = new QPushButton("Зарегистрироваться");
    m_registerButton->setObjectName("primaryButton");
    m_registerButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_registerButton);

    // Кнопка назад
    m_backButton = new QPushButton("← Вернуться к входу");
    m_backButton->setObjectName("secondaryButton");
    m_backButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_backButton);

    // Тень
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 10);
    formContainer->setGraphicsEffect(shadow);

    mainLayout->addWidget(formContainer);

    // Подключение сигналов
    connect(m_registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
    connect(m_backButton, &QPushButton::clicked, this, &RegisterWindow::backToLogin);
    connect(m_adminCheckBox, &QCheckBox::stateChanged, this, &RegisterWindow::onAdminCheckChanged);
}

void RegisterWindow::applyStyles() {
    // Стили теперь применяются глобально через ThemeManager
    // Здесь можно оставить только минимальные локальные стили при необходимости
}

void RegisterWindow::onAdminCheckChanged(int state) {
    m_adminCodeEdit->setVisible(state == Qt::Checked);
}

bool RegisterWindow::validateInput() {
    QString username = m_usernameEdit->text().trimmed();
    QString email = m_emailEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();

    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("Заполните все обязательные поля");
        m_errorLabel->show();
        return false;
    }

    if (username.length() < 3) {
        m_errorLabel->setText("Логин должен содержать минимум 3 символа");
        m_errorLabel->show();
        return false;
    }

    QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!emailRegex.match(email).hasMatch()) {
        m_errorLabel->setText("Введите корректный email адрес");
        m_errorLabel->show();
        return false;
    }

    if (password.length() < 4) {
        m_errorLabel->setText("Пароль должен содержать минимум 4 символа");
        m_errorLabel->show();
        return false;
    }

    if (password != confirmPassword) {
        m_errorLabel->setText("Пароли не совпадают");
        m_errorLabel->show();
        return false;
    }

    return true;
}

void RegisterWindow::onRegisterClicked() {
    if (!validateInput()) return;

    QString adminCode = "";
    if (m_adminCheckBox->isChecked()) {
        adminCode = m_adminCodeEdit->text();
        if (!Database::isValidAdminCode(adminCode.toStdString())) {
            m_errorLabel->setText("Неверный код администратора");
            m_errorLabel->show();
            return;
        }
    }

    bool success = m_database->registerUser(
        m_usernameEdit->text().trimmed().toStdString(),
        m_passwordEdit->text().toStdString(),
        m_emailEdit->text().trimmed().toStdString(),
        adminCode.toStdString()
        );

    if (success) {
        m_errorLabel->hide();
        emit registerSuccess();
    } else {
        m_errorLabel->setText("Пользователь с таким логином или email уже существует");
        m_errorLabel->show();
    }
}