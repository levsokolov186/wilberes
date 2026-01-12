#include "productdetaildialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

ProductDetailDialog::ProductDetailDialog(const Product &product, QWidget *parent)
    : QDialog(parent), m_product(product)
{
    setWindowTitle(product.name());
    setMinimumSize(700, 500);
    setupUI();
}

void ProductDetailDialog::setupUI()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(30);

    // Левая часть - изображение
    QVBoxLayout *imageLayout = new QVBoxLayout;

    QLabel *mainImage = new QLabel;
    mainImage->setPixmap(m_product.getImage(350, 350));
    mainImage->setFixedSize(350, 350);
    mainImage->setStyleSheet("border: 1px solid #ddd; border-radius: 12px;");
    imageLayout->addWidget(mainImage);

    // Мини-изображения
    QHBoxLayout *thumbLayout = new QHBoxLayout;
    for (int i = 0; i < 4; i++) {
        QLabel *thumb = new QLabel;
        thumb->setPixmap(m_product.getImage(70, 70));
        thumb->setFixedSize(70, 70);
        thumb->setStyleSheet("border: 1px solid #ddd; border-radius: 6px;");
        thumbLayout->addWidget(thumb);
    }
    imageLayout->addLayout(thumbLayout);
    imageLayout->addStretch();

    mainLayout->addLayout(imageLayout);

    // Правая часть - информация
    QVBoxLayout *infoLayout = new QVBoxLayout;

    // Бренд
    QLabel *brandLabel = new QLabel(m_product.brand());
    brandLabel->setStyleSheet("font-size: 14px; color: #666;");
    infoLayout->addWidget(brandLabel);

    // Название
    QLabel *nameLabel = new QLabel(m_product.name());
    nameLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #000;");
    nameLabel->setWordWrap(true);
    infoLayout->addWidget(nameLabel);

    // Рейтинг
    QLabel *ratingLabel = new QLabel(
        QString("★ %1  |  %2 отзывов")
            .arg(m_product.rating(), 0, 'f', 1)
            .arg(m_product.reviews()));
    ratingLabel->setStyleSheet("font-size: 14px; color: #f90;");
    infoLayout->addWidget(ratingLabel);

    infoLayout->addSpacing(20);

    // Цена
    QHBoxLayout *priceLayout = new QHBoxLayout;

    QLabel *priceLabel = new QLabel(
        QString("%1 ₽").arg(m_product.price(), 0, 'f', 0));
    priceLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #000;");
    priceLayout->addWidget(priceLabel);

    if (m_product.hasDiscount()) {
        QLabel *oldPriceLabel = new QLabel(
            QString("%1 ₽").arg(m_product.oldPrice(), 0, 'f', 0));
        oldPriceLabel->setStyleSheet(
            "font-size: 18px; color: #999; text-decoration: line-through;");
        priceLayout->addWidget(oldPriceLabel);

        QLabel *discountLabel = new QLabel(
            QString("-%1%").arg(m_product.discount()));
        discountLabel->setStyleSheet(
            "background: #cb11ab; color: white; padding: 5px 10px; "
            "border-radius: 4px; font-weight: bold;");
        priceLayout->addWidget(discountLabel);
    }
    priceLayout->addStretch();
    infoLayout->addLayout(priceLayout);

    infoLayout->addSpacing(20);

    // Описание
    QLabel *descTitle = new QLabel("Описание");
    descTitle->setStyleSheet("font-size: 16px; font-weight: bold;");
    infoLayout->addWidget(descTitle);

    QLabel *descLabel = new QLabel(m_product.description());
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet("font-size: 14px; color: #333; line-height: 1.5;");
    infoLayout->addWidget(descLabel);

    infoLayout->addSpacing(20);

    // Характеристики
    QLabel *specTitle = new QLabel("Характеристики");
    specTitle->setStyleSheet("font-size: 16px; font-weight: bold;");
    infoLayout->addWidget(specTitle);

    QGridLayout *specLayout = new QGridLayout;
    specLayout->addWidget(new QLabel("Категория:"), 0, 0);
    specLayout->addWidget(new QLabel(m_product.category()), 0, 1);
    specLayout->addWidget(new QLabel("Бренд:"), 1, 0);
    specLayout->addWidget(new QLabel(m_product.brand()), 1, 1);
    specLayout->addWidget(new QLabel("Артикул:"), 2, 0);
    specLayout->addWidget(new QLabel(QString::number(m_product.id())), 2, 1);
    infoLayout->addLayout(specLayout);

    infoLayout->addStretch();

    // Кнопки
    QHBoxLayout *btnLayout = new QHBoxLayout;

    QPushButton *addToCartBtn = new QPushButton("Добавить в корзину");
    addToCartBtn->setStyleSheet(
        "QPushButton { background-color: #cb11ab; color: white; "
        "border: none; padding: 15px 40px; border-radius: 10px; "
        "font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #a00e8a; }");
    connect(addToCartBtn, &QPushButton::clicked, [this]() {
        emit addToCart(m_product);
        accept();
    });
    btnLayout->addWidget(addToCartBtn);

    QPushButton *favoriteBtn = new QPushButton("♡ В избранное");
    favoriteBtn->setStyleSheet(
        "QPushButton { background: white; border: 2px solid #cb11ab; "
        "color: #cb11ab; padding: 15px 30px; border-radius: 10px; "
        "font-size: 16px; }"
        "QPushButton:hover { background: #fff0f8; }");
    btnLayout->addWidget(favoriteBtn);

    btnLayout->addStretch();
    infoLayout->addLayout(btnLayout);

    mainLayout->addLayout(infoLayout, 1);
}
