#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include "product.h"

class AdminPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPanel(QWidget *parent = nullptr);

signals:
    void productChanged();

private slots:
    void refreshProducts();
    void refreshUsers();
    void addProduct();
    void editProduct();
    void deleteProduct();
    void deleteUser();

private:
    void setupUI();
    void createDashboard();
    void createProductsTab();
    void createUsersTab();
    void createOrdersTab();
    void updateStats();

    QTabWidget *m_tabWidget;

    // Dashboard
    QLabel *m_totalProductsLabel;
    QLabel *m_totalUsersLabel;
    QLabel *m_totalOrdersLabel;
    QLabel *m_totalRevenueLabel;

    // Products tab
    QTableWidget *m_productsTable;

    // Users tab
    QTableWidget *m_usersTable;
};

#endif // ADMINPANEL_H
