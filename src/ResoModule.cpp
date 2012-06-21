#include "ResoModule.h"

#include "AeroButton.h"
#include "QTranscluentWidget.h"
#include "FMOD_Sound.h"
#include "ResoTree.h"

ResoModule::ResoModule(QWidget * parent) : QWidget(parent), m_expanded(false), 
m_id(-1),
m_paused(false),
m_master_volume(50),
m_tone_loaded(false),
m_sound_class((Resonance::SoundClass)0)
{
	m_sound_color_map.insert(Resonance::Nature, QColor(Qt::green));
	m_sound_color_map.insert(Resonance::Civilization, QColor(Qt::yellow));
	m_sound_color_map.insert(Resonance::Tone, QColor(2, 135, 237));
	m_sound_color_map.insert(Resonance::Music, QColor(175, 0, 175));
	m_sound_color_map.insert(Resonance::Personal, QColor(Qt::red));

	setupGUI();
}

ResoModule::~ResoModule()
{
}

ModuleState ResoModule::currentState()
{
	ModuleState current_state;

	current_state.id = m_id;
	current_state.sound_class = (int)m_sound_class;
	current_state.sound_path = m_current_sound_path;
	current_state.volume = m_max_slider->value();

	current_state.expanded = m_expanded;

	bool module_enabled;
	m_max_checkbox->checkState() == Qt::Checked ? module_enabled = true : module_enabled = false;
	current_state.enabled = module_enabled;

	if(m_tone_loaded)
	{
		current_state.tone_loaded = true;

		current_state.octave_number = m_octave_group->checkedId();

		current_state.binaural_gap = m_binaural_gap_slider->value();

		current_state.white_noise_enabled = m_white_noise_cb->isChecked();
		current_state.white_noise_volume = m_white_noise_slider->value();
	}
	else
	{
		current_state.tone_loaded = false;

		current_state.reverb_value = m_reverb_slider->value();
		current_state.pitch_value = m_pitch_slider->value();
		current_state.distortion_value = m_distortion_slider->value();
		current_state.echo_value = m_echo_slider->value();
		current_state.flange_value = m_flange_slider->value();

		bool effect_enabled;

		m_reverb_checkbox->checkState() == Qt::Checked ? effect_enabled = true : effect_enabled = false;
		current_state.reverb_enabled = effect_enabled;

		m_pitch_checkbox->checkState() == Qt::Checked ? effect_enabled = true : effect_enabled = false;
		current_state.pitch_enabled = effect_enabled;

		m_distortion_checkbox->checkState() == Qt::Checked ? effect_enabled = true : effect_enabled = false;
		current_state.distortion_enabled = effect_enabled;

		m_echo_checkbox->checkState() == Qt::Checked ? effect_enabled = true : effect_enabled = false;
		current_state.echo_enabled = effect_enabled;

		m_flange_checkbox->checkState() == Qt::Checked ? effect_enabled = true : effect_enabled = false;
		current_state.flange_enabled = effect_enabled;
	}

	return current_state;
}

