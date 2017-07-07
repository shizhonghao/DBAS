#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T23:44:33
#
#-------------------------------------------------

QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = db_sys
TEMPLATE = app


SOURCES += main.cpp\
        db_sys.cpp \
    tab_data_io.cpp \
    tab_interfere_analysis.cpp \
    tab_info_query.cpp

HEADERS  += db_sys.h \
    tab_data_io.h \
    tab_interfere_analysis.h \
    tab_info_query.h

FORMS    += db_sys.ui \
    tab_data_io.ui \
    tab_interfere_analysis.ui \
    tab_info_query.ui
