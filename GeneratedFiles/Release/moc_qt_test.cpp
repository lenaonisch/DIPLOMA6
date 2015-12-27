/****************************************************************************
** Meta object code from reading C++ file 'qt_test.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../qt_test.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qt_test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_qt_test_t {
    QByteArrayData data[19];
    char stringdata0[489];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_qt_test_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_qt_test_t qt_meta_stringdata_qt_test = {
    {
QT_MOC_LITERAL(0, 0, 7), // "qt_test"
QT_MOC_LITERAL(1, 8, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 35), // "on_actionLoad_config_file_tri..."
QT_MOC_LITERAL(4, 69, 24), // "on_actionTrain_triggered"
QT_MOC_LITERAL(5, 94, 35), // "on_actionLoad_test_images_tri..."
QT_MOC_LITERAL(6, 130, 30), // "on_actionShow_leaves_triggered"
QT_MOC_LITERAL(7, 161, 25), // "on_actionDetect_triggered"
QT_MOC_LITERAL(8, 187, 28), // "on_actionZoom_in_2_triggered"
QT_MOC_LITERAL(9, 216, 29), // "on_actionZoom_out_2_triggered"
QT_MOC_LITERAL(10, 246, 31), // "on_actionBatch_detect_triggered"
QT_MOC_LITERAL(11, 278, 33), // "on_actionTest_local_max_trigg..."
QT_MOC_LITERAL(12, 312, 29), // "on_actionMean_shift_triggered"
QT_MOC_LITERAL(13, 342, 25), // "on_btnAddPositive_clicked"
QT_MOC_LITERAL(14, 368, 25), // "on_btnAddNegative_clicked"
QT_MOC_LITERAL(15, 394, 21), // "on_btnRefresh_clicked"
QT_MOC_LITERAL(16, 416, 28), // "on_btnDropAllResults_clicked"
QT_MOC_LITERAL(17, 445, 20), // "on_btnRemove_clicked"
QT_MOC_LITERAL(18, 466, 22) // "on_treeResults_clicked"

    },
    "qt_test\0on_actionOpen_triggered\0\0"
    "on_actionLoad_config_file_triggered\0"
    "on_actionTrain_triggered\0"
    "on_actionLoad_test_images_triggered\0"
    "on_actionShow_leaves_triggered\0"
    "on_actionDetect_triggered\0"
    "on_actionZoom_in_2_triggered\0"
    "on_actionZoom_out_2_triggered\0"
    "on_actionBatch_detect_triggered\0"
    "on_actionTest_local_max_triggered\0"
    "on_actionMean_shift_triggered\0"
    "on_btnAddPositive_clicked\0"
    "on_btnAddNegative_clicked\0"
    "on_btnRefresh_clicked\0"
    "on_btnDropAllResults_clicked\0"
    "on_btnRemove_clicked\0on_treeResults_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qt_test[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x0a /* Public */,
       3,    0,  100,    2, 0x0a /* Public */,
       4,    0,  101,    2, 0x0a /* Public */,
       5,    0,  102,    2, 0x0a /* Public */,
       6,    0,  103,    2, 0x0a /* Public */,
       7,    0,  104,    2, 0x0a /* Public */,
       8,    0,  105,    2, 0x0a /* Public */,
       9,    0,  106,    2, 0x0a /* Public */,
      10,    0,  107,    2, 0x0a /* Public */,
      11,    0,  108,    2, 0x0a /* Public */,
      12,    0,  109,    2, 0x0a /* Public */,
      13,    0,  110,    2, 0x0a /* Public */,
      14,    0,  111,    2, 0x0a /* Public */,
      15,    0,  112,    2, 0x0a /* Public */,
      16,    0,  113,    2, 0x0a /* Public */,
      17,    0,  114,    2, 0x0a /* Public */,
      18,    0,  115,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void qt_test::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        qt_test *_t = static_cast<qt_test *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionOpen_triggered(); break;
        case 1: _t->on_actionLoad_config_file_triggered(); break;
        case 2: _t->on_actionTrain_triggered(); break;
        case 3: _t->on_actionLoad_test_images_triggered(); break;
        case 4: _t->on_actionShow_leaves_triggered(); break;
        case 5: _t->on_actionDetect_triggered(); break;
        case 6: _t->on_actionZoom_in_2_triggered(); break;
        case 7: _t->on_actionZoom_out_2_triggered(); break;
        case 8: _t->on_actionBatch_detect_triggered(); break;
        case 9: _t->on_actionTest_local_max_triggered(); break;
        case 10: _t->on_actionMean_shift_triggered(); break;
        case 11: _t->on_btnAddPositive_clicked(); break;
        case 12: _t->on_btnAddNegative_clicked(); break;
        case 13: _t->on_btnRefresh_clicked(); break;
        case 14: _t->on_btnDropAllResults_clicked(); break;
        case 15: _t->on_btnRemove_clicked(); break;
        case 16: _t->on_treeResults_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject qt_test::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_qt_test.data,
      qt_meta_data_qt_test,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *qt_test::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qt_test::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_qt_test.stringdata0))
        return static_cast<void*>(const_cast< qt_test*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int qt_test::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
