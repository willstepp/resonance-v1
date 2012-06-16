#include "BinauralTone.h"

BinauralTone::BinauralTone(FMOD::System * system, BinauralData data, QObject * parent) : QObject(parent)
{
	m_system = system;
	m_data = data;

	m_system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &m_primary_tone);
	m_system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &m_secondary_tone);
	m_system->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &m_white_noise);
	
	m_frequency = m_data.default_frequency;
	m_binaural_gap = m_data.default_gap;

	m_loaded = true;
}

BinauralTone::~BinauralTone()
{}

void BinauralTone::play(float volume)
{
	m_primary_tone->setParameter(FMOD_DSP_OSCILLATOR_RATE, m_frequency);
	m_system->playDSP(FMOD_CHANNEL_FREE, m_primary_tone, true, &m_primary_channel);
	m_primary_tone->setParameter(FMOD_DSP_OSCILLATOR_TYPE, 0);
	m_primary_channel->setPan(-1.0);

	int frequency = 0;
	m_system->getSoftwareFormat(&frequency, NULL, NULL, NULL, NULL, NULL);
	m_primary_channel->setFrequency(frequency); 

	m_secondary_tone->setParameter(FMOD_DSP_OSCILLATOR_RATE, m_frequency + m_binaural_gap);
	m_system->playDSP(FMOD_CHANNEL_FREE, m_secondary_tone, true, &m_secondary_channel);
	m_secondary_tone->setParameter(FMOD_DSP_OSCILLATOR_TYPE, 0);
	m_secondary_channel->setPan(1.0);
	m_secondary_channel->setFrequency(frequency);

	setVolume(volume);

	m_primary_channel->setPaused(false);
	m_secondary_channel->setPaused(false);
}

void BinauralTone::stop()
{
	m_primary_channel->stop();
	m_secondary_channel->stop();
}

void BinauralTone::setPaused(bool state)
{
	m_primary_channel->setPaused(state);
	m_secondary_channel->setPaused(state);
	m_white_noise_channel->setPaused(state);
}

void BinauralTone::setVolume(float volume)
{
	float normalized_volume = volume / 100.0f;
	float quartered = normalized_volume / 8.0f;

	m_primary_channel->setVolume(quartered);
	m_secondary_channel->setVolume(quartered);
}

void BinauralTone::setFrequencey(float frequency)
{
	m_frequency = frequency;

	m_primary_tone->setParameter(FMOD_DSP_OSCILLATOR_RATE, m_frequency);
	m_secondary_tone->setParameter(FMOD_DSP_OSCILLATOR_RATE, m_frequency + m_binaural_gap);
}

void BinauralTone::setBinauralGap(int gap)
{
	m_binaural_gap = gap;
	m_secondary_tone->setParameter(FMOD_DSP_OSCILLATOR_RATE, m_frequency + m_binaural_gap);
}

void BinauralTone::release()
{
	m_primary_tone->release();
	m_secondary_tone->release();
	m_white_noise->release();
}

void BinauralTone::createWhiteNoise(float volume)
{
	m_white_noise->setParameter(FMOD_DSP_OSCILLATOR_RATE, 100.0);
	m_system->playDSP(FMOD_CHANNEL_FREE, m_white_noise, true, &m_white_noise_channel);
	m_white_noise->setParameter(FMOD_DSP_OSCILLATOR_TYPE, 5);
	m_white_noise_channel->setPan(0.0);
	int frequency = 0;
	m_system->getSoftwareFormat(&frequency, NULL, NULL, NULL, NULL, NULL);
	m_white_noise_channel->setFrequency(frequency);

	setWhiteNoiseVolume(volume);

	m_white_noise_channel->setPaused(false);
}

void BinauralTone::destroyWhiteNoise()
{
	m_white_noise_channel->stop();
}

void BinauralTone::setWhiteNoiseVolume(float volume)
{
	float normalized_volume = volume / 100.0f;
	float quartered = normalized_volume / 20.0f;

	m_white_noise_channel->setVolume(quartered);
}