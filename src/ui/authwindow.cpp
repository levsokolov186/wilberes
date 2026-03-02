#include "pch.h"
#include "authwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QRegularExpression>

AuthWindow::AuthWindow(Database* db, QWidget *parent)
    : QWidget(parent)
    , m_database(db)
{
    setupUI();
}

void AuthWindow::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    QWidget* formContainer = new QWidget;
    formContainer->setObjectName("authContainer");
    formContainer->setFixedWidth(420);

    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setSpacing(0);
    formLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* logoLabel = new QLabel("MarketPlace");
    logoLabel->setObjectName("authLogo");
    logoLabel->setAlignment(Qt::AlignCenter);
    formLayout->addWidget(logoLabel);

    formLayout->addSpacing(20);

    m_tabWidget = new QTabWidget;
    m_tabWidget->setObjectName("authTabs");
    m_tabWidget->setDocumentMode(true);

    setupLoginTab();
    setupRegisterTab();

    formLayout->addWidget(m_tabWidget);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(40);
    shadow->setColor(QColor(0, 0, 0, 60));
    shadow->setOffset(0, 15);
    formContainer->setGraphicsEffect(shadow);

    mainLayout->addWidget(formContainer);
}

void AuthWindow::setupLoginTab() {
    QWidget* loginTab = new QWidget;
    loginTab->setObjectName("authTab");

    QVBoxLayout* tabLayout = new QVBoxLayout(loginTab);
    tabLayout->setSpacing(16);
    tabLayout->setContentsMargins(32, 24, 32, 32);

    QLabel* titleLabel = new QLabel("Welcome Back");
    titleLabel->setObjectName("authTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    tabLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("Sign in to continue");
    subtitleLabel->setObjectName("authSubtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    tabLayout->addWidget(subtitleLabel);

    tabLayout->addSpacing(8);

    m_loginUsernameEdit = new QLineEdit;
    m_loginUsernameEdit->setObjectName("authInput");
    m_loginUsernameEdit->setPlaceholderText("Username or email");
    m_loginUsernameEdit->setClearButtonEnabled(true);
    tabLayout->addWidget(m_loginUsernameEdit);

    m_loginPasswordEdit = new QLineEdit;
    m_loginPasswordEdit->setObjectName("authInput");
    m_loginPasswordEdit->setPlaceholderText("Password");
    m_loginPasswordEdit->setEchoMode(QLineEdit::Password);
    m_loginPasswordEdit->setClearButtonEnabled(true);
    tabLayout->addWidget(m_loginPasswordEdit);

    m_loginErrorLabel = new QLabel;
    m_loginErrorLabel->setObjectName("authError");
    m_loginErrorLabel->setAlignment(Qt::AlignCenter);
    m_loginErrorLabel->hide();
    tabLayout->addWidget(m_loginErrorLabel);

    QPushButton* loginButton = new QPushButton("Sign In");
    loginButton->setObjectName("authPrimaryButton");
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setDefault(true);
    tabLayout->addWidget(loginButton);

    tabLayout->addStretch();

    m_tabWidget->addTab(loginTab, "Sign In");

    connect(loginButton, &QPushButton::clicked, this, &AuthWindow::onLoginClicked);
    connect(m_loginPasswordEdit, &QLineEdit::returnPressed, this, &AuthWindow::onLoginClicked);
    connect(m_loginUsernameEdit, &QLineEdit::returnPressed, [this]() {
        m_loginPasswordEdit->setFocus();
    });
}

void AuthWindow::setupRegisterTab() {
    QWidget* registerTab = new QWidget;
    registerTab->setObjectName("authTab");

    QVBoxLayout* tabLayout = new QVBoxLayout(registerTab);
    tabLayout->setSpacing(12);
    tabLayout->setContentsMargins(32, 20, 32, 32);

    QLabel* titleLabel = new QLabel("Create Account");
    titleLabel->setObjectName("authTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    tabLayout->addWidget(titleLabel);

    QLabel* subtitleLabel = new QLabel("Join our marketplace");
    subtitleLabel->setObjectName("authSubtitle");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    tabLayout->addWidget(subtitleLabel);

    tabLayout->addSpacing(4);

    m_regUsernameEdit = new QLineEdit;
    m_regUsernameEdit->setObjectName("authInput");
    m_regUsernameEdit->setPlaceholderText("Username");
    m_regUsernameEdit->setClearButtonEnabled(true);
    tabLayout->addWidget(m_regUsernameEdit);

    m_regEmailEdit = new QLineEdit;
    m_regEmailEdit->setObjectName("authInput");
    m_regEmailEdit->setPlaceholderText("Email");
    m_regEmailEdit->setClearButtonEnabled(true);
    tabLayout->addWidget(m_regEmailEdit);

    m_regPasswordEdit = new QLineEdit;
    m_regPasswordEdit->setObjectName("authInput");
    m_regPasswordEdit->setPlaceholderText("Password");
    m_regPasswordEdit->setEchoMode(QLineEdit::Password);
    m_regPasswordEdit->setClearButtonEnabled(true);
    tabLayout->addWidget(m_regPasswordEdit);

    m_regConfirmPasswordEdit = new QLineEdit;
    m_regConfirmPasswordEdit->setObjectName("authInput");
    m_regConfirmPasswordEdit->setPlaceholderText("Confirm password");
    m_regConfirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_regConfirmPasswordEdit->setClearButtonEnabled(true);
    tabLayout->addWidget(m_regConfirmPasswordEdit);

    m_regAdminCheckBox = new QCheckBox("I have an administrator code");
    m_regAdminCheckBox->setObjectName("authCheckBox");
    tabLayout->addWidget(m_regAdminCheckBox);

    m_regAdminCodeEdit = new QLineEdit;
    m_regAdminCodeEdit->setObjectName("authInput");
    m_regAdminCodeEdit->setPlaceholderText("Enter secret code");
    m_regAdminCodeEdit->setEchoMode(QLineEdit::Password);
    m_regAdminCodeEdit->hide();
    tabLayout->addWidget(m_regAdminCodeEdit);

    m_regErrorLabel = new QLabel;
    m_regErrorLabel->setObjectName("authError");
    m_regErrorLabel->setAlignment(Qt::AlignCenter);
    m_regErrorLabel->setWordWrap(true);
    m_regErrorLabel->hide();
    tabLayout->addWidget(m_regErrorLabel);

    QPushButton* registerButton = new QPushButton("Create Account");
    registerButton->setObjectName("authPrimaryButton");
    registerButton->setCursor(Qt::PointingHandCursor);
    tabLayout->addWidget(registerButton);

    tabLayout->addStretch();

    m_tabWidget->addTab(registerTab, "Register");

    connect(registerButton, &QPushButton::clicked, this, &AuthWindow::onRegisterClicked);
    connect(m_regAdminCheckBox, &QCheckBox::stateChanged, this, &AuthWindow::onAdminCheckChanged);

    connect(m_regUsernameEdit, &QLineEdit::returnPressed, [this]() {
        m_regEmailEdit->setFocus();
    });
    connect(m_regEmailEdit, &QLineEdit::returnPressed, [this]() {
        m_regPasswordEdit->setFocus();
    });
    connect(m_regPasswordEdit, &QLineEdit::returnPressed, [this]() {
        m_regConfirmPasswordEdit->setFocus();
    });
    connect(m_regConfirmPasswordEdit, &QLineEdit::returnPressed, this, &AuthWindow::onRegisterClicked);
}

void AuthWindow::onLoginClicked() {
    const QString username = m_loginUsernameEdit->text().trimmed();
    const QString password = m_loginPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        m_loginErrorLabel->setText("Please fill in all fields");
        m_loginErrorLabel->show();
        return;
    }

    User* user = m_database->authenticateUser(
        username.toStdString(),
        password.toStdString()
    );

    if (user) {
        m_loginErrorLabel->hide();
        m_loginUsernameEdit->clear();
        m_loginPasswordEdit->clear();
        emit loginSuccess(user);
    } else {
        m_loginErrorLabel->setText("Invalid username or password");
        m_loginErrorLabel->show();
        m_loginPasswordEdit->clear();
        m_loginPasswordEdit->setFocus();
    }
}

void AuthWindow::onAdminCheckChanged(int state) {
    m_regAdminCodeEdit->setVisible(state == Qt::Checked);
    if (state != Qt::Checked) {
        m_regAdminCodeEdit->clear();
    }
}

bool AuthWindow::validateRegistration() {
    const QString username = m_regUsernameEdit->text().trimmed();
    const QString email = m_regEmailEdit->text().trimmed();
    const QString password = m_regPasswordEdit->text();
    const QString confirmPassword = m_regConfirmPasswordEdit->text();

    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        m_regErrorLabel->setText("Please fill in all required fields");
        m_regErrorLabel->show();
        return false;
    }

    if (username.length() < 3) {
        m_regErrorLabel->setText("Username must be at least 3 characters");
        m_regErrorLabel->show();
        m_regUsernameEdit->setFocus();
        return false;
    }

    static const QRegularExpression usernameRegex("^[a-zA-Z0-9_]+$");
    if (!usernameRegex.match(username).hasMatch()) {
        m_regErrorLabel->setText("Username can only contain letters, numbers and underscores");
        m_regErrorLabel->show();
        m_regUsernameEdit->setFocus();
        return false;
    }

    static const QRegularExpression emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    if (!emailRegex.match(email).hasMatch()) {
        m_regErrorLabel->setText("Please enter a valid email address");
        m_regErrorLabel->show();
        m_regEmailEdit->setFocus();
        return false;
    }

    if (password.length() < 4) {
        m_regErrorLabel->setText("Password must be at least 4 characters");
        m_regErrorLabel->show();
        m_regPasswordEdit->setFocus();
        return false;
    }

    if (password != confirmPassword) {
        m_regErrorLabel->setText("Passwords do not match");
        m_regErrorLabel->show();
        m_regConfirmPasswordEdit->clear();
        m_regConfirmPasswordEdit->setFocus();
        return false;
    }

    return true;
}

void AuthWindow::onRegisterClicked() {
    if (!validateRegistration()) {
        return;
    }

    QString adminCode;
    if (m_regAdminCheckBox->isChecked()) {
        adminCode = m_regAdminCodeEdit->text();
        if (!Database::isValidAdminCode(adminCode.toStdString())) {
            m_regErrorLabel->setText("Invalid administrator code");
            m_regErrorLabel->show();
            m_regAdminCodeEdit->clear();
            m_regAdminCodeEdit->setFocus();
            return;
        }
    }

    const bool success = m_database->registerUser(
        m_regUsernameEdit->text().trimmed().toStdString(),
        m_regPasswordEdit->text().toStdString(),
        m_regEmailEdit->text().trimmed().toStdString(),
        adminCode.toStdString()
    );

    if (success) {
        m_regErrorLabel->hide();
        clearRegistrationForm();
        m_tabWidget->setCurrentIndex(0);
        m_loginUsernameEdit->setFocus();
    } else {
        m_regErrorLabel->setText("User with this username or email already exists");
        m_regErrorLabel->show();
    }
}

void AuthWindow::clearRegistrationForm() {
    m_regUsernameEdit->clear();
    m_regEmailEdit->clear();
    m_regPasswordEdit->clear();
    m_regConfirmPasswordEdit->clear();
    m_regAdminCodeEdit->clear();
    m_regAdminCheckBox->setChecked(false);
    m_regErrorLabel->hide();
}
