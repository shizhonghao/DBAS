#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T23:44:33
#
#-------------------------------------------------

QT += core gui
QT += sql
CONFIG(debug,debug|release){
LIBS += -lQt5AxContainerd
LIBS += -lQt5AxBased
}else{
LIBS += -lQt5AxContainer
LIBS += -lQt5AxBase
}

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = db_sys
TEMPLATE = app


SOURCES += main.cpp\
        db_sys.cpp \
    tab_data_io.cpp \
    tab_interfere_analysis.cpp \
    tab_info_query.cpp \
    act_data_io.cpp \
    graph.cpp

HEADERS  += db_sys.h \
    tab_data_io.h \
    tab_interfere_analysis.h \
    tab_info_query.h \
    inputbuffer.h \
    act_data_io.h \
    graph.h

FORMS    += db_sys.ui \
    tab_data_io.ui \
    tab_interfere_analysis.ui \
    tab_info_query.ui \
    graph.ui


