/****************************************************************************
** Meta object code from reading C++ file 'qxtflowview.h'
**
** Created: Wed Feb 25 19:49:30 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "C:/Workspace/Resonance/../../Library/Qt/libqxt-0.4/src/gui/qxtflowview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qxtflowview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QxtFlowView[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       6,   55, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x05,
      62,   52,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   13,   12,   12, 0x0a,
     115,   12,   12,   12, 0x0a,
     130,   12,   12,   12, 0x0a,
     141,   13,   12,   12, 0x0a,
     164,   12,   12,   12, 0x0a,
     173,   12,   12,   12, 0x0a,
     189,   12,   12,   12, 0x08,

 // properties: name, type, flags
     214,  207, 0x43095103,
     236,  230, 0x15095103,
     258,  246, 0x0009510b,
     275,  271, 0x02095103,
     287,  271, 0x02095103,
     300,  246, 0x0009510b,

       0        // eod
};

static const char qt_meta_stringdata_QxtFlowView[] = {
    "QxtFlowView\0\0index\0currentIndexChanged(QModelIndex)\0"
    "new_image\0newImageChosen(QString)\0"
    "setCurrentIndex(QModelIndex)\0"
    "showPrevious()\0showNext()\0"
    "showSlide(QModelIndex)\0render()\0"
    "triggerRender()\0updateAnimation()\0"
    "QColor\0backgroundColor\0QSize\0slideSize\0"
    "QModelIndex\0currentIndex\0int\0pictureRole\0"
    "picureColumn\0rootIndex\0"
};

const QMetaObject QxtFlowView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QxtFlowView,
      qt_meta_data_QxtFlowView, 0 }
};

const QMetaObject *QxtFlowView::metaObject() const
{
    return &staticMetaObject;
}

void *QxtFlowView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QxtFlowView))
	return static_cast<void*>(const_cast< QxtFlowView*>(this));
    return QWidget::qt_metacast(_clname);
}

int QxtFlowView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentIndexChanged((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: newImageChosen((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: setCurrentIndex((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 3: showPrevious(); break;
        case 4: showNext(); break;
        case 5: showSlide((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 6: render(); break;
        case 7: triggerRender(); break;
        case 8: updateAnimation(); break;
        }
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QColor*>(_v) = backgroundColor(); break;
        case 1: *reinterpret_cast< QSize*>(_v) = slideSize(); break;
        case 2: *reinterpret_cast< QModelIndex*>(_v) = currentIndex(); break;
        case 3: *reinterpret_cast< int*>(_v) = pictureRole(); break;
        case 4: *reinterpret_cast< int*>(_v) = picureColumn(); break;
        case 5: *reinterpret_cast< QModelIndex*>(_v) = rootIndex(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setBackgroundColor(*reinterpret_cast< QColor*>(_v)); break;
        case 1: setSlideSize(*reinterpret_cast< QSize*>(_v)); break;
        case 2: setCurrentIndex(*reinterpret_cast< QModelIndex*>(_v)); break;
        case 3: setPictureRole(*reinterpret_cast< int*>(_v)); break;
        case 4: setPicureColumn(*reinterpret_cast< int*>(_v)); break;
        case 5: setRootIndex(*reinterpret_cast< QModelIndex*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QxtFlowView::currentIndexChanged(QModelIndex _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QxtFlowView::newImageChosen(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
