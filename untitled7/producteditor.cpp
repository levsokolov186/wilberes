#include "producteditor.h"
#include "datamanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

ProductEditor::ProductEditor(QWidget *parent)
    : QDialog(parent), m_isEdit(false), m_productId(0)
{
    setWindowTitle("Добавить товар");
    setupUI();
}

ProductEditor::ProductEditor(const Product &product, QWidget *parent)
    : QDialog(parent), m_isEdit(true), m_productId(product.id())
{
    setWindowTitle("Редактировать товар");
    setupUI();
    loadProduct(product);
}

void ProductEditor::setupUI()
{
    setMinimumWidth(500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Заголовок
    QLabel *title = new QLabel(m_isEdit ? "Редактирование товара" : "Новый товар");
    title->setStyleSheet("font-size: 20px; font-weight: bold; color: #333;");
    mainLayout->addWidget(title);

    // Форма
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setSpacing(12);

    QString inputStyle =
        "QLineEdit, QTextEdit, QSpinBox, QDoubleSpinBox, QComboBox { "
        "padding: 10px; border: 2px solid #ddd; border-radius: 8px; font-size: 14px; }"
        "QLineEdit:focus, QTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus { "
        "border-color: #cb11ab; }";

    // Название
    m_nameEdit = new QLineEdit;
    m_nameEdit->setPlaceholderText("Введите название товара");
    m_nameEdit->setStyleSheet(inputStyle);
    formLayout->addRow("Название *", m_nameEdit);

    // Описание
    m_descEdit = new QTextEdit;
    m_descEdit->setPlaceholderText("Введите описание товара");
    m_descEdit->setMaximumHeight(100);
    m_descEdit->setStyleSheet(inputStyle);
    formLayout->addRow("Описание", m_descEdit);

    // Цена
    m_priceSpinBox = new QDoubleSpinBox;
    m_priceSpinBox->setRange(0, 10000000);
    m_priceSpinBox->setSuffix(" Rub");
    m_priceSpinBox->setStyleSheet(inputStyle);
    formLayout->addRow("Цена *", m_priceSpinBox);

    // Старая цена
    m_oldPriceSpinBox = new QDoubleSpinBox;
    m_oldPriceSpinBox->setRange(0, 10000000);
    m_oldPriceSpinBox->setSuffix(" Rub");
    m_oldPriceSpinBox->setStyleSheet(inputStyle);
    formLayout->addRow("Старая цена", m_oldPriceSpinBox);

    // Категория
    m_categoryCombo = new QComboBox;
    m_categoryCombo->addItems({"Электроника", "Одежда", "Обувь",
                               "Дом и сад", "Красота", "Спорт"});
    m_categoryCombo->setStyleSheet(inputStyle);
    formLayout->addRow("Категория *", m_categoryCombo);

    // Бренд
    m_brandEdit = new QLineEdit;
    m_brandEdit->setPlaceholderText("Введите бренд");
    m_brandEdit->setStyleSheet(inputStyle);
    formLayout->addRow("Бренд", m_brandEdit);

    // Рейтинг
    m_ratingSpinBox = new QDoubleSpinBox;
    m_ratingSpinBox->setRange(0, 5);
    m_ratingSpinBox->setSingleStep(0.1);
    m_ratingSpinBox->setValue(4.5);
    m_ratingSpinBox->setStyleSheet(inputStyle);
    formLayout->addRow("Рейтинг", m_ratingSpinBox);

    // Отзывы
    m_reviewsSpinBox = new QSpinBox;
    m_reviewsSpinBox->setRange(0, 1000000);
    m_reviewsSpinBox->setStyleSheet(inputStyle);
    formLayout->addRow("Отзывов", m_reviewsSpinBox);

    mainLayout->addLayout(formLayout);

    // Кнопки
    QHBoxLayout *btnLayout = new QHBoxLayout;

    QPushButton *cancelBtn = new QPushButton("Отмена");
    cancelBtn->setStyleSheet(
        "QPushButton { background: #f0f0f0; border: none; padding: 12px 30px; "
        "border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #e0e0e0; }");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(cancelBtn);

    QPushButton *saveBtn = new QPushButton(m_isEdit ? "Сохранить" : "Добавить");
    saveBtn->setStyleSheet(
        "QPushButton { background-color: #cb11ab; color: white; "
        "border: none; padding: 12px 30px; border-radius: 8px; "
        "font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #a00e8a; }");
    connect(saveBtn, &QPushButton::clicked, [this]() {
        if (m_nameEdit->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите название товара");
            return;
        }
        if (m_priceSpinBox->value() <= 0) {
            QMessageBox::warning(this, "Ошибка", "Укажите цену товара");
            return;
        }
        accept();
    });
    btnLayout->addWidget(saveBtn);

    mainLayout->addLayout(btnLayout);
}

void ProductEditor::loadProduct(const Product &product)
{
    m_nameEdit->setText(product.name());
    m_descEdit->setText(product.description());
    m_priceSpinBox->setValue(product.price());
    m_oldPriceSpinBox->setValue(product.oldPrice());
    m_categoryCombo->setCurrentText(product.category());
    m_brandEdit->setText(product.brand());
    m_ratingSpinBox->setValue(product.rating());
    m_reviewsSpinBox->setValue(product.reviews());
}

Product ProductEditor::getProduct() const
{
    int id = m_isEdit ? m_productId : DataManager::instance()->nextProductId();

    return Product(
        id,
        m_nameEdit->text().trimmed(),
        m_descEdit->toPlainText().trimmed(),
        m_priceSpinBox->value(),
        m_oldPriceSpinBox->value(),
        m_categoryCombo->currentText(),
        m_brandEdit->text().trimmed(),
        m_ratingSpinBox->value(),
        m_reviewsSpinBox->value()
        );
}
