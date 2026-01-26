#include "adminpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QtWidgets>

AdminPanel::AdminPanel(Database* db, QWidget *parent)
    : QWidget(parent), m_database(db), m_selectedProductId(-1) {
    setupUI();
    refreshTable();
}

void AdminPanel::setupUI() {
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Левая панель - таблица
    QVBoxLayout* leftLayout = new QVBoxLayout;

    QLabel* titleLabel = new QLabel("🛠 Панель администратора");
    titleLabel->setObjectName("pageTitle");
    leftLayout->addWidget(titleLabel);

    m_table = new QTableWidget;
    m_table->setObjectName("adminTable");
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels({
        "ID", "Название", "Категория", "Бренд", "Цена", "Остаток", "Скидка"
    });
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    leftLayout->addWidget(m_table);

    mainLayout->addLayout(leftLayout, 2);

    // Правая панель - форма
    QGroupBox* formGroup = new QGroupBox("Товар");
    formGroup->setObjectName("formGroup");

    QFormLayout* formLayout = new QFormLayout(formGroup);
    formLayout->setSpacing(15);

    m_nameEdit = new QLineEdit;
    m_nameEdit->setPlaceholderText("Название товара");
    m_nameEdit->setObjectName("inputField");
    formLayout->addRow("Название:", m_nameEdit);

    m_descEdit = new QTextEdit;
    m_descEdit->setPlaceholderText("Описание товара");
    m_descEdit->setMaximumHeight(100);
    m_descEdit->setObjectName("descEdit");
    formLayout->addRow("Описание:", m_descEdit);

    m_brandEdit = new QLineEdit;
    m_brandEdit->setPlaceholderText("Бренд");
    m_brandEdit->setObjectName("inputField");
    formLayout->addRow("Бренд:", m_brandEdit);

    m_categoryCombo = new QComboBox;
    m_categoryCombo->addItems({
        "Электроника", "Одежда", "Обувь", "Дом и сад",
        "Красота", "Детские товары", "Спорт", "Авто"
    });
    formLayout->addRow("Категория:", m_categoryCombo);

    m_priceSpin = new QDoubleSpinBox;
    m_priceSpin->setRange(0, 9999999);
    m_priceSpin->setSuffix(" ₽");
    formLayout->addRow("Цена:", m_priceSpin);

    m_stockSpin = new QSpinBox;
    m_stockSpin->setRange(0, 99999);
    m_stockSpin->setSuffix(" шт.");
    formLayout->addRow("Остаток:", m_stockSpin);

    m_discountSpin = new QSpinBox;
    m_discountSpin->setRange(0, 99);
    m_discountSpin->setSuffix(" %");
    formLayout->addRow("Скидка:", m_discountSpin);

    // Кнопки
    QHBoxLayout* btnLayout = new QHBoxLayout;

    m_addButton = new QPushButton("➕ Добавить");
    m_addButton->setObjectName("primaryButton");
    m_addButton->setCursor(Qt::PointingHandCursor);
    m_addButton->setStyleSheet("background: #27ae60;");
    btnLayout->addWidget(m_addButton);

    m_editButton = new QPushButton("✏️ Изменить");
    m_editButton->setObjectName("primaryButton");
    m_editButton->setEnabled(false);
    m_editButton->setCursor(Qt::PointingHandCursor);
    m_editButton->setStyleSheet("background: #3498db;");
    btnLayout->addWidget(m_editButton);

    m_deleteButton = new QPushButton("🗑️ Удалить");
    m_deleteButton->setObjectName("primaryButton");
    m_deleteButton->setEnabled(false);
    m_deleteButton->setCursor(Qt::PointingHandCursor);
    m_deleteButton->setStyleSheet("background: #e74c3c;");
    btnLayout->addWidget(m_deleteButton);

    formLayout->addRow(btnLayout);

    QPushButton* clearBtn = new QPushButton("🔄 Очистить форму");
    clearBtn->setObjectName("secondaryButton");
    clearBtn->setCursor(Qt::PointingHandCursor);
    formLayout->addRow(clearBtn);

    mainLayout->addWidget(formGroup, 1);

    // Подключения
    connect(m_table, &QTableWidget::itemSelectionChanged,
            this, &AdminPanel::onTableSelectionChanged);
    connect(m_addButton, &QPushButton::clicked, this, &AdminPanel::onAddProduct);
    connect(m_editButton, &QPushButton::clicked, this, &AdminPanel::onEditProduct);
    connect(m_deleteButton, &QPushButton::clicked, this, &AdminPanel::onDeleteProduct);
    connect(clearBtn, &QPushButton::clicked, this, &AdminPanel::clearForm);
}

