/****************************************************************************
** Meta object code from reading C++ file 'sceneview.h'
**
** Created: Thu 2. Dec 01:56:53 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vstboard/views/sceneview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sceneview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__SceneView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   17,   16,   16, 0x0a,
      77,   73,   16,   16, 0x0a,
     118,  113,   16,   16, 0x0a,
     139,  113,   16,   16, 0x0a,
     163,  113,   16,   16, 0x0a,
     187,  113,   16,   16, 0x0a,
     230,  210,   16,   16, 0x09,
     284,  267,   16,   16, 0x09,
     326,  267,   16,   16, 0x09,
     364,  360,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_View__SceneView[] = {
    "View::SceneView\0\0pinOut,pinIn\0"
    "ConnectPins(ConnectionInfo,ConnectionInfo)\0"
    "pin\0RemoveCablesFromPin(ConnectionInfo)\0"
    "show\0ToggleHostView(bool)\0"
    "ToggleProjectView(bool)\0ToggleProgramView(bool)\0"
    "ToggleInsertView(bool)\0topLeft,bottomRight\0"
    "dataChanged(QModelIndex,QModelIndex)\0"
    "parent,start,end\0"
    "rowsAboutToBeRemoved(QModelIndex,int,int)\0"
    "rowsInserted(QModelIndex,int,int)\0obj\0"
    "graphicObjectRemoved(QObject*)\0"
};

const QMetaObject View::SceneView::staticMetaObject = {
    { &QAbstractItemView::staticMetaObject, qt_meta_stringdata_View__SceneView,
      qt_meta_data_View__SceneView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::SceneView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::SceneView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::SceneView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__SceneView))
        return static_cast<void*>(const_cast< SceneView*>(this));
    return QAbstractItemView::qt_metacast(_clname);
}

int View::SceneView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ConnectPins((*reinterpret_cast< ConnectionInfo(*)>(_a[1])),(*reinterpret_cast< ConnectionInfo(*)>(_a[2]))); break;
        case 1: RemoveCablesFromPin((*reinterpret_cast< ConnectionInfo(*)>(_a[1]))); break;
        case 2: ToggleHostView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: ToggleProjectView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: ToggleProgramView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: ToggleInsertView((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 7: rowsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 8: rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: graphicObjectRemoved((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
