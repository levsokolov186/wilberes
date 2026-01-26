QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Include paths
INCLUDEPATH += \
    include \
    include/core \
    include/models \
    include/ui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# === Source Files ===

SOURCES += \
    src/main.cpp \
    # Core
    src/core/database.cpp \
    src/core/theme_manager.cpp \
    # Models
    src/models/cart.cpp \
    src/models/product.cpp \
    src/models/user.cpp \
    # UI
    src/ui/adminpanel.cpp \
    src/ui/cartwidget.cpp \
    src/ui/loginwindow.cpp \
    src/ui/mainwindow.cpp \
    src/ui/productcard.cpp \
    src/ui/profilewidget.cpp \
    src/ui/registerwindow.cpp

# === Header Files ===

HEADERS += \
    # Core
    include/core/database.h \
    include/core/theme_manager.h \
    # Models
    include/models/cart.h \
    include/models/product.h \
    include/models/user.h \
    # UI
    include/ui/adminpanel.h \
    include/ui/cartwidget.h \
    include/ui/loginwindow.h \
    include/ui/mainwindow.h \
    include/ui/productcard.h \
    include/ui/profilewidget.h \
    include/ui/registerwindow.h

# === UI Forms ===

FORMS += \
    forms/mainwindow.ui

# === Resources ===

RESOURCES += \
    resources/styles/styles.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
