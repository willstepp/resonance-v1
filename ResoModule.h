#ifndef RESOMODULE_H
#define RESOMODULE_H

#include <QtGui>
#include <QtCore>

#include "Resonance.h"
#include "BinauralTone.h"

class AeroButton;
class FMOD_Sound;
class ResoTree;

struct ModuleState
{
	QString sound_path;

	int id;

	int volume;
	bool enabled;
	int sound_class;

	bool reverb_enabled;
	bool pitch_enabled;
	bool distortion_enabled;
	bool echo_enabled;
	bool flange_enabled;

	int reverb_value;
	int pitch_value;
	int distortion_value;
	int echo_value;
	int flange_value;

	int octave_number;
	int binaural_gap;
	bool white_noise_enabled;
	int white_noise_volume;

	bool tone_loaded;
	bool expanded;
};

class ResoModule : public QWidget
{
	Q_OBJECT

public:
	ResoModule(QWidget * parent = 0);
	virtual ~ResoModule();

	Resonance::SoundClass soundClass()	{ return m_sound_class; }

	void setId(int module_id)			{ m_id = module_id; }

	AeroButton * minModule()			{ return m_min_module; }
	AeroButton * maxModule()			{ return m_max_module; }

	bool enabled()						{ return m_max_checkbox->isChecked(); }

	bool expanded()						{ return m_expanded; }
	void setExpanded(bool expanded)		{ m_expanded = expanded; }

	ModuleState currentState();
	void setModuleState(ModuleState state);

	void play();
	void stop();
	void pause();

signals:
	void clicked();
	void moduleStateUpdated();

	bool newSound(int module_id, QString sound_path);
	bool newTone(int module_id, BinauralData data);

	bool play(int module_id, float volume);
	void stop(int module_id);
	void setPaused(int module_id, bool state);

	void setVolume(int module_id, float volume);

	void addEffect(int module_id, int effect_type, float value);
	void removeEffect(int module_id, int effect_type);

	void setEffectValue(int module_id, int effect_type, float value);

	void changeOctave(int module_id, float octave);
	void changeBinauralGap(int module_id, int gap);

	void addWhiteNoise(int module_id, float volume);
	void removeWhiteNoise(int module_id);

	void setWhiteNoiseVolume(int module_id, float volume);

	bool playTone(int module_id, float volume);
	void stopTone(int module_id);
	void setTonePaused(int module_id, bool state);
	void setToneVolume(int module_id, float volume);

public slots:
	void updateMinCheckState(int state);
	void updateMaxCheckState(int state);

	void loadSoundList(int current_class);
	void loadChildSounds(QTreeWidgetItem * parent);

	void loadSound(QTreeWidgetItem * new_sound, int column);

	void resetModule();

	void updatePlaying(int state);
	void updateVolume(int value);

	void updateEffectsState(int effect_type);
	void updateEffectsValue(int effect_type);

	void updateForMasterVolume(int master_volume);

	void removeSoundFromList(QTreeWidgetItem * sound);
	void deleteSoundDir(const QString & dir);

	void updateToneOctave(int octave);

	void updatePanelDisplay();

	void updateBinauralGap(int gap);

	void toggleWhiteNoise(int state);
	void updateWhiteNoiseVolume(int volume);

private:
	void setupGUI();
	void setSoundClass(Resonance::SoundClass new_class);

	void loadTonesList();
	void loadBinauralData(const QString & data_path);

private:
	//minimized widgets
	AeroButton * m_min_module;

	QLabel * m_min_label;
	QSlider * m_min_slider;
	QCheckBox * m_min_checkbox;

	//maximized widgets
	AeroButton * m_max_module;

	QLabel * m_max_label;
	QSlider * m_max_slider;
	QCheckBox * m_max_checkbox;

	QSlider * m_reverb_slider;
	QSlider * m_pitch_slider;
	QSlider * m_distortion_slider;
	QSlider * m_echo_slider;
	QSlider * m_flange_slider;

	QCheckBox * m_reverb_checkbox;
	QCheckBox * m_pitch_checkbox;
	QCheckBox * m_distortion_checkbox;
	QCheckBox * m_echo_checkbox;
	QCheckBox * m_flange_checkbox;

	QTranscluentWidget * m_effects_panel;

	QButtonGroup * m_octave_group;

	QCheckBox * m_octave_one_cb;
	QCheckBox * m_octave_two_cb;
	QCheckBox * m_octave_three_cb;
	QCheckBox * m_octave_four_cb;
	QCheckBox * m_octave_five_cb;

	QLabel * m_oct_one_label;
	QLabel * m_oct_two_label;
	QLabel * m_oct_three_label;
	QLabel * m_oct_four_label;
	QLabel * m_oct_five_label;

	QLabel * m_binaural_label;
	QSlider * m_binaural_gap_slider;
	QLabel * m_binaural_gap_update_label;
	
	QSlider * m_white_noise_slider;
	QCheckBox * m_white_noise_cb;

	QTranscluentWidget * m_tone_panel;

	QVBoxLayout * m_max_layout_left;

	QSignalMapper * m_sound_buttons;

	ResoTree * m_sound_list;

	Resonance::SoundClass m_sound_class;

	QMap<int, QColor> m_sound_color_map;

	QButtonGroup * m_sound_button_group;

	bool m_expanded;

	QString m_current_sound_path;

	int m_id;

	bool m_paused;

	QButtonGroup * m_effect_boxes;
	QList<QCheckBox*> m_effects_list;
	QList<QSlider*> m_effect_sliders;

	int m_master_volume;

	BinauralData m_binaural_data;

	bool m_tone_loaded;
};

#endif // RESOMODULE_H
