#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "database.h"

class AuthWindow : public QWidget {
    Q_OBJECT

public:
    explicit AuthWindow(Database* db, QWidget *parent = nullptr);

signals:
    void loginSuccess(User* user);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onAdminCheckChanged(int state);

private:
    void setupUI();
    void setupLoginTab();
    void setupRegisterTab();
    bool validateRegistration();
    void clearRegistrationForm();

    Database* m_database;

    QTabWidget* m_tabWidget;

    QLineEdit* m_loginUsernameEdit;
    QLineEdit* m_loginPasswordEdit;
    QLabel* m_loginErrorLabel;

    QLineEdit* m_regUsernameEdit;
    QLineEdit* m_regEmailEdit;
    QLineEdit* m_regPasswordEdit;
    QLineEdit* m_regConfirmPasswordEdit;
    QCheckBox* m_regAdminCheckBox;
    QLineEdit* m_regAdminCodeEdit;
    QLabel* m_regErrorLabel;
};

#endif // AUTHWINDOW_H
