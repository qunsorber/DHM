#-------------------------------------------------
#
# Project created by QtCreator 2021-01-20T13:41:21
#
#-------------------------------------------------

QT       += core gui

QT       += datavisualization

QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = camera3d
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH +=D:\opencv\build\include

CONFIG(release,debug|release):LIBS += -LD:\opencv\build\x64\vc14\lib -lopencv_world450
else:CONFIG(debug,debug|release):LIBS += -LD:\opencv\build\x64\vc14\lib -lopencv_world450d

PYLON_DEV_DIR = C:\Program Files\Basler\pylon 5\Development
INCLUDEPATH += \
$$(PYLON_DEV_DIR)/include \
$$(PYLON_DEV_DIR)/include/pylon

LIBS += \
-L$$(PYLON_DEV_DIR)/lib/x64

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    paramset.cpp

HEADERS += \
        mainwindow.h \
    TransDHMDll.h \
    paramset.h

FORMS += \
        mainwindow.ui \
    paramset.ui

RESOURCES += \
    res.qrc

