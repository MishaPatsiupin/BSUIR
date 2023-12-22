/****************************************************************************
** Meta object code from reading C++ file 'uivector.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../calc001/uivector.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uivector.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSuivectorENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSuivectorENDCLASS = QtMocHelpers::stringData(
    "uivector",
    "sendVec",
    "",
    "data",
    "input_vec1",
    "input_vec2",
    "input_vec_max",
    "action_kol",
    "action_scal",
    "action_vec",
    "prepareVec"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSuivectorENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[9];
    char stringdata1[8];
    char stringdata2[1];
    char stringdata3[5];
    char stringdata4[11];
    char stringdata5[11];
    char stringdata6[14];
    char stringdata7[11];
    char stringdata8[12];
    char stringdata9[11];
    char stringdata10[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSuivectorENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSuivectorENDCLASS_t qt_meta_stringdata_CLASSuivectorENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "uivector"
        QT_MOC_LITERAL(9, 7),  // "sendVec"
        QT_MOC_LITERAL(17, 0),  // ""
        QT_MOC_LITERAL(18, 4),  // "data"
        QT_MOC_LITERAL(23, 10),  // "input_vec1"
        QT_MOC_LITERAL(34, 10),  // "input_vec2"
        QT_MOC_LITERAL(45, 13),  // "input_vec_max"
        QT_MOC_LITERAL(59, 10),  // "action_kol"
        QT_MOC_LITERAL(70, 11),  // "action_scal"
        QT_MOC_LITERAL(82, 10),  // "action_vec"
        QT_MOC_LITERAL(93, 10)   // "prepareVec"
    },
    "uivector",
    "sendVec",
    "",
    "data",
    "input_vec1",
    "input_vec2",
    "input_vec_max",
    "action_kol",
    "action_scal",
    "action_vec",
    "prepareVec"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSuivectorENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       4,    1,   65,    2, 0x0a,    3 /* Public */,
       5,    1,   68,    2, 0x0a,    5 /* Public */,
       6,    1,   71,    2, 0x0a,    7 /* Public */,
       7,    0,   74,    2, 0x0a,    9 /* Public */,
       8,    0,   75,    2, 0x0a,   10 /* Public */,
       9,    0,   76,    2, 0x0a,   11 /* Public */,
      10,    0,   77,    2, 0x0a,   12 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject uivector::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSuivectorENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSuivectorENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSuivectorENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<uivector, std::true_type>,
        // method 'sendVec'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        // method 'input_vec1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        // method 'input_vec2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        // method 'input_vec_max'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        // method 'action_kol'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_scal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'action_vec'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'prepareVec'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void uivector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<uivector *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendVec((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 1: _t->input_vec1((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 2: _t->input_vec2((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 3: _t->input_vec_max((*reinterpret_cast< std::add_pointer_t<QVariant>>(_a[1]))); break;
        case 4: _t->action_kol(); break;
        case 5: _t->action_scal(); break;
        case 6: _t->action_vec(); break;
        case 7: _t->prepareVec(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (uivector::*)(QVariant );
            if (_t _q_method = &uivector::sendVec; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *uivector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *uivector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSuivectorENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "vector"))
        return static_cast< vector*>(this);
    if (!strcmp(_clname, "log"))
        return static_cast< log*>(this);
    return QObject::qt_metacast(_clname);
}

int uivector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void uivector::sendVec(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
