/****************************************************************************
** Meta object code from reading C++ file 'maingraphicsview.h'
**
** Created: Thu 2. Dec 01:56:34 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstdll/views/maingraphicsview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maingraphicsview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainGraphicsView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   18,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,   17,   17,   17, 0x0a,
      53,   17,   17,   17, 0x0a,
      63,   17,   17,   17, 0x0a,
      75,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainGraphicsView[] = {
    "MainGraphicsView\0\0trans\0viewResized(QRectF)\0"
    "zoomIn()\0zoomOut()\0zoomReset()\0"
    "ForceResize()\0"
};

const QMetaObject MainGraphicsView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_MainGraphicsView,
      qt_meta_data_MainGraphicsView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainGraphicsView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainGraphicsView))
        return static_cast<void*>(const_cast< MainGraphicsView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int MainGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: viewResized((*reinterpret_cast< QRectF(*)>(_a[1]))); break;
        case 1: zoomIn(); break;
        case 2: zoomOut(); break;
        case 3: zoomReset(); break;
        case 4: ForceResize(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MainGraphicsView::viewResized(QRectF _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
