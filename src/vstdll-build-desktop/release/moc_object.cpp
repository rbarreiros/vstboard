/****************************************************************************
** Meta object code from reading C++ file 'object.h'
**
** Created: Thu 2. Dec 01:56:58 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/connectables/object.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'object.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connectables__Object[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   22,   21,   21, 0x05,
      42,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   21,   21,   21, 0x0a,
      76,   21,   21,   21, 0x0a,
      97,   92,   21,   21, 0x0a,
     114,   21,   21,   21, 0x0a,
     136,  131,   21,   21, 0x0a,
     161,  156,   21,   21, 0x0a,
     182,   21,   21,   21, 0x2a,
     212,  198,   21,   21, 0x0a,
     261,  253,   21,   21, 0x0a,
     280,  253,   21,   21, 0x0a,
     312,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connectables__Object[] = {
    "Connectables::Object\0\0load\0CpuLoad(float)\0"
    "CloseEditorWindow()\0SaveProgram()\0"
    "UnloadProgram()\0prog\0LoadProgram(int)\0"
    "OnProgramDirty()\0size\0SetBufferSize(long)\0"
    "rate\0SetSampleRate(float)\0SetSampleRate()\0"
    "pinInfo,value\0OnParameterChanged(ConnectionInfo,float)\0"
    "visible\0ToggleEditor(bool)\0"
    "OnEditorVisibilityChanged(bool)\0"
    "CloseSlot()\0"
};

const QMetaObject Connectables::Object::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Connectables__Object,
      qt_meta_data_Connectables__Object, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connectables::Object::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connectables::Object::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connectables::Object::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connectables__Object))
        return static_cast<void*>(const_cast< Object*>(this));
    return QObject::qt_metacast(_clname);
}

int Connectables::Object::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: CpuLoad((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: CloseEditorWindow(); break;
        case 2: SaveProgram(); break;
        case 3: UnloadProgram(); break;
        case 4: LoadProgram((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: OnProgramDirty(); break;
        case 6: SetBufferSize((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 7: SetSampleRate((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: SetSampleRate(); break;
        case 9: OnParameterChanged((*reinterpret_cast< ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 10: ToggleEditor((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: OnEditorVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: CloseSlot(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Connectables::Object::CpuLoad(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Connectables::Object::CloseEditorWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
