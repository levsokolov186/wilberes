#include <QApplication>
#include <QFont>
#include <QStyleFactory>

#include "mainwindow.h"
#include "theme_manager.h"

int main(int argc, char *argv[]) {
    // Включение масштабирования для высокого DPI
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app(argc, argv);

    // Метаданные приложения для QSettings
    QCoreApplication::setOrganizationName("MarketPlace");
    QCoreApplication::setOrganizationDomain("marketplace.local");
    QCoreApplication::setApplicationName("MarketPlace");
    QCoreApplication::setApplicationVersion("1.0.0");

    // Установка шрифта по умолчанию
    QFont defaultFont("Segoe UI", 10);
    defaultFont.setStyleHint(QFont::SansSerif);
    app.setFont(defaultFont);

    // Инициализация и загрузка сохраненной темы
    ThemeManager::instance().loadSavedTheme();

    // Создание и отображение главного окна
    MainWindow window;
    window.show();

    return app.exec();
}