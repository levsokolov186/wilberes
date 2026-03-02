#include "pch.h"
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
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    QLabel* titleLabel = new QLabel("Shopping Cart");
    titleLabel->setObjectName("pageTitle");
    mainLayout->addWidget(titleLabel);

    m_scrollArea = new QScrollArea;
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setObjectName("cartScrollArea");

    QWidget* scrollContent = new QWidget;
    m_itemsLayout = new QVBoxLayout(scrollContent);
    m_itemsLayout->setSpacing(16);
    m_itemsLayout->setAlignment(Qt::AlignTop);
    m_scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(m_scrollArea, 1);

    m_emptyLabel = new QLabel("Your cart is empty\n\nAdd items to place an order");
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setObjectName("subtitleLabel");
    m_emptyLabel->setStyleSheet("font-size: 16px; padding: 50px; color: #9ca3af;");
    mainLayout->addWidget(m_emptyLabel);

    QWidget* totalContainer = new QWidget;
    totalContainer->setObjectName("card");
    totalContainer->setMaximumWidth(450);

    QVBoxLayout* totalLayout = new QVBoxLayout(totalContainer);
    totalLayout->setContentsMargins(24, 24, 24, 24);
    totalLayout->setSpacing(18);

    m_discountLabel = new QLabel;
    m_discountLabel->setObjectName("discountLabel");
    m_discountLabel->setStyleSheet("font-size: 15px; color: #10b981; font-weight: 600;");
    totalLayout->addWidget(m_discountLabel);

    m_totalLabel = new QLabel;
    m_totalLabel->setObjectName("titleLabel");
    m_totalLabel->setStyleSheet("font-size: 26px; font-weight: 800; color: #1a1a2e;");
    totalLayout->addWidget(m_totalLabel);

    m_checkoutButton = new QPushButton("Checkout");
    m_checkoutButton->setObjectName("primaryButton");
    m_checkoutButton->setCursor(Qt::PointingHandCursor);
    m_checkoutButton->setMinimumHeight(50);
    m_checkoutButton->setStyleSheet("font-size: 15px; font-weight: 600; border-radius: 14px;");
    totalLayout->addWidget(m_checkoutButton);

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(totalContainer);
    shadow->setBlurRadius(25);
    shadow->setColor(QColor(124, 58, 237, 40));
    shadow->setOffset(0, 8);
    totalContainer->setGraphicsEffect(shadow);

    mainLayout->addWidget(totalContainer);

    connect(m_checkoutButton, &QPushButton::clicked, this, &CartWidget::checkoutClicked);

    updateCart();
}

QWidget* CartWidget::createCartItemWidget(const CartItem& item) {
    QWidget* widget = new QWidget;
    widget->setObjectName("card");

    QHBoxLayout* layout = new QHBoxLayout(widget);
    layout->setSpacing(18);
    layout->setContentsMargins(18, 18, 18, 18);

    QLabel* imageLabel = new QLabel;
    imageLabel->setFixedSize(70, 70);
    imageLabel->setAlignment(Qt::AlignCenter);
    
    QString iconText = "[P]";
    const std::string& category = item.product.getCategory();
    if (category == "Electronics") iconText = "[E]";
    else if (category == "Clothing") iconText = "[C]";
    else if (category == "Footwear") iconText = "[F]";
    
    imageLabel->setText(iconText);
    imageLabel->setObjectName("imageContainer");
    imageLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #9ca3af; background: #f3f4f6; border-radius: 12px;");
    layout->addWidget(imageLabel);

    QVBoxLayout* infoLayout = new QVBoxLayout;

    QLabel* nameLabel = new QLabel(QString::fromStdString(item.product.getName()));
    nameLabel->setObjectName("nameLabel");
    nameLabel->setWordWrap(true);
    nameLabel->setStyleSheet("font-size: 15px; font-weight: 600; color: #1a1a2e;");
    infoLayout->addWidget(nameLabel);

    QLabel* priceLabel = new QLabel(QString("%1 $").arg(
        static_cast<int>(item.product.getPrice())));
    priceLabel->setObjectName("priceLabel");
    priceLabel->setStyleSheet("font-size: 14px; color: #6b7280;");
    infoLayout->addWidget(priceLabel);

    layout->addLayout(infoLayout, 1);

    QVBoxLayout* qtyLayout = new QVBoxLayout;
    qtyLayout->setAlignment(Qt::AlignCenter);

    QSpinBox* qtySpinBox = new QSpinBox;
    qtySpinBox->setRange(1, std::max(1, item.product.getStock()));
    qtySpinBox->setValue(item.quantity);
    qtySpinBox->setMinimumWidth(80);
    qtySpinBox->setStyleSheet("padding: 8px 12px; border-radius: 8px;");

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

    QLabel* totalLabel = new QLabel(QString("%1 $").arg(
        static_cast<int>(item.getTotalPrice())));
    totalLabel->setObjectName("subtitleLabel");
    totalLabel->setStyleSheet("font-size: 15px; font-weight: 700; color: #7c3aed;");
    qtyLayout->addWidget(totalLabel);

    layout->addLayout(qtyLayout);

    QPushButton* removeBtn = new QPushButton("X");
    removeBtn->setObjectName("removeButton");
    removeBtn->setFixedSize(36, 36);
    removeBtn->setCursor(Qt::PointingHandCursor);
    removeBtn->setStyleSheet(R"(
        QPushButton {
            background: #fef2f2;
            color: #ef4444;
            border: none;
            border-radius: 10px;
            font-size: 14px;
            font-weight: bold;
        }
        QPushButton:hover {
            background: #ef4444;
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
    shadow->setBlurRadius(12);
    shadow->setColor(QColor(124, 58, 237, 20));
    shadow->setOffset(0, 4);
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