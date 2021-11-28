QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    build.cpp \
    collection.cpp \
    components.cpp \
    createbuilddialog.cpp \
    createcollectiondialog.cpp \
    dbmanagerwindow.cpp \
    idbmanager.cpp \
    main.cpp \
    pc_constructor.cpp \
    specifications.cpp \
    sqlitedbmanager.cpp

HEADERS += \
    build.h \
    collection.h \
    components.h \
    createbuilddialog.h \
    createcollectiondialog.h \
    dbmanagerwindow.h \
    idbmanager.h \
    pc_constructor.h \
    specifications.h \
    sqlitedbmanager.h

FORMS += \
    components.ui \
    createbuilddialog.ui \
    createcollectiondialog.ui \
    dbmanagerwindow.ui \
    pc_constructor.ui \
    specifications.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
