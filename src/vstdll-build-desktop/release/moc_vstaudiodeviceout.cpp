/****************************************************************************
** Meta object code from reading C++ file 'vstaudiodeviceout.h'
**
** Created: Thu 2. Dec 01:56:49 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstdll/connectables/vstaudiodeviceout.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vstaudiodeviceout.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connectables__VstAudioDeviceOut[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      38,   33,   32,   32, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connectables__VstAudioDeviceOut[] = {
    "Connectables::VstAudioDeviceOut\0\0size\0"
    "SetBufferSize(long)\0"
};

const QMetaObject Connectables::VstAudioDeviceOut::staticMetaObject = {
    { &Object::staticMetaObject, qt_meta_stringdata_Connectables__VstAudioDeviceOut,
      qt_meta_data_Connectables__VstAudioDeviceOut, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connectables::VstAudioDeviceOut::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connectables::VstAudioDeviceOut::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connectables::VstAudioDeviceOut::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connectables__VstAudioDeviceOut))
        return static_cast<void*>(const_cast< VstAudioDeviceOut*>(this));
    return Object::qt_metacast(_clname);
}

int Connectables::VstAudioDeviceOut::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Object::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: SetBufferSize((*reinterpret_cast< long(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
