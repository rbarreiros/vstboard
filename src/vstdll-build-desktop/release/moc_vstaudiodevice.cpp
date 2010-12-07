/****************************************************************************
** Meta object code from reading C++ file 'vstaudiodevice.h'
**
** Created: Thu 2. Dec 01:56:50 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstdll/connectables/vstaudiodevice.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vstaudiodevice.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connectables__VstAudioDevice[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   30,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   74,   29,   29, 0x0a,
     100,   29,   29,   29, 0x2a,
     116,   29,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connectables__VstAudioDevice[] = {
    "Connectables::VstAudioDevice\0\0"
    "objInfo,inUse\0InUseChanged(ObjectInfo,bool)\0"
    "rate\0SetSampleRate(float)\0SetSampleRate()\0"
    "DeleteIfUnused()\0"
};

const QMetaObject Connectables::VstAudioDevice::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Connectables__VstAudioDevice,
      qt_meta_data_Connectables__VstAudioDevice, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connectables::VstAudioDevice::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connectables::VstAudioDevice::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connectables::VstAudioDevice::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connectables__VstAudioDevice))
        return static_cast<void*>(const_cast< VstAudioDevice*>(this));
    return QObject::qt_metacast(_clname);
}

int Connectables::VstAudioDevice::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: InUseChanged((*reinterpret_cast< const ObjectInfo(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: SetSampleRate((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: SetSampleRate(); break;
        case 3: DeleteIfUnused(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Connectables::VstAudioDevice::InUseChanged(const ObjectInfo & _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
