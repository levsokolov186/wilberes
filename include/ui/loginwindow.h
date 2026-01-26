#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include "database.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(Database* db, QWidget *parent = nullptr);

signals:
    void loginSuccess(User* user);
    void registerClicked();

private slots:
    void onLoginClicked();

private:
    void setupUI();
    void applyStyles();

    Database* m_database;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QPushButton* m_loginButton;
    QPushButton* m_registerButton;
    QLabel* m_errorLabel;
};

#endif // LOGINWINDOW_H
