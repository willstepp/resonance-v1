/****************************************************************************
** Meta object code from reading C++ file 'drawerwidget.h'
**
** Created: Wed Feb 25 19:49:30 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "drawerwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'drawerwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DrawerWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,   13,   13,   13, 0x0a,
      41,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DrawerWidget[] = {
    "DrawerWidget\0\0drawerMoved()\0openDrawer()\0"
    "closeDrawer()\0"
};

const QMetaObject DrawerWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DrawerWidget,
      qt_meta_data_DrawerWidget, 0 }
};

const QMetaObject *DrawerWidget::metaObject() const
{
    return &staticMetaObject;
}

void *DrawerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DrawerWidget))
	return static_cast<void*>(const_cast< DrawerWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DrawerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: drawerMoved(); break;
        case 1: openDrawer(); break;
        case 2: closeDrawer(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DrawerWidget::drawerMoved()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
