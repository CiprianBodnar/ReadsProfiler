TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    db.cpp

HEADERS += \
    db.h \
    query.h \
    header.h

LIBS += -l sqlite3

LIBS += -lpthread
