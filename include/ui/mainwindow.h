#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QComboBox>
#include <QButtonGroup>
#include "database.h"
#include "cart.h"
#include "loginwindow.h"
#include "registerwindow.h"
#include "cartwidget.h"
#include "adminpanel.h"
#include "profilewidget.h"
#include "theme_manager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccess(User* user);
    void onRegisterSuccess();
    void showLogin();
    void showRegister();
    void showCatalog();
    void showCart();
    void showProfile();
    void showAdmin();
    void showSettings();
    void onCategorySelected(const QString& category);
    void onSearch();
    void onAddToCart(const Product& product);
    void onCheckout();
    void updateCartBadge();
    void refreshCatalog();
    void logout();
    void onThemeChanged(int index);
    void onThemeApplied(ThemeType newTheme);

private:
    void setupUI();
    void setupMainUI();
    void setupSettingsPage();
    void displayProducts(const std::vector<Product>& products);
    void updateSidebarActiveButton(QPushButton* activeBtn);
    void syncThemeComboBox();

    Database* m_database;
    User* m_currentUser;
    Cart* m_cart;

    // Страницы
    QStackedWidget* m_stackedWidget;
    LoginWindow* m_loginWindow;
    RegisterWindow* m_registerWindow;
    QWidget* m_mainPage;
    CartWidget* m_cartWidget;
    ProfileWidget* m_profileWidget;
    AdminPanel* m_adminPanel;
    QWidget* m_settingsPage;
    
    // Обертки страниц для навигации в стеке
    QWidget* m_cartPageWrapper;
    QWidget* m_profilePageWrapper;
    QWidget* m_settingsPageWrapper;
    QWidget* m_adminPageWrapper;

    // Элементы главной страницы
    QWidget* m_header;
    QLineEdit* m_searchEdit;
    QListWidget* m_categoryList;
    QGridLayout* m_productsGrid;
    QScrollArea* m_productsScrollArea;
    QPushButton* m_cartButton;
    QLabel* m_cartBadge;
    QPushButton* m_profileButton;
    QPushButton* m_adminButton;
    QPushButton* m_settingsButton;
    QLabel* m_userLabel;

    // Кнопки боковой панели
    QPushButton* m_sidebarCatalogBtn;
    QPushButton* m_sidebarCartBtn;
    QPushButton* m_sidebarProfileBtn;
    QPushButton* m_sidebarSettingsBtn;
    QPushButton* m_sidebarAdminBtn;
    QButtonGroup* m_sidebarButtonGroup;

    // Элементы страницы настроек
    QComboBox* m_themeComboBox;
};

#endif // MAINWINDOW_H