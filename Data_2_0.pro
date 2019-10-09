QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Visualizer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    newarrow.h \
    lnode.h \
    mainwindow.h \
    linklist.h \
    uidefine.h \
    mystack.h \
    myqueue.h \
    mycqueue.h

SOURCES += \
    newarrow.cpp \
    lnode.cpp \
    mainwindow.cpp \
    linklist.cpp \
    main.cpp \
    mystack.cpp \
    myqueue.cpp \
    mycqueue.cpp

FORMS += \
    mainwindow.ui \
    linklist.ui \
    mystack.ui \
    myqueue.ui \
    mycqueue.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

