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
    mycqueue.h \
    avltree.h \
    newedge.h \
    newnode.h \
    myavltree.h \
    bsttree.h \
    mybsttree.h \
    newbnode.h \
    newbedge.h

SOURCES += \
    newarrow.cpp \
    lnode.cpp \
    mainwindow.cpp \
    linklist.cpp \
    main.cpp \
    mystack.cpp \
    myqueue.cpp \
    mycqueue.cpp \
    avltree.cpp \
    newedge.cpp \
    newnode.cpp \
    myavltree.cpp \
    bsttree.cpp \
    mybsttree.cpp \
    newbnode.cpp \
    newbedge.cpp

FORMS += \
    mainwindow.ui \
    linklist.ui \
    mystack.ui \
    myqueue.ui \
    mycqueue.ui \
    myavltree.ui \
    mybsttree.ui

DISTFILES +=

RESOURCES += \
    resources.qrc

