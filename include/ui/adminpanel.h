#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include "database.h"

class AdminPanel : public QWidget {
    Q_OBJECT

public:
    explicit AdminPanel(Database* db, QWidget *parent = nullptr);

signals:
    void productsChanged();

private slots:
    void onAddProduct();
    void onEditProduct();
    void onDeleteProduct();
    void onTableSelectionChanged();
    void refreshTable();

private:
    void setupUI();
    void clearForm();

    Database* m_database;
    QTableWidget* m_table;

    // Форма
    QLineEdit* m_nameEdit;
    QTextEdit* m_descEdit;
    QLineEdit* m_brandEdit;
    QComboBox* m_categoryCombo;
    QDoubleSpinBox* m_priceSpin;
    QSpinBox* m_stockSpin;
    QSpinBox* m_discountSpin;

    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;

    int m_selectedProductId;
};

#endif // ADMINPANEL_H
