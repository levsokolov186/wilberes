#include "cartwidget.h"
#include <QHBoxLayout>
#include <QSpinBox>
#include <QGraphicsDropShadowEffect>

CartWidget::CartWidget(Cart* cart, QWidget *parent)
    : QWidget(parent), m_cart(cart) {
    setupUI();
}

void CartWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Заголовок
    QLabel* titleLabel = new QLabel("🛒 Корзина");
    titleLabel->setObjectName("pageTitle");
    mainLayout->addWidget(titleLabel);

    // Скролл область для товаров
    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setObjectName("cartScrollArea");

    QWidget* scrollContent = new QWidget;
    m_itemsLayout = new QVBoxLayout(scrollContent);
    m_itemsLayout->setSpacing(10);
    m_itemsLayout->setAlignment(Qt::AlignTop);
    m_scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(m_scrollArea, 1);

    // Пустая корзина
    m_emptyLabel = new QLabel("Корзина пуста\n🛒\nДобавьте товары для оформления заказа");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setObjectName("subtitleLabel");
    m_emptyLabel->setStyleSheet("font-size: 16px; padding: 40px;");
    mainLayout->addWidget(m_emptyLabel);

    // Итого
    QWidget* totalContainer = new QWidget;
    totalContainer->setObjectName("card");
    totalContainer->setMaximumWidth(500);

    QVBoxLayout* totalLayout = new QVBoxLayout(totalContainer);
    totalLayout->setContentsMargins(20, 20, 20, 20);
    totalLayout->setSpacing(15);

    m_discountLabel = new QLabel;
    m_discountLabel->setObjectName("discountLabel");
    m_discountLabel->setStyleSheet("font-size: 14px; color: #27ae60;");
    totalLayout->addWidget(m_discountLabel);

    m_totalLabel = new QLabel;
    m_totalLabel->setObjectName("titleLabel");
    totalLayout->addWidget(m_totalLabel);

    // Кнопка оформления
    m_checkoutButton = new QPushButton("Оформить заказ");
    m_checkoutButton->setObjectName("primaryButton");
    m_checkoutButton->setCursor(Qt::PointingHandCursor);
    totalLayout->addWidget(m_checkoutButton);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 30));
    shadow->setOffset(0, 5);
    totalContainer->setGraphicsEffect(shadow);

    mainLayout->addWidget(totalContainer);

    connect(m_checkoutButton, &QPushButton::clicked, this, &CartWidget::checkoutClicked);

    updateCart();
}

QWidget* CartWidget::createCartItemWidget(const CartItem& item) {
    QWidget* widget = new QWidget;
    widget->setObjectName("card");

    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setSpacing(15);
    layout->setContentsMargins(15, 15, 15, 15);

    // Эмодзи товара
    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(60, 60);
    imageLabel->setAlignment(Qt::AlignCenter);
    QString emoji = "📦";
    if (item.product.getCategory() == "Электроника") emoji = "📱";
    else if (item.product.getCategory() == "Одежда") emoji = "👕";
    else if (item.product.getCategory() == "Обувь") emoji = "👟";
    imageLabel->setText(emoji);
    imageLabel->setObjectName("imageContainer");
    imageLabel->setStyleSheet("font-size: 30px;");
    layout->addWidget(imageLabel);

    // Информация
    QVBoxLayout* infoLayout = new QVBoxLayout;

    QLabel* nameLabel = new QLabel(QString::fromStdString(item.product.getName()));
    nameLabel->setObjectName("nameLabel");
    nameLabel->setWordWrap(true);
    infoLayout->addWidget(nameLabel);

    QLabel* priceLabel = new QLabel(QString("%1 ₽").arg(
        static_cast<int>(item.product.getPrice())));
    priceLabel->setObjectName("priceLabel");
    infoLayout->addWidget(priceLabel);

    layout->addLayout(infoLayout, 1);

    // Количество
    QVBoxLayout* qtyLayout = new QVBoxLayout;
    qtyLayout->setAlignment(Qt::AlignCenter);

    QSpinBox* qtySpinBox = new QSpinBox;
    qtySpinBox->setRange(1, item.product.getStock());
    qtySpinBox->setValue(item.quantity);
    qtySpinBox->setMinimumWidth(70);

    int productId = item.product.getId();
    connect(qtySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [this, productId](int value) {
                m_cart->updateQuantity(productId, value);
                updateCart();
                emit cartUpdated();
            });
    qtyLayout->addWidget(qtySpinBox);

    // Итого за позицию
    QLabel* totalLabel = new QLabel(QString("%1 ₽").arg(
        static_cast<int>(item.getTotalPrice())));
    totalLabel->setObjectName("subtitleLabel");
    qtyLayout->addWidget(totalLabel);

    layout->addLayout(qtyLayout);

    // Кнопка удаления
    QPushButton* removeBtn = new QPushButton("✕");
    removeBtn->setObjectName("removeButton");
    removeBtn->setFixedSize(30, 30);
    removeBtn->setCursor(Qt::PointingHandCursor);
    removeBtn->setStyleSheet(R"(
        QPushButton {
            background: rgba(231, 76, 60, 0.1);
            color: #e74c3c;
            border: none;
            border-radius: 15px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: #e74c3c;
            color: white;
        }
    )");

    connect(removeBtn, &QPushButton::clicked, [this, productId]() {
        m_cart->removeItem(productId);
        updateCart();
        emit cartUpdated();
    });
    layout->addWidget(removeBtn);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 20));
    shadow->setOffset(0, 3);
    widget->setGraphicsEffect(shadow);

    return widget;
}

void CartWidget::updateCart() {
    // Очищаем старые элементы
    QLayoutItem* child;
    while ((child = m_itemsLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    bool isEmpty = !m_cart || m_cart->isEmpty();
    m_emptyLabel->setVisible(isEmpty);
    m_scrollArea->setVisible(!isEmpty);

    if (!isEmpty) {
        for (const auto& item : m_cart->getItems()) {
            m_itemsLayout->addWidget(createCartItemWidget(item));
        }
    }

    // Обновляем итого
    double total = m_cart ? m_cart->getTotalPrice() : 0;
    double discount = m_cart ? m_cart->getTotalDiscount() : 0;

    if (discount > 0) {
        m_discountLabel->setText(QString("Скидка: -%1 ₽").arg(static_cast<int>(discount)));
        m_discountLabel->show();
    } else {
        m_discountLabel->hide();
    }

    m_totalLabel->setText(QString("Итого: %1 ₽").arg(static_cast<int>(total)));
    m_checkoutButton->setEnabled(!isEmpty);
}