void ResoModule::setModuleState(ModuleState state)
{
	//first disable the current sound and effects
	ModuleState new_state = state;
	m_max_checkbox->setChecked(false);

	m_current_sound_path = state.sound_path;

	m_expanded = state.expanded;
	m_tone_loaded = state.tone_loaded;

	//update the current sound class
	m_sound_button_group->button(new_state.sound_class)->click();
	setSoundClass((Resonance::SoundClass)m_sound_button_group->checkedId());

	if(state.tone_loaded)
	{
		QFile file(new_state.sound_path);
		if(!file.open(QIODevice::ReadOnly))
			return;

		QString octave_info;

		m_binaural_data.name = QString(file.readLine()).split(":").at(1);
		m_binaural_data.default_frequency = QString(file.readLine()).split(":").at(1).toFloat();

		m_binaural_data.octave_one = QString(file.readLine()).split(":").at(1).toFloat();
		octave_info = "octave one: " + QString::number((int)m_binaural_data.octave_one);
		m_oct_one_label->setText(octave_info);

		m_binaural_data.octave_two = QString(file.readLine()).split(":").at(1).toFloat();
		octave_info = "octave two: " + QString::number((int)m_binaural_data.octave_two);
		m_oct_two_label->setText(octave_info);

		m_binaural_data.octave_three = QString(file.readLine()).split(":").at(1).toFloat();
		octave_info = "octave three: " + QString::number((int)m_binaural_data.octave_three);
		m_oct_three_label->setText(octave_info);

		m_binaural_data.octave_four = QString(file.readLine()).split(":").at(1).toFloat();
		octave_info = "octave four: " + QString::number((int)m_binaural_data.octave_four);
		m_oct_four_label->setText(octave_info);

		m_binaural_data.octave_five = QString(file.readLine()).split(":").at(1).toFloat();
		octave_info = "octave five: " + QString::number((int)m_binaural_data.octave_five);
		m_oct_five_label->setText(octave_info);

		m_binaural_data.octave_six = QString(file.readLine()).split(":").at(1).toFloat();

		file.close();

		m_octave_group->button(new_state.octave_number)->blockSignals(true);
		m_octave_group->button(new_state.octave_number)->setChecked(true);
		m_octave_group->button(new_state.octave_number)->blockSignals(false);

		m_binaural_gap_slider->blockSignals(true);
		m_binaural_gap_slider->setValue(new_state.binaural_gap);
		m_binaural_gap_slider->blockSignals(false);

		QString binaural_label = QString::number(new_state.binaural_gap) + "hz";
		m_binaural_gap_update_label->setText(binaural_label);

		m_white_noise_slider->blockSignals(true);
		m_white_noise_slider->setValue(new_state.white_noise_volume);
		m_white_noise_slider->blockSignals(false);

		m_white_noise_cb->blockSignals(true);
		m_white_noise_cb->setChecked(new_state.white_noise_enabled);
		m_white_noise_cb->blockSignals(false);

		//update the current sound label
		QFileInfo info(QFile(new_state.sound_path));
		m_max_label->clear();

		QString tone_label = info.baseName() + " - ";
		if(new_state.binaural_gap < 4)
		{
			tone_label += "delta (unconscious)";
		}
		else if(new_state.binaural_gap < 8 && new_state.binaural_gap > 3)
		{
			tone_label += "theta (meditation)";
		}
		else if(new_state.binaural_gap < 13 && new_state.binaural_gap > 7)
		{
			tone_label += "alpha (relaxation)";
		}
		else if(new_state.binaural_gap < 41 && new_state.binaural_gap > 12)
		{
			tone_label += "beta (concentration)";
		}

		m_max_label->setText(tone_label);
		m_max_label->adjustSize();

		m_min_label->clear();
		m_min_label->setText(tone_label);
		m_min_label->adjustSize();

		//update the module volume
		m_min_slider->blockSignals(true);
		m_min_slider->setValue(new_state.volume);
		m_min_slider->blockSignals(false);

		m_max_slider->blockSignals(true);
		m_max_slider->setValue(new_state.volume);
		m_max_slider->blockSignals(false);

		m_max_layout_left->removeWidget(m_effects_panel);
		m_effects_panel->hide();
		m_max_layout_left->insertWidget(2, m_tone_panel);
		m_tone_panel->show();

		bool loaded = emit newTone(m_id, m_binaural_data);
		updateToneOctave(new_state.octave_number);
		updateBinauralGap(new_state.binaural_gap);
		if(loaded && new_state.enabled)
		{
			m_max_checkbox->setChecked(true);
		}
	}
	else
	{
		//now add the new effects and values
		m_reverb_checkbox->blockSignals(true);
		m_reverb_slider->blockSignals(true);
		m_reverb_checkbox->setChecked(new_state.reverb_enabled);
		m_reverb_slider->setValue(new_state.reverb_value);
		m_reverb_checkbox->blockSignals(false);
		m_reverb_slider->blockSignals(false);

		m_pitch_checkbox->blockSignals(true);
		m_pitch_slider->blockSignals(true);
		m_pitch_checkbox->setChecked(new_state.pitch_enabled);
		m_pitch_slider->setValue(new_state.pitch_value);
		m_pitch_checkbox->blockSignals(false);
		m_pitch_slider->blockSignals(false);

		m_distortion_checkbox->blockSignals(true);
		m_distortion_slider->blockSignals(true);
		m_distortion_checkbox->setChecked(new_state.distortion_enabled);
		m_distortion_slider->setValue(new_state.distortion_value);
		m_distortion_checkbox->blockSignals(false);
		m_distortion_slider->blockSignals(false);

		m_echo_checkbox->blockSignals(true);
		m_echo_slider->blockSignals(true);
		m_echo_checkbox->setChecked(new_state.echo_enabled);
		m_echo_slider->setValue(new_state.echo_value);
		m_echo_checkbox->blockSignals(false);
		m_echo_slider->blockSignals(false);

		m_flange_checkbox->blockSignals(true);
		m_flange_slider->blockSignals(true);
		m_flange_checkbox->setChecked(new_state.flange_enabled);
		m_flange_slider->setValue(new_state.flange_value);
		m_flange_checkbox->blockSignals(false);
		m_flange_slider->blockSignals(false);
	}

	if(!m_tone_loaded)
	{
		//update the current sound label
		QFileInfo info(QFile(new_state.sound_path));
		m_max_label->clear();
		m_max_label->setText(info.baseName());
		m_max_label->adjustSize();

		m_min_label->clear();
		m_min_label->setText(info.baseName());
		m_min_label->adjustSize();

		//update the module volume
		m_min_slider->blockSignals(true);
		m_min_slider->setValue(new_state.volume);
		m_min_slider->blockSignals(false);

		m_max_slider->blockSignals(true);
		m_max_slider->setValue(new_state.volume);
		m_max_slider->blockSignals(false);

		m_max_layout_left->removeWidget(m_tone_panel);
		m_tone_panel->hide();
		m_max_layout_left->insertWidget(2, m_effects_panel);
		m_effects_panel->show();

		//finally set in motion to play the new sound
		if(new_state.sound_path.endsWith(".rs"))
		{
			QFile file(new_state.sound_path);
			if(!file.open(QIODevice::ReadOnly))
				return;
			new_state.sound_path = file.readLine();
			file.close();
		}

		bool loaded = emit newSound(m_id, new_state.sound_path);
		if(loaded && new_state.enabled)
		{
			m_max_checkbox->setChecked(true);
			//for(int e = 0; e < 5; e++)
			//{
			//	updateEffectsState(e);
			//}
		}
	}
}

void ResoModule::play()
{
	if(!m_tone_loaded)
	{
		emit setPaused(m_id, false);
	}
	else
	{
		emit setTonePaused(m_id, false);
	}
	m_paused = false;
}

void ResoModule::stop()
{

}

void ResoModule::pause()
{
	if(!m_tone_loaded)
	{
		emit setPaused(m_id, true);
	}
	else
	{
		emit setTonePaused(m_id, true);
	}
	m_paused = true;
}

void ResoModule::updateMinCheckState(int state)
{
	m_min_checkbox->blockSignals(true);
	if(Qt::Checked == state)
	{
		m_min_checkbox->setChecked(true);
	}
	if(Qt::Unchecked == state)
	{
		m_min_checkbox->setChecked(false);
	}
	m_min_checkbox->blockSignals(false);
}

void ResoModule::updateMaxCheckState(int state)
{
	if(Qt::Checked == state)
	{
		m_max_checkbox->setChecked(true);
	}
	if(Qt::Unchecked == state)
	{
		m_max_checkbox->setChecked(false);
	}
}

