/****************************************************************************
** Meta object code from reading C++ file 'vstpluginwindow.h'
**
** Created: Thu 2. Dec 01:57:01 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/vstpluginwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vstpluginwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__VstPluginWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   30,   22,   22, 0x0a,
      88,   72,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_View__VstPluginWindow[] = {
    "View::VstPluginWindow\0\0Hide()\0"
    "newWidth,newHeight\0SetWindowSize(int,int)\0"
    "Hscroll,Vscroll\0SetScrollValue(int,int)\0"
};

const QMetaObject View::VstPluginWindow::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_View__VstPluginWindow,
      qt_meta_data_View__VstPluginWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::VstPluginWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::VstPluginWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::VstPluginWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__VstPluginWindow))
        return static_cast<void*>(const_cast< VstPluginWindow*>(this));
    return QFrame::qt_metacast(_clname);
}

int View::VstPluginWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Hide(); break;
        case 1: SetWindowSize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: SetScrollValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void View::VstPluginWindow::Hide()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
