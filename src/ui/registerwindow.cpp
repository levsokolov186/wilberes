#include "registerwindow.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QRegularExpression>

RegisterWindow::RegisterWindow(Database* db, QWidget *parent)
    : QWidget(parent)
    , m_database(db) 
{
    setupUI();
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
    QLabel* logoLabel = new QLabel("MarketPlace");
    logoLabel->setObjectName("logoLabel");
    logoLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(logoLabel);

    // Заголовок
    QLabel* titleLabel = new QLabel("Create Account");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(titleLabel);

    // Имя пользователя
    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setPlaceholderText("Choose a username");
    m_usernameEdit->setObjectName("inputField");
    formLayout->addWidget(m_usernameEdit);

    // Электронная почта
    m_emailEdit = new QLineEdit;
    m_emailEdit->setPlaceholderText("Email");
    m_emailEdit->setObjectName("inputField");
    formLayout->addWidget(m_emailEdit);

    // Пароль
    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setPlaceholderText("Create a password");
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setObjectName("inputField");
    formLayout->addWidget(m_passwordEdit);

    // Подтверждение пароля
    m_confirmPasswordEdit = new QLineEdit;
    m_confirmPasswordEdit->setPlaceholderText("Confirm password");
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setObjectName("inputField");
    formLayout->addWidget(m_confirmPasswordEdit);

    // Чекбокс администратора
    m_adminCheckBox = new QCheckBox("I have an administrator code");
    m_adminCheckBox->setObjectName("adminCheckBox");
    formLayout->addWidget(m_adminCheckBox);

    // Поле кода администратора (скрыто по умолчанию)
    m_adminCodeEdit = new QLineEdit;
    m_adminCodeEdit->setPlaceholderText("Enter secret code");
    m_adminCodeEdit->setObjectName("inputField");
    m_adminCodeEdit->setEchoMode(QLineEdit::Password);
    m_adminCodeEdit->hide();
    formLayout->addWidget(m_adminCodeEdit);

    // Метка ошибки
    m_errorLabel = new QLabel;
    m_errorLabel->setObjectName("errorLabel");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->hide();
    formLayout->addWidget(m_errorLabel);

    // Кнопка регистрации
    m_registerButton = new QPushButton("Register");
    m_registerButton->setObjectName("primaryButton");
    m_registerButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_registerButton);

    // Кнопка назад
    m_backButton = new QPushButton("Back to Sign In");
    m_backButton->setObjectName("secondaryButton");
    m_backButton->setCursor(Qt::PointingHandCursor);
    formLayout->addWidget(m_backButton);

    // Эффект тени
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 10);
    formContainer->setGraphicsEffect(shadow);

    mainLayout->addWidget(formContainer);

    // Соединение сигналов
    connect(m_registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterClicked);
    connect(m_backButton, &QPushButton::clicked, this, &RegisterWindow::backToLogin);
    connect(m_adminCheckBox, &QCheckBox::stateChanged, this, &RegisterWindow::onAdminCheckChanged);
    
    // Навигация с помощью клавиши Enter
    connect(m_usernameEdit, &QLineEdit::returnPressed, [this]() {
        m_emailEdit->setFocus();
    });
    connect(m_emailEdit, &QLineEdit::returnPressed, [this]() {
        m_passwordEdit->setFocus();
    });
    connect(m_passwordEdit, &QLineEdit::returnPressed, [this]() {
        m_confirmPasswordEdit->setFocus();
    });
    connect(m_confirmPasswordEdit, &QLineEdit::returnPressed, this, &RegisterWindow::onRegisterClicked);
}

void RegisterWindow::applyStyles() {
    // Стили теперь применяются глобально через ThemeManager
}

void RegisterWindow::onAdminCheckChanged(int state) {
    m_adminCodeEdit->setVisible(state == Qt::Checked);
    if (state != Qt::Checked) {
        m_adminCodeEdit->clear();
    }
}

bool RegisterWindow::validateInput() {
    const QString username = m_usernameEdit->text().trimmed();
    const QString email = m_emailEdit->text().trimmed();
    const QString password = m_passwordEdit->text();
    const QString confirmPassword = m_confirmPasswordEdit->text();

    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("Please fill in all required fields");
        m_errorLabel->show();
        return false;
    }

    if (username.length() < 3) {
        m_errorLabel->setText("Username must be at least 3 characters");
        m_errorLabel->show();
        m_usernameEdit->setFocus();
        return false;
    }

    // Валидация имени пользователя - только буквы, цифры и подчеркивание
    static const QRegularExpression usernameRegex("^[a-zA-Z0-9_]+$");
    if (!usernameRegex.match(username).hasMatch()) {
        m_errorLabel->setText("Username can only contain letters, numbers and underscores");
        m_errorLabel->show();
        m_usernameEdit->setFocus();
        return false;
    }

    static const QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!emailRegex.match(email).hasMatch()) {
        m_errorLabel->setText("Please enter a valid email address");
        m_errorLabel->show();
        m_emailEdit->setFocus();
        return false;
    }

    if (password.length() < 4) {
        m_errorLabel->setText("Password must be at least 4 characters");
        m_errorLabel->show();
        m_passwordEdit->setFocus();
        return false;
    }

    if (password != confirmPassword) {
        m_errorLabel->setText("Passwords do not match");
        m_errorLabel->show();
        m_confirmPasswordEdit->clear();
        m_confirmPasswordEdit->setFocus();
        return false;
    }

    return true;
}

void RegisterWindow::onRegisterClicked() {
    if (!validateInput()) {
        return;
    }

    QString adminCode;
    if (m_adminCheckBox->isChecked()) {
        adminCode = m_adminCodeEdit->text();
        if (!Database::isValidAdminCode(adminCode.toStdString())) {
            m_errorLabel->setText("Invalid administrator code");
            m_errorLabel->show();
            m_adminCodeEdit->clear();
            m_adminCodeEdit->setFocus();
            return;
        }
    }

    const bool success = m_database->registerUser(
        m_usernameEdit->text().trimmed().toStdString(),
        m_passwordEdit->text().toStdString(),
        m_emailEdit->text().trimmed().toStdString(),
        adminCode.toStdString()
    );

    if (success) {
        m_errorLabel->hide();
        clearForm();
        emit registerSuccess();
    } else {
        m_errorLabel->setText("User with this username or email already exists");
        m_errorLabel->show();
    }
}

void RegisterWindow::clearForm() {
    m_usernameEdit->clear();
    m_emailEdit->clear();
    m_passwordEdit->clear();
    m_confirmPasswordEdit->clear();
    m_adminCodeEdit->clear();
    m_adminCheckBox->setChecked(false);
    m_errorLabel->hide();
}