void ResoModule::loadSoundList(int current_class)
{
	QString path;
	switch(current_class)
	{
	case Resonance::Nature:
		path = "/sounds/nature";
		break;
	case Resonance::Civilization:
		path = "/sounds/civilization";
		break;
	case Resonance::Tone:
		loadTonesList();
		return;
		break;
	case Resonance::Music:
		path = "/sounds/music";
		break;
	case Resonance::Personal:
		path = "/sounds/personal";
		break;
	default:
		break;
	}

	QString dir = QDir::currentPath() + path;
	QDir e_dir;
	e_dir.setPath(dir);

	QFileInfoList files = e_dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	m_sound_list->clear();
	for(int i = 0; i < files.count(); i++)
	{
		QTreeWidgetItem * item = new QTreeWidgetItem(m_sound_list);
		QString sound_name = files.at(i).baseName();
		if(files.at(i).isFile())
			sound_name.prepend(":: ");
		item->setText(0, sound_name);
		item->setData(0, Qt::UserRole, files.at(i).absoluteFilePath());
		m_sound_list->addTopLevelItem(item);

		if(files.at(i).isDir())
		{
			loadChildSounds(item);
		}
	}

	m_sound_list->setSoundClass(current_class);
}

void ResoModule::loadChildSounds(QTreeWidgetItem * parent)
{
	QDir parent_dir(parent->data(0, Qt::UserRole).toString());
	QFileInfoList files = parent_dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	for(int i = 0; i < files.count(); i++)
	{
		QTreeWidgetItem * item = new QTreeWidgetItem(parent);
		QString sound_name = files.at(i).baseName();
		if(files.at(i).isFile())
			sound_name.prepend(":: ");
		item->setText(0, sound_name);
		item->setData(0, Qt::UserRole, files.at(i).absoluteFilePath());
		parent->addChild(item);

		if(files.at(i).isDir())
		{
			loadChildSounds(item);
		}
	}
}

void ResoModule::loadSound(QTreeWidgetItem * new_sound, int column)
{
	QString sound_path = new_sound->data(0, Qt::UserRole).toString();
	m_current_sound_path = sound_path;

	if(m_sound_button_group->checkedId() == Resonance::Personal)
	{
		QFile file(sound_path);
		if(!file.open(QIODevice::ReadOnly))
			return;
		sound_path = file.readLine();
	}
	else if(m_sound_button_group->checkedId() == Resonance::Tone)
	{
		loadBinauralData(sound_path);
		return;
	}

	bool loaded = emit newSound(m_id, sound_path);
	if(loaded)
	{
		m_tone_loaded = false;
		updatePanelDisplay();

		int delay;
		(Resonance::SoundClass)m_sound_button_group->checkedId() == m_sound_class ? delay = 0 : delay = 400;

		setSoundClass((Resonance::SoundClass)m_sound_button_group->checkedId());
		
		//delay to let the module button update its color first, 
		//otherwise the label update shows the old color, sucks, maybe a way around it?
		QTimer::singleShot(delay, this, SLOT(resetModule()));
	}
}

void ResoModule::resetModule()
{
	QFileInfo file_info(m_current_sound_path);
	QString sound_name = file_info.baseName();

	if(!m_tone_loaded)
	{
		m_min_label->clear();
		m_min_label->setText(sound_name);
		m_min_label->adjustSize();

		m_max_label->clear();
		m_max_label->setText(sound_name);
		m_max_label->adjustSize();

		m_reverb_checkbox->setCheckState(Qt::Unchecked);
		m_pitch_checkbox->setCheckState(Qt::Unchecked);
		m_distortion_checkbox->setCheckState(Qt::Unchecked);
		m_echo_checkbox->setCheckState(Qt::Unchecked);
		m_flange_checkbox->setCheckState(Qt::Unchecked);

		m_reverb_slider->setValue(50);
		m_pitch_slider->setValue(100);
		m_distortion_slider->setValue(50);
		m_echo_slider->setValue(50000);
		m_flange_slider->setValue(50);
	}
	else
	{
		sound_name += " - delta (unconscious)";

		m_min_label->clear();
		m_min_label->setText(sound_name);
		m_min_label->adjustSize();

		m_max_label->clear();
		m_max_label->setText(sound_name);
		m_max_label->adjustSize();

		//set octaves
		QString binaural_labels = "octave one: " + QString::number((int)m_binaural_data.octave_one) + "hz";
		m_oct_one_label->setText(binaural_labels);

		binaural_labels = "octave two: " + QString::number((int)m_binaural_data.octave_two) + "hz";
		m_oct_two_label->setText(binaural_labels);

		binaural_labels = "octave three: " + QString::number((int)m_binaural_data.octave_three) + "hz";
		m_oct_three_label->setText(binaural_labels);

		binaural_labels = "octave four: " + QString::number((int)m_binaural_data.octave_four) + "hz";
		m_oct_four_label->setText(binaural_labels);

		binaural_labels = "octave five: " + QString::number((int)m_binaural_data.octave_five) + "hz";
		m_oct_five_label->setText(binaural_labels);


		//check default octave, always 3
		m_octave_three_cb->blockSignals(true);
		m_octave_three_cb->setChecked(true);
		m_octave_three_cb->blockSignals(false);

		//set default binaural gap, check box, slider
		m_binaural_gap_slider->blockSignals(true);
		m_binaural_gap_slider->setValue(1);
		m_binaural_gap_slider->blockSignals(false);

		m_binaural_gap_update_label->setText("1hz");

		//uncheck white noise, move to 50
		m_white_noise_cb->blockSignals(true);
		m_white_noise_cb->setChecked(false);
		m_white_noise_cb->blockSignals(false);

		m_white_noise_slider->blockSignals(true);
		m_white_noise_slider->setValue(50);
		m_white_noise_slider->blockSignals(false);
	}

	if(m_max_checkbox->checkState() == Qt::Checked)
	{
		float percentage = (float)m_master_volume / 100.0f;
		float volume = m_max_slider->value() * percentage;

		if(!m_tone_loaded)
		{
			emit play(m_id, volume);
			if(m_paused)
				emit setPaused(m_id, true);
		}
		else
		{
			emit playTone(m_id, volume);
			if(m_paused)
				emit setTonePaused(m_id, true);
		}
	}
}

