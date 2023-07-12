/****************************************************************************
** Meta object code from reading C++ file 'mqttrequest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../VideoPlayer_2/src/videoplayer/mqttrequest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mqttrequest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mqttrequest_t {
    QByteArrayData data[15];
    char stringdata0[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mqttrequest_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mqttrequest_t qt_meta_stringdata_mqttrequest = {
    {
QT_MOC_LITERAL(0, 0, 11), // "mqttrequest"
QT_MOC_LITERAL(1, 12, 18), // "sig_NeedGetLabelAd"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 6), // "Advert"
QT_MOC_LITERAL(4, 39, 3), // "str"
QT_MOC_LITERAL(5, 43, 7), // "getBack"
QT_MOC_LITERAL(6, 51, 14), // "QNetworkReply*"
QT_MOC_LITERAL(7, 66, 5), // "reply"
QT_MOC_LITERAL(8, 72, 15), // "onMQTT_Received"
QT_MOC_LITERAL(9, 88, 14), // "QMQTT::Message"
QT_MOC_LITERAL(10, 103, 17), // "onMQTT_subscribed"
QT_MOC_LITERAL(11, 121, 19), // "onMQTT_unSubscribed"
QT_MOC_LITERAL(12, 141, 12), // "onMQTT_error"
QT_MOC_LITERAL(13, 154, 18), // "QMQTT::ClientError"
QT_MOC_LITERAL(14, 173, 5) // "error"

    },
    "mqttrequest\0sig_NeedGetLabelAd\0\0Advert\0"
    "str\0getBack\0QNetworkReply*\0reply\0"
    "onMQTT_Received\0QMQTT::Message\0"
    "onMQTT_subscribed\0onMQTT_unSubscribed\0"
    "onMQTT_error\0QMQTT::ClientError\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mqttrequest[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   47,    2, 0x08 /* Private */,
       8,    1,   50,    2, 0x08 /* Private */,
      10,    2,   53,    2, 0x08 /* Private */,
      11,    1,   58,    2, 0x08 /* Private */,
      12,    1,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::UChar,    2,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void mqttrequest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mqttrequest *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_NeedGetLabelAd((*reinterpret_cast< Advert(*)>(_a[1]))); break;
        case 1: _t->getBack((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 2: _t->onMQTT_Received((*reinterpret_cast< QMQTT::Message(*)>(_a[1]))); break;
        case 3: _t->onMQTT_subscribed((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2]))); break;
        case 4: _t->onMQTT_unSubscribed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->onMQTT_error((*reinterpret_cast< const QMQTT::ClientError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMQTT::Message >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMQTT::ClientError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (mqttrequest::*)(Advert );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mqttrequest::sig_NeedGetLabelAd)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mqttrequest::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_mqttrequest.data,
    qt_meta_data_mqttrequest,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mqttrequest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mqttrequest::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mqttrequest.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int mqttrequest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void mqttrequest::sig_NeedGetLabelAd(Advert _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
