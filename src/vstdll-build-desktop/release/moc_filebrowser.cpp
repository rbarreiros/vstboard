/****************************************************************************
** Meta object code from reading C++ file 'filebrowser.h'
**
** Created: Thu 2. Dec 01:56:57 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/filebrowser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'filebrowser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileBrowser[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      34,   12,   12,   12, 0x08,
      59,   12,   12,   12, 0x08,
      80,   12,   12,   12, 0x08,
     109,  103,   12,   12, 0x08,
     149,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FileBrowser[] = {
    "FileBrowser\0\0on_nextDir_clicked()\0"
    "on_previousDir_clicked()\0on_rootDir_clicked()\0"
    "on_parentDir_clicked()\0index\0"
    "on_treeFiles_doubleClicked(QModelIndex)\0"
    "on_path_textEdited(QString)\0"
};

const QMetaObject FileBrowser::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FileBrowser,
      qt_meta_data_FileBrowser, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileBrowser::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileBrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileBrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileBrowser))
        return static_cast<void*>(const_cast< FileBrowser*>(this));
    return QWidget::qt_metacast(_clname);
}

int FileBrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_nextDir_clicked(); break;
        case 1: on_previousDir_clicked(); break;
        case 2: on_rootDir_clicked(); break;
        case 3: on_parentDir_clicked(); break;
        case 4: on_treeFiles_doubleClicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 5: on_path_textEdited((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