void ResoModule::updatePlaying(int state)
{
	if(Qt::Checked == state)
	{
		float percentage = (float)m_master_volume / 100.0f;
		float volume = m_max_slider->value() * percentage;

		if(!m_tone_loaded)
		{
			bool playing = emit play(m_id, volume);
			if(m_paused)
			{
				emit setPaused(m_id, true);
			}
			if(playing)
			{
				for(int effect = 0; effect < 5; effect++)
				{
					updateEffectsState(effect);
				}
			}
		}
		else
		{
			bool playing = emit playTone(m_id, volume);
			toggleWhiteNoise(m_white_noise_cb->checkState());
			if(m_paused)
			{
				emit setTonePaused(m_id, true);
			}
		}
	}
	if(Qt::Unchecked == state)
	{
		if(!m_tone_loaded)
		{
			for(int effect = 0; effect < 5; effect++)
			{
				emit removeEffect(m_id, effect);
			}

			emit stop(m_id);
		}
		else
		{
			emit stopTone(m_id);
			if(m_white_noise_cb->isChecked())
				emit removeWhiteNoise(m_id);
		}
	}
}

void ResoModule::updateVolume(int value)
{
	float percentage = (float)m_master_volume / 100.0f;
	float volume = value * percentage;

	if(!m_tone_loaded)
	{
		emit setVolume(m_id, volume);
	}
	else
	{
		emit setToneVolume(m_id, volume);
	}
}

void ResoModule::updateEffectsState(int effect_type)
{
	if(m_effects_list.at(effect_type)->isChecked())
		emit addEffect(m_id, effect_type, (float)m_effect_sliders.at(effect_type)->value());
	else
		emit removeEffect(m_id, effect_type);
}

void ResoModule::updateEffectsValue(int effect_type)
{
	emit setEffectValue(m_id, effect_type, (float)m_effect_sliders.at(effect_type)->value());
}

void ResoModule::updateForMasterVolume(int master_volume)
{
	m_master_volume = master_volume;

	float percentage = (float)master_volume / 100.0f;
	float new_volume = m_max_slider->value() * percentage;

	if(!m_tone_loaded)
	{
		emit setVolume(m_id, new_volume);
	}
	else
	{
		emit setToneVolume(m_id, new_volume);

		float white_noise_volume = m_white_noise_slider->value() * percentage;
		emit setWhiteNoiseVolume(m_id, white_noise_volume);
	}
}

void ResoModule::removeSoundFromList(QTreeWidgetItem * sound)
{
	QString path = sound->data(0, Qt::UserRole).toString();
	if(m_sound_button_group->checkedId() == Resonance::Personal)
	{
		//delete file or dir
		QFileInfo file_info(path);
		if(file_info.isFile())
		{
			QDir file_dir = file_info.dir();
			file_dir.remove(file_info.fileName());
		}
		else if(file_info.isDir())
		{
			deleteSoundDir(file_info.absoluteFilePath());
		}
		loadSoundList(Resonance::Personal);
	}
}

void ResoModule::deleteSoundDir(const QString & dir)
{
	QDir sound_dir(dir);

	//First delete any files in the current directory
	QFileInfoList files = sound_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
	for(int file = 0; file < files.count(); file++)
	{
		sound_dir.remove(files.at(file).fileName());
	}

	//Now recursively delete any child directories
	QFileInfoList dirs = sound_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
	for(int dir = 0; dir < dirs.count(); dir++)
	{
		deleteSoundDir(dirs.at(dir).absoluteFilePath());
	}

	//Finally, remove empty parent directory
	sound_dir.rmdir(sound_dir.path());
}

void ResoModule::updateToneOctave(int octave)
{
	float new_octave;
	switch(octave)
	{
	case 0:
		new_octave = m_binaural_data.octave_one;
		break;
	case 1:
		new_octave = m_binaural_data.octave_two;
		break;
	case 2:
		new_octave = m_binaural_data.octave_three;
		break;
	case 3:
		new_octave = m_binaural_data.octave_four;
		break;
	case 4:
		new_octave = m_binaural_data.octave_five;
		break;
	case 5:
		new_octave = m_binaural_data.octave_six;
		break;
	default:
		break;
	}

	emit changeOctave(m_id, new_octave);
}

void ResoModule::updatePanelDisplay()
{
	if(m_tone_loaded)
	{
		if(m_effects_panel->isVisible())
		{
			m_max_layout_left->removeWidget(m_effects_panel);
			m_effects_panel->hide();
			m_max_layout_left->insertWidget(2, m_tone_panel);
			m_tone_panel->show();
		}
	}
	else
	{
		if(m_tone_panel->isVisible())
		{
			m_max_layout_left->removeWidget(m_tone_panel);
			m_tone_panel->hide();
			m_max_layout_left->insertWidget(2, m_effects_panel);
			m_effects_panel->show();
		}
	}
}

void ResoModule::updateBinauralGap(int gap)
{
	emit changeBinauralGap(m_id, gap);

	m_binaural_gap_update_label->clear();
	QString text = QString::number(gap) + "hz";
	m_binaural_gap_update_label->setText(text);
	m_binaural_gap_update_label->adjustSize();

	QString tone_label = m_binaural_data.name.trimmed() + " - ";
	if(gap < 4)
	{
		tone_label += "delta (unconscious)";
	}
	else if(gap < 8 && gap > 3)
	{
		tone_label += "theta (meditation)";
	}
	else if(gap < 13 && gap > 7)
	{
		tone_label += "alpha (relaxation)";
	}
	else if(gap < 41 && gap > 12)
	{
		tone_label += "beta (concentration)";
	}

	m_max_label->clear();
	m_max_label->setText(tone_label);
	m_max_label->adjustSize();

	m_min_label->clear();
	m_min_label->setText(tone_label);
	m_min_label->adjustSize();
}

