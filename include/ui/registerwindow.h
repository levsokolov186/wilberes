#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include "database.h"

class RegisterWindow : public QWidget {
    Q_OBJECT

public:
    explicit RegisterWindow(Database* db, QWidget *parent = nullptr);

signals:
    void registerSuccess();
    void backToLogin();

private slots:
    void onRegisterClicked();
    void onAdminCheckChanged(int state);

private:
    void setupUI();
    void applyStyles();
    bool validateInput();
    void clearForm();

    Database* m_database;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_emailEdit;
    QLineEdit* m_passwordEdit;
    QLineEdit* m_confirmPasswordEdit;
    QLineEdit* m_adminCodeEdit;
    QCheckBox* m_adminCheckBox;
    QPushButton* m_registerButton;
    QPushButton* m_backButton;
    QLabel* m_errorLabel;
};

#endif // REGISTERWINDOW_H