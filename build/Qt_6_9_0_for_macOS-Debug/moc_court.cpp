/****************************************************************************
** Meta object code from reading C++ file 'court.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../court.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'court.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN5courtE_t {};
} // unnamed namespace

template <> constexpr inline auto court::qt_create_metaobjectdata<qt_meta_tag_ZN5courtE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "court",
        "scoreChanged",
        "",
        "player1Score",
        "player2Score",
        "player1ScoreChanged",
        "score",
        "player2ScoreChanged",
        "pointsScored",
        "animateCrowd"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'scoreChanged'
        QtMocHelpers::SignalData<void(int, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 },
        }}),
        // Signal 'player1ScoreChanged'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'player2ScoreChanged'
        QtMocHelpers::SignalData<void(int)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'pointsScored'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'animateCrowd'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<court, qt_meta_tag_ZN5courtE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject court::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5courtE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5courtE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN5courtE_t>.metaTypes,
    nullptr
} };

void court::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<court *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->scoreChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->player1ScoreChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->player2ScoreChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->pointsScored(); break;
        case 4: _t->animateCrowd(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (court::*)(int , int )>(_a, &court::scoreChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (court::*)(int )>(_a, &court::player1ScoreChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (court::*)(int )>(_a, &court::player2ScoreChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (court::*)()>(_a, &court::pointsScored, 3))
            return;
    }
}

const QMetaObject *court::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *court::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN5courtE_t>.strings))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int court::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void court::scoreChanged(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void court::player1ScoreChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void court::player2ScoreChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void court::pointsScored()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
