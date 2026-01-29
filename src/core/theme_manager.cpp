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
    }
    return "wildberries";
}

ThemeType ThemeManager::stringToTheme(const QString& str)
{
    if (str == "light") {
        return ThemeType::Light;
    } else if (str == "dark") {
        return ThemeType::Dark;
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