QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets quickwidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algrot.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    uialgrot.cpp \
    uimatrix.cpp \
    uivector.cpp \
    vector.cpp

HEADERS += \
    algrot.h \
    log.h \
    mainwindow.h \
    matrix.h \
    uialgrot.h \
    uimatrix.h \
    uivector.h \
    vector.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qml.qrc \
    resource.qrc


win32: LIBS += -LC:/Users/misha/miniconda3/Library/lib/ -lsymengine

INCLUDEPATH += C:/Users/misha/miniconda3/Library/include
DEPENDPATH += C:/Users/misha/miniconda3/Library/include


