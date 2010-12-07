/****************************************************************************
** Meta object code from reading C++ file 'programs.h'
**
** Created: Thu 2. Dec 01:57:00 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/programs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'programs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Programs[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   44,    9,    9, 0x0a,
      77,   70,    9,    9, 0x0a,
      94,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Programs[] = {
    "Programs\0\0prgIndex\0ProgChanged(QModelIndex)\0"
    "midiPrgId\0ChangeProg(int)\0grpNum\0"
    "ChangeGroup(int)\0ChangeProg(QModelIndex)\0"
};

const QMetaObject Programs::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Programs,
      qt_meta_data_Programs, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Programs::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Programs::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Programs::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Programs))
        return static_cast<void*>(const_cast< Programs*>(this));
    return QObject::qt_metacast(_clname);
}

int Programs::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ProgChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: ChangeProg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: ChangeGroup((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: ChangeProg((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Programs::ProgChanged(const QModelIndex & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
