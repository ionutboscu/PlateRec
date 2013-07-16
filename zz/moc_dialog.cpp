/****************************************************************************
** Meta object code from reading C++ file 'dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Dialog_t {
    QByteArrayData data[23];
    char stringdata[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Dialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Dialog_t qt_meta_stringdata_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 6),
QT_MOC_LITERAL(1, 7, 14),
QT_MOC_LITERAL(2, 22, 0),
QT_MOC_LITERAL(3, 23, 6),
QT_MOC_LITERAL(4, 30, 3),
QT_MOC_LITERAL(5, 34, 3),
QT_MOC_LITERAL(6, 38, 6),
QT_MOC_LITERAL(7, 45, 7),
QT_MOC_LITERAL(8, 53, 11),
QT_MOC_LITERAL(9, 65, 14),
QT_MOC_LITERAL(10, 80, 16),
QT_MOC_LITERAL(11, 97, 19),
QT_MOC_LITERAL(12, 117, 9),
QT_MOC_LITERAL(13, 127, 4),
QT_MOC_LITERAL(14, 132, 6),
QT_MOC_LITERAL(15, 139, 8),
QT_MOC_LITERAL(16, 148, 13),
QT_MOC_LITERAL(17, 162, 5),
QT_MOC_LITERAL(18, 168, 11),
QT_MOC_LITERAL(19, 180, 11),
QT_MOC_LITERAL(20, 192, 2),
QT_MOC_LITERAL(21, 195, 6),
QT_MOC_LITERAL(22, 202, 21)
    },
    "Dialog\0on_btn_clicked\0\0toGray\0Mat\0mat\0"
    "toBlur\0toSobel\0toThreshold\0toMorphologyEx\0"
    "drawBlueContours\0vector<RotatedRect>\0"
    "threshold\0Mat&\0result\0getPlate\0"
    "vector<Plate>\0rects\0verifySizes\0"
    "RotatedRect\0mr\0histeq\0processFrameAndUpdate\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08,
       3,    1,   70,    2, 0x08,
       6,    1,   73,    2, 0x08,
       7,    1,   76,    2, 0x08,
       8,    1,   79,    2, 0x08,
       9,    1,   82,    2, 0x08,
      10,    3,   85,    2, 0x08,
      15,    3,   92,    2, 0x08,
      18,    1,   99,    2, 0x08,
      21,    1,  102,    2, 0x08,
      22,    0,  105,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    0x80000000 | 4, 0x80000000 | 4,    5,
    0x80000000 | 4, 0x80000000 | 4,    5,
    0x80000000 | 4, 0x80000000 | 4,    5,
    0x80000000 | 4, 0x80000000 | 4,    5,
    0x80000000 | 4, 0x80000000 | 4,    5,
    0x80000000 | 11, 0x80000000 | 4, 0x80000000 | 4, 0x80000000 | 13,    5,   12,   14,
    0x80000000 | 16, 0x80000000 | 4, 0x80000000 | 4, 0x80000000 | 11,    5,   14,   17,
    QMetaType::Bool, 0x80000000 | 19,   20,
    0x80000000 | 4, 0x80000000 | 4,    5,
    QMetaType::Void,

       0        // eod
};

void Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Dialog *_t = static_cast<Dialog *>(_o);
        switch (_id) {
        case 0: _t->on_btn_clicked(); break;
        case 1: { Mat _r = _t->toGray((*reinterpret_cast< Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 2: { Mat _r = _t->toBlur((*reinterpret_cast< Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 3: { Mat _r = _t->toSobel((*reinterpret_cast< Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 4: { Mat _r = _t->toThreshold((*reinterpret_cast< Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 5: { Mat _r = _t->toMorphologyEx((*reinterpret_cast< Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 6: { vector<RotatedRect> _r = _t->drawBlueContours((*reinterpret_cast< Mat(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2])),(*reinterpret_cast< Mat(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< vector<RotatedRect>*>(_a[0]) = _r; }  break;
        case 7: { vector<Plate> _r = _t->getPlate((*reinterpret_cast< Mat(*)>(_a[1])),(*reinterpret_cast< Mat(*)>(_a[2])),(*reinterpret_cast< vector<RotatedRect>(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< vector<Plate>*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->verifySizes((*reinterpret_cast< RotatedRect(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { Mat _r = _t->histeq((*reinterpret_cast< Mat(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< Mat*>(_a[0]) = _r; }  break;
        case 10: _t->processFrameAndUpdate(); break;
        default: ;
        }
    }
}

const QMetaObject Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog.data,
      qt_meta_data_Dialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog.stringdata))
        return static_cast<void*>(const_cast< Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
