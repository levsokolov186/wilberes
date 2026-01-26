#include "cartwidget.h"
#include <QHBoxLayout>
#include <QSpinBox>
#include <QGraphicsDropShadowEffect>

CartWidget::CartWidget(Cart* cart, QWidget *parent)
    : QWidget(parent)
    , m_cart(cart)
    , m_itemsLayout(nullptr)
    , m_totalLabel(nullptr)
    , m_discountLabel(nullptr)
    , m_emptyLabel(nullptr)
    , m_checkoutButton(nullptr)
    , m_scrollArea(nullptr)
{
    setupUI();
}

void CartWidget::setCart(Cart* cart) {
    m_cart = cart;
    updateCart();
}

void CartWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Заголовок
    QLabel* titleLabel = new QLabel("Shopping Cart");
    titleLabel->setObjectName("pageTitle");
    mainLayout->addWidget(titleLabel);

    // Область прокрутки для элементов
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

    // Метка пустой корзины
    m_emptyLabel = new QLabel("Your cart is empty\n\nAdd items to place an order");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setObjectName("subtitleLabel");
    m_emptyLabel->setStyleSheet("font-size: 16px; padding: 40px;");
    mainLayout->addWidget(m_emptyLabel);

    // Контейнер итогов
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

    // Кнопка оформления заказа
    m_checkoutButton = new QPushButton("Checkout");
    m_checkoutButton->setObjectName("primaryButton");
    m_checkoutButton->setCursor(Qt::PointingHandCursor);
    totalLayout->addWidget(m_checkoutButton);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(totalContainer);
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

    // Плейсхолдер иконки товара
    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(60, 60);
    imageLabel->setAlignment(Qt::AlignCenter);
    
    // Используем текстовую иконку на основе категории вместо эмодзи
    QString iconText = "[P]";
    const std::string& category = item.product.getCategory();
    if (category == "Electronics") iconText = "[E]";
    else if (category == "Clothing") iconText = "[C]";
    else if (category == "Footwear") iconText = "[F]";
    
    imageLabel->setText(iconText);
    imageLabel->setObjectName("imageContainer");
    imageLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #888;");
    layout->addWidget(imageLabel);

    // Информация о товаре
    QVBoxLayout* infoLayout = new QVBoxLayout;

    QLabel* nameLabel = new QLabel(QString::fromStdString(item.product.getName()));
    nameLabel->setObjectName("nameLabel");
    nameLabel->setWordWrap(true);
    infoLayout->addWidget(nameLabel);

    QLabel* priceLabel = new QLabel(QString("%1 $").arg(
        static_cast<int>(item.product.getPrice())));
    priceLabel->setObjectName("priceLabel");
    infoLayout->addWidget(priceLabel);

    layout->addLayout(infoLayout, 1);

    // Элементы управления количеством
    QVBoxLayout* qtyLayout = new QVBoxLayout;
    qtyLayout->setAlignment(Qt::AlignCenter);

    QSpinBox* qtySpinBox = new QSpinBox;
    qtySpinBox->setRange(1, std::max(1, item.product.getStock()));
    qtySpinBox->setValue(item.quantity);
    qtySpinBox->setMinimumWidth(70);

    const int productId = item.product.getId();
    connect(qtySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this, productId](int value) {
                if (m_cart) {
                    m_cart->updateQuantity(productId, value);
                    updateCart();
                    emit cartUpdated();
                }
            });
    qtyLayout->addWidget(qtySpinBox);

    // Общая цена элемента
    QLabel* totalLabel = new QLabel(QString("%1 $").arg(
        static_cast<int>(item.getTotalPrice())));
    totalLabel->setObjectName("subtitleLabel");
    qtyLayout->addWidget(totalLabel);

    layout->addLayout(qtyLayout);

    // Кнопка удаления
    QPushButton* removeBtn = new QPushButton("X");
    removeBtn->setObjectName("removeButton");
    removeBtn->setFixedSize(30, 30);
    removeBtn->setCursor(Qt::PointingHandCursor);
    removeBtn->setStyleSheet(R"(
        QPushButton {
            background: rgba(231, 76, 60, 0.1);
            color: #e74c3c;
            border: none;
            border-radius: 15px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: #e74c3c;
            color: white;
        }
    )");

    connect(removeBtn, &QPushButton::clicked, this, [this, productId]() {
        if (m_cart) {
            m_cart->removeItem(productId);
            updateCart();
            emit cartUpdated();
        }
    });
    layout->addWidget(removeBtn);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 20));
    shadow->setOffset(0, 3);
    widget->setGraphicsEffect(shadow);

    return widget;
}

void CartWidget::updateCart() {
    // Очистка старых элементов
    QLayoutItem* child;
    while ((child = m_itemsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    const bool isEmpty = !m_cart || m_cart->isEmpty();
    m_emptyLabel->setVisible(isEmpty);
    m_scrollArea->setVisible(!isEmpty);

    if (!isEmpty) {
        const auto items = m_cart->getItems();
        for (const auto& item : items) {
            m_itemsLayout->addWidget(createCartItemWidget(item));
        }
    }

    // Обновление итогов
    const double total = m_cart ? m_cart->getTotalPrice() : 0.0;
    const double discount = m_cart ? m_cart->getTotalDiscount() : 0.0;

    if (discount > 0) {
        m_discountLabel->setText(QString("Discount: -%1 $").arg(static_cast<int>(discount)));
        m_discountLabel->show();
    } else {
        m_discountLabel->hide();
    }

    m_totalLabel->setText(QString("Total: %1 $").arg(static_cast<int>(total)));
    m_checkoutButton->setEnabled(!isEmpty);
}