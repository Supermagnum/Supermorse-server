/****************************************************************************
** Meta object code from reading C++ file 'ChannelListenerManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/murmur/ChannelListenerManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ChannelListenerManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChannelListenerManager_t {
    QByteArrayData data[11];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChannelListenerManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChannelListenerManager_t qt_meta_stringdata_ChannelListenerManager = {
    {
QT_MOC_LITERAL(0, 0, 22), // "ChannelListenerManager"
QT_MOC_LITERAL(1, 23, 13), // "listenerAdded"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 10), // "ServerUser"
QT_MOC_LITERAL(4, 49, 4), // "user"
QT_MOC_LITERAL(5, 54, 7), // "Channel"
QT_MOC_LITERAL(6, 62, 7), // "channel"
QT_MOC_LITERAL(7, 70, 15), // "listenerRemoved"
QT_MOC_LITERAL(8, 86, 31), // "listenerVolumeAdjustmentChanged"
QT_MOC_LITERAL(9, 118, 16), // "VolumeAdjustment"
QT_MOC_LITERAL(10, 135, 16) // "volumeAdjustment"

    },
    "ChannelListenerManager\0listenerAdded\0"
    "\0ServerUser\0user\0Channel\0channel\0"
    "listenerRemoved\0listenerVolumeAdjustmentChanged\0"
    "VolumeAdjustment\0volumeAdjustment"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChannelListenerManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       7,    2,   34,    2, 0x06 /* Public */,
       8,    3,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 9,    4,    6,   10,

       0        // eod
};

void ChannelListenerManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChannelListenerManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->listenerAdded((*reinterpret_cast< const ServerUser(*)>(_a[1])),(*reinterpret_cast< const Channel(*)>(_a[2]))); break;
        case 1: _t->listenerRemoved((*reinterpret_cast< const ServerUser(*)>(_a[1])),(*reinterpret_cast< const Channel(*)>(_a[2]))); break;
        case 2: _t->listenerVolumeAdjustmentChanged((*reinterpret_cast< const ServerUser(*)>(_a[1])),(*reinterpret_cast< const Channel(*)>(_a[2])),(*reinterpret_cast< const VolumeAdjustment(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChannelListenerManager::*)(const ServerUser & , const Channel & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChannelListenerManager::listenerAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChannelListenerManager::*)(const ServerUser & , const Channel & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChannelListenerManager::listenerRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ChannelListenerManager::*)(const ServerUser & , const Channel & , const VolumeAdjustment & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChannelListenerManager::listenerVolumeAdjustmentChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChannelListenerManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ChannelListenerManager.data,
    qt_meta_data_ChannelListenerManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChannelListenerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChannelListenerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChannelListenerManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ChannelListenerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ChannelListenerManager::listenerAdded(const ServerUser & _t1, const Channel & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ChannelListenerManager::listenerRemoved(const ServerUser & _t1, const Channel & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ChannelListenerManager::listenerVolumeAdjustmentChanged(const ServerUser & _t1, const Channel & _t2, const VolumeAdjustment & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
