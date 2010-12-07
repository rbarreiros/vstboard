/****************************************************************************
** Meta object code from reading C++ file 'objectview.h'
**
** Created: Thu 2. Dec 01:56:52 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/objectview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'objectview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__ObjectView[] = {

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
      18,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_View__ObjectView[] = {
    "View::ObjectView\0\0ShrinkNow()\0"
};

const QMetaObject View::ObjectView::staticMetaObject = {
    { &QGraphicsWidget::staticMetaObject, qt_meta_stringdata_View__ObjectView,
      qt_meta_data_View__ObjectView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::ObjectView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::ObjectView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::ObjectView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__ObjectView))
        return static_cast<void*>(const_cast< ObjectView*>(this));
    return QGraphicsWidget::qt_metacast(_clname);
}

int View::ObjectView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ShrinkNow(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
