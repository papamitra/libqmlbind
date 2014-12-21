TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

SOURCES += \
    api.c \
    main.cpp \
    engine_test.cpp \
    interface_test.cpp \
    value_test.cpp

QMAKE_CFLAGS += "-std=c89"

LIBS += -L$$PWD/../qmlbind/ -lqmlbind
INCLUDEPATH += $$PWD/../qmlbind/include $$PWD/lib/Catch/include
DEPENDPATH += $$PWD/../qmlbind

HEADERS += \
    test_helper.h
