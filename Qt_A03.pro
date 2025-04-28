# project.pro
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = QtGameApp
TEMPLATE = app

SOURCES += \
    court.cpp \
    main.cpp \
    loginwindow.cpp \
    userdatabase.cpp \
    gamewindow.cpp \
    user.cpp

HEADERS += \
    court.h \
    loginwindow.h \
    userdatabase.h \
    gamewindow.h \
    user.h

RESOURCES += \
    resources.qrc \
    resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
