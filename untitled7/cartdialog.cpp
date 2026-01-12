#include "cartdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QFrame>

CartDialog::CartDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Корзина");
    setMinimumSize(500, 600);
    setupUI();
}

void CartDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);

    // Заголовок
    QLabel *titleLabel = new QLabel("🛒 Ваша корзина");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    mainLayout->addWidget(titleLabel);

    // Список товаров
    m_cartList = new QListWidget;
    m_cartList->setStyleSheet(
        "QListWidget { border: 1px solid #ddd; border-radius: 8px; }"
        "QListWidget::item { border-bottom: 1px solid #eee; padding: 10px; }");
    m_cartList->setSpacing(5);
    mainLayout->addWidget(m_cartList, 1);

    // Разделитель
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: #ddd;");
    mainLayout->addWidget(line);

    // Итого
    QVBoxLayout *totalsLayout = new QVBoxLayout;

    m_itemsCountLabel = new QLabel("Товаров: 0");
    m_itemsCountLabel->setStyleSheet("font-size: 14px; color: #666;");
    totalsLayout->addWidget(m_itemsCountLabel);

    m_discountLabel = new QLabel("Скидка: 0 ₽");
    m_discountLabel->setStyleSheet("font-size: 14px; color: #cb11ab;");
    totalsLayout->addWidget(m_discountLabel);

    m_totalLabel = new QLabel("Итого: 0 ₽");
    m_totalLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #000;");
    totalsLayout->addWidget(m_totalLabel);

    mainLayout->addLayout(totalsLayout);

    // Кнопки
    QHBoxLayout *btnLayout = new QHBoxLayout;

    m_clearBtn = new QPushButton("Очистить корзину");
    m_clearBtn->setStyleSheet(
        "QPushButton { background: #f5f5f5; border: 1px solid #ddd; "
        "padding: 12px 20px; border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background: #eee; }");
    connect(m_clearBtn, &QPushButton::clicked, this, &CartDialog::clear);
    btnLayout->addWidget(m_clearBtn);

    m_checkoutBtn = new QPushButton("Оформить заказ");
    m_checkoutBtn->setStyleSheet(
        "QPushButton { background-color: #cb11ab; color: white; "
        "border: none; padding: 12px 30px; border-radius: 8px; "
        "font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #a00e8a; }");
    connect(m_checkoutBtn, &QPushButton::clicked, [this]() {
        if (m_items.isEmpty()) {
            QMessageBox::information(this, "Корзина пуста",
                                     "Добавьте товары в корзину");
            return;
        }
        QMessageBox::information(this, "Заказ оформлен",
                                 QString("Спасибо за заказ!\n\nСумма: %1 ₽\nТоваров: %2")
                                     .arg(totalPrice(), 0, 'f', 0)
                                     .arg(totalItems()));
        clear();
    });
    btnLayout->addWidget(m_checkoutBtn);

    mainLayout->addLayout(btnLayout);
}

void CartDialog::addProduct(const Product &product)
{
    if (m_items.contains(product.id())) {
        m_items[product.id()].quantity++;
    } else {
        m_items[product.id()] = {product, 1};
    }
    updateCartList();
    emit cartChanged();
}

void CartDialog::clear()
{
    m_items.clear();
    updateCartList();
    emit cartChanged();
}

int CartDialog::totalItems() const
{
    int count = 0;
    for (const auto &item : m_items) {
        count += item.quantity;
    }
    return count;
}

double CartDialog::totalPrice() const
{
    double total = 0;
    for (const auto &item : m_items) {
        total += item.product.price() * item.quantity;
    }
    return total;
}