void ResoModule::toggleWhiteNoise(int state)
{
	if(state == Qt::Checked)
	{
		float percentage = (float)m_master_volume / 100.0f;
		float volume = m_white_noise_slider->value() * percentage;

		if(m_max_checkbox->isChecked())
			emit addWhiteNoise(m_id, volume);
	}
	if(state == Qt::Unchecked)
	{
		emit removeWhiteNoise(m_id);
	}
}

void ResoModule::updateWhiteNoiseVolume(int volume)
{
	float percentage = (float)m_master_volume / 100.0f;
	float new_volume = volume * percentage;

	emit setWhiteNoiseVolume(m_id, new_volume);
}

void ResoModule::setupGUI()
{
	//MINIMIZED SETUP
	m_min_module = new AeroButton();
	m_min_module->setHighlight(QColor(Qt::red));
	m_min_module->setGlassOpacity(0.00);
	m_min_module->setRoundness(2);
	m_min_module->setMaximumHeight(50);
	m_min_module->setOpacity(0.15);
	m_min_module->setCheckable(true);

	m_min_slider = new QSlider();
	m_min_slider->setOrientation(Qt::Horizontal);

	m_min_label = new QLabel();

	m_min_checkbox = new QCheckBox();

	QHBoxLayout * min_layout = new QHBoxLayout();
	
	min_layout->addWidget(m_min_label);
	min_layout ->addWidget(m_min_slider);
	min_layout->addWidget(m_min_checkbox);

	m_min_module->setLayout(min_layout);

	//MAXIMIZED SETUP
	m_max_module = new AeroButton();
	m_max_module->setHighlight(QColor(Qt::red));
	m_max_module->setGlassOpacity(0.00);
	m_max_module->setRoundness(2);
	m_max_module->setMaximumHeight(300);
	m_max_module->setOpacity(0.15);
	m_max_module->setCheckable(true);

	m_max_label = new QLabel();

	m_max_slider = new QSlider();
	m_max_slider->setOrientation(Qt::Horizontal);

	m_max_checkbox = new QCheckBox();

	QHBoxLayout * sound_slider_checkbox_layout = new QHBoxLayout();
	sound_slider_checkbox_layout->addWidget(m_max_slider);
	sound_slider_checkbox_layout->addWidget(m_max_checkbox);

	//effects
	QFont effect_font("Arial", 7);

	//effects panel
	//reverb
	QLabel * reverb_label = new QLabel();
	reverb_label->setFont(effect_font);
	reverb_label->setText(tr("reverb"));

	m_reverb_slider = new QSlider();
	m_reverb_slider->setOrientation(Qt::Vertical);
	m_reverb_slider->setValue(50);

	m_reverb_checkbox = new QCheckBox();
	m_reverb_checkbox->setTristate(false);

	QVBoxLayout * reverb_layout = new QVBoxLayout();
	reverb_layout->addWidget(reverb_label);
	reverb_layout->addWidget(m_reverb_slider);
	reverb_layout->addWidget(m_reverb_checkbox);

	QHBoxLayout * reverb_h_layout = new QHBoxLayout();
	reverb_h_layout->addStretch(1);
	reverb_h_layout->addLayout(reverb_layout);
	reverb_h_layout->addStretch(1);

	//pitch
	QLabel * pitch_label = new QLabel();
	pitch_label->setFont(effect_font);
	pitch_label->setText(tr("pitch"));

	m_pitch_slider = new QSlider();
	m_pitch_slider->setOrientation(Qt::Vertical);
	m_pitch_slider->setRange(50, 200);
	m_pitch_slider->setValue(100);
	
	m_pitch_checkbox = new QCheckBox();
	m_pitch_checkbox->setTristate(false);

	QVBoxLayout * pitch_layout = new QVBoxLayout();
	pitch_layout->addWidget(pitch_label);
	pitch_layout->addWidget(m_pitch_slider);
	pitch_layout->addWidget(m_pitch_checkbox);

	QHBoxLayout * pitch_h_layout = new QHBoxLayout();
	pitch_h_layout->addStretch(1);
	pitch_h_layout->addLayout(pitch_layout);
	pitch_h_layout->addStretch(1);

	//distortion
	QLabel * distortion_label = new QLabel();
	distortion_label->setFont(effect_font);
	distortion_label->setText(tr("distortion"));

	m_distortion_slider = new QSlider();
	m_distortion_slider->setOrientation(Qt::Vertical);
	m_distortion_slider->setValue(50);

	m_distortion_checkbox = new QCheckBox();
	m_distortion_checkbox->setTristate(false);

	QHBoxLayout * distortion_label_layout = new QHBoxLayout();
	distortion_label_layout->addStretch(1);
	distortion_label_layout->addWidget(distortion_label);
	distortion_label_layout->addStretch(2);

	QVBoxLayout * distortion_layout = new QVBoxLayout();
	distortion_layout->addLayout(distortion_label_layout);
	distortion_layout->addWidget(m_distortion_slider);
	distortion_layout->addWidget(m_distortion_checkbox);

	QHBoxLayout * distortion_h_layout = new QHBoxLayout();
	distortion_h_layout->addStretch(1);
	distortion_h_layout->addLayout(distortion_layout);
	distortion_h_layout->addStretch(1);

	//echo
	QLabel * echo_label = new QLabel();
	echo_label->setFont(effect_font);
	echo_label->setText(tr("echo"));

	m_echo_slider = new QSlider();
	m_echo_slider->setOrientation(Qt::Vertical);
	m_echo_slider->setRange(10000, 300000);
	m_echo_slider->setValue(50000);

	m_echo_checkbox = new QCheckBox();
	m_echo_checkbox->setTristate(false);

	QVBoxLayout * echo_layout = new QVBoxLayout();
	echo_layout->addWidget(echo_label);
	echo_layout->addWidget(m_echo_slider);
	echo_layout->addWidget(m_echo_checkbox);

	QHBoxLayout * echo_h_layout = new QHBoxLayout();
	echo_h_layout->addStretch(1);
	echo_h_layout->addLayout(echo_layout);
	echo_h_layout->addStretch(1);

	//flange
	QLabel * flange_label = new QLabel();
	flange_label->setFont(effect_font);
	flange_label->setText(tr("flange"));

	m_flange_slider = new QSlider();
	m_flange_slider->setOrientation(Qt::Vertical);
	m_flange_slider->setValue(50);

	m_flange_checkbox = new QCheckBox();
	m_flange_checkbox->setTristate(false);

	QVBoxLayout * flange_layout = new QVBoxLayout();
	flange_layout->addWidget(flange_label);
	flange_layout->addWidget(m_flange_slider);
	flange_layout->addWidget(m_flange_checkbox);

	QHBoxLayout * flange_h_layout = new QHBoxLayout();
	flange_h_layout->addStretch(1);
	flange_h_layout->addLayout(flange_layout);
	flange_h_layout->addStretch(1);

	m_effect_boxes = new QButtonGroup(this);
	m_effect_boxes->setExclusive(false);
	m_effect_boxes->addButton(m_reverb_checkbox, 0);
	m_effect_boxes->addButton(m_pitch_checkbox, 1);
	m_effect_boxes->addButton(m_distortion_checkbox, 2);
	m_effect_boxes->addButton(m_echo_checkbox, 3);
	m_effect_boxes->addButton(m_flange_checkbox, 4);
	connect(m_effect_boxes, SIGNAL(buttonClicked(int)), this, SLOT(updateEffectsState(int)));

	m_effects_list.push_back(m_reverb_checkbox);
	m_effects_list.push_back(m_pitch_checkbox);
	m_effects_list.push_back(m_distortion_checkbox);
	m_effects_list.push_back(m_echo_checkbox);
	m_effects_list.push_back(m_flange_checkbox);

	m_effect_sliders.push_back(m_reverb_slider);
	m_effect_sliders.push_back(m_pitch_slider);
	m_effect_sliders.push_back(m_distortion_slider);
	m_effect_sliders.push_back(m_echo_slider);
	m_effect_sliders.push_back(m_flange_slider);

	QSignalMapper * effect_slider_mapper = new QSignalMapper(this);
	for(int i = 0; i < 5; i++)
	{
		connect(m_effect_sliders.at(i), SIGNAL(valueChanged(int)), effect_slider_mapper, SLOT(map()));
		effect_slider_mapper->setMapping(m_effect_sliders.at(i), i);
	}
	connect(effect_slider_mapper, SIGNAL(mapped(int)), this, SLOT(updateEffectsValue(int)));
	
	m_effects_panel = new QTranscluentWidget();
	m_effects_panel->setOpacity(0.5);

	QHBoxLayout * effects_layout = new QHBoxLayout();
	effects_layout->addLayout(reverb_h_layout);
	effects_layout->addStretch(1);
	effects_layout->addLayout(pitch_h_layout);
	effects_layout->addStretch(1);
	effects_layout->addLayout(distortion_h_layout);
	effects_layout->addStretch(1);
	effects_layout->addLayout(echo_h_layout);
	effects_layout->addStretch(1);
	effects_layout->addLayout(flange_h_layout);

	m_effects_panel->setLayout(effects_layout);

	//tone panel
	QFont tone_font("Arial", 7);

	m_octave_one_cb = new QCheckBox();
	m_octave_two_cb = new QCheckBox();
	m_octave_three_cb = new QCheckBox();
	m_octave_four_cb = new QCheckBox();
	m_octave_five_cb = new QCheckBox();

	m_oct_one_label = new QLabel();
	m_oct_one_label->setFont(tone_font);
	m_oct_two_label = new QLabel();
	m_oct_two_label->setFont(tone_font);
	m_oct_three_label = new QLabel();
	m_oct_three_label->setFont(tone_font);
	m_oct_four_label = new QLabel();
	m_oct_four_label->setFont(tone_font);
	m_oct_five_label = new QLabel();
	m_oct_five_label->setFont(tone_font);

	m_octave_group = new QButtonGroup();
	m_octave_group->setExclusive(true);
	m_octave_group->addButton(m_octave_one_cb, 0);
	m_octave_group->addButton(m_octave_two_cb, 1);
	m_octave_group->addButton(m_octave_three_cb, 2);
	m_octave_group->addButton(m_octave_four_cb, 3);
	m_octave_group->addButton(m_octave_five_cb, 4);
	connect(m_octave_group, SIGNAL(buttonClicked(int)), this, SLOT(updateToneOctave(int)));

	QHBoxLayout * oct_one_layout = new QHBoxLayout();
	oct_one_layout->setAlignment(Qt::AlignLeft);
	oct_one_layout->addWidget(m_octave_one_cb);
	oct_one_layout->addWidget(m_oct_one_label);

	QHBoxLayout * oct_two_layout = new QHBoxLayout();
	oct_two_layout->setAlignment(Qt::AlignLeft);
	oct_two_layout->addWidget(m_octave_two_cb);
	oct_two_layout->addWidget(m_oct_two_label);

	QHBoxLayout * oct_three_layout = new QHBoxLayout();
	oct_three_layout->setAlignment(Qt::AlignLeft);
	oct_three_layout->addWidget(m_octave_three_cb);
	oct_three_layout->addWidget(m_oct_three_label);

	QHBoxLayout * oct_four_layout = new QHBoxLayout();
	oct_four_layout->setAlignment(Qt::AlignLeft);
	oct_four_layout->addWidget(m_octave_four_cb);
	oct_four_layout->addWidget(m_oct_four_label);

	QHBoxLayout * oct_five_layout = new QHBoxLayout();
	oct_five_layout->setAlignment(Qt::AlignLeft);
	oct_five_layout->addWidget(m_octave_five_cb);
	oct_five_layout->addWidget(m_oct_five_label);

	QVBoxLayout * octave_layout = new QVBoxLayout();
	octave_layout->addLayout(oct_one_layout);
	octave_layout->addLayout(oct_two_layout);
	octave_layout->addLayout(oct_three_layout);
	octave_layout->addLayout(oct_four_layout);
	octave_layout->addLayout(oct_five_layout);

	m_binaural_gap_slider = new QSlider();
	m_binaural_gap_slider->setOrientation(Qt::Vertical);
	m_binaural_gap_slider->setRange(1, 25);
	connect(m_binaural_gap_slider, SIGNAL(valueChanged(int)), this, SLOT(updateBinauralGap(int)));

	m_binaural_label = new QLabel();
	m_binaural_label->setText("binaural gap");
	m_binaural_label->setFont(tone_font);

	m_binaural_gap_update_label = new QLabel();
	m_binaural_gap_update_label->setFont(tone_font);

	QHBoxLayout * binaural_update_layout = new QHBoxLayout();
	binaural_update_layout->addStretch(1);
	binaural_update_layout->addWidget(m_binaural_gap_update_label);
	binaural_update_layout->addStretch(1);

	QHBoxLayout * binaural_slider_layout = new QHBoxLayout();
	binaural_slider_layout->addStretch(1);
	binaural_slider_layout->addWidget(m_binaural_gap_slider);
	binaural_slider_layout->addStretch(1);

	QHBoxLayout * binaural_update_label_layout = new QHBoxLayout();
	binaural_update_label_layout->addStretch(1);
	binaural_update_label_layout->addWidget(m_binaural_label);
	binaural_update_label_layout->addStretch(1);

	QVBoxLayout * binaural_gap_layout = new QVBoxLayout();
	binaural_gap_layout->addLayout(binaural_update_label_layout);
	binaural_gap_layout->addLayout(binaural_slider_layout);
	binaural_gap_layout->addLayout(binaural_update_layout);

	m_white_noise_slider = new QSlider();
	m_white_noise_slider->setOrientation(Qt::Vertical);
	m_white_noise_slider->setRange(0, 100);
	connect(m_white_noise_slider, SIGNAL(valueChanged(int)), this, SLOT(updateWhiteNoiseVolume(int)));

	QLabel * white_noise_label = new QLabel();
	white_noise_label->setFont(tone_font);
	white_noise_label->setText(tr("white noise"));

	m_white_noise_cb = new QCheckBox();
	connect(m_white_noise_cb, SIGNAL(stateChanged(int)), this, SLOT(toggleWhiteNoise(int)));

	QHBoxLayout * white_label_layout = new QHBoxLayout();
	white_label_layout->addStretch(1);
	white_label_layout->addWidget(white_noise_label);
	white_label_layout->addStretch(1);

	QHBoxLayout * white_slider_layout = new QHBoxLayout();
	white_slider_layout->addStretch(1);
	white_slider_layout->addWidget(m_white_noise_slider);
	white_slider_layout->addStretch(1);

	QHBoxLayout * white_cb_layout = new QHBoxLayout();
	white_cb_layout->addStretch(2);
	white_cb_layout->addWidget(m_white_noise_cb);
	white_cb_layout->addSpacing(3);
	white_cb_layout->addStretch(1);

	QVBoxLayout * white_noise_layout = new QVBoxLayout();
	white_noise_layout->setAlignment(Qt::AlignHCenter);
	white_noise_layout->addLayout(white_label_layout);
	white_noise_layout->addLayout(white_slider_layout);
	white_noise_layout->addLayout(white_cb_layout);

	QHBoxLayout * tone_layout = new QHBoxLayout();
	tone_layout->addLayout(octave_layout);
	tone_layout->addStretch(1);
	tone_layout->addLayout(binaural_gap_layout);
	tone_layout->addSpacing(5);
	tone_layout->addLayout(white_noise_layout);

	m_tone_panel = new QTranscluentWidget();
	m_tone_panel->setOpacity(0.5);
	m_tone_panel->setLayout(tone_layout);
	m_tone_panel->hide();

	m_max_layout_left = new QVBoxLayout();
	m_max_layout_left->addWidget(m_max_label);
	m_max_layout_left->addLayout(sound_slider_checkbox_layout);
	m_max_layout_left->addWidget(m_effects_panel);

	//sound properties
	AeroButton * nature_button = new AeroButton();
	nature_button->setCheckable(true);
	nature_button->setHighlight(QColor(Qt::green));
	nature_button->setMinimumHeight(40);
	nature_button->setMaximumWidth(80);
	nature_button->setRoundness(10);
	nature_button->setOpacity(0.55);
	nature_button->setToolTip("nature");

	AeroButton * civilization_button = new AeroButton();
	civilization_button->setCheckable(true);
	civilization_button->setHighlight(QColor(Qt::yellow));
	civilization_button->setMinimumHeight(40);
	civilization_button->setMaximumWidth(80);
	civilization_button->setRoundness(10);
	civilization_button->setOpacity(0.55);
	civilization_button->setToolTip("civilization");

	AeroButton * tones_button = new AeroButton();
	tones_button->setCheckable(true);
	tones_button->setHighlight(QColor(2, 135, 237));
	tones_button->setMinimumHeight(40);
	tones_button->setMaximumWidth(80);
	tones_button->setRoundness(10);
	tones_button->setOpacity(0.55);
	tones_button->setToolTip("tones");

	AeroButton * music_button = new AeroButton();
	music_button->setCheckable(true);
	music_button->setHighlight(QColor(175, 0, 175));
	music_button->setMinimumHeight(40);
	music_button->setMaximumWidth(80);
	music_button->setRoundness(10);
	music_button->setOpacity(0.55);
	music_button->setToolTip("music");

	AeroButton * personal_button = new AeroButton();
	personal_button->setCheckable(true);
	personal_button->setHighlight(QColor(Qt::red));
	personal_button->setMinimumHeight(40);
	personal_button->setMaximumWidth(80);
	personal_button->setRoundness(10);
	personal_button->setOpacity(0.55);
	personal_button->setToolTip("personal");

	m_sound_button_group = new QButtonGroup();
	m_sound_button_group->setExclusive(true);
	m_sound_button_group->addButton(nature_button, 0);
	m_sound_button_group->addButton(civilization_button, 1);
	m_sound_button_group->addButton(tones_button, 2);
	m_sound_button_group->addButton(music_button, 3);
	m_sound_button_group->addButton(personal_button, 4);

	QHBoxLayout * button_layout = new QHBoxLayout();
	button_layout->setSpacing(1);
	button_layout->addWidget(personal_button);
	button_layout->addWidget(civilization_button);
	button_layout->addWidget(nature_button);
	button_layout->addWidget(tones_button);
	button_layout->addWidget(music_button);
	
	m_sound_list = new ResoTree();
	m_sound_list->setFocusPolicy(Qt::NoFocus);
	m_sound_list->setPalette(QPalette(Qt::white));
	m_sound_list->setHeaderHidden(true);
	m_sound_list->setUniformRowHeights(true);
	m_sound_list->setMaximumWidth(325);
	connect(m_sound_list, SIGNAL(removeSound(QTreeWidgetItem*)), this, SLOT(removeSoundFromList(QTreeWidgetItem*)));

	QVBoxLayout * properties_layout = new QVBoxLayout();
	properties_layout->setAlignment(Qt::AlignCenter);
	properties_layout->addLayout(button_layout);
	properties_layout->addWidget(m_sound_list);

	QHBoxLayout * max_layout = new QHBoxLayout();
	max_layout->addLayout(m_max_layout_left);
	max_layout->addLayout(properties_layout);

	m_max_module->setLayout(max_layout);

	connect(m_min_module, SIGNAL(clicked()), this, SIGNAL(clicked()));
	connect(m_max_module, SIGNAL(clicked()), this, SIGNAL(clicked()));

	connect(m_min_slider, SIGNAL(valueChanged(int)), m_max_slider, SLOT(setValue(int)));
	connect(m_max_slider, SIGNAL(valueChanged(int)), m_min_slider, SLOT(setValue(int)));
	connect(m_max_slider, SIGNAL(valueChanged(int)), this, SLOT(updateVolume(int)));

	connect(m_max_checkbox, SIGNAL(stateChanged(int)), this, SLOT(updateMinCheckState(int)));

	connect(m_min_checkbox, SIGNAL(stateChanged(int)), this, SLOT(updateMaxCheckState(int)));
	connect(m_max_checkbox, SIGNAL(stateChanged(int)), this, SLOT(updatePlaying(int)));
	connect(m_sound_list, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(loadSound(QTreeWidgetItem*, int)));
	connect(m_sound_button_group, SIGNAL(buttonClicked(int)), this, SLOT(loadSoundList(int)));
}

