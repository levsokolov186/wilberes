#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "user.h"
#include "database.h"

class ProfileWidget : public QWidget {
    Q_OBJECT

public:
    explicit ProfileWidget(User* user, Database* db, QWidget *parent = nullptr);
    
    void setUser(User* user);
    void updateProfile();
    
    User* getUser() const noexcept { return m_user; }

signals:
    void balanceChanged();
    void logoutClicked();

private slots:
    void onSaveClicked();
    void onAddBalanceClicked();

private:
    void setupUI();

    User* m_user;
    Database* m_database;

    QLabel* m_avatarLabel;
    QLabel* m_roleLabel;
    QLabel* m_balanceLabel;
    QLineEdit* m_usernameEdit;
    QLineEdit* m_emailEdit;
    QLineEdit* m_phoneEdit;
    QLineEdit* m_addressEdit;
    QLineEdit* m_addBalanceEdit;
};

#endif // PROFILEWIDGET_H