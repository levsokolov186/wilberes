#ifndef PRODUCTEDITOR_H
#define PRODUCTEDITOR_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "product.h"

class ProductEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ProductEditor(QWidget *parent = nullptr);
    explicit ProductEditor(const Product &product, QWidget *parent = nullptr);

    Product getProduct() const;

private:
    void setupUI();
    void loadProduct(const Product &product);

    bool m_isEdit;
    int m_productId;

    QLineEdit *m_nameEdit;
    QTextEdit *m_descEdit;
    QDoubleSpinBox *m_priceSpinBox;
    QDoubleSpinBox *m_oldPriceSpinBox;
    QComboBox *m_categoryCombo;
    QLineEdit *m_brandEdit;
    QDoubleSpinBox *m_ratingSpinBox;
    QSpinBox *m_reviewsSpinBox;
};

#endif // PRODUCTEDITOR_H
