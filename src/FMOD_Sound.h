#ifndef _SOUND_H_
#define _SOUND_H_

#include <QtCore>

#include "fmod.hpp"
#include "BinauralTone.h"

class FMOD_Sound : public QObject
{
	Q_OBJECT

public:
	FMOD_Sound(int maxChannels = DEFAULT_MAX_CHANNELS, QObject * parent = 0);
	virtual ~FMOD_Sound();

	enum Effect { Reverb, Pitch, Distortion, Echo, Flange };

public slots:
	bool loadNewSound(int module_id, QString sound_path);
	bool loadNewTone(int module_id, BinauralData data);

	bool play(int module_id, float volume);
	void stop(int module_id);
	void setPaused(int module_id, bool state);
	void setVolume(int module_id, float volume);

	void addEffect(int module_id, int type, float value);
	void removeEffect(int module_id, int type);

	void setEffectValue(int module_id, int type, float new_value);

	bool playTone(int module_id, float volume);
	void stopTone(int module_id);
	void setTonePaused(int module_id, bool state);
	void setToneVolume(int module_id, float volume);

	void changeOctave(int module_id, float octave);
	void changeBinauralGap(int module_id, int gap);

	void addWhiteNoise(int module_id, float volume);
	void removeWhiteNoise(int module_id);

	void setWhiteNoiseVolume(int module_id, float volume);

private:
	FMOD::System * system;
	FMOD::System * recording;

	QMap<int, FMOD::Channel *> channelMap;
	QMap<int, FMOD::Sound *> soundMap;

	QMap<int, FMOD::DSP *> reverbMap;
	QMap<int, FMOD::DSP *> pitchMap;
	QMap<int, FMOD::DSP *> flangeMap;
	QMap<int, FMOD::DSP *> echoMap;
	QMap<int, FMOD::DSP *> distortionMap;

	static const int DEFAULT_MAX_CHANNELS = 128;

	QMap<int, BinauralTone *> toneMap;
	QMap<int, bool> toneLoadedMap;
};

#endif //_SOUND_H_
