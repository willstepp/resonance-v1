/****************************************************************************
** Meta object code from reading C++ file 'ResoTree.h'
**
** Created: Wed Feb 25 19:49:32 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ResoTree.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ResoTree.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ResoTree[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ResoTree[] = {
    "ResoTree\0\0item\0removeSound(QTreeWidgetItem*)\0"
    "removeSoundClicked()\0"
};

const QMetaObject ResoTree::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_ResoTree,
      qt_meta_data_ResoTree, 0 }
};

const QMetaObject *ResoTree::metaObject() const
{
    return &staticMetaObject;
}

void *ResoTree::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ResoTree))
	return static_cast<void*>(const_cast< ResoTree*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int ResoTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: removeSound((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 1: removeSoundClicked(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ResoTree::removeSound(QTreeWidgetItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
