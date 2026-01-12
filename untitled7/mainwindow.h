#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QSlider>
#include <QStackedWidget>
#include "product.h"
#include "cartdialog.h"
#include "adminpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearch();
    void onCategoryChanged(int index);
    void onSortChanged(int index);
    void onPriceFilterChanged();
    void addToCart(const Product &product);
    void showProductDetails(const Product &product);
    void showCart();
    void onLoginLogout();
    void switchToShop();
    void switchToAdmin();
    void updateUserUI();

private:
    void setupUI();
    void createHeader();
    void createSidebar();
    void createProductGrid();
    void filterProducts();
    void displayProducts(const QList<Product> &products);
    void showLoginDialog();

    // Main stack
    QStackedWidget *m_mainStack;
    QWidget *m_shopWidget;
    AdminPanel *m_adminPanel;

    // Header widgets
    QLineEdit *m_searchEdit;
    QPushButton *m_cartButton;
    QLabel *m_cartBadge;
    QPushButton *m_userButton;
    QPushButton *m_adminButton;
    QLabel *m_userLabel;

    // Sidebar widgets
    QComboBox *m_categoryCombo;
    QComboBox *m_sortCombo;
    QSlider *m_minPriceSlider;
    QSlider *m_maxPriceSlider;
    QLabel *m_priceRangeLabel;

    // Content widgets
    QScrollArea *m_scrollArea;
    QWidget *m_productContainer;
    QGridLayout *m_productGrid;

    // Data
    QList<Product> m_filteredProducts;
    CartDialog *m_cartDialog;
};

#endif // MAINWINDOW_H
