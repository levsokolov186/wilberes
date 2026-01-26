#include "adminpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QtWidgets>

AdminPanel::AdminPanel(Database* db, QWidget *parent)
    : QWidget(parent)
    , m_database(db)
    , m_table(nullptr)
    , m_nameEdit(nullptr)
    , m_descEdit(nullptr)
    , m_brandEdit(nullptr)
    , m_categoryCombo(nullptr)
    , m_priceSpin(nullptr)
    , m_stockSpin(nullptr)
    , m_discountSpin(nullptr)
    , m_addButton(nullptr)
    , m_editButton(nullptr)
    , m_deleteButton(nullptr)
    , m_selectedProductId(-1)
{
    setupUI();
    refreshTable();
}

void AdminPanel::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Левая панель - таблица
    QVBoxLayout* leftLayout = new QVBoxLayout;

    QLabel* titleLabel = new QLabel("Admin Panel");
    titleLabel->setObjectName("pageTitle");
    leftLayout->addWidget(titleLabel);

    m_table = new QTableWidget;
    m_table->setObjectName("adminTable");
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({
        "ID", "Name", "Category", "Brand", "Price", "Stock", "Discount"
    });
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    leftLayout->addWidget(m_table);

    mainLayout->addLayout(leftLayout, 2);

    // Правая панель - форма
    QGroupBox* formGroup = new QGroupBox("Product Details");
    formGroup->setObjectName("formGroup");

    QFormLayout* formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(15);

    m_nameEdit = new QLineEdit;
    m_nameEdit->setPlaceholderText("Product name");
    m_nameEdit->setObjectName("inputField");
    m_nameEdit->setMaxLength(100);
    formLayout->addRow("Name:", m_nameEdit);

    m_descEdit = new QTextEdit;
    m_descEdit->setPlaceholderText("Product description");
    m_descEdit->setMaximumHeight(100);
    m_descEdit->setObjectName("descEdit");
    formLayout->addRow("Description:", m_descEdit);

    m_brandEdit = new QLineEdit;
    m_brandEdit->setPlaceholderText("Brand name");
    m_brandEdit->setObjectName("inputField");
    m_brandEdit->setMaxLength(50);
    formLayout->addRow("Brand:", m_brandEdit);

    m_categoryCombo = new QComboBox;
    m_categoryCombo->addItems({
        "Electronics", "Clothing", "Footwear", "Home & Garden",
        "Beauty", "Kids", "Sports", "Auto"
    });
    m_categoryCombo->setObjectName("categoryCombo");
    formLayout->addRow("Category:", m_categoryCombo);

    m_priceSpin = new QDoubleSpinBox;
    m_priceSpin->setRange(0, 9999999);
    m_priceSpin->setSuffix(" $");
    m_priceSpin->setDecimals(2);
    m_priceSpin->setObjectName("priceSpin");
    formLayout->addRow("Price:", m_priceSpin);

    m_stockSpin = new QSpinBox;
    m_stockSpin->setRange(0, 99999);
    m_stockSpin->setSuffix(" pcs");
    m_stockSpin->setObjectName("stockSpin");
    formLayout->addRow("Stock:", m_stockSpin);

    m_discountSpin = new QSpinBox;
    m_discountSpin->setRange(0, 99);
    m_discountSpin->setSuffix(" %");
    m_discountSpin->setObjectName("discountSpin");
    formLayout->addRow("Discount:", m_discountSpin);

    // Кнопки действий
    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(10);

    m_addButton = new QPushButton("Add");
    m_addButton->setObjectName("primaryButton");
    m_addButton->setCursor(Qt::PointingHandCursor);
    m_addButton->setStyleSheet("background: #27ae60;");
    m_addButton->setMinimumHeight(36);
    btnLayout->addWidget(m_addButton);

    m_editButton = new QPushButton("Update");
    m_editButton->setObjectName("primaryButton");
    m_editButton->setEnabled(false);
    m_editButton->setCursor(Qt::PointingHandCursor);
    m_editButton->setStyleSheet("background: #3498db;");
    m_editButton->setMinimumHeight(36);
    btnLayout->addWidget(m_editButton);

    m_deleteButton = new QPushButton("Delete");
    m_deleteButton->setObjectName("primaryButton");
    m_deleteButton->setEnabled(false);
    m_deleteButton->setCursor(Qt::PointingHandCursor);
    m_deleteButton->setStyleSheet("background: #e74c3c;");
    m_deleteButton->setMinimumHeight(36);
    btnLayout->addWidget(m_deleteButton);

    formLayout->addRow(btnLayout);

    QPushButton* clearBtn = new QPushButton("Clear Form");
    clearBtn->setObjectName("secondaryButton");
    clearBtn->setCursor(Qt::PointingHandCursor);
    clearBtn->setMinimumHeight(36);
    formLayout->addRow(clearBtn);

    mainLayout->addWidget(formGroup, 1);

    // Соединение сигналов
    connect(m_table, &QTableWidget::itemSelectionChanged,
            this, &AdminPanel::onTableSelectionChanged);
    connect(m_addButton, &QPushButton::clicked, this, &AdminPanel::onAddProduct);
    connect(m_editButton, &QPushButton::clicked, this, &AdminPanel::onEditProduct);
    connect(m_deleteButton, &QPushButton::clicked, this, &AdminPanel::onDeleteProduct);
    connect(clearBtn, &QPushButton::clicked, this, &AdminPanel::clearForm);
    
    // Клавиша Enter запускает добавление/обновление в зависимости от состояния выбора
    connect(m_nameEdit, &QLineEdit::returnPressed, this, [this]() {
        if (m_selectedProductId >= 0) {
            onEditProduct();
        } else {
            onAddProduct();
        }
    });
}

