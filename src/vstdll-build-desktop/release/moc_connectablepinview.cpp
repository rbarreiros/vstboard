/****************************************************************************
** Meta object code from reading C++ file 'connectablepinview.h'
**
** Created: Thu 2. Dec 01:56:53 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/connectablepinview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connectablepinview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__ConnectablePinView[] = {

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
      26,   25,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_View__ConnectablePinView[] = {
    "View::ConnectablePinView\0\0updateVu()\0"
};

const QMetaObject View::ConnectablePinView::staticMetaObject = {
    { &PinView::staticMetaObject, qt_meta_stringdata_View__ConnectablePinView,
      qt_meta_data_View__ConnectablePinView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::ConnectablePinView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::ConnectablePinView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::ConnectablePinView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__ConnectablePinView))
        return static_cast<void*>(const_cast< ConnectablePinView*>(this));
    return PinView::qt_metacast(_clname);
}

int View::ConnectablePinView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PinView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateVu(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
