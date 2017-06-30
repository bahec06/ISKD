#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T13:30:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

QT += printsupport

TARGET = iskd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pulseform.cpp \
    spectrum.cpp \
    qt_ni_fgen.cpp \
    qt_ni_fpga.cpp \
    qt_ni_p2p.cpp \
    qcustomplot.cpp \
    form_options.cpp \
    spec_options.cpp \
    model_file_generator.cpp \
    bgif_generator.cpp \
    global_options.cpp \
    react_constants.cpp \
    wdg_params.cpp

HEADERS  += mainwindow.h \
    pulseform.h \
    spectrum.h \
    qt_ni_fgen.h \
    qt_ni_fpga.h \
    qt_ni_p2p.h \
    qcustomplot.h \
    form_options.h \
    spec_options.h \
    model_file_generator.h \
    service_types.h \
    bgif_generator.h \
    global_options.h \
    react_constants.h \
    wdg_params.h

FORMS    += mainwindow.ui \
    form_options.ui \
    spec_options.ui \
    global_options.ui

INCLUDEPATH += D:\Qt_repo\iskd\libs
INCLUDEPATH += C:\Users\Ivan\Downloads\QCustomPlot

win32: LIBS += -L$$PWD/libs/ -lniFgen

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32: LIBS += -L$$PWD/libs/ -lNiFpga

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs

win32: LIBS += -L$$PWD/libs/ -lnip2p

INCLUDEPATH += $$PWD/libs
DEPENDPATH += $$PWD/libs
