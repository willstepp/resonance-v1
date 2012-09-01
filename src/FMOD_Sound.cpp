#include "FMOD_Sound.h"

FMOD_Sound::FMOD_Sound(int maxChannels, QObject * parent) : QObject(parent)
{
	FMOD_RESULT result;

	FMOD::System_Create(&system);

	//FMOD::System_Create(&system);
	//result = system->setOutput(FMOD_OUTPUTTYPE_WAVWRITER);
	//result = system->init(maxChannels, FMOD_INIT_NORMAL, "c:/test.wav");

	result = system->init(maxChannels, FMOD_INIT_NORMAL, 0);
}

FMOD_Sound::~FMOD_Sound()
{
	system->close();
	system->release();
}

bool FMOD_Sound::loadNewSound(int module_id, QString sound_path)
{
	//unload previous sound
	if(!toneLoadedMap.value(module_id))
	{
		if(soundMap.contains(module_id))
		{
			channelMap.value(module_id)->stop();
			soundMap.value(module_id)->release();
		}
	}
	else
	{
		if(toneMap.contains(module_id))
		{
			toneMap.value(module_id)->stop();
			toneMap.value(module_id)->release();
			delete toneMap[module_id];
			toneMap[module_id] = 0;
		}
	}

	//load new sound
	FMOD::Sound * sound;
	FMOD::Channel * channel = 0;

	toneLoadedMap.insert(module_id, false);

	soundMap.insert(module_id, sound);
	channelMap.insert(module_id, channel);

	system->createStream(qPrintable(sound_path), FMOD_SOFTWARE | FMOD_LOOP_NORMAL | FMOD_2D, 0, &soundMap[module_id]);

	return true;
}

bool FMOD_Sound::loadNewTone(int module_id, BinauralData data)
{
	if(toneLoadedMap.value(module_id))
	{
		if(toneMap.contains(module_id))
		{
			toneMap.value(module_id)->stop();
			toneMap.value(module_id)->release();
			delete toneMap[module_id];
			toneMap[module_id] = 0;
		}
	}
	else
	{
		if(soundMap.contains(module_id))
		{
			channelMap.value(module_id)->stop();
			soundMap.value(module_id)->release();
		}
	}

	BinauralTone * tone = new BinauralTone(system, data);
	if(tone->loaded())
	{
		toneLoadedMap.insert(module_id, true);
		toneMap.insert(module_id, tone);
		return true;
	}
	else
	{
		toneLoadedMap.insert(module_id, false);
		delete tone;
		tone = 0;
		return false;
	}
}

bool FMOD_Sound::play(int module_id, float volume)
{
	if(soundMap.contains(module_id))
	{
		system->playSound(FMOD_CHANNEL_FREE, soundMap[module_id], false, &channelMap[module_id]);
		setVolume(module_id, volume);

		return true;
	}
	else
	{
		return false;
	}
}

void FMOD_Sound::stop(int module_id)
{
	if(channelMap.contains(module_id))
	{
		channelMap[module_id]->stop();
	}
}

void FMOD_Sound::setPaused(int module_id, bool state)
{
	if(channelMap.contains(module_id))
	{
		channelMap[module_id]->setPaused(state);
	}
}

void FMOD_Sound::setVolume(int module_id, float volume)
{
	if(channelMap.contains(module_id))
	{
		float normalized_volume = volume / 100.0f;
		channelMap[module_id]->setVolume(normalized_volume);
	}
}

