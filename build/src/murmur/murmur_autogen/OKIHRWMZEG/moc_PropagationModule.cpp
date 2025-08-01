/****************************************************************************
** Meta object code from reading C++ file 'PropagationModule.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/murmur/modules/PropagationModule.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropagationModule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PropagationModule_t {
    QByteArrayData data[21];
    char stringdata0[259];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropagationModule_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropagationModule_t qt_meta_stringdata_PropagationModule = {
    {
QT_MOC_LITERAL(0, 0, 17), // "PropagationModule"
QT_MOC_LITERAL(1, 18, 20), // "signalQualityChanged"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 12), // "userSession1"
QT_MOC_LITERAL(4, 53, 12), // "userSession2"
QT_MOC_LITERAL(5, 66, 7), // "quality"
QT_MOC_LITERAL(6, 74, 28), // "propagationConditionsChanged"
QT_MOC_LITERAL(7, 103, 3), // "sfi"
QT_MOC_LITERAL(8, 107, 6), // "kIndex"
QT_MOC_LITERAL(9, 114, 6), // "season"
QT_MOC_LITERAL(10, 121, 17), // "updatePropagation"
QT_MOC_LITERAL(11, 139, 20), // "onPropagationUpdated"
QT_MOC_LITERAL(12, 160, 23), // "onSignalStrengthChanged"
QT_MOC_LITERAL(13, 184, 5), // "grid1"
QT_MOC_LITERAL(14, 190, 5), // "grid2"
QT_MOC_LITERAL(15, 196, 8), // "strength"
QT_MOC_LITERAL(16, 205, 12), // "onMUFChanged"
QT_MOC_LITERAL(17, 218, 3), // "muf"
QT_MOC_LITERAL(18, 222, 21), // "onExternalDataUpdated"
QT_MOC_LITERAL(19, 244, 6), // "source"
QT_MOC_LITERAL(20, 251, 7) // "success"

    },
    "PropagationModule\0signalQualityChanged\0"
    "\0userSession1\0userSession2\0quality\0"
    "propagationConditionsChanged\0sfi\0"
    "kIndex\0season\0updatePropagation\0"
    "onPropagationUpdated\0onSignalStrengthChanged\0"
    "grid1\0grid2\0strength\0onMUFChanged\0muf\0"
    "onExternalDataUpdated\0source\0success"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropagationModule[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x06 /* Public */,
       6,    3,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   63,    2, 0x0a /* Public */,
      11,    0,   64,    2, 0x0a /* Public */,
      12,    3,   65,    2, 0x0a /* Public */,
      16,    1,   72,    2, 0x0a /* Public */,
      18,    2,   75,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::Float,    3,    4,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    7,    8,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Float,   13,   14,   15,
    QMetaType::Void, QMetaType::Float,   17,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   19,   20,

       0        // eod
};

void PropagationModule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PropagationModule *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->signalQualityChanged((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 1: _t->propagationConditionsChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->updatePropagation(); break;
        case 3: _t->onPropagationUpdated(); break;
        case 4: _t->onSignalStrengthChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 5: _t->onMUFChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->onExternalDataUpdated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PropagationModule::*)(unsigned int , unsigned int , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PropagationModule::signalQualityChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PropagationModule::*)(int , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PropagationModule::propagationConditionsChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PropagationModule::staticMetaObject = { {
    QMetaObject::SuperData::link<IServerModule::staticMetaObject>(),
    qt_meta_stringdata_PropagationModule.data,
    qt_meta_data_PropagationModule,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PropagationModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropagationModule::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PropagationModule.stringdata0))
        return static_cast<void*>(this);
    return IServerModule::qt_metacast(_clname);
}

int PropagationModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = IServerModule::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void PropagationModule::signalQualityChanged(unsigned int _t1, unsigned int _t2, float _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PropagationModule::propagationConditionsChanged(int _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
