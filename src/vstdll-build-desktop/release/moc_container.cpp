/****************************************************************************
** Meta object code from reading C++ file 'container.h'
**
** Created: Thu 2. Dec 01:56:59 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/connectables/container.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'container.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connectables__Container[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      51,   25,   24,   24, 0x0a,
     115,   96,   24,   24, 0x2a,
     155,   96,   24,   24, 0x0a,
     202,  198,   24,   24, 0x0a,
     243,  237,   24,   24, 0x0a,
     267,   24,   24,   24, 0x0a,
     281,   24,   24,   24, 0x0a,
     302,  297,   24,   24, 0x0a,
     328,  319,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connectables__Container[] = {
    "Connectables::Container\0\0"
    "outputPin,inputPin,hidden\0"
    "AddCable(ConnectionInfo,ConnectionInfo,bool)\0"
    "outputPin,inputPin\0"
    "AddCable(ConnectionInfo,ConnectionInfo)\0"
    "RemoveCable(ConnectionInfo,ConnectionInfo)\0"
    "pin\0RemoveCableFromPin(ConnectionInfo)\0"
    "objId\0RemoveCableFromObj(int)\0"
    "SaveProgram()\0UnloadProgram()\0prog\0"
    "LoadProgram(int)\0sleeping\0SetSleep(bool)\0"
};

const QMetaObject Connectables::Container::staticMetaObject = {
    { &Object::staticMetaObject, qt_meta_stringdata_Connectables__Container,
      qt_meta_data_Connectables__Container, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connectables::Container::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connectables::Container::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connectables::Container::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connectables__Container))
        return static_cast<void*>(const_cast< Container*>(this));
    return Object::qt_metacast(_clname);
}

int Connectables::Container::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Object::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: AddCable((*reinterpret_cast< const ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< const ConnectionInfo(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 1: AddCable((*reinterpret_cast< const ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< const ConnectionInfo(*)>(_a[2]))); break;
        case 2: RemoveCable((*reinterpret_cast< const ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< const ConnectionInfo(*)>(_a[2]))); break;
        case 3: RemoveCableFromPin((*reinterpret_cast< const ConnectionInfo(*)>(_a[1]))); break;
        case 4: RemoveCableFromObj((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: SaveProgram(); break;
        case 6: UnloadProgram(); break;
        case 7: LoadProgram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: SetSleep((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
