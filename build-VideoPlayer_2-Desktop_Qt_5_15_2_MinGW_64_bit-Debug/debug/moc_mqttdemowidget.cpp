/****************************************************************************
** Meta object code from reading C++ file 'mqttdemowidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../VideoPlayer_2/src/mqtt/mqttdemowidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mqttdemowidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MqttDemoWidget_t {
    QByteArrayData data[6];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MqttDemoWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MqttDemoWidget_t qt_meta_stringdata_MqttDemoWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MqttDemoWidget"
QT_MOC_LITERAL(1, 15, 15), // "onMQTT_Received"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 14), // "QMQTT::Message"
QT_MOC_LITERAL(4, 47, 17), // "onMQTT_subscribed"
QT_MOC_LITERAL(5, 65, 19) // "onMQTT_unSubscribed"

    },
    "MqttDemoWidget\0onMQTT_Received\0\0"
    "QMQTT::Message\0onMQTT_subscribed\0"
    "onMQTT_unSubscribed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MqttDemoWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08 /* Private */,
       4,    2,   32,    2, 0x08 /* Private */,
       5,    1,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::UChar,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void MqttDemoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MqttDemoWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onMQTT_Received((*reinterpret_cast< QMQTT::Message(*)>(_a[1]))); break;
        case 1: _t->onMQTT_subscribed((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 2: _t->onMQTT_unSubscribed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMQTT::Message >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MqttDemoWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MqttDemoWidget.data,
    qt_meta_data_MqttDemoWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MqttDemoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MqttDemoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MqttDemoWidget.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MqttDemoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
