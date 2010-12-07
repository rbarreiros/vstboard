/****************************************************************************
** Meta object code from reading C++ file 'mainhost.h'
**
** Created: Thu 2. Dec 01:56:50 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/mainhost.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainhost.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainHost[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   10,    9,    9, 0x05,
      45,   40,    9,    9, 0x05,
      81,   69,    9,    9, 0x05,
     123,  106,    9,    9, 0x05,
     146,    9,    9,    9, 0x05,
     177,  163,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     238,  219,    9,    9, 0x0a,
     282,  219,    9,    9, 0x0a,
     337,  328,    9,    9, 0x0a,
     373,  361,    9,    9, 0x0a,
     392,    9,    9,    9, 0x2a,
     425,  407,    9,    9, 0x0a,
     459,  447,    9,    9, 0x2a,
     483,  477,    9,    9, 0x2a,
     497,    9,    9,    9, 0x2a,
     508,   69,    9,    9, 0x0a,
     551,  543,    9,    9, 0x0a,
     565,    9,    9,    9, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_MainHost[] = {
    "MainHost\0\0rate\0SampleRateChanged(float)\0"
    "size\0BufferSizeChanged(long)\0renderLines\0"
    "NewSolver(orderedNodes*)\0contrainerId,obj\0"
    "ObjectRemoved(int,int)\0SolverToUpdate()\0"
    "objInfo,inUse\0OnAudioDeviceToggleInUse(ObjectInfo,bool)\0"
    "outputPin,inputPin\0"
    "OnCableAdded(ConnectionInfo,ConnectionInfo)\0"
    "OnCableRemoved(ConnectionInfo,ConnectionInfo)\0"
    "prgIndex\0SetProgram(QModelIndex)\0"
    "forceUpdate\0UpdateSolver(bool)\0"
    "UpdateSolver()\0tempo,sign1,sign2\0"
    "SetTempo(int,int,int)\0tempo,sign1\0"
    "SetTempo(int,int)\0tempo\0SetTempo(int)\0"
    "SetTempo()\0OnNewRenderingOrder(orderedNodes*)\0"
    "samples\0Render(ulong)\0Render()\0"
};

const QMetaObject MainHost::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MainHost,
      qt_meta_data_MainHost, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainHost::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainHost::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainHost::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainHost))
        return static_cast<void*>(const_cast< MainHost*>(this));
    return QObject::qt_metacast(_clname);
}

int MainHost::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SampleRateChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: BufferSizeChanged((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 2: NewSolver((*reinterpret_cast< orderedNodes*(*)>(_a[1]))); break;
        case 3: ObjectRemoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: SolverToUpdate(); break;
        case 5: OnAudioDeviceToggleInUse((*reinterpret_cast< const ObjectInfo(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: OnCableAdded((*reinterpret_cast< const ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< const ConnectionInfo(*)>(_a[2]))); break;
        case 7: OnCableRemoved((*reinterpret_cast< const ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< const ConnectionInfo(*)>(_a[2]))); break;
        case 8: SetProgram((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: UpdateSolver((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: UpdateSolver(); break;
        case 11: SetTempo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 12: SetTempo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: SetTempo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: SetTempo(); break;
        case 15: OnNewRenderingOrder((*reinterpret_cast< orderedNodes*(*)>(_a[1]))); break;
        case 16: Render((*reinterpret_cast< ulong(*)>(_a[1]))); break;
        case 17: Render(); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void MainHost::SampleRateChanged(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainHost::BufferSizeChanged(long _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainHost::NewSolver(orderedNodes * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainHost::ObjectRemoved(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainHost::SolverToUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MainHost::OnAudioDeviceToggleInUse(const ObjectInfo & _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
