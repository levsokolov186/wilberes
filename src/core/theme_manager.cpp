#include "pch.h"
#include "theme_manager.h"

#include <QApplication>
#include <QFile>
#include <QPalette>
#include <QSettings>
#include <QDebug>

// Ключи для QSettings
static const QString SETTINGS_GROUP = "Appearance";
static const QString SETTINGS_THEME_KEY = "theme";

ThemeManager::ThemeManager(QObject* parent)
    : QObject(parent)
    , m_currentTheme(ThemeType::Wildberries)
{
}

ThemeManager& ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

void ThemeManager::applyTheme(ThemeType type)
{
    QString qssPath;
    
    switch (type) {
    case ThemeType::Light:
        qssPath = ":/styles/style_light.qss";
        break;
    case ThemeType::Dark:
        qssPath = ":/styles/style_dark.qss";
        break;
    case ThemeType::Wildberries:
        qssPath = ":/styles/style_wb.qss";
        break;
    case ThemeType::Glass:
        qssPath = ":/styles/style_glass.qss";
        break;
    case ThemeType::Modern:
        qssPath = ":/styles/style_modern.qss";
        break;
    }

    // Загружаем и применяем QSS
    QString styleSheet = loadStyleSheet(qssPath);
    if (!styleSheet.isEmpty()) {
        qApp->setStyleSheet(styleSheet);
    }

    // Настраиваем палитру
    setupPalette(type);

    // Сохраняем выбор
    m_currentTheme = type;
    saveTheme(type);

    emit themeChanged(type);
}

ThemeType ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::loadSavedTheme()
{
    QSettings settings;
    settings.beginGroup(SETTINGS_GROUP);
    
    QString savedTheme = settings.value(SETTINGS_THEME_KEY, "wildberries").toString();
    settings.endGroup();

    ThemeType type = stringToTheme(savedTheme);
    applyTheme(type);
}

QString ThemeManager::themeDisplayName(ThemeType type)
{
    switch (type) {
    case ThemeType::Light:
        return QObject::tr("Светлая");
    case ThemeType::Dark:
        return QObject::tr("Тёмная");
    case ThemeType::Wildberries:
        return QObject::tr("Wildberries");
    case ThemeType::Glass:
        return QObject::tr("Стекло");
    case ThemeType::Modern:
        return QObject::tr("Современная");
    }
    return QString();
}

QString ThemeManager::themeToString(ThemeType type)
{
    switch (type) {
    case ThemeType::Light:
        return "light";
    case ThemeType::Dark:
        return "dark";
    case ThemeType::Wildberries:
        return "wildberries";
    case ThemeType::Glass:
        return "glass";
    case ThemeType::Modern:
        return "modern";
    }
    return "wildberries";
}

ThemeType ThemeManager::stringToTheme(const QString& str)
{
    if (str == "light") {
        return ThemeType::Light;
    } else if (str == "dark") {
        return ThemeType::Dark;
    } else if (str == "glass") {
        return ThemeType::Glass;
    } else if (str == "modern") {
        return ThemeType::Modern;
    }
    return ThemeType::Wildberries;
}

QString ThemeManager::loadStyleSheet(const QString& resourcePath)
{
    QFile file(resourcePath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "ThemeManager: не удалось загрузить стиль" << resourcePath
                   << "- файл не найден или недоступен";
        return QString();
    }

    QString content = QString::fromUtf8(file.readAll());
    file.close();

    if (content.isEmpty()) {
        qWarning() << "ThemeManager: файл стилей пуст" << resourcePath;
    }

    return content;
}

