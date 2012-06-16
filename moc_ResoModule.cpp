/****************************************************************************
** Meta object code from reading C++ file 'ResoModule.h'
**
** Created: Wed Feb 25 19:49:33 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ResoModule.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ResoModule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ResoModule[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      38,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      22,   11,   11,   11, 0x05,
      69,   48,   43,   11, 0x05,
     106,   91,   43,   11, 0x05,
     149,  132,   43,   11, 0x05,
     175,  165,   11,   11, 0x05,
     201,  185,   11,   11, 0x05,
     221,  132,   11,   11, 0x05,
     270,  242,   11,   11, 0x05,
     317,  295,   11,   11, 0x05,
     339,  242,   11,   11, 0x05,
     386,  369,   11,   11, 0x05,
     424,  410,   11,   11, 0x05,
     451,  132,   11,   11, 0x05,
     476,  165,   11,   11, 0x05,
     498,  132,   11,   11, 0x05,
     529,  132,   43,   11, 0x05,
     549,  165,   11,   11, 0x05,
     563,  185,   11,   11, 0x05,
     587,  132,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     618,  612,   11,   11, 0x0a,
     643,  612,   11,   11, 0x0a,
     682,  668,   11,   11, 0x0a,
     708,  701,   11,   11, 0x0a,
     759,  742,   11,   11, 0x0a,
     791,   11,   11,   11, 0x0a,
     805,  612,   11,   11, 0x0a,
     830,  824,   11,   11, 0x0a,
     860,  848,   11,   11, 0x0a,
     884,  848,   11,   11, 0x0a,
     922,  908,   11,   11, 0x0a,
     955,  949,   11,   11, 0x0a,
     997,  993,   11,   11, 0x0a,
    1028, 1021,   11,   11, 0x0a,
    1050,   11,   11,   11, 0x0a,
    1075, 1071,   11,   11, 0x0a,
    1098,  612,   11,   11, 0x0a,
    1127, 1120,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ResoModule[] = {
    "ResoModule\0\0clicked()\0moduleStateUpdated()\0"
    "bool\0module_id,sound_path\0"
    "newSound(int,QString)\0module_id,data\0"
    "newTone(int,BinauralData)\0module_id,volume\0"
    "play(int,float)\0module_id\0stop(int)\0"
    "module_id,state\0setPaused(int,bool)\0"
    "setVolume(int,float)\0module_id,effect_type,value\0"
    "addEffect(int,int,float)\0module_id,effect_type\0"
    "removeEffect(int,int)\0"
    "setEffectValue(int,int,float)\0"
    "module_id,octave\0changeOctave(int,float)\0"
    "module_id,gap\0changeBinauralGap(int,int)\0"
    "addWhiteNoise(int,float)\0removeWhiteNoise(int)\0"
    "setWhiteNoiseVolume(int,float)\0"
    "playTone(int,float)\0stopTone(int)\0"
    "setTonePaused(int,bool)\0"
    "setToneVolume(int,float)\0state\0"
    "updateMinCheckState(int)\0"
    "updateMaxCheckState(int)\0current_class\0"
    "loadSoundList(int)\0parent\0"
    "loadChildSounds(QTreeWidgetItem*)\0"
    "new_sound,column\0loadSound(QTreeWidgetItem*,int)\0"
    "resetModule()\0updatePlaying(int)\0value\0"
    "updateVolume(int)\0effect_type\0"
    "updateEffectsState(int)\0updateEffectsValue(int)\0"
    "master_volume\0updateForMasterVolume(int)\0"
    "sound\0removeSoundFromList(QTreeWidgetItem*)\0"
    "dir\0deleteSoundDir(QString)\0octave\0"
    "updateToneOctave(int)\0updatePanelDisplay()\0"
    "gap\0updateBinauralGap(int)\0"
    "toggleWhiteNoise(int)\0volume\0"
    "updateWhiteNoiseVolume(int)\0"
};

const QMetaObject ResoModule::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ResoModule,
      qt_meta_data_ResoModule, 0 }
};

const QMetaObject *ResoModule::metaObject() const
{
    return &staticMetaObject;
}

void *ResoModule::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ResoModule))
	return static_cast<void*>(const_cast< ResoModule*>(this));
    return QWidget::qt_metacast(_clname);
}

int ResoModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked(); break;
        case 1: moduleStateUpdated(); break;
        case 2: { bool _r = newSound((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = newTone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< BinauralData(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = play((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: stop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setPaused((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 7: setVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 8: addEffect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 9: removeEffect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: setEffectValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 11: changeOctave((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 12: changeBinauralGap((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: addWhiteNoise((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 14: removeWhiteNoise((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: setWhiteNoiseVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 16: { bool _r = playTone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: stopTone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: setTonePaused((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 19: setToneVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 20: updateMinCheckState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: updateMaxCheckState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: loadSoundList((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: loadChildSounds((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 24: loadSound((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 25: resetModule(); break;
        case 26: updatePlaying((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: updateVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: updateEffectsState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: updateEffectsValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: updateForMasterVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: removeSoundFromList((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 32: deleteSoundDir((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 33: updateToneOctave((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: updatePanelDisplay(); break;
        case 35: updateBinauralGap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: toggleWhiteNoise((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: updateWhiteNoiseVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 38;
    }
    return _id;
}

// SIGNAL 0
void ResoModule::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ResoModule::moduleStateUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
bool ResoModule::newSound(int _t1, QString _t2)
{
    bool _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
    return _t0;
}

// SIGNAL 3
bool ResoModule::newTone(int _t1, BinauralData _t2)
{
    bool _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
    return _t0;
}

// SIGNAL 4
bool ResoModule::play(int _t1, float _t2)
{
    bool _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
    return _t0;
}

// SIGNAL 5
void ResoModule::stop(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ResoModule::setPaused(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ResoModule::setVolume(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ResoModule::addEffect(int _t1, int _t2, float _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ResoModule::removeEffect(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ResoModule::setEffectValue(int _t1, int _t2, float _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ResoModule::changeOctave(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void ResoModule::changeBinauralGap(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void ResoModule::addWhiteNoise(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void ResoModule::removeWhiteNoise(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void ResoModule::setWhiteNoiseVolume(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
bool ResoModule::playTone(int _t1, float _t2)
{
    bool _t0;
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
    return _t0;
}

// SIGNAL 17
void ResoModule::stopTone(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}

// SIGNAL 18
void ResoModule::setTonePaused(int _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 18, _a);
}

// SIGNAL 19
void ResoModule::setToneVolume(int _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 19, _a);
}
QT_END_MOC_NAMESPACE
