/****************************************************************************
** Meta object code from reading C++ file 'maincontainerview.h'
**
** Created: Thu 2. Dec 01:56:55 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/maincontainerview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maincontainerview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__MainContainerView[] = {

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
      30,   25,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_View__MainContainerView[] = {
    "View::MainContainerView\0\0rect\0"
    "OnViewChanged(QRectF)\0"
};

const QMetaObject View::MainContainerView::staticMetaObject = {
    { &ContainerView::staticMetaObject, qt_meta_stringdata_View__MainContainerView,
      qt_meta_data_View__MainContainerView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::MainContainerView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::MainContainerView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::MainContainerView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__MainContainerView))
        return static_cast<void*>(const_cast< MainContainerView*>(this));
    return ContainerView::qt_metacast(_clname);
}

int View::MainContainerView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ContainerView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnViewChanged((*reinterpret_cast< QRectF(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
