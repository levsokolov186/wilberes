#include "productcard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

ProductCard::ProductCard(const Product& product, QWidget *parent)
    : QWidget(parent), m_product(product) {
    setupUI();
}

void ProductCard::setupUI() {
    setFixedSize(220, 320);
    setObjectName("productCard");
    setCursor(Qt::PointingHandCursor);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(8);
    layout->setContentsMargins(12, 12, 12, 12);

    // Контейнер для изображения
    QWidget* imageContainer = new QWidget;
    imageContainer->setFixedHeight(150);
    imageContainer->setObjectName("imageContainer");

    QVBoxLayout* imageLayout = new QVBoxLayout(imageContainer);
    imageLayout->setContentsMargins(0, 0, 0, 0);

    // Изображение (эмодзи как заглушка)
    m_imageLabel = new QLabel;
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setObjectName("productImage");

    // Выбираем эмодзи в зависимости от категории
    QString emoji = "📦";
    QString category = QString::fromStdString(m_product.getCategory());
    if (category == "Электроника") emoji = "📱";
    else if (category == "Одежда") emoji = "👕";
    else if (category == "Обувь") emoji = "👟";
    else if (category == "Дом и сад") emoji = "🏠";
    else if (category == "Красота") emoji = "💄";
    else if (category == "Детские товары") emoji = "🧸";
    else if (category == "Спорт") emoji = "⚽";
    else if (category == "Авто") emoji = "🚗";
    m_imageLabel->setText(emoji);

    imageLayout->addWidget(m_imageLabel);

    // Скидка
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

    m_ratingLabel = new QLabel(QString("★ %1 (%2)")
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

    m_priceLabel = new QLabel(QString("%1 ₽").arg(
        static_cast<int>(m_product.getPrice())));
    m_priceLabel->setObjectName("priceLabel");
    priceLayout->addWidget(m_priceLabel);

    if (m_product.getDiscount() > 0) {
        m_oldPriceLabel = new QLabel(QString("%1 ₽").arg(
            static_cast<int>(m_product.getOldPrice())));
        m_oldPriceLabel->setObjectName("oldPriceLabel");
        priceLayout->addWidget(m_oldPriceLabel);
    }

    priceLayout->addStretch();
    layout->addLayout(priceLayout);

    // Кнопка добавить в корзину
    m_addButton = new QPushButton("В корзину");
    m_addButton->setObjectName("addButton");
    m_addButton->setCursor(Qt::PointingHandCursor);
    m_addButton->setFixedHeight(36);
    layout->addWidget(m_addButton);

    connect(m_addButton, &QPushButton::clicked, [this]() {
        emit addToCartClicked(m_product);
    });

    // Тень для карточки
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 25));
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);
}

void ProductCard::updateProduct(const Product& product) {
    m_product = product;
    m_priceLabel->setText(QString("%1 ₽").arg(static_cast<int>(product.getPrice())));
}

void ProductCard::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit productClicked(m_product);
    }
    QWidget::mousePressEvent(event);
}

void ProductCard::enterEvent(QEnterEvent *event) {
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 50));
    shadow->setOffset(0, 6);
    setGraphicsEffect(shadow);
    QWidget::enterEvent(event);
}

void ProductCard::leaveEvent(QEvent *event) {
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(15);
    shadow->setColor(QColor(0, 0, 0, 25));
    shadow->setOffset(0, 4);
    setGraphicsEffect(shadow);
    QWidget::leaveEvent(event);
}