void ResoModule::setSoundClass(Resonance::SoundClass new_class)
{
	m_sound_class = new_class;

	QColor new_color = m_sound_color_map.value(new_class);
	m_min_module->setHighlight(new_color);
	m_max_module->setHighlight(new_color);

	update();
}

void ResoModule::loadTonesList()
{
	QDir tones_dir("./sounds/tones");
	QFileInfoList tones = tones_dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

	m_sound_list->clear();

	QTreeWidgetItem * binaural_folder = new QTreeWidgetItem(m_sound_list, QStringList("binaural tones"));

	for(int t = 0; t < tones.count(); t++)
	{
		QString name = tones.at(t).baseName().prepend(":: ");
		QTreeWidgetItem * tone = new QTreeWidgetItem(binaural_folder, QStringList(name));
		tone->setData(0, Qt::UserRole, tones.at(t).absoluteFilePath());
	}
}

void ResoModule::loadBinauralData(const QString & tone_path)
{
	QFile file(tone_path);
	if(!file.open(QIODevice::ReadOnly))
		return;

	m_binaural_data.name = QString(file.readLine()).split(":").at(1);
	m_binaural_data.default_frequency = QString(file.readLine()).split(":").at(1).toFloat();
	m_binaural_data.octave_one = QString(file.readLine()).split(":").at(1).toFloat();
	m_binaural_data.octave_two = QString(file.readLine()).split(":").at(1).toFloat();
	m_binaural_data.octave_three = QString(file.readLine()).split(":").at(1).toFloat();
	m_binaural_data.octave_four = QString(file.readLine()).split(":").at(1).toFloat();
	m_binaural_data.octave_five = QString(file.readLine()).split(":").at(1).toFloat();
	m_binaural_data.octave_six = QString(file.readLine()).split(":").at(1).toFloat();

	file.close();

	bool loaded = emit newTone(m_id, m_binaural_data);
	if(loaded)
	{
		m_tone_loaded = true;

		updatePanelDisplay();
		setSoundClass(Resonance::Tone);

		int delay;
		(Resonance::SoundClass)m_sound_button_group->checkedId() == m_sound_class ? delay = 0 : delay = 500;
		QTimer::singleShot(delay, this, SLOT(resetModule()));
	}
}
