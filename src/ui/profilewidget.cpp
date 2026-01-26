#include "profilewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

ProfileWidget::ProfileWidget(User* user, Database* db, QWidget *parent)
    : QWidget(parent)
    , m_user(user)
    , m_database(db)
    , m_avatarLabel(nullptr)
    , m_roleLabel(nullptr)
    , m_balanceLabel(nullptr)
    , m_usernameEdit(nullptr)
    , m_emailEdit(nullptr)
    , m_phoneEdit(nullptr)
    , m_addressEdit(nullptr)
    , m_addBalanceEdit(nullptr)
{
    setupUI();
}

void ProfileWidget::setUser(User* user) {
    m_user = user;
    updateProfile();
}

void ProfileWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Заголовок страницы
    QLabel* pageTitle = new QLabel("Profile");
    pageTitle->setObjectName("pageTitle");
    mainLayout->addWidget(pageTitle);

    // Карточка профиля
    QWidget* profileCard = new QWidget;
    profileCard->setObjectName("card");
    profileCard->setFixedWidth(500);

    QVBoxLayout* cardLayout = new QVBoxLayout(profileCard);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(30, 30, 30, 30);

    // Аватар и основная информация
    QHBoxLayout* headerLayout = new QHBoxLayout;

    m_avatarLabel = new QLabel("[U]");
    m_avatarLabel->setObjectName("avatarLabel");
    m_avatarLabel->setFixedSize(100, 100);
    m_avatarLabel->setAlignment(Qt::AlignCenter);
    m_avatarLabel->setStyleSheet("font-size: 40px; font-weight: bold; color: #888; "
                                  "background: #f0f0f0; border-radius: 50px;");
    headerLayout->addWidget(m_avatarLabel);

    QVBoxLayout* headerInfoLayout = new QVBoxLayout;

    m_roleLabel = new QLabel;
    m_roleLabel->setObjectName("subtitleLabel");
    m_roleLabel->setStyleSheet("font-weight: bold;");
    headerInfoLayout->addWidget(m_roleLabel);

    m_balanceLabel = new QLabel;
    m_balanceLabel->setObjectName("titleLabel");
    m_balanceLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #27ae60;");
    headerInfoLayout->addWidget(m_balanceLabel);

    headerLayout->addLayout(headerInfoLayout);
    headerLayout->addStretch();
    cardLayout->addLayout(headerLayout);

    // Форма профиля
    QFormLayout* formLayout = new QFormLayout;
    formLayout->setSpacing(15);

    m_usernameEdit = new QLineEdit;
    m_usernameEdit->setReadOnly(true);
    m_usernameEdit->setObjectName("inputField");
    formLayout->addRow("Username:", m_usernameEdit);

    m_emailEdit = new QLineEdit;
    m_emailEdit->setObjectName("inputField");
    formLayout->addRow("Email:", m_emailEdit);

    m_phoneEdit = new QLineEdit;
    m_phoneEdit->setPlaceholderText("+1 (___) ___-____");
    m_phoneEdit->setObjectName("inputField");
    formLayout->addRow("Phone:", m_phoneEdit);

    m_addressEdit = new QLineEdit;
    m_addressEdit->setPlaceholderText("City, Street, Building, Apartment");
    m_addressEdit->setObjectName("inputField");
    formLayout->addRow("Address:", m_addressEdit);

    cardLayout->addLayout(formLayout);

    // Кнопка сохранения
    QPushButton* saveButton = new QPushButton("Save Changes");
    saveButton->setObjectName("primaryButton");
    saveButton->setCursor(Qt::PointingHandCursor);
    cardLayout->addWidget(saveButton);

    // Раздел пополнения баланса
    QGroupBox* balanceGroup = new QGroupBox("Add Funds");
    balanceGroup->setObjectName("balanceGroup");

    QHBoxLayout* balanceLayout = new QHBoxLayout(balanceGroup);

    m_addBalanceEdit = new QLineEdit;
    m_addBalanceEdit->setPlaceholderText("Amount");
    m_addBalanceEdit->setObjectName("inputField");
    balanceLayout->addWidget(m_addBalanceEdit);

    QPushButton* addBalanceBtn = new QPushButton("Add");
    addBalanceBtn->setObjectName("primaryButton");
    addBalanceBtn->setCursor(Qt::PointingHandCursor);
    addBalanceBtn->setStyleSheet("background: #27ae60;");
    balanceLayout->addWidget(addBalanceBtn);

    cardLayout->addWidget(balanceGroup);

    // Кнопка выхода
    QPushButton* logoutButton = new QPushButton("Sign Out");
    logoutButton->setObjectName("secondaryButton");
    logoutButton->setCursor(Qt::PointingHandCursor);
    logoutButton->setStyleSheet("color: #e74c3c; border-color: #e74c3c;");
    cardLayout->addWidget(logoutButton);

    // Эффект тени для карточки
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(profileCard);
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 40));
    shadow->setOffset(0, 10);
    profileCard->setGraphicsEffect(shadow);

    mainLayout->addWidget(profileCard);
    mainLayout->addStretch();

    // Соединение сигналов
    connect(saveButton, &QPushButton::clicked, this, &ProfileWidget::onSaveClicked);
    connect(addBalanceBtn, &QPushButton::clicked, this, &ProfileWidget::onAddBalanceClicked);
    connect(logoutButton, &QPushButton::clicked, this, &ProfileWidget::logoutClicked);
    
    // Позволить клавише Enter запускать пополнение баланса
    connect(m_addBalanceEdit, &QLineEdit::returnPressed, this, &ProfileWidget::onAddBalanceClicked);
}

