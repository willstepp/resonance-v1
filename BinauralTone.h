#ifndef BINAURALTONE_H
#define BINAURALTONE_H

#include <QtCore>

#include "fmod.hpp"

struct BinauralData
{
	QString name;

	float default_frequency;
	int default_gap;

	float octave_one;
	float octave_two;
	float octave_three;
	float octave_four;
	float octave_five;
	float octave_six;

	BinauralData() : name(""), default_gap(1){}
};

class BinauralTone : public QObject
{
	Q_OBJECT

public:
	enum Type { Delta, Theta, Alpha, Beta };
	enum Note { A, A_Sharp, B, C, C_Sharp, D, D_Sharp, E, F, F_Sharp, G, G_Sharp }; 

	BinauralTone(FMOD::System * system, BinauralData data, QObject * parent = 0);
	virtual ~BinauralTone();

	bool loaded()			{ return m_loaded; }

	void setType(Type t)	{ m_type = t; }
	void setNote(Note n)	{ m_note = n; }

	void setId(int id)		{ m_id = id; }

	void play(float volume);
	void stop();

	void setPaused(bool state);
	void setVolume(float volume);

	void setFrequencey(float frequency);
	void setBinauralGap(int gap);

	void release();

	void createWhiteNoise(float volume);
	void destroyWhiteNoise();

	void setWhiteNoiseVolume(float volume);

private:
	FMOD::System * m_system;

	FMOD::DSP * m_primary_tone;
	FMOD::DSP * m_secondary_tone;

	FMOD::DSP * m_white_noise;

	FMOD::Channel * m_primary_channel;
	FMOD::Channel * m_secondary_channel;

	FMOD::Channel * m_white_noise_channel;

	Type m_type;
	Note m_note;

	int m_id;

	int m_frequency;
	int m_binaural_gap;

	BinauralData m_data;

	bool m_loaded;
};

#endif // BINAURALTONE_H
