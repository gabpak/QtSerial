QT       += core gui serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = ressources/ece.ico

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    seriallink.cpp \
    sqlmanagement.cpp

HEADERS += \
    mainwindow.h \
    seriallink.h \
    sqlmanagement.h

FORMS += \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../wamp64/bin/mysql/mysql8.2.0/lib -llibmysql

INCLUDEPATH += $$PWD/../../../../../../wamp64/bin/mysql/mysql8.2.0/lib
DEPENDPATH += $$PWD/../../../../../../wamp64/bin/mysql/mysql8.2.0/lib
