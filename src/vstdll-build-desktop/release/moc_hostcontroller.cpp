/****************************************************************************
** Meta object code from reading C++ file 'hostcontroller.h'
**
** Created: Thu 2. Dec 01:56:51 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/connectables/hostcontroller.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hostcontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connectables__HostController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   30,   29,   29, 0x05,
      55,   51,   29,   29, 0x05,
      88,   70,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
     127,  113,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connectables__HostController[] = {
    "Connectables::HostController\0\0prog\0"
    "progChange(int)\0grp\0grpChange(int)\0"
    "tempo,sign1,sign2\0tempoChange(int,int,int)\0"
    "pinInfo,value\0OnParameterChanged(ConnectionInfo,float)\0"
};

const QMetaObject Connectables::HostController::staticMetaObject = {
    { &Object::staticMetaObject, qt_meta_stringdata_Connectables__HostController,
      qt_meta_data_Connectables__HostController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connectables::HostController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connectables::HostController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connectables::HostController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connectables__HostController))
        return static_cast<void*>(const_cast< HostController*>(this));
    return Object::qt_metacast(_clname);
}

int Connectables::HostController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Object::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: progChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: grpChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: tempoChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: OnParameterChanged((*reinterpret_cast< ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Connectables::HostController::progChange(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Connectables::HostController::grpChange(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Connectables::HostController::tempoChange(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
