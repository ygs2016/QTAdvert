/****************************************************************************
** Meta object code from reading C++ file 'downloadtool.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../VideoPlayer_2/src/videoplayer/downloadtool.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'downloadtool.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DownloadTool_t {
    QByteArrayData data[10];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DownloadTool_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DownloadTool_t qt_meta_stringdata_DownloadTool = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DownloadTool"
QT_MOC_LITERAL(1, 13, 11), // "sigProgress"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 9), // "bytesRead"
QT_MOC_LITERAL(4, 36, 10), // "totalBytes"
QT_MOC_LITERAL(5, 47, 8), // "progress"
QT_MOC_LITERAL(6, 56, 19), // "sigDownloadFinished"
QT_MOC_LITERAL(7, 76, 12), // "httpFinished"
QT_MOC_LITERAL(8, 89, 13), // "httpReadyRead"
QT_MOC_LITERAL(9, 103, 20) // "networkReplyProgress"

    },
    "DownloadTool\0sigProgress\0\0bytesRead\0"
    "totalBytes\0progress\0sigDownloadFinished\0"
    "httpFinished\0httpReadyRead\0"
    "networkReplyProgress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DownloadTool[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,
       6,    0,   46,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   47,    2, 0x08 /* Private */,
       8,    0,   48,    2, 0x08 /* Private */,
       9,    2,   49,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong, QMetaType::QReal,    3,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,    3,    4,

       0        // eod
};

void DownloadTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DownloadTool *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qreal(*)>(_a[3]))); break;
        case 1: _t->sigDownloadFinished(); break;
        case 2: _t->httpFinished(); break;
        case 3: _t->httpReadyRead(); break;
        case 4: _t->networkReplyProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DownloadTool::*)(qint64 , qint64 , qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownloadTool::sigProgress)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DownloadTool::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DownloadTool::sigDownloadFinished)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DownloadTool::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_DownloadTool.data,
    qt_meta_data_DownloadTool,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DownloadTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DownloadTool::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DownloadTool.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DownloadTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DownloadTool::sigProgress(qint64 _t1, qint64 _t2, qreal _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DownloadTool::sigDownloadFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
