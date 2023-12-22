/****************************************************************************
** Meta object code from reading C++ file 'uimatrix.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../calc001/uimatrix.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uimatrix.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSUImatrixENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSUImatrixENDCLASS = QtMocHelpers::stringData(
    "UImatrix",
    "sendMatrix",
    "",
    "data",
    "cols",
    "rows",
    "input_matrix1",
    "input_matrix2",
    "action_add",
    "action_sub",
    "action_mul",
    "action_tra",
    "action_rnk",
    "action_sly",
    "action_det",
    "prepareMatrix"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSUImatrixENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[9];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[5];
    char stringdata5[5];
    char stringdata6[14];
    char stringdata7[14];
    char stringdata8[11];
    char stringdata9[11];
    char stringdata10[11];
    char stringdata11[11];
    char stringdata12[11];
    char stringdata13[11];
    char stringdata14[11];
    char stringdata15[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSUImatrixENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSUImatrixENDCLASS_t qt_meta_stringdata_CLASSUImatrixENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "UImatrix"
        QT_MOC_LITERAL(9, 10),  // "sendMatrix"
        QT_MOC_LITERAL(20, 0),  // ""
        QT_MOC_LITERAL(21, 4),  // "data"
        QT_MOC_LITERAL(26, 4),  // "cols"
        QT_MOC_LITERAL(31, 4),  // "rows"
        QT_MOC_LITERAL(36, 13),  // "input_matrix1"
        QT_MOC_LITERAL(50, 13),  // "input_matrix2"
        QT_MOC_LITERAL(64, 10),  // "action_add"
        QT_MOC_LITERAL(75, 10),  // "action_sub"
        QT_MOC_LITERAL(86, 10),  // "action_mul"
        QT_MOC_LITERAL(97, 10),  // "action_tra"
        QT_MOC_LITERAL(108, 10),  // "action_rnk"
        QT_MOC_LITERAL(119, 10),  // "action_sly"
        QT_MOC_LITERAL(130, 10),  // "action_det"
        QT_MOC_LITERAL(141, 13)   // "prepareMatrix"
    },
    "UImatrix",
    "sendMatrix",
    "",
    "data",
    "cols",
    "rows",
    "input_matrix1",
    "input_matrix2",
    "action_add",
    "action_sub",
    "action_mul",
    "action_tra",
    "action_rnk",
    "action_sly",
    "action_det",
    "prepareMatrix"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSUImatrixENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    3,   80,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    3,   87,    2, 0x0a,    5 /* Public */,
       7,    3,   94,    2, 0x0a,    9 /* Public */,
       8,    0,  101,    2, 0x0a,   13 /* Public */,
       9,    0,  102,    2, 0x0a,   14 /* Public */,
      10,    0,  103,    2, 0x0a,   15 /* Public */,
      11,    0,  104,    2, 0x0a,   16 /* Public */,
      12,    0,  105,    2, 0x0a,   17 /* Public */,
      13,    0,  106,    2, 0x0a,   18 /* Public */,
      14,    0,  107,    2, 0x0a,   19 /* Public */,
      15,    0,  108,    2, 0x0a,   20 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant, QMetaType::Int, QMetaType::Int,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QVariant, QMetaType::Int, QMetaType::Int,    3,    4,    5,
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

Q_CONSTINIT const QMetaObject UImatrix::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSUImatrixENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSUImatrixENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSUImatrixENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<UImatrix, std::true_type>,
        // method 'sendMatrix'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'input_matrix1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'input_matrix2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'action_add'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_sub'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_mul'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_tra'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_rnk'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_sly'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_det'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'prepareMatrix'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void UImatrix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UImatrix *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendMatrix((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 1: _t->input_matrix1((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 2: _t->input_matrix2((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3]))); break;
        case 3: _t->action_add(); break;
        case 4: _t->action_sub(); break;
        case 5: _t->action_mul(); break;
        case 6: _t->action_tra(); break;
        case 7: _t->action_rnk(); break;
        case 8: _t->action_sly(); break;
        case 9: _t->action_det(); break;
        case 10: _t->prepareMatrix(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UImatrix::*)(QVariant , int , int );
            if (_t _q_method = &UImatrix::sendMatrix; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *UImatrix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UImatrix::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSUImatrixENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "matrix"))
        return static_cast< matrix*>(this);
    if (!strcmp(_clname, "log"))
        return static_cast< log*>(this);
    return QObject::qt_metacast(_clname);
}

int UImatrix::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void UImatrix::sendMatrix(QVariant _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
