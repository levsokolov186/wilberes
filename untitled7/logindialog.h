#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QCheckBox>
#include "user.h"

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

    User* loggedInUser() const { return m_loggedInUser; }
    bool isGuest() const { return m_isGuest; }

private slots:
    void onLogin();
    void onRegister();
    void onGuestLogin();
    void switchToLogin();
    void switchToRegister();

private:
    void setupUI();
    void createLoginPage();
    void createRegisterPage();
    void showError(const QString &message);
    void showSuccess(const QString &message);

    QStackedWidget *m_stackedWidget;

    // Страница входа
    QLineEdit *m_loginUsername;
    QLineEdit *m_loginPassword;
    QLabel *m_loginError;

    // Страница регистрации
    QLineEdit *m_regUsername;
    QLineEdit *m_regEmail;
    QLineEdit *m_regPassword;
    QLineEdit *m_regPasswordConfirm;
    QCheckBox *m_regAdminCheck;
    QLineEdit *m_regAdminCode;
    QLabel *m_regError;

    User *m_loggedInUser;
    bool m_isGuest;
};

#endif // LOGINDIALOG_H
