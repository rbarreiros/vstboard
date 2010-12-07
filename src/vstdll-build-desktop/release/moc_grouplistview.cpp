/****************************************************************************
** Meta object code from reading C++ file 'grouplistview.h'
**
** Created: Thu 2. Dec 01:56:57 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/grouplistview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'grouplistview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GroupListView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   15,   14,   14, 0x05,
      79,   73,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     106,  102,   14,   14, 0x0a,
     128,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GroupListView[] = {
    "GroupListView\0\0source,index\0"
    "DragOverItemFromWidget(QWidget*,QModelIndex)\0"
    "index\0StartDrag(QModelIndex)\0pos\0"
    "OnContextMenu(QPoint)\0DeleteItem()\0"
};

const QMetaObject GroupListView::staticMetaObject = {
    { &QListView::staticMetaObject, qt_meta_stringdata_GroupListView,
      qt_meta_data_GroupListView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GroupListView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GroupListView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GroupListView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GroupListView))
        return static_cast<void*>(const_cast< GroupListView*>(this));
    return QListView::qt_metacast(_clname);
}

int GroupListView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: DragOverItemFromWidget((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 1: StartDrag((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: OnContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: DeleteItem(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GroupListView::DragOverItemFromWidget(QWidget * _t1, const QModelIndex & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GroupListView::StartDrag(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
