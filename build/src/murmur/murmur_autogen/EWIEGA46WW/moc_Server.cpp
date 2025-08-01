/****************************************************************************
** Meta object code from reading C++ file 'Server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../src/murmur/Server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SslServer_t {
    QByteArrayData data[1];
    char stringdata0[10];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SslServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SslServer_t qt_meta_stringdata_SslServer = {
    {
QT_MOC_LITERAL(0, 0, 9) // "SslServer"

    },
    "SslServer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SslServer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void SslServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject SslServer::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_meta_stringdata_SslServer.data,
    qt_meta_data_SslServer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SslServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SslServer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SslServer.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int SslServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_Server_t {
    QByteArrayData data[55];
    char stringdata0[768];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Server_t qt_meta_stringdata_Server = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Server"
QT_MOC_LITERAL(1, 7, 7), // "reqSync"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 11), // "tcpTransmit"
QT_MOC_LITERAL(4, 28, 2), // "id"
QT_MOC_LITERAL(5, 31, 20), // "signalQualityChanged"
QT_MOC_LITERAL(6, 52, 12), // "userSession1"
QT_MOC_LITERAL(7, 65, 12), // "userSession2"
QT_MOC_LITERAL(8, 78, 7), // "quality"
QT_MOC_LITERAL(9, 86, 15), // "registerUserSig"
QT_MOC_LITERAL(10, 102, 4), // "int&"
QT_MOC_LITERAL(11, 107, 17), // "QMap<int,QString>"
QT_MOC_LITERAL(12, 125, 17), // "unregisterUserSig"
QT_MOC_LITERAL(13, 143, 21), // "getRegisteredUsersSig"
QT_MOC_LITERAL(14, 165, 18), // "QMap<int,QString>&"
QT_MOC_LITERAL(15, 184, 18), // "getRegistrationSig"
QT_MOC_LITERAL(16, 203, 15), // "authenticateSig"
QT_MOC_LITERAL(17, 219, 8), // "QString&"
QT_MOC_LITERAL(18, 228, 22), // "QList<QSslCertificate>"
QT_MOC_LITERAL(19, 251, 10), // "setInfoSig"
QT_MOC_LITERAL(20, 262, 13), // "setTextureSig"
QT_MOC_LITERAL(21, 276, 11), // "idToNameSig"
QT_MOC_LITERAL(22, 288, 11), // "nameToIdSig"
QT_MOC_LITERAL(23, 300, 14), // "idToTextureSig"
QT_MOC_LITERAL(24, 315, 11), // "QByteArray&"
QT_MOC_LITERAL(25, 327, 16), // "userStateChanged"
QT_MOC_LITERAL(26, 344, 11), // "const User*"
QT_MOC_LITERAL(27, 356, 15), // "userTextMessage"
QT_MOC_LITERAL(28, 372, 11), // "TextMessage"
QT_MOC_LITERAL(29, 384, 13), // "userConnected"
QT_MOC_LITERAL(30, 398, 16), // "userDisconnected"
QT_MOC_LITERAL(31, 415, 19), // "channelStateChanged"
QT_MOC_LITERAL(32, 435, 14), // "const Channel*"
QT_MOC_LITERAL(33, 450, 14), // "channelCreated"
QT_MOC_LITERAL(34, 465, 14), // "channelRemoved"
QT_MOC_LITERAL(35, 480, 20), // "textMessageFilterSig"
QT_MOC_LITERAL(36, 501, 25), // "MumbleProto::TextMessage&"
QT_MOC_LITERAL(37, 527, 13), // "contextAction"
QT_MOC_LITERAL(38, 541, 11), // "regSslError"
QT_MOC_LITERAL(39, 553, 16), // "QList<QSslError>"
QT_MOC_LITERAL(40, 570, 8), // "finished"
QT_MOC_LITERAL(41, 579, 6), // "update"
QT_MOC_LITERAL(42, 586, 9), // "newClient"
QT_MOC_LITERAL(43, 596, 16), // "connectionClosed"
QT_MOC_LITERAL(44, 613, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(45, 642, 8), // "sslError"
QT_MOC_LITERAL(46, 651, 7), // "message"
QT_MOC_LITERAL(47, 659, 32), // "Mumble::Protocol::TCPMessageType"
QT_MOC_LITERAL(48, 692, 11), // "ServerUser*"
QT_MOC_LITERAL(49, 704, 4), // "cCon"
QT_MOC_LITERAL(50, 709, 12), // "checkTimeout"
QT_MOC_LITERAL(51, 722, 15), // "tcpTransmitData"
QT_MOC_LITERAL(52, 738, 6), // "doSync"
QT_MOC_LITERAL(53, 745, 9), // "encrypted"
QT_MOC_LITERAL(54, 755, 12) // "udpActivated"

    },
    "Server\0reqSync\0\0tcpTransmit\0id\0"
    "signalQualityChanged\0userSession1\0"
    "userSession2\0quality\0registerUserSig\0"
    "int&\0QMap<int,QString>\0unregisterUserSig\0"
    "getRegisteredUsersSig\0QMap<int,QString>&\0"
    "getRegistrationSig\0authenticateSig\0"
    "QString&\0QList<QSslCertificate>\0"
    "setInfoSig\0setTextureSig\0idToNameSig\0"
    "nameToIdSig\0idToTextureSig\0QByteArray&\0"
    "userStateChanged\0const User*\0"
    "userTextMessage\0TextMessage\0userConnected\0"
    "userDisconnected\0channelStateChanged\0"
    "const Channel*\0channelCreated\0"
    "channelRemoved\0textMessageFilterSig\0"
    "MumbleProto::TextMessage&\0contextAction\0"
    "regSslError\0QList<QSslError>\0finished\0"
    "update\0newClient\0connectionClosed\0"
    "QAbstractSocket::SocketError\0sslError\0"
    "message\0Mumble::Protocol::TCPMessageType\0"
    "ServerUser*\0cCon\0checkTimeout\0"
    "tcpTransmitData\0doSync\0encrypted\0"
    "udpActivated"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Server[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      35,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      22,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  189,    2, 0x06 /* Public */,
       3,    2,  192,    2, 0x06 /* Public */,
       5,    3,  197,    2, 0x06 /* Public */,
       9,    2,  204,    2, 0x06 /* Public */,
      12,    2,  209,    2, 0x06 /* Public */,
      13,    2,  214,    2, 0x06 /* Public */,
      15,    3,  219,    2, 0x06 /* Public */,
      16,    7,  226,    2, 0x06 /* Public */,
      19,    3,  241,    2, 0x06 /* Public */,
      20,    3,  248,    2, 0x06 /* Public */,
      21,    2,  255,    2, 0x06 /* Public */,
      22,    2,  260,    2, 0x06 /* Public */,
      23,    2,  265,    2, 0x06 /* Public */,
      25,    1,  270,    2, 0x06 /* Public */,
      27,    2,  273,    2, 0x06 /* Public */,
      29,    1,  278,    2, 0x06 /* Public */,
      30,    1,  281,    2, 0x06 /* Public */,
      31,    1,  284,    2, 0x06 /* Public */,
      33,    1,  287,    2, 0x06 /* Public */,
      34,    1,  290,    2, 0x06 /* Public */,
      35,    3,  293,    2, 0x06 /* Public */,
      37,    4,  300,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      38,    1,  309,    2, 0x0a /* Public */,
      40,    0,  312,    2, 0x0a /* Public */,
      41,    0,  313,    2, 0x0a /* Public */,
      42,    0,  314,    2, 0x0a /* Public */,
      43,    2,  315,    2, 0x0a /* Public */,
      45,    1,  320,    2, 0x0a /* Public */,
      46,    3,  323,    2, 0x0a /* Public */,
      46,    2,  330,    2, 0x2a /* Public | MethodCloned */,
      50,    0,  335,    2, 0x0a /* Public */,
      51,    2,  336,    2, 0x0a /* Public */,
      52,    1,  341,    2, 0x0a /* Public */,
      53,    0,  344,    2, 0x0a /* Public */,
      54,    1,  345,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt,    2,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::UInt,    2,    4,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::Float,    6,    7,    8,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 11,    2,    2,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 14,    2,    2,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int, 0x80000000 | 14,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 17, QMetaType::Int, 0x80000000 | 18, QMetaType::QString, QMetaType::Bool, QMetaType::QString,    2,    2,    2,    2,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int, 0x80000000 | 11,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int, QMetaType::QByteArray,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 17, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 24, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 26,    2,
    QMetaType::Void, 0x80000000 | 26, 0x80000000 | 28,    2,    2,
    QMetaType::Void, 0x80000000 | 26,    2,
    QMetaType::Void, 0x80000000 | 26,    2,
    QMetaType::Void, 0x80000000 | 32,    2,
    QMetaType::Void, 0x80000000 | 32,    2,
    QMetaType::Void, 0x80000000 | 32,    2,
    QMetaType::Void, 0x80000000 | 10, 0x80000000 | 26, 0x80000000 | 36,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 26, QMetaType::QString, QMetaType::UInt, QMetaType::Int,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 39,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 44, QMetaType::QString,    2,    2,
    QMetaType::Void, 0x80000000 | 39,    2,
    QMetaType::Void, 0x80000000 | 47, QMetaType::QByteArray, 0x80000000 | 48,    2,    2,   49,
    QMetaType::Void, 0x80000000 | 47, QMetaType::QByteArray,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray, QMetaType::UInt,    2,    2,
    QMetaType::Void, QMetaType::UInt,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Server *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->reqSync((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 1: _t->tcpTransmit((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 2: _t->signalQualityChanged((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 3: _t->registerUserSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QMap<int,QString>(*)>(_a[2]))); break;
        case 4: _t->unregisterUserSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->getRegisteredUsersSig((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QMap<int,QString>(*)>(_a[2]))); break;
        case 6: _t->getRegistrationSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QMap<int,QString>(*)>(_a[3]))); break;
        case 7: _t->authenticateSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QList<QSslCertificate>(*)>(_a[4])),(*reinterpret_cast< const QString(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< const QString(*)>(_a[7]))); break;
        case 8: _t->setInfoSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QMap<int,QString>(*)>(_a[3]))); break;
        case 9: _t->setTextureSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 10: _t->idToNameSig((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->nameToIdSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 12: _t->idToTextureSig((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->userStateChanged((*reinterpret_cast< const User*(*)>(_a[1]))); break;
        case 14: _t->userTextMessage((*reinterpret_cast< const User*(*)>(_a[1])),(*reinterpret_cast< const TextMessage(*)>(_a[2]))); break;
        case 15: _t->userConnected((*reinterpret_cast< const User*(*)>(_a[1]))); break;
        case 16: _t->userDisconnected((*reinterpret_cast< const User*(*)>(_a[1]))); break;
        case 17: _t->channelStateChanged((*reinterpret_cast< const Channel*(*)>(_a[1]))); break;
        case 18: _t->channelCreated((*reinterpret_cast< const Channel*(*)>(_a[1]))); break;
        case 19: _t->channelRemoved((*reinterpret_cast< const Channel*(*)>(_a[1]))); break;
        case 20: _t->textMessageFilterSig((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const User*(*)>(_a[2])),(*reinterpret_cast< MumbleProto::TextMessage(*)>(_a[3]))); break;
        case 21: _t->contextAction((*reinterpret_cast< const User*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 22: _t->regSslError((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 23: _t->finished(); break;
        case 24: _t->update(); break;
        case 25: _t->newClient(); break;
        case 26: _t->connectionClosed((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 27: _t->sslError((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 28: _t->message((*reinterpret_cast< Mumble::Protocol::TCPMessageType(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2])),(*reinterpret_cast< ServerUser*(*)>(_a[3]))); break;
        case 29: _t->message((*reinterpret_cast< Mumble::Protocol::TCPMessageType(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 30: _t->checkTimeout(); break;
        case 31: _t->tcpTransmitData((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 32: _t->doSync((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 33: _t->encrypted(); break;
        case 34: _t->udpActivated((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 3:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslCertificate> >(); break;
            }
            break;
        case 22:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        case 26:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        case 27:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Server::*)(unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::reqSync)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Server::*)(QByteArray , unsigned int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::tcpTransmit)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Server::*)(unsigned int , unsigned int , float );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::signalQualityChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , const QMap<int,QString> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::registerUserSig)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::unregisterUserSig)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Server::*)(const QString & , QMap<int,QString> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::getRegisteredUsersSig)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , int , QMap<int,QString> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::getRegistrationSig)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , QString & , int , const QList<QSslCertificate> & , const QString & , bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::authenticateSig)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , int , const QMap<int,QString> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::setInfoSig)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , int , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::setTextureSig)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (Server::*)(QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::idToNameSig)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::nameToIdSig)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (Server::*)(QByteArray & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::idToTextureSig)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (Server::*)(const User * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::userStateChanged)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (Server::*)(const User * , const TextMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::userTextMessage)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (Server::*)(const User * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::userConnected)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (Server::*)(const User * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::userDisconnected)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (Server::*)(const Channel * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::channelStateChanged)) {
                *result = 17;
                return;
            }
        }
        {
            using _t = void (Server::*)(const Channel * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::channelCreated)) {
                *result = 18;
                return;
            }
        }
        {
            using _t = void (Server::*)(const Channel * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::channelRemoved)) {
                *result = 19;
                return;
            }
        }
        {
            using _t = void (Server::*)(int & , const User * , MumbleProto::TextMessage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::textMessageFilterSig)) {
                *result = 20;
                return;
            }
        }
        {
            using _t = void (Server::*)(const User * , const QString & , unsigned int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Server::contextAction)) {
                *result = 21;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Server::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_Server.data,
    qt_meta_data_Server,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Server.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 35)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 35;
    }
    return _id;
}

// SIGNAL 0
void Server::reqSync(unsigned int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Server::tcpTransmit(QByteArray _t1, unsigned int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Server::signalQualityChanged(unsigned int _t1, unsigned int _t2, float _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Server::registerUserSig(int & _t1, const QMap<int,QString> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Server::unregisterUserSig(int & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Server::getRegisteredUsersSig(const QString & _t1, QMap<int,QString> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Server::getRegistrationSig(int & _t1, int _t2, QMap<int,QString> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Server::authenticateSig(int & _t1, QString & _t2, int _t3, const QList<QSslCertificate> & _t4, const QString & _t5, bool _t6, const QString & _t7)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t6))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t7))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Server::setInfoSig(int & _t1, int _t2, const QMap<int,QString> & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Server::setTextureSig(int & _t1, int _t2, const QByteArray & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Server::idToNameSig(QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void Server::nameToIdSig(int & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void Server::idToTextureSig(QByteArray & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void Server::userStateChanged(const User * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void Server::userTextMessage(const User * _t1, const TextMessage & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void Server::userConnected(const User * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void Server::userDisconnected(const User * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void Server::channelStateChanged(const Channel * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void Server::channelCreated(const Channel * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void Server::channelRemoved(const Channel * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}

// SIGNAL 20
void Server::textMessageFilterSig(int & _t1, const User * _t2, MumbleProto::TextMessage & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 20, _a);
}

// SIGNAL 21
void Server::contextAction(const User * _t1, const QString & _t2, unsigned int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 21, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
