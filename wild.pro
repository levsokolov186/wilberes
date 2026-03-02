QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# === Профили сборки ===
# Debug: отладочная сборка с информацией для отладчика
CONFIG(debug, debug|release) {
    DEFINES += QT_DEBUG
}
# Release: оптимизированная сборка
CONFIG(release, debug|release) {
    # Оптимизация по скорости (-O3)
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3
    # LTO для лучшей оптимизации между файлами
    QMAKE_CXXFLAGS_RELEASE += -flto
    QMAKE_LFLAGS_RELEASE += -flto
    # Убираем отладочную информацию для уменьшения размера
    QMAKE_CXXFLAGS_RELEASE += -s
    # Оптимизация размера
    QMAKE_CXXFLAGS_RELEASE += -ffunction-sections -fdata-sections
    QMAKE_LFLAGS_RELEASE += -Wl,--gc-sections
}

# === Предкомпилированные заголовки ===
CONFIG += precompile_header
PRECOMPILED_HEADER = include/pch.h

# === Дополнительные настройки ===
# Убрать предупреждения о неиспользуемых параметрах
CONFIG += warn_off

# Пути для подключения заголовков
INCLUDEPATH += \
    include \
    include/core \
    include/models \
    include/ui

# Вы можете заставить код не компилироваться, если он использует устаревшие API.
# Для этого раскомментируйте следующую строку.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # отключает все API, устаревшие до Qt 6.0.0

# === Исходные файлы ===

SOURCES += \
    src/main.cpp \
    # Ядро
    src/core/database.cpp \
    src/core/theme_manager.cpp \
    # Модели
    src/models/cart.cpp \
    src/models/product.cpp \
    src/models/user.cpp \
    # Интерфейс
    src/ui/adminpanel.cpp \
    src/ui/authwindow.cpp \
    src/ui/cartwidget.cpp \
    src/ui/mainwindow.cpp \
    src/ui/productcard.cpp \
    src/ui/profilewidget.cpp

# === Заголовочные файлы ===

HEADERS += \
    include/pch.h \
    # Ядро
    include/core/database.h \
    include/core/theme_manager.h \
    # Модели
    include/models/cart.h \
    include/models/product.h \
    include/models/user.h \
    # Интерфейс
    include/ui/adminpanel.h \
    include/ui/authwindow.h \
    include/ui/cartwidget.h \
    include/ui/mainwindow.h \
    include/ui/productcard.h \
    include/ui/profilewidget.h

# === Формы интерфейса ===

FORMS += \
    forms/mainwindow.ui

# === Ресурсы ===

RESOURCES += \
    resources/styles/styles.qrc

# Правила развертывания по умолчанию.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