void AdminPanel::refreshTable() {
    m_table->setRowCount(0);

    for (const auto& product : m_database->getAllProducts()) {
        int row = m_table->rowCount();
        m_table->insertRow(row);

        m_table->setItem(row, 0, new QTableWidgetItem(QString::number(product.getId())));
        m_table->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(product.getName())));
        m_table->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(product.getCategory())));
        m_table->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(product.getBrand())));
        m_table->setItem(row, 4, new QTableWidgetItem(QString("%1 ₽").arg(product.getPrice())));
        m_table->setItem(row, 5, new QTableWidgetItem(QString::number(product.getStock())));
        m_table->setItem(row, 6, new QTableWidgetItem(QString("%1%").arg(product.getDiscount())));
    }
}

void AdminPanel::onTableSelectionChanged() {
    QList<QTableWidgetItem*> selected = m_table->selectedItems();

    if (!selected.isEmpty()) {
        int row = selected.first()->row();
        int id = m_table->item(row, 0)->text().toInt();
        m_selectedProductId = id;

        Product* product = m_database->getProductById(id);
        if (product) {
            m_nameEdit->setText(QString::fromStdString(product->getName()));
            m_descEdit->setText(QString::fromStdString(product->getDescription()));
            m_brandEdit->setText(QString::fromStdString(product->getBrand()));
            m_categoryCombo->setCurrentText(QString::fromStdString(product->getCategory()));
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
}

void AdminPanel::onAddProduct() {
    if (m_nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название товара");
        return;
    }

    Product product(0,
                    m_nameEdit->text().toStdString(),
                    m_descEdit->toPlainText().toStdString(),
                    m_categoryCombo->currentText().toStdString(),
                    m_priceSpin->value(),
                    m_stockSpin->value()
                    );
    product.setBrand(m_brandEdit->text().toStdString());
    product.setDiscount(m_discountSpin->value());

    if (m_database->addProduct(product)) {
        refreshTable();
        clearForm();
        emit productsChanged();
        QMessageBox::information(this, "Успех", "Товар добавлен!");
    }
}

void AdminPanel::onEditProduct() {
    if (m_selectedProductId < 0) return;

    Product* product = m_database->getProductById(m_selectedProductId);
    if (product) {
        product->setName(m_nameEdit->text().toStdString());
        product->setDescription(m_descEdit->toPlainText().toStdString());
        product->setBrand(m_brandEdit->text().toStdString());
        product->setCategory(m_categoryCombo->currentText().toStdString());
        product->setPrice(m_priceSpin->value());
        product->setOldPrice(m_priceSpin->value());
        product->setStock(m_stockSpin->value());
        product->setDiscount(m_discountSpin->value());

        if (m_database->updateProduct(*product)) {
            refreshTable();
            emit productsChanged();
            QMessageBox::information(this, "Успех", "Товар обновлен!");
        }
    }
}

void AdminPanel::onDeleteProduct() {
    if (m_selectedProductId < 0) return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Подтверждение",
        "Вы уверены, что хотите удалить этот товар?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (m_database->deleteProduct(m_selectedProductId)) {
            refreshTable();
            clearForm();
            emit productsChanged();
            QMessageBox::information(this, "Успех", "Товар удален!");
        }
    }
}