void ThemeManager::setupPalette(ThemeType type)
{
    QPalette palette;

    switch (type) {
    case ThemeType::Light:
        // Светлая тема - классический светлый фон, тёмный текст
        palette.setColor(QPalette::Window, QColor("#f5f5f5"));
        palette.setColor(QPalette::WindowText, QColor("#333333"));
        palette.setColor(QPalette::Base, QColor("#ffffff"));
        palette.setColor(QPalette::AlternateBase, QColor("#f0f0f0"));
        palette.setColor(QPalette::Text, QColor("#333333"));
        palette.setColor(QPalette::Button, QColor("#e0e0e0"));
        palette.setColor(QPalette::ButtonText, QColor("#333333"));
        palette.setColor(QPalette::Highlight, QColor("#4a90d9"));
        palette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
        palette.setColor(QPalette::ToolTipBase, QColor("#333333"));
        palette.setColor(QPalette::ToolTipText, QColor("#ffffff"));
        palette.setColor(QPalette::Link, QColor("#4a90d9"));
        palette.setColor(QPalette::PlaceholderText, QColor("#888888"));
        
        // Disabled состояния
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#aaaaaa"));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#aaaaaa"));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#aaaaaa"));
        break;

    case ThemeType::Dark:
        // Тёмная тема - тёмный фон #1e1e1e, светлый текст, синие акценты
        palette.setColor(QPalette::Window, QColor("#1e1e1e"));
        palette.setColor(QPalette::WindowText, QColor("#e6e6e6"));
        palette.setColor(QPalette::Base, QColor("#252526"));
        palette.setColor(QPalette::AlternateBase, QColor("#2d2d30"));
        palette.setColor(QPalette::Text, QColor("#e6e6e6"));
        palette.setColor(QPalette::Button, QColor("#3c3c3c"));
        palette.setColor(QPalette::ButtonText, QColor("#e6e6e6"));
        palette.setColor(QPalette::Highlight, QColor("#3794ff"));
        palette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
        palette.setColor(QPalette::ToolTipBase, QColor("#252526"));
        palette.setColor(QPalette::ToolTipText, QColor("#e6e6e6"));
        palette.setColor(QPalette::Link, QColor("#3794ff"));
        palette.setColor(QPalette::PlaceholderText, QColor("#6e6e6e"));
        
        // Disabled состояния
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#5e5e5e"));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#5e5e5e"));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#5e5e5e"));
        break;

    case ThemeType::Wildberries:
        // Тема Wildberries - светлый фон, фиолетовые/розовые акценты #cb11ab
        palette.setColor(QPalette::Window, QColor("#f5f3f7"));
        palette.setColor(QPalette::WindowText, QColor("#333333"));
        palette.setColor(QPalette::Base, QColor("#ffffff"));
        palette.setColor(QPalette::AlternateBase, QColor("#faf8fc"));
        palette.setColor(QPalette::Text, QColor("#333333"));
        palette.setColor(QPalette::Button, QColor("#ffffff"));
        palette.setColor(QPalette::ButtonText, QColor("#333333"));
        palette.setColor(QPalette::Highlight, QColor("#cb11ab"));
        palette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
        palette.setColor(QPalette::ToolTipBase, QColor("#333333"));
        palette.setColor(QPalette::ToolTipText, QColor("#ffffff"));
        palette.setColor(QPalette::Link, QColor("#cb11ab"));
        palette.setColor(QPalette::PlaceholderText, QColor("#999999"));
        
        // Disabled состояния
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#b0b0b0"));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#b0b0b0"));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#b0b0b0"));
        break;

    case ThemeType::Glass:
        // Тема Glass - полупрозрачный стиль с градиентом, фиолетовые акценты
        palette.setColor(QPalette::Window, QColor(102, 126, 234));
        palette.setColor(QPalette::WindowText, QColor("#ffffff"));
        palette.setColor(QPalette::Base, QColor(255, 255, 255, 30));
        palette.setColor(QPalette::AlternateBase, QColor(255, 255, 255, 20));
        palette.setColor(QPalette::Text, QColor("#ffffff"));
        palette.setColor(QPalette::Button, QColor(255, 255, 255, 40));
        palette.setColor(QPalette::ButtonText, QColor("#ffffff"));
        palette.setColor(QPalette::Highlight, QColor("#6c5ce7"));
        palette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
        palette.setColor(QPalette::ToolTipBase, QColor(45, 45, 80, 242));
        palette.setColor(QPalette::ToolTipText, QColor("#ffffff"));
        palette.setColor(QPalette::Link, QColor("#a29bfe"));
        palette.setColor(QPalette::PlaceholderText, QColor(255, 255, 255, 128));
        
        // Disabled состояния
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(255, 255, 255, 102));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(255, 255, 255, 102));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(255, 255, 255, 102));
        break;

    case ThemeType::Modern:
        // Современная тёмная тема с фиолетовыми акцентами
        palette.setColor(QPalette::Window, QColor("#0f0f23"));
        palette.setColor(QPalette::WindowText, QColor("#e0e0e0"));
        palette.setColor(QPalette::Base, QColor("#1a1a2e"));
        palette.setColor(QPalette::AlternateBase, QColor("#12121f"));
        palette.setColor(QPalette::Text, QColor("#e0e0e0"));
        palette.setColor(QPalette::Button, QColor("#252540"));
        palette.setColor(QPalette::ButtonText, QColor("#ffffff"));
        palette.setColor(QPalette::Highlight, QColor("#764ba2"));
        palette.setColor(QPalette::HighlightedText, QColor("#ffffff"));
        palette.setColor(QPalette::ToolTipBase, QColor("#1a1a2e"));
        palette.setColor(QPalette::ToolTipText, QColor("#ffffff"));
        palette.setColor(QPalette::Link, QColor("#667eea"));
        palette.setColor(QPalette::PlaceholderText, QColor("#666688"));
        
        // Disabled состояния
        palette.setColor(QPalette::Disabled, QPalette::Text, QColor("#666688"));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor("#666688"));
        palette.setColor(QPalette::Disabled, QPalette::WindowText, QColor("#666688"));
        break;
    }

    qApp->setPalette(palette);
}

void ThemeManager::saveTheme(ThemeType type)
{
    QSettings settings;
    settings.beginGroup(SETTINGS_GROUP);
    settings.setValue(SETTINGS_THEME_KEY, themeToString(type));
    settings.endGroup();
    settings.sync();
}