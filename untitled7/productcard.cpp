#include "productcard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

ProductCard::ProductCard(const Product &product, QWidget *parent)
    : QWidget(parent), m_product(product)
{
    setupUI();
    setFixedSize(220, 380);
    setCursor(Qt::PointingHandCursor);
    updateStyle(false);
}

void ProductCard::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(5);

    // Изображение товара
    m_imageLabel = new QLabel;
    m_imageLabel->setPixmap(m_product.getImage(200, 200));
    m_imageLabel->setFixedSize(200, 200);
    m_imageLabel->setScaledContents(true);
    mainLayout->addWidget(m_imageLabel, 0, Qt::AlignCenter);

    // Скидка (если есть)
    if (m_product.hasDiscount()) {
        m_discountLabel = new QLabel(QString("-%1%").arg(m_product.discount()));
        m_discountLabel->setStyleSheet(
            "background-color: #cb11ab; color: white; "
            "padding: 3px 8px; border-radius: 4px; font-weight: bold;");
        m_discountLabel->setFixedSize(50, 25);
        m_discountLabel->move(15, 15);
        m_discountLabel->setParent(m_imageLabel);
        m_discountLabel->show();
    }

    // Кнопка избранного
    m_favoriteBtn = new QPushButton("♡");
    m_favoriteBtn->setFixedSize(30, 30);
    m_favoriteBtn->setStyleSheet(
        "QPushButton { background: white; border: 1px solid #ddd; "
        "border-radius: 15px; font-size: 16px; }"
        "QPushButton:hover { background: #ffe0f0; color: #cb11ab; }");
    m_favoriteBtn->setParent(m_imageLabel);
    m_favoriteBtn->move(165, 10);
    m_favoriteBtn->show();
    connect(m_favoriteBtn, &QPushButton::clicked, [this]() {
        emit addToFavorites(m_product);
        m_favoriteBtn->setText("♥");
        m_favoriteBtn->setStyleSheet(
            "QPushButton { background: white; border: 1px solid #ddd; "
            "border-radius: 15px; font-size: 16px; color: #cb11ab; }");
    });

    // Блок цены
    QHBoxLayout *priceLayout = new QHBoxLayout;

    m_priceLabel = new QLabel(QString("%1 ₽").arg(m_product.price(), 0, 'f', 0));
    m_priceLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #000;");
    priceLayout->addWidget(m_priceLabel);

    if (m_product.hasDiscount()) {
        m_oldPriceLabel = new QLabel(QString("%1 ₽").arg(m_product.oldPrice(), 0, 'f', 0));
        m_oldPriceLabel->setStyleSheet(
            "font-size: 14px; color: #999; text-decoration: line-through;");
        priceLayout->addWidget(m_oldPriceLabel);
    }
    priceLayout->addStretch();
    mainLayout->addLayout(priceLayout);

    // Бренд
    m_brandLabel = new QLabel(m_product.brand());
    m_brandLabel->setStyleSheet("color: #999; font-size: 12px;");
    mainLayout->addWidget(m_brandLabel);

    // Название товара
    m_nameLabel = new QLabel(m_product.name());
    m_nameLabel->setWordWrap(true);
    m_nameLabel->setMaximumHeight(40);
    m_nameLabel->setStyleSheet("font-size: 13px; color: #333;");
    mainLayout->addWidget(m_nameLabel);

    // Рейтинг
    m_ratingLabel = new QLabel(QString("%1 ★ %2 отзывов")
                                   .arg(m_product.rating(), 0, 'f', 1)
                                   .arg(m_product.reviews()));
    m_ratingLabel->setStyleSheet("color: #f90; font-size: 12px;");
    mainLayout->addWidget(m_ratingLabel);

    // Кнопка "В корзину"
    m_addToCartBtn = new QPushButton("В корзину");
    m_addToCartBtn->setStyleSheet(
        "QPushButton { background-color: #cb11ab; color: white; "
        "border: none; padding: 10px; border-radius: 8px; font-size: 14px; }"
        "QPushButton:hover { background-color: #a00e8a; }"
        "QPushButton:pressed { background-color: #8a0c75; }");
    connect(m_addToCartBtn, &QPushButton::clicked, [this]() {
        emit addToCart(m_product);
    });
    mainLayout->addWidget(m_addToCartBtn);
}

void ProductCard::enterEvent(QEnterEvent *event)
{
    updateStyle(true);
    QWidget::enterEvent(event);
}

void ProductCard::leaveEvent(QEvent *event)
{
    updateStyle(false);
    QWidget::leaveEvent(event);
}

void ProductCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit viewDetails(m_product);
    }
    QWidget::mousePressEvent(event);
}

void ProductCard::updateStyle(bool hovered)
{
    if (hovered) {
        setStyleSheet("ProductCard { background: white; border: 1px solid #cb11ab; "
                      "border-radius: 12px; }");
    } else {
        setStyleSheet("ProductCard { background: white; border: 1px solid #eee; "
                      "border-radius: 12px; }");
    }
}
