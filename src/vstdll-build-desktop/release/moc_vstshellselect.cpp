/****************************************************************************
** Meta object code from reading C++ file 'vstshellselect.h'
**
** Created: Thu 2. Dec 01:57:01 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/vstshellselect.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vstshellselect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__VstShellSelect[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x08,
      48,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_View__VstShellSelect[] = {
    "View::VstShellSelect\0\0on_buttonCancel_clicked()\0"
    "on_buttonOk_clicked()\0"
};

const QMetaObject View::VstShellSelect::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_View__VstShellSelect,
      qt_meta_data_View__VstShellSelect, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::VstShellSelect::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::VstShellSelect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::VstShellSelect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__VstShellSelect))
        return static_cast<void*>(const_cast< VstShellSelect*>(this));
    return QWidget::qt_metacast(_clname);
}

int View::VstShellSelect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_buttonCancel_clicked(); break;
        case 1: on_buttonOk_clicked(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
