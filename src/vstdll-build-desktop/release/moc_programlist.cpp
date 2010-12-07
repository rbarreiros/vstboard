/****************************************************************************
** Meta object code from reading C++ file 'programlist.h'
**
** Created: Thu 2. Dec 01:56:57 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/programlist.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'programlist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ProgramList[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   13,   12,   12, 0x0a,
      82,   69,   12,   12, 0x0a,
     121,   13,   12,   12, 0x0a,
     149,   12,   12,   12, 0x0a,
     185,  168,   12,   12, 0x0a,
     219,   13,   12,   12, 0x08,
     253,   13,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ProgramList[] = {
    "ProgramList\0\0index\0ChangeProg(QModelIndex)\0"
    "OnProgChange(QModelIndex)\0source,index\0"
    "OnDragOverGroups(QWidget*,QModelIndex)\0"
    "OnGrpStartDrag(QModelIndex)\0"
    "ShowCurrentGroup()\0parent,start,end\0"
    "rowsInserted(QModelIndex,int,int)\0"
    "on_listProgs_clicked(QModelIndex)\0"
    "on_listGrps_clicked(QModelIndex)\0"
};

const QMetaObject ProgramList::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ProgramList,
      qt_meta_data_ProgramList, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProgramList::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProgramList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProgramList::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProgramList))
        return static_cast<void*>(const_cast< ProgramList*>(this));
    return QWidget::qt_metacast(_clname);
}

int ProgramList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ChangeProg((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: OnProgChange((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: OnDragOverGroups((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 3: OnGrpStartDrag((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: ShowCurrentGroup(); break;
        case 5: rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: on_listProgs_clicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 7: on_listGrps_clicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ProgramList::ChangeProg(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
