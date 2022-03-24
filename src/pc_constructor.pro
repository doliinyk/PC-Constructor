QT += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui
unix: OBJECTS_DIR = unix
win32: OBJECTS_DIR = win32
macx: OBJECT_DIR = mac

SOURCES += \
    componentsmanager.cpp \
    componentswidget.cpp \
    createbuilddialog.cpp \
    idbmanager.cpp \
    main.cpp \
    pc_constructor.cpp \
    singlecomponentwidget.cpp \
    specificationswidget.cpp \
    sqlitedbmanager.cpp

HEADERS += \
    componentsmanager.h \
    componentswidget.h \
    createbuilddialog.h \
    idbmanager.h \
    pc_constructor.h \
    singlecomponentwidget.h \
    specificationswidget.h \
    sqlitedbmanager.h

FORMS += \
    componentsmanager.ui \
    componentswidget.ui \
    createbuilddialog.ui \
    pc_constructor.ui \
    singlecomponentwidget.ui \
    specificationswidget.ui

CONFIG(release, debug|release) {
    win32: QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt $$OUT_PWD/release
}

RC_ICONS = pc_constructor_icon.ico