void FMOD_Sound::addEffect(int module_id, int type, float value)
{
	float normalized_value = value / 100.0f;

	switch(type)
	{
		case Reverb:
			FMOD::DSP * reverb;
			system->createDSPByType(FMOD_DSP_TYPE_REVERB, &reverb);
			channelMap[module_id]->addDSP(reverb, 0);
			reverb->setParameter(FMOD_DSP_REVERB_ROOMSIZE, normalized_value);
			reverbMap[module_id] = reverb;
			break;
		case Pitch:
			FMOD::DSP * pitch;
			system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pitch);
			channelMap[module_id]->addDSP(pitch, 0);
			pitch->setParameter(0, normalized_value);
			pitchMap[module_id] = pitch;
			break;
		case Distortion:
			FMOD::DSP * distortion;
			system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &distortion);
			channelMap[module_id]->addDSP(distortion, 0);
			distortion->setParameter(0, normalized_value);
			distortionMap[module_id] = distortion;
			break;
		case Echo:
			FMOD::DSP * echo;
			system->createDSPByType(FMOD_DSP_TYPE_ECHO, &echo);
			channelMap[module_id]->addDSP(echo, 0);
			echo->setParameter(0, normalized_value);
			echoMap[module_id] = echo;
			break;
		case Flange:
			FMOD::DSP * flange;
			system->createDSPByType(FMOD_DSP_TYPE_FLANGE, &flange);
			channelMap[module_id]->addDSP(flange, 0);
			flange->setParameter(0, normalized_value);
			flangeMap[module_id] = flange;
			break;
		default:
			break;
	}
}

void FMOD_Sound::removeEffect(int module_id, int type)
{
	switch(type)
	{
		case Reverb:
			if(reverbMap.contains(module_id))
			{
				reverbMap[module_id]->remove();
			}
			break;
		case Pitch:
			if(pitchMap.contains(module_id))
			{
				pitchMap[module_id]->remove();
			}
			break;
		case Distortion:
			if(distortionMap.contains(module_id))
			{
				distortionMap[module_id]->remove();
			}
			break;
		case Echo:
			if(echoMap.contains(module_id))
			{
				echoMap[module_id]->remove();
			}
			break;
		case Flange:
			if(flangeMap.contains(module_id))
			{
				flangeMap[module_id]->remove();
			}
			break;
		default:
			break;
	}
}

void FMOD_Sound::setEffectValue(int module_id, int type, float new_value)
{
	float normalized_value = new_value / 100.0f;

	switch(type)
	{
		case Reverb:
			if(reverbMap.contains(module_id))
			{
				reverbMap[module_id]->setParameter(FMOD_DSP_REVERB_ROOMSIZE, normalized_value);
			}
			break;
		case Pitch:
			if(pitchMap.contains(module_id))
			{
				pitchMap[module_id]->setParameter(0, normalized_value);
			}
			break;
		case Distortion:
			if(distortionMap.contains(module_id))
			{
				distortionMap[module_id]->setParameter(0, normalized_value);
			}
			break;
		case Echo:
			if(echoMap.contains(module_id))
			{
				echoMap[module_id]->setParameter(0, normalized_value);
			}
			break;
		case Flange:
			if(flangeMap.contains(module_id))
			{
				flangeMap[module_id]->setParameter(0, normalized_value);
			}
			break;
		default:
			break;
	}
}

bool FMOD_Sound::playTone(int module_id, float volume)
{
	if(toneMap.contains(module_id))
	{
		toneMap.value(module_id)->play(volume);
		return true;
	}
	else
	{
		return false;
	}
}

void FMOD_Sound::stopTone(int module_id)
{
	if(toneMap.contains(module_id))
		toneMap.value(module_id)->stop();
}

void FMOD_Sound::setTonePaused(int module_id, bool state)
{
	if(toneMap.contains(module_id))
		toneMap.value(module_id)->setPaused(state);
}

void FMOD_Sound::setToneVolume(int module_id, float volume)
{
	if(toneMap.contains(module_id))
	{
		toneMap.value(module_id)->setVolume(volume);
	}
}

void FMOD_Sound::changeOctave(int module_id, float octave)
{
	if(toneMap.contains(module_id))
		toneMap.value(module_id)->setFrequencey(octave);
}

void FMOD_Sound::changeBinauralGap(int module_id, int gap)
{
	if(toneMap.contains(module_id))
		toneMap.value(module_id)->setBinauralGap(gap);
}

void FMOD_Sound::addWhiteNoise(int module_id, float volume)
{
	if(toneMap.contains(module_id))
		toneMap.value(module_id)->createWhiteNoise(volume);
}

void FMOD_Sound::removeWhiteNoise(int module_id)
{
	if(toneMap.contains(module_id))
		toneMap.value(module_id)->destroyWhiteNoise();
}

void FMOD_Sound::setWhiteNoiseVolume(int module_id, float volume)
{
	if(toneMap.contains(module_id))
		toneMap.value(module_id)->setWhiteNoiseVolume(volume);
}
