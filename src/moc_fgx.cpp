/****************************************************************************
** Meta object code from reading C++ file 'fgx.h'
**
** Created: Tue Mar 22 14:02:46 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "fgx.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fgx.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fgx[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      26,    4,    4,    4, 0x08,
      62,    4,    4,    4, 0x08,
      74,    4,    4,    4, 0x08,
      98,    4,    4,    4, 0x08,
     129,    4,    4,    4, 0x08,
     150,    4,    4,    4, 0x08,
     172,    4,    4,    4, 0x08,
     200,    4,    4,    4, 0x08,
     229,    4,    4,    4, 0x08,
     257,    4,    4,    4, 0x08,
     271,    4,    4,    4, 0x08,
     286,    4,    4,    4, 0x08,
     301,    4,    4,    4, 0x08,
     317,    4,    4,    4, 0x08,
     341,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_fgx[] = {
    "fgx\0\0on_fgStart_clicked()\0"
    "on_set_fgdata_path_Button_clicked()\0"
    "checkFGFS()\0on_useFGXfgfs_clicked()\0"
    "on_enableMultiplayer_clicked()\0"
    "on_setTime_clicked()\0on_useMetar_clicked()\0"
    "on_useCoordinates_clicked()\0"
    "on_useParkPosition_clicked()\0"
    "on_locationIcao_activated()\0checkCoords()\0"
    "checkScenery()\0readSettings()\0"
    "writeSettings()\0on_airCraft_activated()\0"
    "on_tabs_currentChanged()\0"
};

const QMetaObject fgx::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_fgx,
      qt_meta_data_fgx, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fgx::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fgx::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fgx::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fgx))
        return static_cast<void*>(const_cast< fgx*>(this));
    if (!strcmp(_clname, "Ui::mainWindow"))
        return static_cast< Ui::mainWindow*>(const_cast< fgx*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int fgx::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_fgStart_clicked(); break;
        case 1: on_set_fgdata_path_Button_clicked(); break;
        case 2: checkFGFS(); break;
        case 3: on_useFGXfgfs_clicked(); break;
        case 4: on_enableMultiplayer_clicked(); break;
        case 5: on_setTime_clicked(); break;
        case 6: on_useMetar_clicked(); break;
        case 7: on_useCoordinates_clicked(); break;
        case 8: on_useParkPosition_clicked(); break;
        case 9: on_locationIcao_activated(); break;
        case 10: checkCoords(); break;
        case 11: checkScenery(); break;
        case 12: readSettings(); break;
        case 13: writeSettings(); break;
        case 14: on_airCraft_activated(); break;
        case 15: on_tabs_currentChanged(); break;
        default: ;
        }
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE