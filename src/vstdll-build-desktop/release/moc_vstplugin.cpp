/****************************************************************************
** Meta object code from reading C++ file 'vstplugin.h'
**
** Created: Thu 2. Dec 01:57:00 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/connectables/vstplugin.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vstplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connectables__VstPlugin[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      44,   25,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      75,   70,   24,   24, 0x0a,
     100,   95,   24,   24, 0x0a,
     121,   24,   24,   24, 0x2a,
     137,   24,   24,   24, 0x0a,
     151,   24,   24,   24, 0x0a,
     169,   24,   24,   24, 0x0a,
     194,  180,   24,   24, 0x0a,
     243,  235,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connectables__VstPlugin[] = {
    "Connectables::VstPlugin\0\0newWidth,newHeight\0"
    "WindowSizeChange(int,int)\0size\0"
    "SetBufferSize(long)\0rate\0SetSampleRate(float)\0"
    "SetSampleRate()\0RaiseEditor()\0"
    "EditorDestroyed()\0EditIdle()\0pinInfo,value\0"
    "OnParameterChanged(ConnectionInfo,float)\0"
    "visible\0OnEditorVisibilityChanged(bool)\0"
};

const QMetaObject Connectables::VstPlugin::staticMetaObject = {
    { &Object::staticMetaObject, qt_meta_stringdata_Connectables__VstPlugin,
      qt_meta_data_Connectables__VstPlugin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connectables::VstPlugin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connectables::VstPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connectables::VstPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connectables__VstPlugin))
        return static_cast<void*>(const_cast< VstPlugin*>(this));
    if (!strcmp(_clname, "vst::CEffect"))
        return static_cast< vst::CEffect*>(const_cast< VstPlugin*>(this));
    return Object::qt_metacast(_clname);
}

int Connectables::VstPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Object::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: WindowSizeChange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: SetBufferSize((*reinterpret_cast< long(*)>(_a[1]))); break;
        case 2: SetSampleRate((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: SetSampleRate(); break;
        case 4: RaiseEditor(); break;
        case 5: EditorDestroyed(); break;
        case 6: EditIdle(); break;
        case 7: OnParameterChanged((*reinterpret_cast< ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 8: OnEditorVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Connectables::VstPlugin::WindowSizeChange(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
