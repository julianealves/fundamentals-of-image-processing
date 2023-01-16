QT += core gui
QT += charts


HEADERS = photoprocessor.h \
          mainwindow.h

SOURCES = main.cpp \
          photoprocessor.cpp \
          mainwindow.cpp

FORMS += \
        mainwindow.ui

target.path = $$[QT_INSTALL_EXAMPLES]/charts/barchart
INSTALLS += target
