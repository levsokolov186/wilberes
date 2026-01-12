QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = WildberriesLocal
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    product.cpp \
    productcard.cpp \
    cartdialog.cpp \
    productdetaildialog.cpp \
    user.cpp \
    logindialog.cpp \
    adminpanel.cpp \
    producteditor.cpp \
    datamanager.cpp

HEADERS += \
    mainwindow.h \
    product.h \
    productcard.h \
    cartdialog.h \
    productdetaildialog.h \
    user.h \
    logindialog.h \
    adminpanel.h \
    producteditor.h \
    datamanager.h

RESOURCES += resources.qrc
