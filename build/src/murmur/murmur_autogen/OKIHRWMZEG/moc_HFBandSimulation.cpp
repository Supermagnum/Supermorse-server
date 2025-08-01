/****************************************************************************
** Meta object code from reading C++ file 'HFBandSimulation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/murmur/modules/HFBandSimulation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HFBandSimulation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HFBandSimulation_t {
    QByteArrayData data[12];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HFBandSimulation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HFBandSimulation_t qt_meta_stringdata_HFBandSimulation = {
    {
QT_MOC_LITERAL(0, 0, 16), // "HFBandSimulation"
QT_MOC_LITERAL(1, 17, 18), // "propagationUpdated"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 21), // "signalStrengthChanged"
QT_MOC_LITERAL(4, 59, 5), // "grid1"
QT_MOC_LITERAL(5, 65, 5), // "grid2"
QT_MOC_LITERAL(6, 71, 8), // "strength"
QT_MOC_LITERAL(7, 80, 10), // "mufChanged"
QT_MOC_LITERAL(8, 91, 3), // "muf"
QT_MOC_LITERAL(9, 95, 19), // "externalDataUpdated"
QT_MOC_LITERAL(10, 115, 6), // "source"
QT_MOC_LITERAL(11, 122, 7) // "success"

    },
    "HFBandSimulation\0propagationUpdated\0"
    "\0signalStrengthChanged\0grid1\0grid2\0"
    "strength\0mufChanged\0muf\0externalDataUpdated\0"
    "source\0success"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HFBandSimulation[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    3,   35,    2, 0x06 /* Public */,
       7,    1,   42,    2, 0x06 /* Public */,
       9,    2,   45,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Float,    4,    5,    6,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   10,   11,

       0        // eod
};

void HFBandSimulation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HFBandSimulation *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->propagationUpdated(); break;
        case 1: _t->signalStrengthChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 2: _t->mufChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->externalDataUpdated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HFBandSimulation::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HFBandSimulation::propagationUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (HFBandSimulation::*)(const QString & , const QString & , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HFBandSimulation::signalStrengthChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (HFBandSimulation::*)(float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HFBandSimulation::mufChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (HFBandSimulation::*)(const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HFBandSimulation::externalDataUpdated)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HFBandSimulation::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_HFBandSimulation.data,
    qt_meta_data_HFBandSimulation,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HFBandSimulation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HFBandSimulation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HFBandSimulation.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HFBandSimulation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void HFBandSimulation::propagationUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HFBandSimulation::signalStrengthChanged(const QString & _t1, const QString & _t2, float _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HFBandSimulation::mufChanged(float _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HFBandSimulation::externalDataUpdated(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
