#include "profilewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

ProfileWidget::ProfileWidget(User* user, Database* db, QWidget *parent)
    : QWidget(parent), m_user(user), m_database(db) {
    setupUI();
    updateProfile();
}

void ProfileWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Заголовок страницы
    QLabel* pageTitle = new QLabel("👤 Профиль");
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

    m_avatarLabel = new QLabel("👤");
    m_avatarLabel->setObjectName("avatarLabel");
    m_avatarLabel->setFixedSize(100, 100);
    m_avatarLabel->setAlignment(Qt::AlignCenter);
    m_avatarLabel->setStyleSheet("font-size: 60px;");
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
    formLayout->addRow("👤 Логин:", m_usernameEdit);

    m_emailEdit = new QLineEdit;
    m_emailEdit->setObjectName("inputField");
    formLayout->addRow("📧 Email:", m_emailEdit);

    m_phoneEdit = new QLineEdit;
    m_phoneEdit->setPlaceholderText("+7 (___) ___-__-__");
    m_phoneEdit->setObjectName("inputField");
    formLayout->addRow("📱 Телефон:", m_phoneEdit);

    m_addressEdit = new QLineEdit;
    m_addressEdit->setPlaceholderText("Город, улица, дом, квартира");
    m_addressEdit->setObjectName("inputField");
    formLayout->addRow("🏠 Адрес:", m_addressEdit);

    cardLayout->addLayout(formLayout);

    // Кнопка сохранения
    QPushButton* saveButton = new QPushButton("💾 Сохранить изменения");
    saveButton->setObjectName("primaryButton");
    saveButton->setCursor(Qt::PointingHandCursor);
    cardLayout->addWidget(saveButton);

    // Пополнение баланса
    QGroupBox* balanceGroup = new QGroupBox("💰 Пополнить баланс");
    balanceGroup->setObjectName("balanceGroup");

    QHBoxLayout* balanceLayout = new QHBoxLayout(balanceGroup);

    m_addBalanceEdit = new QLineEdit;
    m_addBalanceEdit->setPlaceholderText("Сумма");
    m_addBalanceEdit->setObjectName("inputField");
    balanceLayout->addWidget(m_addBalanceEdit);

    QPushButton* addBalanceBtn = new QPushButton("Пополнить");
    addBalanceBtn->setObjectName("primaryButton");
    addBalanceBtn->setCursor(Qt::PointingHandCursor);
    addBalanceBtn->setStyleSheet("background: #27ae60;");
    balanceLayout->addWidget(addBalanceBtn);

    cardLayout->addWidget(balanceGroup);

    // Кнопка выхода
    QPushButton* logoutButton = new QPushButton("🚪 Выйти из аккаунта");
    logoutButton->setObjectName("secondaryButton");
    logoutButton->setCursor(Qt::PointingHandCursor);
    logoutButton->setStyleSheet("color: #e74c3c; border-color: #e74c3c;");
    cardLayout->addWidget(logoutButton);

    // Тень для карточки
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(30);
    shadow->setColor(QColor(0, 0, 0, 40));
    shadow->setOffset(0, 10);
    profileCard->setGraphicsEffect(shadow);

    mainLayout->addWidget(profileCard);
    mainLayout->addStretch();

    // Подключения
    connect(saveButton, &QPushButton::clicked, this, &ProfileWidget::onSaveClicked);
    connect(addBalanceBtn, &QPushButton::clicked, this, &ProfileWidget::onAddBalanceClicked);
    connect(logoutButton, &QPushButton::clicked, this, &ProfileWidget::logoutClicked);
}

void ProfileWidget::updateProfile() {
    if (!m_user) return;
    
    m_usernameEdit->setText(QString::fromStdString(m_user->getUsername()));
    m_emailEdit->setText(QString::fromStdString(m_user->getEmail()));
    m_phoneEdit->setText(QString::fromStdString(m_user->getPhone()));
    m_addressEdit->setText(QString::fromStdString(m_user->getAddress()));

    m_balanceLabel->setText(QString("Баланс: %1 ₽").arg(m_user->getBalance(), 0, 'f', 2));
    m_roleLabel->setText(m_user->isAdmin() ? "👑 Администратор" : "👤 Покупатель");
    m_avatarLabel->setText(m_user->isAdmin() ? "👑" : "👤");
}

void ProfileWidget::onSaveClicked() {
    m_user->setEmail(m_emailEdit->text().toStdString());
    m_user->setPhone(m_phoneEdit->text().toStdString());
    m_user->setAddress(m_addressEdit->text().toStdString());

    if (m_database->updateUser(*m_user)) {
        QMessageBox::information(this, "Успех", "Профиль сохранен!");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить профиль");
    }
}

void ProfileWidget::onAddBalanceClicked() {
    bool ok;
    double amount = m_addBalanceEdit->text().toDouble(&ok);

    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректную сумму");
        return;
    }

    m_user->addBalance(amount);
    m_database->updateUser(*m_user);
    m_addBalanceEdit->clear();
    updateProfile();
    emit balanceChanged();

    QMessageBox::information(this, "Успех",
                             QString("Баланс пополнен на %1 ₽").arg(amount));
}