void ProfileWidget::updateProfile() {
    if (!m_user) {
        // Очистка полей, если пользователь не задан
        m_usernameEdit->clear();
        m_emailEdit->clear();
        m_phoneEdit->clear();
        m_addressEdit->clear();
        m_balanceLabel->setText("Balance: 0 $");
        m_roleLabel->setText("Guest");
        m_avatarLabel->setText("[?]");
        return;
    }
    
    m_usernameEdit->setText(QString::fromStdString(m_user->getUsername()));
    m_emailEdit->setText(QString::fromStdString(m_user->getEmail()));
    m_phoneEdit->setText(QString::fromStdString(m_user->getPhone()));
    m_addressEdit->setText(QString::fromStdString(m_user->getAddress()));

    m_balanceLabel->setText(QString("Balance: %1 $").arg(m_user->getBalance(), 0, 'f', 2));
    
    if (m_user->isAdmin()) {
        m_roleLabel->setText("Administrator");
        m_avatarLabel->setText("[A]");
    } else {
        m_roleLabel->setText("Customer");
        m_avatarLabel->setText("[U]");
    }
}

void ProfileWidget::onSaveClicked() {
    if (!m_user) {
        QMessageBox::warning(this, "Error", "No user logged in");
        return;
    }
    
    m_user->setEmail(m_emailEdit->text().trimmed().toStdString());
    m_user->setPhone(m_phoneEdit->text().trimmed().toStdString());
    m_user->setAddress(m_addressEdit->text().trimmed().toStdString());

    if (m_database->updateUser(*m_user)) {
        QMessageBox::information(this, "Success", "Profile saved successfully!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save profile");
    }
}

void ProfileWidget::onAddBalanceClicked() {
    if (!m_user) {
        QMessageBox::warning(this, "Error", "No user logged in");
        return;
    }
    
    const QString amountText = m_addBalanceEdit->text().trimmed();
    
    if (amountText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter an amount");
        m_addBalanceEdit->setFocus();
        return;
    }
    
    bool ok = false;
    const double amount = amountText.toDouble(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Error", "Please enter a valid positive amount");
        m_addBalanceEdit->selectAll();
        m_addBalanceEdit->setFocus();
        return;
    }
    
    if (amount > 1000000) {
        QMessageBox::warning(this, "Error", "Maximum amount per transaction is 1,000,000");
        m_addBalanceEdit->selectAll();
        m_addBalanceEdit->setFocus();
        return;
    }

    m_user->addBalance(amount);
    
    if (m_database->updateUser(*m_user)) {
        m_addBalanceEdit->clear();
        updateProfile();
        emit balanceChanged();

        QMessageBox::information(this, "Success",
                                 QString("Balance topped up by %1 $").arg(amount, 0, 'f', 2));
    } else {
        // Отмена изменения баланса при ошибке сохранения
        m_user->withdrawBalance(amount);
        QMessageBox::warning(this, "Error", "Failed to update balance");
    }
}