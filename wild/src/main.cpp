#include <QApplication>
#include "mainwindow.h"
#include "theme_manager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Настройки приложения для QSettings
    QCoreApplication::setOrganizationName("MarketPlace");
    QCoreApplication::setApplicationName("MarketPlace");

    // Устанавливаем шрифт
    QFont font("Segoe UI", 10);
    app.setFont(font);

    // Инициализация и загрузка сохранённой темы
    ThemeManager::instance().loadSavedTheme();

    MainWindow window;
    window.show();

    return app.exec();
}