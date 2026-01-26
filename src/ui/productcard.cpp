#include "productcard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

ProductCard::ProductCard(const Product& product, QWidget *parent)
    : QWidget(parent)
    , m_product(product)
    , m_imageLabel(nullptr)
    , m_discountLabel(nullptr)
    , m_nameLabel(nullptr)
    , m_brandLabel(nullptr)
    , m_priceLabel(nullptr)
    , m_oldPriceLabel(nullptr)
    , m_ratingLabel(nullptr)
    , m_addButton(nullptr)
    , m_shadowEffect(nullptr)
{
    setupUI();
}

void ProductCard::setupUI() {
    setFixedSize(220, 320);
    setObjectName("productCard");
    setCursor(Qt::PointingHandCursor);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(12, 12, 12, 12);

    // Контейнер изображения
    QWidget* imageContainer = new QWidget;
    imageContainer->setFixedHeight(150);
    imageContainer->setObjectName("imageContainer");

    QVBoxLayout* imageLayout = new QVBoxLayout(imageContainer);
    imageLayout->setContentsMargins(0, 0, 0, 0);

    // Плейсхолдер изображения (текстовая иконка категории)
    m_imageLabel = new QLabel;
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setObjectName("productImage");

    // Выбор иконки на основе категории
    QString iconText = "[P]";  // По умолчанию: Товар
    const std::string& category = m_product.getCategory();
    
    if (category == "Electronics") {
        iconText = "[E]";
    } else if (category == "Clothing") {
        iconText = "[C]";
    } else if (category == "Footwear") {
        iconText = "[F]";
    } else if (category == "Home & Garden") {
        iconText = "[H]";
    } else if (category == "Beauty") {
        iconText = "[B]";
    } else if (category == "Kids") {
        iconText = "[K]";
    } else if (category == "Sports") {
        iconText = "[S]";
    } else if (category == "Auto") {
        iconText = "[A]";
    }
    
    m_imageLabel->setText(iconText);
    m_imageLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #888;");
    imageLayout->addWidget(m_imageLabel);

    // Значок скидки
    if (m_product.getDiscount() > 0) {
        m_discountLabel = new QLabel(QString("-%1%").arg(m_product.getDiscount()));
        m_discountLabel->setObjectName("discountBadge");
        m_discountLabel->setFixedSize(45, 22);
        m_discountLabel->setAlignment(Qt::AlignCenter);
        m_discountLabel->move(8, 8);
        m_discountLabel->setParent(imageContainer);
    }

    layout->addWidget(imageContainer);

    // Бренд
    m_brandLabel = new QLabel(QString::fromStdString(m_product.getBrand()));
    m_brandLabel->setObjectName("brandLabel");
    layout->addWidget(m_brandLabel);

    // Название
    m_nameLabel = new QLabel(QString::fromStdString(m_product.getName()));
    m_nameLabel->setObjectName("nameLabel");
    m_nameLabel->setWordWrap(true);
    m_nameLabel->setMaximumHeight(36);
    layout->addWidget(m_nameLabel);

    // Рейтинг
    QHBoxLayout* ratingLayout = new QHBoxLayout;
    ratingLayout->setSpacing(4);
    ratingLayout->setContentsMargins(0, 0, 0, 0);

    m_ratingLabel = new QLabel(QString("* %1 (%2)")
                                   .arg(m_product.getRating(), 0, 'f', 1)
                                   .arg(m_product.getReviewCount()));
    m_ratingLabel->setObjectName("ratingLabel");
    ratingLayout->addWidget(m_ratingLabel);
    ratingLayout->addStretch();
    layout->addLayout(ratingLayout);

    // Цены
    QHBoxLayout* priceLayout = new QHBoxLayout;
    priceLayout->setSpacing(8);
    priceLayout->setContentsMargins(0, 0, 0, 0);

    m_priceLabel = new QLabel(QString("%1 $").arg(
        static_cast<int>(m_product.getPrice())));
    m_priceLabel->setObjectName("priceLabel");
    priceLayout->addWidget(m_priceLabel);

    if (m_product.getDiscount() > 0) {
        m_oldPriceLabel = new QLabel(QString("%1 $").arg(
            static_cast<int>(m_product.getOldPrice())));
        m_oldPriceLabel->setObjectName("oldPriceLabel");
        priceLayout->addWidget(m_oldPriceLabel);
    }

    priceLayout->addStretch();
    layout->addLayout(priceLayout);

    // Кнопка добавления в корзину
    m_addButton = new QPushButton("Add to Cart");
    m_addButton->setObjectName("addButton");
    m_addButton->setCursor(Qt::PointingHandCursor);
    m_addButton->setFixedHeight(36);
    layout->addWidget(m_addButton);

    connect(m_addButton, &QPushButton::clicked, this, [this]() {
        emit addToCartClicked(m_product);
    });

    // Эффект тени для карточки - создание один раз и повторное использование
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(15);
    m_shadowEffect->setColor(QColor(0, 0, 0, 25));
    m_shadowEffect->setOffset(0, 4);
    setGraphicsEffect(m_shadowEffect);
}

void ProductCard::updateProduct(const Product& product) {
    m_product = product;
    
    m_priceLabel->setText(QString("%1 $").arg(static_cast<int>(product.getPrice())));
    m_nameLabel->setText(QString::fromStdString(product.getName()));
    m_brandLabel->setText(QString::fromStdString(product.getBrand()));
    m_ratingLabel->setText(QString("* %1 (%2)")
                               .arg(product.getRating(), 0, 'f', 1)
                               .arg(product.getReviewCount()));
    
    if (m_oldPriceLabel) {
        m_oldPriceLabel->setText(QString("%1 $").arg(static_cast<int>(product.getOldPrice())));
    }
    
    if (m_discountLabel) {
        if (product.getDiscount() > 0) {
            m_discountLabel->setText(QString("-%1%").arg(product.getDiscount()));
            m_discountLabel->show();
        } else {
            m_discountLabel->hide();
        }
    }
}

void ProductCard::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit productClicked(m_product);
    }
    QWidget::mousePressEvent(event);
}

void ProductCard::enterEvent(QEnterEvent *event) {
    // Повторное использование существующего эффекта тени - просто изменение свойств
    if (m_shadowEffect) {
        m_shadowEffect->setBlurRadius(20);
        m_shadowEffect->setColor(QColor(0, 0, 0, 50));
        m_shadowEffect->setOffset(0, 6);
    }
    QWidget::enterEvent(event);
}

void ProductCard::leaveEvent(QEvent *event) {
    // Повторное использование существующего эффекта тени - восстановление исходных свойств
    if (m_shadowEffect) {
        m_shadowEffect->setBlurRadius(15);
        m_shadowEffect->setColor(QColor(0, 0, 0, 25));
        m_shadowEffect->setOffset(0, 4);
    }
    QWidget::leaveEvent(event);
}