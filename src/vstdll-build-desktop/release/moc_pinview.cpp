/****************************************************************************
** Meta object code from reading C++ file 'pinview.h'
**
** Created: Thu 2. Dec 01:56:52 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/pinview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pinview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__PinView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      34,   15,   14,   14, 0x05,
      81,   77,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     117,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_View__PinView[] = {
    "View::PinView\0\0outputPin,inputPin\0"
    "ConnectPins(ConnectionInfo,ConnectionInfo)\0"
    "pin\0RemoveCablesFromPin(ConnectionInfo)\0"
    "updateVu()\0"
};

const QMetaObject View::PinView::staticMetaObject = {
    { &QGraphicsWidget::staticMetaObject, qt_meta_stringdata_View__PinView,
      qt_meta_data_View__PinView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::PinView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::PinView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::PinView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__PinView))
        return static_cast<void*>(const_cast< PinView*>(this));
    return QGraphicsWidget::qt_metacast(_clname);
}

int View::PinView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ConnectPins((*reinterpret_cast< ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< ConnectionInfo(*)>(_a[2]))); break;
        case 1: RemoveCablesFromPin((*reinterpret_cast< ConnectionInfo(*)>(_a[1]))); break;
        case 2: updateVu(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void View::PinView::ConnectPins(ConnectionInfo _t1, ConnectionInfo _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void View::PinView::RemoveCablesFromPin(ConnectionInfo _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
