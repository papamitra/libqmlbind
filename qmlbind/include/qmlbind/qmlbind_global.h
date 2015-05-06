#pragma once

# if defined(_WIN32) || defined(_WIN64)
#   ifdef QMLBIND_LIBRARY
#     define QMLBIND_API __declspec(dllexport)
#   else
#     define QMLBIND_API __declspec(dllimport)
#   endif
# else
#   define QMLBIND_API
# endif

#ifdef QMLBIND_LIBRARY

template <typename T> class QSharedPointer;
class QApplication;
class QQmlEngine;
class QQmlComponent;
class QJSValue;
class QJSValueIterator;
class QByteArray;

namespace QmlBind {

class Interface;
class MetaObject;
class Exporter;
class SignalEmitter;

}

typedef QApplication *qmlbind_application;

typedef QQmlEngine *qmlbind_engine;
typedef QQmlComponent *qmlbind_component;

typedef QJSValue *qmlbind_value;
typedef QJSValueIterator *qmlbind_iterator;
typedef QByteArray *qmlbind_string;

// Use shared pointer for widely referenced classes
typedef QSharedPointer<QmlBind::Interface> *qmlbind_interface;
typedef QSharedPointer<QmlBind::MetaObject> *qmlbind_metaobject;
typedef QSharedPointer<QmlBind::Exporter> *qmlbind_exporter;

typedef QmlBind::SignalEmitter *qmlbind_signal_emitter;

#else

typedef struct {} *qmlbind_application;

typedef struct {} *qmlbind_engine;
typedef struct {} *qmlbind_component;

typedef struct {} *qmlbind_value;
typedef struct {} *qmlbind_iterator;
typedef struct {} *qmlbind_string;

typedef struct {} *qmlbind_interface;
typedef struct {} *qmlbind_metaobject;
typedef struct {} *qmlbind_exporter;

typedef struct {} *qmlbind_signal_emitter;

#endif

typedef struct qmlbind_backref_s *qmlbind_backref;

typedef struct {
    qmlbind_backref (*new_object)(qmlbind_backref classRef, qmlbind_signal_emitter signalEmitter);
    void (*delete_object)(qmlbind_backref objRef);
    qmlbind_value (*call_method)(qmlbind_engine engine, qmlbind_backref objRef, qmlbind_backref methodRef, int argc, qmlbind_value *argv);
    qmlbind_value (*get_property)(qmlbind_engine engine, qmlbind_backref objRef, qmlbind_backref getterRef);
    void (*set_property)(qmlbind_engine engine, qmlbind_backref objRef, qmlbind_backref setterRef, qmlbind_value value);
} qmlbind_interface_handlers;