void CartDialog::updateCartList()
{
    m_cartList->clear();

    for (const auto &item : m_items) {
        QWidget *itemWidget = new QWidget;
        QHBoxLayout *layout = new QHBoxLayout(itemWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Изображение
        QLabel *imgLabel = new QLabel;
        imgLabel->setPixmap(item.product.getImage(60, 60));
        imgLabel->setFixedSize(60, 60);
        layout->addWidget(imgLabel);

        // Информация
        QVBoxLayout *infoLayout = new QVBoxLayout;
        QLabel *nameLabel = new QLabel(item.product.name());
        nameLabel->setStyleSheet("font-weight: bold;");
        infoLayout->addWidget(nameLabel);

        QLabel *priceLabel = new QLabel(
            QString("%1 ₽ × %2 = %3 ₽")
                .arg(item.product.price(), 0, 'f', 0)
                .arg(item.quantity)
                .arg(item.product.price() * item.quantity, 0, 'f', 0));
        priceLabel->setStyleSheet("color: #cb11ab; font-size: 14px;");
        infoLayout->addWidget(priceLabel);
        layout->addLayout(infoLayout, 1);

        // Кнопки количества
        QHBoxLayout *qtyLayout = new QHBoxLayout;

        QPushButton *minusBtn = new QPushButton("-");
        minusBtn->setFixedSize(30, 30);
        minusBtn->setStyleSheet(
            "QPushButton { background: #f0f0f0; border-radius: 15px; font-size: 16px; }"
            "QPushButton:hover { background: #ddd; }");
        connect(minusBtn, &QPushButton::clicked, [this, id = item.product.id()]() {
            changeQuantity(id, -1);
        });
        qtyLayout->addWidget(minusBtn);

        QLabel *qtyLabel = new QLabel(QString::number(item.quantity));
        qtyLabel->setAlignment(Qt::AlignCenter);
        qtyLabel->setMinimumWidth(30);
        qtyLayout->addWidget(qtyLabel);

        QPushButton *plusBtn = new QPushButton("+");
        plusBtn->setFixedSize(30, 30);
        plusBtn->setStyleSheet(
            "QPushButton { background: #f0f0f0; border-radius: 15px; font-size: 16px; }"
            "QPushButton:hover { background: #ddd; }");
        connect(plusBtn, &QPushButton::clicked, [this, id = item.product.id()]() {
            changeQuantity(id, 1);
        });
        qtyLayout->addWidget(plusBtn);

        layout->addLayout(qtyLayout);

        // Кнопка удаления
        QPushButton *removeBtn = new QPushButton("✕");
        removeBtn->setFixedSize(30, 30);
        removeBtn->setStyleSheet(
            "QPushButton { background: #ffebee; color: #f44336; "
            "border-radius: 15px; font-size: 14px; }"
            "QPushButton:hover { background: #f44336; color: white; }");
        connect(removeBtn, &QPushButton::clicked, [this, id = item.product.id()]() {
            removeItem(id);
        });
        layout->addWidget(removeBtn);

        QListWidgetItem *listItem = new QListWidgetItem(m_cartList);
        listItem->setSizeHint(itemWidget->sizeHint());
        m_cartList->addItem(listItem);
        m_cartList->setItemWidget(listItem, itemWidget);
    }

    updateTotals();
}

void CartDialog::updateTotals()
{
    double total = 0;
    double discount = 0;
    int count = 0;

    for (const auto &item : m_items) {
        total += item.product.price() * item.quantity;
        if (item.product.hasDiscount()) {
            discount += (item.product.oldPrice() - item.product.price()) * item.quantity;
        }
        count += item.quantity;
    }

    m_itemsCountLabel->setText(QString("Товаров: %1").arg(count));
    m_discountLabel->setText(QString("Скидка: %1 ₽").arg(discount, 0, 'f', 0));
    m_totalLabel->setText(QString("Итого: %1 ₽").arg(total, 0, 'f', 0));
}

void CartDialog::removeItem(int productId)
{
    m_items.remove(productId);
    updateCartList();
    emit cartChanged();
}

void CartDialog::changeQuantity(int productId, int delta)
{
    if (m_items.contains(productId)) {
        m_items[productId].quantity += delta;
        if (m_items[productId].quantity <= 0) {
            m_items.remove(productId);
        }
        updateCartList();
        emit cartChanged();
    }
}
