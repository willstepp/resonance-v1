/****************************************************************************
** Meta object code from reading C++ file 'FMOD_Sound.h'
**
** Created: Wed Feb 25 19:49:34 2009
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FMOD_Sound.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FMOD_Sound.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FMOD_Sound[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      38,   17,   12,   11, 0x0a,
      79,   64,   12,   11, 0x0a,
     126,  109,   12,   11, 0x0a,
     152,  142,   11,   11, 0x0a,
     178,  162,   11,   11, 0x0a,
     198,  109,   11,   11, 0x0a,
     240,  219,   11,   11, 0x0a,
     280,  265,   11,   11, 0x0a,
     327,  302,   11,   11, 0x0a,
     357,  109,   12,   11, 0x0a,
     377,  142,   11,   11, 0x0a,
     391,  162,   11,   11, 0x0a,
     415,  109,   11,   11, 0x0a,
     457,  440,   11,   11, 0x0a,
     495,  481,   11,   11, 0x0a,
     522,  109,   11,   11, 0x0a,
     547,  142,   11,   11, 0x0a,
     569,  109,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FMOD_Sound[] = {
    "FMOD_Sound\0\0bool\0module_id,sound_path\0"
    "loadNewSound(int,QString)\0module_id,data\0"
    "loadNewTone(int,BinauralData)\0"
    "module_id,volume\0play(int,float)\0"
    "module_id\0stop(int)\0module_id,state\0"
    "setPaused(int,bool)\0setVolume(int,float)\0"
    "module_id,type,value\0addEffect(int,int,float)\0"
    "module_id,type\0removeEffect(int,int)\0"
    "module_id,type,new_value\0"
    "setEffectValue(int,int,float)\0"
    "playTone(int,float)\0stopTone(int)\0"
    "setTonePaused(int,bool)\0"
    "setToneVolume(int,float)\0module_id,octave\0"
    "changeOctave(int,float)\0module_id,gap\0"
    "changeBinauralGap(int,int)\0"
    "addWhiteNoise(int,float)\0removeWhiteNoise(int)\0"
    "setWhiteNoiseVolume(int,float)\0"
};

const QMetaObject FMOD_Sound::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FMOD_Sound,
      qt_meta_data_FMOD_Sound, 0 }
};

const QMetaObject *FMOD_Sound::metaObject() const
{
    return &staticMetaObject;
}

void *FMOD_Sound::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FMOD_Sound))
	return static_cast<void*>(const_cast< FMOD_Sound*>(this));
    return QObject::qt_metacast(_clname);
}

int FMOD_Sound::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { bool _r = loadNewSound((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = loadNewTone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< BinauralData(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = play((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: stop((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setPaused((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: setVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 6: addEffect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 7: removeEffect((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: setEffectValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 9: { bool _r = playTone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: stopTone((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: setTonePaused((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: setToneVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 13: changeOctave((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 14: changeBinauralGap((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: addWhiteNoise((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 16: removeWhiteNoise((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: setWhiteNoiseVolume((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        }
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