void AdminPanel::refreshTable() {
    m_table->setRowCount(0);

    const auto& products = m_database->getAllProducts();
    m_table->setRowCount(static_cast<int>(products.size()));
    
    int row = 0;
    for (const auto& product : products) {
        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(product.getId())));
        m_table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(product.getName())));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(product.getCategory())));
        m_table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(product.getBrand())));
        m_table->setItem(row, 4, new QTableWidgetItem(QString("%1 $").arg(product.getPrice(), 0, 'f', 2)));
        m_table->setItem(row, 5, new QTableWidgetItem(QString::number(product.getStock())));
        m_table->setItem(row, 6, new QTableWidgetItem(QString("%1%").arg(product.getDiscount())));
        
        // Выравнивание по центру для числовых столбцов
        for (int col = 4; col <= 6; ++col) {
            if (auto* item = m_table->item(row, col)) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
        
        ++row;
    }
    
    // Изменение размера столбцов по содержимому
    m_table->resizeColumnsToContents();
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void AdminPanel::onTableSelectionChanged() {
    const QList<QTableWidgetItem*> selected = m_table->selectedItems();

    if (!selected.isEmpty()) {
        const int row = selected.first()->row();
        QTableWidgetItem* idItem = m_table->item(row, 0);
        
        if (!idItem) {
            m_selectedProductId = -1;
            m_editButton->setEnabled(false);
            m_deleteButton->setEnabled(false);
            return;
        }
        
        const int id = idItem->text().toInt();
        m_selectedProductId = id;

        const Product* product = m_database->getProductById(id);
        if (product) {
            m_nameEdit->setText(QString::fromStdString(product->getName()));
            m_descEdit->setText(QString::fromStdString(product->getDescription()));
            m_brandEdit->setText(QString::fromStdString(product->getBrand()));
            
            const int categoryIndex = m_categoryCombo->findText(
                QString::fromStdString(product->getCategory()));
            if (categoryIndex >= 0) {
                m_categoryCombo->setCurrentIndex(categoryIndex);
            }
            
            m_priceSpin->setValue(product->getOldPrice());
            m_stockSpin->setValue(product->getStock());
            m_discountSpin->setValue(product->getDiscount());
        }

        m_editButton->setEnabled(true);
        m_deleteButton->setEnabled(true);
    } else {
        m_selectedProductId = -1;
        m_editButton->setEnabled(false);
        m_deleteButton->setEnabled(false);
    }
}

void AdminPanel::clearForm() {
    m_nameEdit->clear();
    m_descEdit->clear();
    m_brandEdit->clear();
    m_categoryCombo->setCurrentIndex(0);
    m_priceSpin->setValue(0);
    m_stockSpin->setValue(0);
    m_discountSpin->setValue(0);
    m_selectedProductId = -1;
    m_table->clearSelection();
    m_editButton->setEnabled(false);
    m_deleteButton->setEnabled(false);
    m_nameEdit->setFocus();
}

void AdminPanel::onAddProduct() {
    const QString name = m_nameEdit->text().trimmed();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a product name");
        m_nameEdit->setFocus();
        return;
    }
    
    if (m_priceSpin->value() <= 0) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid price");
        m_priceSpin->setFocus();
        return;
    }

    Product product(0,
                    name.toStdString(),
                    m_descEdit->toPlainText().trimmed().toStdString(),
                    m_categoryCombo->currentText().toStdString(),
                    m_priceSpin->value(),
                    m_stockSpin->value());
                    
    product.setBrand(m_brandEdit->text().trimmed().toStdString());
    product.setDiscount(m_discountSpin->value());

    if (m_database->addProduct(product)) {
        refreshTable();
        clearForm();
        emit productsChanged();
        QMessageBox::information(this, "Success", "Product added successfully!");
    } else {
        QMessageBox::critical(this, "Error", "Failed to add product");
    }
}

void AdminPanel::onEditProduct() {
    if (m_selectedProductId < 0) {
        return;
    }
    
    const QString name = m_nameEdit->text().trimmed();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a product name");
        m_nameEdit->setFocus();
        return;
    }

    Product* product = m_database->getProductById(m_selectedProductId);
    if (product) {
        product->setName(name.toStdString());
        product->setDescription(m_descEdit->toPlainText().trimmed().toStdString());
        product->setBrand(m_brandEdit->text().trimmed().toStdString());
        product->setCategory(m_categoryCombo->currentText().toStdString());
        product->setPrice(m_priceSpin->value());
        product->setOldPrice(m_priceSpin->value());
        product->setStock(m_stockSpin->value());
        product->setDiscount(m_discountSpin->value());

        if (m_database->updateProduct(*product)) {
            refreshTable();
            emit productsChanged();
            QMessageBox::information(this, "Success", "Product updated successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Failed to update product");
        }
    }
}

void AdminPanel::onDeleteProduct() {
    if (m_selectedProductId < 0) {
        return;
    }

    const QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Deletion",
        "Are you sure you want to delete this product?\n\nThis action cannot be undone.",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        if (m_database->deleteProduct(m_selectedProductId)) {
            refreshTable();
            clearForm();
            emit productsChanged();
            QMessageBox::information(this, "Success", "Product deleted successfully!");
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete product");
        }
    }
}