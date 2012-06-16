/****************************************************************************
** Meta object code from reading C++ file 'Resonance.h'
**
** Created: Wed Feb 25 19:49:31 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Resonance.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Resonance.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Resonance[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      18,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   43,   10,   10, 0x0a,
      74,   10,   10,   10, 0x0a,
      90,   10,   10,   10, 0x0a,
     106,   10,   10,   10, 0x0a,
     122,  119,   10,   10, 0x0a,
     147,  140,   10,   10, 0x0a,
     179,  170,   10,   10, 0x0a,
     199,   10,   10,   10, 0x0a,
     219,   10,   10,   10, 0x0a,
     249,  238,   10,   10, 0x0a,
     282,  275,   10,   10, 0x0a,
     346,  332,   10,   10, 0x0a,
     386,  379,   10,   10, 0x0a,
     406,   10,   10,   10, 0x2a,
     436,  419,   10,   10, 0x0a,
     462,  379,   10,   10, 0x0a,
     499,  493,   10,   10, 0x0a,
     526,   10,   10,   10, 0x0a,
     550,  545,   10,   10, 0x0a,
     570,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Resonance[] = {
    "Resonance\0\0volume\0masterVolumeChanged(int)\0"
    "module\0updateModuleLayout(int)\0"
    "toggleModules()\0togglePresets()\0"
    "togglePlay()\0on\0toggleTimer(bool)\0"
    "window\0toggleMainWindows(int)\0tutorial\0"
    "toggleTutorial(int)\0startSoundTimeout()\0"
    "stopSoundTimeout()\0time_to_go\0"
    "updateCountdownClock(int)\0reason\0"
    "maximizeWindow(QSystemTrayIcon::ActivationReason)\0"
    "preset,column\0loadPreset(QTreeWidgetItem*,int)\0"
    "preset\0savePreset(QString)\0savePreset()\0"
    "module_id,paused\0setModulePaused(int,bool)\0"
    "removePreset(QTreeWidgetItem*)\0index\0"
    "newBackgroundInfo(QString)\0"
    "setNewBackground()\0menu\0changeHelpMenu(int)\0"
    "gotoWebSite()\0"
};

const QMetaObject Resonance::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Resonance,
      qt_meta_data_Resonance, 0 }
};

const QMetaObject *Resonance::metaObject() const
{
    return &staticMetaObject;
}

void *Resonance::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Resonance))
	return static_cast<void*>(const_cast< Resonance*>(this));
    return QWidget::qt_metacast(_clname);
}

int Resonance::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: masterVolumeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: updateModuleLayout((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: toggleModules(); break;
        case 3: togglePresets(); break;
        case 4: togglePlay(); break;
        case 5: toggleTimer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: toggleMainWindows((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: toggleTutorial((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: startSoundTimeout(); break;
        case 9: stopSoundTimeout(); break;
        case 10: updateCountdownClock((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: maximizeWindow((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 12: loadPreset((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: savePreset((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: savePreset(); break;
        case 15: setModulePaused((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 16: removePreset((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 17: newBackgroundInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: setNewBackground(); break;
        case 19: changeHelpMenu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: gotoWebSite(); break;
        }
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void Resonance::masterVolumeChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
