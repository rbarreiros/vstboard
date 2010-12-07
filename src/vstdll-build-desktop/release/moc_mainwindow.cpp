/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu 2. Dec 01:56:32 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstdll/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      56,   11,   11,   11, 0x08,
      91,   11,   11,   11, 0x08,
     128,   11,   11,   11, 0x08,
     156,   11,   11,   11, 0x08,
     187,   11,   11,   11, 0x08,
     219,   11,   11,   11, 0x08,
     244,   11,   11,   11, 0x08,
     270,   11,   11,   11, 0x08,
     296,   11,   11,   11, 0x08,
     314,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_actionRestore_default_layout_triggered()\0"
    "on_actionSave_Setup_As_triggered()\0"
    "on_actionSave_Project_As_triggered()\0"
    "on_actionConfig_triggered()\0"
    "on_actionNew_Setup_triggered()\0"
    "on_actionSave_Setup_triggered()\0"
    "on_actionNew_triggered()\0"
    "on_actionLoad_triggered()\0"
    "on_actionSave_triggered()\0openRecentSetup()\0"
    "openRecentProject()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_actionRestore_default_layout_triggered(); break;
        case 1: on_actionSave_Setup_As_triggered(); break;
        case 2: on_actionSave_Project_As_triggered(); break;
        case 3: on_actionConfig_triggered(); break;
        case 4: on_actionNew_Setup_triggered(); break;
        case 5: on_actionSave_Setup_triggered(); break;
        case 6: on_actionNew_triggered(); break;
        case 7: on_actionLoad_triggered(); break;
        case 8: on_actionSave_triggered(); break;
        case 9: openRecentSetup(); break;
        case 10: openRecentProject(); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
