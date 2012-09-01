#include "Resonance.h"

#include "drawerwidget.h"
#include "AeroButton.h"
#include "QTranscluentWidget.h"
#include "ResoModule.h"
#include "FMOD_Sound.h"
#include "ResoStyle.h"
#include "ResoTree.h"

#include "fsimagemodel.h"
#include "qxtflowview_p.h"
#include "BinauralTone.h"
#include <QxtFlowView>

Resonance::Resonance(QWidget * parent) : QWidget(parent),
m_bg_opacity(0.0), 
m_expanded_module(-1), 
m_bg_index(1), 
m_bg_timer(0), 
m_fade_out_id(0),
m_pre_fade_out_volume(50)
{
	setupGUI();
	setAcceptDrops(true);

	m_fmod_sound = new FMOD_Sound();
	for(int module = 0; module < m_modules.count(); module++)
	{
		connect(m_modules.at(module), SIGNAL(newSound(int, QString)), m_fmod_sound, SLOT(loadNewSound(int, QString)));
		connect(m_modules.at(module), SIGNAL(newTone(int, BinauralData)), m_fmod_sound, SLOT(loadNewTone(int, BinauralData)));
		connect(m_modules.at(module), SIGNAL(play(int, float)), m_fmod_sound, SLOT(play(int, float)));
		connect(m_modules.at(module), SIGNAL(stop(int)), m_fmod_sound, SLOT(stop(int)));
		connect(m_modules.at(module), SIGNAL(setPaused(int, bool)), m_fmod_sound, SLOT(setPaused(int, bool)));
		connect(m_modules.at(module), SIGNAL(setVolume(int, float)), m_fmod_sound, SLOT(setVolume(int, float)));
		connect(m_modules.at(module), SIGNAL(addEffect(int, int, float)), m_fmod_sound, SLOT(addEffect(int, int, float)));
		connect(m_modules.at(module), SIGNAL(removeEffect(int, int)), m_fmod_sound, SLOT(removeEffect(int, int)));
		connect(m_modules.at(module), SIGNAL(setEffectValue(int, int, float)), m_fmod_sound, SLOT(setEffectValue(int, int, float)));

		connect(m_modules.at(module), SIGNAL(playTone(int, float)), m_fmod_sound, SLOT(playTone(int, float)));
		connect(m_modules.at(module), SIGNAL(stopTone(int)), m_fmod_sound, SLOT(stopTone(int)));
		connect(m_modules.at(module), SIGNAL(setTonePaused(int, bool)), m_fmod_sound, SLOT(setTonePaused(int, bool)));
		connect(m_modules.at(module), SIGNAL(setToneVolume(int, float)), m_fmod_sound, SLOT(setToneVolume(int, float)));

		connect(m_modules.at(module), SIGNAL(changeOctave(int, float)), m_fmod_sound, SLOT(changeOctave(int, float)));
		connect(m_modules.at(module), SIGNAL(changeBinauralGap(int, int)), m_fmod_sound, SLOT(changeBinauralGap(int, int)));

		connect(m_modules.at(module), SIGNAL(addWhiteNoise(int, float)), m_fmod_sound, SLOT(addWhiteNoise(int, float)));
		connect(m_modules.at(module), SIGNAL(removeWhiteNoise(int)), m_fmod_sound, SLOT(removeWhiteNoise(int)));
		connect(m_modules.at(module), SIGNAL(setWhiteNoiseVolume(int, float)), m_fmod_sound, SLOT(setWhiteNoiseVolume(int, float)));

		connect(this, SIGNAL(masterVolumeChanged(int)), m_modules.at(module), SLOT(updateForMasterVolume(int)));
	}

	m_system_tray_icon = new QSystemTrayIcon(this);
	m_system_tray_icon->setIcon(QIcon("images/resoicon.png"));

	connect(m_system_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), 
			this, SLOT(maximizeWindow(QSystemTrayIcon::ActivationReason)));

	connect(m_system_tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), 
			m_system_tray_icon, SLOT(hide(QSystemTrayIcon::ActivationReason)));

	m_sys_tray_menu = new QMenu(this);
	m_sys_tray_menu->setStyle(new QCleanlooksStyle());

	m_sys_tray_pause = new QAction("pause", m_sys_tray_menu);
	m_sys_tray_pause->setCheckable(true);
	connect(m_sys_tray_pause, SIGNAL(triggered()), this, SLOT(togglePlay()));
	m_sys_tray_menu->addAction(m_sys_tray_pause);

	m_sys_tray_action = new QAction("exit", m_sys_tray_menu);
	connect(m_sys_tray_action, SIGNAL(triggered()), qApp, SLOT(quit()));
	m_sys_tray_menu->addAction(m_sys_tray_action);
	
	m_system_tray_icon->setContextMenu(m_sys_tray_menu);
	m_system_tray_icon->setToolTip("resonance");

	QTreeWidgetItem * startup_preset = new QTreeWidgetItem(QStringList("presets/factory/startup.xml"));
	startup_preset->setData(0, Qt::UserRole, "");
	loadPreset(startup_preset, 0);

	m_playing = false;
	for(int m = 0; m < m_modules.count(); m++)
	{
		if(m_modules.at(m)->enabled())
		{
			m_playing = true;
			break;
		}
	}
	if(m_playing)
	{
		QColor m_pause_highlight = QColor(Qt::red);
		m_pause_button->setHighlight(m_pause_highlight);
		m_pause_button->setToolTip("pause");
		m_pause_button->setIcon(QIcon("images/pause.png"));
		m_pause_button->setIconSize(QSize(15, 15));
	}

	m_master_slider->setValue(50);
}

Resonance::~Resonance()
{
}

void Resonance::updateModuleLayout(int clicked_module)
{
	if(m_modules.at(clicked_module)->expanded())
	{
		//collapse the expanded clicked_module
		m_module_layout->removeWidget(m_modules.at(clicked_module)->maxModule());
		m_module_layout->insertWidget(clicked_module, m_modules.at(clicked_module)->minModule());
		m_modules.at(clicked_module)->maxModule()->hide();
		m_modules.at(clicked_module)->minModule()->show();
		m_modules.at(clicked_module)->setExpanded(false);
	}
	else
	{
		//first collapse any expanded clicked_module
		for(int i = 0; i < 5; i++)
		{
			if(m_modules.at(i)->expanded())
			{
				m_module_layout->removeWidget(m_modules.at(i)->maxModule());
				m_module_layout->insertWidget(i, m_modules.at(i)->minModule());
				m_modules.at(i)->maxModule()->hide();
				m_modules.at(i)->minModule()->show();
				m_modules.at(i)->setExpanded(false);
			}
		}
		//now expand the current clicked_module
		m_module_layout->removeWidget(m_modules.at(clicked_module)->minModule());
		m_module_layout->insertWidget(clicked_module, m_modules.at(clicked_module)->maxModule());
		m_modules.at(clicked_module)->minModule()->hide();
		m_modules.at(clicked_module)->maxModule()->show();
		m_modules.at(clicked_module)->setExpanded(true);
		m_modules.at(clicked_module)->minModule()->setChecked(false);
		m_modules.at(clicked_module)->maxModule()->setChecked(true);
	}
}

void Resonance::toggleModules()
{
	static bool shown = true;
	if(shown)
	{
		m_modules_widget->hide();
		shown = false;
	}
	else
	{
		m_modules_widget->show();
		shown = true;
	}
}

void Resonance::togglePresets()
{
	static bool shown = false;
	if(shown)
	{
		m_presets_widget->hide();
		shown = false;
	}
	else
	{
		m_presets_widget->show();
		shown = true;
	}
}

void Resonance::togglePlay()
{
	if(m_playing)
	{
		for(int m = 0; m < m_modules.count(); m++)
		{
			m_modules.at(m)->pause();
		}
		m_pause_button->setIcon(QIcon("images/play.png"));
		m_pause_button->setIconSize(QSize(15, 15));
		QColor m_pause_highlight = QColor(Qt::green);
		m_pause_button->setHighlight(m_pause_highlight);
		m_pause_button->setToolTip("play");

		if(isVisible())
		{
			m_sys_tray_pause->blockSignals(true);
			if(!m_sys_tray_pause->isChecked())
			{
				m_sys_tray_pause->setChecked(true);
			}
			m_sys_tray_pause->blockSignals(false);
		}

		m_playing = false;
	}
	else
	{
		for(int m = 0; m < m_modules.count(); m++)
		{
			m_modules.at(m)->play();
		}
		m_pause_button->setIcon(QIcon("images/pause.png"));
		m_pause_button->setIconSize(QSize(15, 15));
		QColor m_pause_highlight = QColor(Qt::red);
		m_pause_button->setHighlight(m_pause_highlight);
		m_pause_button->setToolTip("pause");

		if(isVisible())
		{
			m_sys_tray_pause->blockSignals(true);
			if(m_sys_tray_pause->isChecked())
			{
				m_sys_tray_pause->setChecked(false);
			}
			m_sys_tray_pause->blockSignals(false);
		}

		m_playing = true;
	}
}

void Resonance::toggleTimer(bool on)
{
	if(on)
	{
		m_timer_spin->show();
		m_go_button->show();
	}
	else
	{
		m_timer_spin->hide();
		m_go_button->hide();
	}
}

void Resonance::toggleMainWindows(int window)
{
	switch(window)
	{
		case 0:
			m_presets_widget->hide();
			m_presets_button->blockSignals(true);
			m_presets_button->setChecked(false);
			m_presets_button->blockSignals(false);

			m_visuals_widget->hide();
			m_visual_button->blockSignals(true);
			m_visual_button->setChecked(false);
			m_visual_button->blockSignals(false);

			m_help_widget->hide();
			m_help_button->blockSignals(true);
			m_help_button->setChecked(false);
			m_help_button->blockSignals(false);

			if(m_module_button->isChecked())
			{
				m_modules_widget->show();
			}
			else
			{
				m_modules_widget->hide();
			}
			break;
		case 1:
			m_modules_widget->hide();
			m_module_button->blockSignals(true);
			m_module_button->setChecked(false);
			m_module_button->blockSignals(false);

			m_visuals_widget->hide();
			m_visual_button->blockSignals(true);
			m_visual_button->setChecked(false);
			m_visual_button->blockSignals(false);

			m_help_widget->hide();
			m_help_button->blockSignals(true);
			m_help_button->setChecked(false);
			m_help_button->blockSignals(false);

			if(m_presets_button->isChecked())
			{
				m_presets_widget->show();
			}
			else
			{
				m_presets_widget->hide();
			}
			break;
		case 2:
			m_modules_widget->hide();
			m_module_button->blockSignals(true);
			m_module_button->setChecked(false);
			m_module_button->blockSignals(false);

			m_presets_widget->hide();
			m_presets_button->blockSignals(true);
			m_presets_button->setChecked(false);
			m_presets_button->blockSignals(false);

			m_help_widget->hide();
			m_help_button->blockSignals(true);
			m_help_button->setChecked(false);
			m_help_button->blockSignals(false);

			if(m_visual_button->isChecked())
			{
				m_visuals_widget->show();
			}
			else
			{
				m_visuals_widget->hide();
			}
			break;
		case 3:
			m_modules_widget->hide();
			m_module_button->blockSignals(true);
			m_module_button->setChecked(false);
			m_module_button->blockSignals(false);

			m_presets_widget->hide();
			m_presets_button->blockSignals(true);
			m_presets_button->setChecked(false);
			m_presets_button->blockSignals(false);

			m_visuals_widget->hide();
			m_visual_button->blockSignals(true);
			m_visual_button->setChecked(false);
			m_visual_button->blockSignals(false);

			if(m_help_button->isChecked())
			{
				m_help_widget->show();
			}
			else
			{
				m_help_widget->hide();
			}
			break;
		default:
			break;
	}
}

void Resonance::toggleTutorial(int tutorial)
{
	QString tutorial_type;
	tutorial == 0 ? tutorial_type = "guide/index.html" : tutorial_type = "guide/binaural_tones.txt";

	QFile html;
	html.setFileName(tutorial_type);
	html.open(QIODevice::ReadOnly);
	QString text = html.readAll();
	html.close();
	m_guide_widget->clear();
	m_guide_widget->setHtml(text);
}

void Resonance::startSoundTimeout()
{
	m_stop_sounds_timer = startTimer(m_timer_spin->value() * 60 * 1000);
	m_stop_sounds_ms = m_timer_spin->value() * 60 * 1000;

	m_timer_button->setCheckable(false);
	QColor m_timer_highlight = QColor(Qt::red);
	m_timer_button->setHighlight(m_timer_highlight);
	m_timer_button->setToolTip("stop timer");
	disconnect(m_timer_button, SIGNAL(toggled(bool)), this, SLOT(toggleTimer(bool)));
	connect(m_timer_button, SIGNAL(clicked()), this, SLOT(stopSoundTimeout()));

	m_timer_spin->hide();
	m_go_button->hide();

	m_countdown_label->show();

	m_countdown_timer_id = startTimer(50);
	m_countdown_timer.start();
}

void Resonance::stopSoundTimeout()
{
	killTimer(m_fade_out_id);
	m_fade_out_id = 0;

	killTimer(m_stop_sounds_timer);
	m_stop_sounds_timer = 0;

	killTimer(m_countdown_timer_id);
	m_countdown_timer_id = 0;

	m_countdown_label->hide();

	m_system_tray_icon->setToolTip("resonance");

	m_timer_button->setCheckable(true);
	QColor m_timer_highlight = QColor(Qt::green);
	m_timer_button->setHighlight(m_timer_highlight);
	m_timer_button->setToolTip("start timer");
	disconnect(m_timer_button, SIGNAL(clicked()), this, SLOT(stopSoundTimeout()));
	connect(m_timer_button, SIGNAL(toggled(bool)), this, SLOT(toggleTimer(bool)));
}

void Resonance::updateCountdownClock(int time_to_go)
{
	int seconds_to_go = time_to_go / 1000;

	static int seconds = 0;
	if(seconds_to_go != seconds)
	{
		int minutes_to_go = seconds_to_go / 60;
		int seconds_minus_minutes = seconds_to_go - (minutes_to_go * 60);
		QString display_seconds;
		seconds_minus_minutes < 10 ? display_seconds = "0" + QString::number(seconds_minus_minutes) : display_seconds = QString::number(seconds_minus_minutes);
		QString time = " :: " + QString::number(minutes_to_go) + QString(":") + display_seconds;

		m_countdown_label->setText(time);
		m_countdown_label->adjustSize();

		time = time.remove(" :: ");
		m_system_tray_icon->setToolTip(time);

		seconds = seconds_to_go;
	}
}

void Resonance::maximizeWindow(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
	{
		m_bg_timer = startTimer(400);

		move(m_current_screen_pos);
		showNormal();
		activateWindow();
		raise();

		m_system_tray_icon->hide();
	}
}

void Resonance::loadPreset(QTreeWidgetItem * preset, int column)
{
	//collapse expanded module
	for(int module = 0; module < m_modules.count(); module++)
	{
		if(m_modules.at(module)->expanded())
		{
			m_modules.at(module)->maxModule()->click();
		}
	}

	QString presetPath = preset->data(0, Qt::UserRole).toString();
	if(presetPath.isEmpty())
	{
		presetPath = preset->text(0);
	}

	QFile presetFile(presetPath);
	if(!presetFile.open(QIODevice::ReadOnly))
		return;

	QDomDocument presetDocument;
	QString errorMessage;
	if(!presetDocument.setContent(&presetFile, false, &errorMessage))
		return;
	presetFile.close();

	QDomNode presetNode = presetDocument.firstChild();
	if(presetNode.isNull())
	{
		Q_ASSERT(!presetNode.isNull());
	}

	presetNode = presetNode.firstChild();
	if(presetNode.isNull())
	{
		Q_ASSERT(!presetNode.isNull());
	}

	QDomElement presetElement = presetNode.toElement();
	if(presetNode.isNull())
	{
		Q_ASSERT(!presetElement.isNull());
	}

	while(!presetElement.isNull())
	{
		ModuleState module_state;

		module_state.id = presetElement.attribute("number").toInt();
		module_state.sound_path = presetElement.attribute("sound");
		module_state.sound_class = presetElement.attribute("class").toInt();
		module_state.expanded = presetElement.attribute("expanded").toInt();
		module_state.enabled = presetElement.attribute("module_enabled").toInt();
		module_state.volume = presetElement.attribute("module_volume").toInt();
		module_state.tone_loaded = presetElement.attribute("tone_loaded").toInt();

		if(presetElement.attribute("tone_loaded").toInt())
		{
			module_state.octave_number = presetElement.attribute("octave").toInt();
			module_state.binaural_gap = presetElement.attribute("binaural_gap").toInt();
			module_state.white_noise_enabled = presetElement.attribute("white_noise_enabled").toInt();
			module_state.white_noise_volume = presetElement.attribute("white_noise_volume").toInt();
		}
		else
		{
			module_state.reverb_enabled = presetElement.attribute("reverb_enabled").toInt();
			module_state.reverb_value = presetElement.attribute("reverb_volume").toInt();
			module_state.pitch_enabled = presetElement.attribute("pitch_enabled").toInt();
			module_state.pitch_value = presetElement.attribute("pitch_volume").toInt();
			module_state.distortion_enabled = presetElement.attribute("distortion_enabled").toInt();
			module_state.distortion_value = presetElement.attribute("distortion_volume").toInt();
			module_state.echo_enabled = presetElement.attribute("echo_enabled").toInt();
			module_state.echo_value = presetElement.attribute("echo_volume").toInt();
			module_state.flange_enabled = presetElement.attribute("flange_enabled").toInt();
			module_state.flange_value = presetElement.attribute("flange_volume").toInt();
		}
		
		m_modules.at(module_state.id)->setModuleState(module_state);

		presetElement = presetElement.nextSiblingElement();
	}

	for(int module = 0; module < m_modules.count(); module++)
	{
		if(m_modules.at(module)->expanded())
		{
			m_modules.at(module)->setExpanded(false);
			m_modules.at(module)->maxModule()->click();
		}
	}
}

void Resonance::savePreset(QString new_preset)
{
	QString preset;
	if(new_preset.contains("preset"))
	{
		if(!m_preset_line_edit->text().isEmpty())
		{
			preset = m_preset_line_edit->text();
			preset.prepend(":: ");
			QList<QTreeWidgetItem*> matching_presets = m_user_list->findItems(preset, Qt::MatchExactly);
			if(matching_presets.count() != 0)
			{
				return;
			}
			preset.remove(":: ");
		}
		else
		{
			return;
		}
	}
	else
	{
		preset = new_preset;
	}

	QDomDocument newPreset;
	QDomElement rootName = newPreset.createElement("Preset");
	newPreset.appendChild(rootName);
	for(int i = 0; i < m_modules.count(); i++)
	{
		QDomElement moduleElement = newPreset.createElement("Module");

		ModuleState module_state = m_modules.at(i)->currentState();

		moduleElement.setAttribute("number", module_state.id);
		moduleElement.setAttribute("expanded", module_state.expanded);
		moduleElement.setAttribute("sound", module_state.sound_path);
		moduleElement.setAttribute("class", module_state.sound_class);
		moduleElement.setAttribute("module_volume", module_state.volume);
		moduleElement.setAttribute("module_enabled", module_state.enabled);
		moduleElement.setAttribute("tone_loaded", module_state.tone_loaded);

		if(module_state.tone_loaded)
		{
			moduleElement.setAttribute("octave", module_state.octave_number);
			moduleElement.setAttribute("binaural_gap", module_state.binaural_gap);
			moduleElement.setAttribute("white_noise_enabled", module_state.white_noise_enabled);
			moduleElement.setAttribute("white_noise_volume", module_state.white_noise_volume);
		}
		else
		{
			moduleElement.setAttribute("reverb_enabled", module_state.reverb_enabled);
			moduleElement.setAttribute("pitch_enabled", module_state.pitch_enabled);
			moduleElement.setAttribute("distortion_enabled", module_state.distortion_enabled);
			moduleElement.setAttribute("echo_enabled", module_state.echo_enabled);
			moduleElement.setAttribute("flange_enabled", module_state.flange_enabled);
			moduleElement.setAttribute("reverb_volume", module_state.reverb_value);
			moduleElement.setAttribute("pitch_volume", module_state.pitch_value);
			moduleElement.setAttribute("distortion_volume", module_state.distortion_value);
			moduleElement.setAttribute("echo_volume", module_state.echo_value);
			moduleElement.setAttribute("flange_volume", module_state.flange_value);
		}
		
		rootName.appendChild(moduleElement);
	}

	QString presetPath;
	new_preset.contains("preset") ? presetPath = "presets/user/" : presetPath = "presets/factory/";
	presetPath += preset + QString(".xml");
	QFile newPresetFile(presetPath);
	if(!newPresetFile.open(QIODevice::WriteOnly))
	{
		return;
	}
	QTextStream xmlStream(&newPresetFile);
	newPreset.save(xmlStream, 0);
	newPresetFile.close();

	if(new_preset.contains("preset"))
	{
		QFileInfo info(newPresetFile);
		QString name = ":: " + preset;
		QTreeWidgetItem * item = new QTreeWidgetItem(m_user_list, QStringList(name));
		item->setData(0, Qt::UserRole, info.absoluteFilePath());
		m_user_list->addTopLevelItem(item);

		m_preset_line_edit->clear();
	}
}

void Resonance::setModulePaused(int module_id, bool paused)
{
	m_modules.at(module_id)->pause();
}

void Resonance::removePreset(QTreeWidgetItem * preset)
{
	QString path = preset->data(0, Qt::UserRole).toString();
	//delete file or dir
	QFileInfo file_info(path);
	if(file_info.isFile())
	{
		QDir file_dir = file_info.dir();
		file_dir.remove(file_info.fileName());
	}
	readInPresets();
}

void Resonance::newBackgroundInfo(QString index)
{
	QString file = "images/backgrounds/thumbnails/" + index;
	QFileInfo info(file);
	m_bg_filename = "images/backgrounds/" + info.fileName();

	m_bg_label->setText(info.baseName());
	m_bg_label->adjustSize();
}

void Resonance::setNewBackground()
{
	if(m_bg_filename.contains("resonance"))
	{
		m_default_bg = true;
		killTimer(m_bg_timer);
		m_bg_list.clear();

		m_bg_one = QPixmap("images/backgrounds/resonance_bg_zero.png");
		m_bg_two = QPixmap("images/backgrounds/resonance_bg_one.png");

		m_bg_list.push_back("images/backgrounds/resonance_bg_one.png");
		m_bg_list.push_back("images/backgrounds/resonance_bg_two.png");
		m_bg_list.push_back("images/backgrounds/resonance_bg_three.png");
		m_bg_list.push_back("images/backgrounds/resonance_bg_four.png");
		m_bg_list.push_back("images/backgrounds/resonance_bg_five.png");

		m_bg_index = 1;
	}
	else
	{
		m_default_bg = false;
		killTimer(m_bg_timer);
		m_bg_list.clear();
		m_bg_list.push_back(m_bg_filename);
		m_bg_one = QPixmap(m_bg_filename);
	}

	m_bg_timer = startTimer(400);
}

void Resonance::changeHelpMenu(int menu)
{
	static int current_menu = 0;
	QString guide;
	QFile file;

	//first take out the current menu
	switch(current_menu)
	{
	case 0:
		m_help_layout->removeWidget(m_about_widget);
		m_about_widget->hide();
		break;
	case 1:
		m_help_layout->removeWidget(m_tutorial_widget);
		m_tutorial_widget->hide();
		break;
	default:
		break;
	}

	//now put in the new one
	switch(menu)
	{
	case 0:
		m_help_layout->insertWidget(1, m_about_widget);
		m_about_widget->show();
		break;
	case 1:
		m_help_layout->insertWidget(1, m_tutorial_widget);
		m_tutorial_widget->show();
		break;
	default:
		break;
	}

	current_menu = menu;
}

void Resonance::gotoWebSite()
{
	QDesktopServices::openUrl(QUrl("http://www.resonance-asm.com"));
}

void Resonance::timerEvent(QTimerEvent * te)
{
	if(te->timerId() == m_fade_out_id)
	{
		int val = m_master_slider->value() - 1;
		m_master_slider->setValue(val);
	}
	if(te->timerId() == m_bg_timer)
	{
		if(m_default_bg)
		{
			if(m_bg_opacity >= 1.0)
			{
				m_bg_one = m_bg_two;
				m_bg_two = QPixmap(m_bg_list.at(m_bg_index));
				m_bg_opacity = 0.0;

				m_bg_index++;
				if(m_bg_index > m_bg_list.count() - 1)
					m_bg_index = 0;
			}
			else
			{
				m_bg_opacity += .005;
			}
		}
		update();
	}
	if(te->timerId() == m_stop_sounds_timer)
	{
		for(int m = 0; m < m_modules.count(); m++)
		{
			setModulePaused(m, true);
		}

		m_playing = false;

		m_pause_button->setIcon(QIcon("images/play.png"));
		m_pause_button->setIconSize(QSize(15, 15));
		QColor m_pause_highlight = QColor(Qt::green);
		m_pause_button->setHighlight(m_pause_highlight);
		m_pause_button->setToolTip("play");

		m_system_tray_icon->setToolTip("resonance");

		m_master_slider->setValue(m_pre_fade_out_volume);

		stopSoundTimeout();
	}
	if(te->timerId() == m_countdown_timer_id)
	{
		int time_to_go = m_stop_sounds_ms - m_countdown_timer.elapsed();
		if(time_to_go < 15000 && m_fade_out_id == 0)
		{
			int current_volume = m_master_slider->value();
			m_pre_fade_out_volume = current_volume;
			if(current_volume == 0)
				current_volume = 1;
			int time_out = 14000 / current_volume;
			m_fade_out_id = startTimer(time_out);
		}
		updateCountdownClock(time_to_go);
	}
}

void Resonance::paintEvent(QPaintEvent * pe)
{
	QWidget::paintEvent(pe);
	if(m_bg_timer != DO_NOT_UPDATE)
	{
		QPainter painter(this);
		painter.fillRect(rect(), QBrush(Qt::black));
		painter.drawPixmap(rect(), m_bg_one, m_bg_one.rect());
		if(m_default_bg)
		{
			painter.setOpacity(m_bg_opacity);
			painter.drawPixmap(rect(), m_bg_two, m_bg_two.rect());
		}
	}
}

void Resonance::resizeEvent(QResizeEvent * re)
{
	m_modules_widget->resize(width(), height() - 50);
	m_presets_widget->resize(width(), height() - 50);
	m_visuals_widget->resize(width(), height() - 50);
	m_help_widget->resize(width(), height() - 50);

	m_guide_widget->setMinimumHeight(height() - 200);
}

void Resonance::moveEvent(QMoveEvent * me)
{
	m_modules_widget->setParentMoved();
	m_modules_widget->move(0, 50);
}

void Resonance::changeEvent(QEvent * ce)
{
	if(ce->type() == QEvent::WindowStateChange)
	{
		if(isMinimized())
		{
			QTimer::singleShot(0, this, SLOT(hide()));
			m_system_tray_icon->show();
			ce->ignore();
			m_current_screen_pos = pos();

			killTimer(m_bg_timer);
			m_bg_timer = DO_NOT_UPDATE;
		}
	}
}

void Resonance::dragEnterEvent(QDragEnterEvent * de)
{
	if(de->mimeData()->hasUrls())
	{
		de->accept();
	}
}

void Resonance::dropEvent(QDropEvent * de)
{
	QList<QUrl> urls = de->mimeData()->urls();
	for(int i = 0; i < urls.count(); i++)
	{
		QString path = urls.at(i).path();
		if(path.startsWith("/"))
		{
			path = path.remove(0, 1);
			loadPersonalSound(path);
		}
	}
}

void Resonance::closeEvent(QCloseEvent * ce)
{
	QSettings settings("resonance", "general");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("background", m_bg_list.at(0));

	savePreset("startup");

	QWidget::closeEvent(ce);
}

void Resonance::setupGUI()
{
	setWindowTitle("resonance");

	setMinimumSize(640, 485);
	setMaximumSize(1024, 768);
	resize(800, 600);

	setupStyleSheets();

	m_modules_widget = new DrawerWidget(this);
	m_modules_widget->setSpeed(10);
	m_modules_widget->resize(800, 500);
	m_modules_widget->move(0, 50);
	m_modules_widget->raise();
	m_modules_widget->setDrawerArea(DrawerWidget::Top);

	QVBoxLayout * drawer_layout = new QVBoxLayout(m_modules_widget->label());

	QTranscluentWidget * drawer_background = new QTranscluentWidget();
	drawer_background->setOpacity(0.96);
	drawer_layout->addWidget(drawer_background);

	m_module_one = new ResoModule();
	m_module_one->setId(0);
	m_module_two = new ResoModule();
	m_module_two->setId(1);
	m_module_three = new ResoModule();
	m_module_three->setId(2);
	m_module_four = new ResoModule();
	m_module_four->setId(3);
	m_module_five = new ResoModule();
	m_module_five->setId(4);

	m_modules.push_back(m_module_one);
	m_modules.push_back(m_module_two);
	m_modules.push_back(m_module_three);
	m_modules.push_back(m_module_four);
	m_modules.push_back(m_module_five);

	QSignalMapper * module_mapper = new QSignalMapper();
	connect(m_module_one, SIGNAL(clicked()), module_mapper, SLOT(map()));
	module_mapper->setMapping(m_module_one, 0);
	connect(m_module_two, SIGNAL(clicked()), module_mapper, SLOT(map()));
	module_mapper->setMapping(m_module_two, 1);
	connect(m_module_three, SIGNAL(clicked()), module_mapper, SLOT(map()));
	module_mapper->setMapping(m_module_three, 2);
	connect(m_module_four, SIGNAL(clicked()), module_mapper, SLOT(map()));
	module_mapper->setMapping(m_module_four, 3);
	connect(m_module_five, SIGNAL(clicked()), module_mapper, SLOT(map()));
	module_mapper->setMapping(m_module_five, 4);

	connect(module_mapper, SIGNAL(mapped(int)), this, SLOT(updateModuleLayout(int)));

	m_module_layout = new QVBoxLayout();
	m_module_layout->addWidget(m_module_one->minModule());
	m_module_layout->addWidget(m_module_two->minModule());
	m_module_layout->addWidget(m_module_three->minModule());
	m_module_layout->addWidget(m_module_four->minModule());
	m_module_layout->addWidget(m_module_five->minModule());

	drawer_background->setLayout(m_module_layout);

	m_presets_widget = new DrawerWidget(this);
	m_presets_widget->resize(800, 500);
	m_presets_widget->move(0, 50);
	m_presets_widget->raise();
	m_presets_widget->setDrawerArea(DrawerWidget::Top);

	drawer_layout = new QVBoxLayout(m_presets_widget->label());

	QTranscluentWidget * presets_background = new QTranscluentWidget();
	drawer_layout->addWidget(presets_background);
	presets_background->setOpacity(0.96);

	QVBoxLayout * presets_layout = new QVBoxLayout();
	presets_background->setLayout(presets_layout);

	m_factory_list = new ResoTree();
	m_factory_list->setMaximumWidth(350);
	m_factory_list->setFocusPolicy(Qt::NoFocus);
	m_factory_list->setPalette(QPalette(Qt::white));
	m_factory_list->setHeaderLabel("standard ::");
	connect(m_factory_list, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(loadPreset(QTreeWidgetItem*, int)));

	m_user_list = new ResoTree();
	m_user_list->setMaximumWidth(350);
	m_user_list->setFocusPolicy(Qt::NoFocus);
	m_user_list->setPalette(QPalette(Qt::white));
	m_user_list->setHeaderLabel("user ::");
	m_user_list->setSoundClass(Resonance::Personal);
	connect(m_user_list, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(loadPreset(QTreeWidgetItem*, int)));
	connect(m_user_list, SIGNAL(removeSound(QTreeWidgetItem*)), this, SLOT(removePreset(QTreeWidgetItem*)));

	QVBoxLayout * factory_layout = new QVBoxLayout();
	factory_layout->addWidget(m_factory_list);

	QVBoxLayout * user_layout = new QVBoxLayout();
	user_layout->addWidget(m_user_list);

	QHBoxLayout * list_layout = new QHBoxLayout();
	list_layout->addLayout(factory_layout);
	list_layout->addSpacing(20);
	list_layout->addLayout(user_layout);

	readInPresets();

	QLabel * create_list_label = new QLabel();
	create_list_label->setText(tr("create preset ::"));

	m_preset_line_edit = new QLineEdit();
	m_preset_line_edit->setMaxLength(50);
	m_preset_line_edit->setContextMenuPolicy(Qt::NoContextMenu);
	m_preset_line_edit->setFixedWidth(300);
	m_preset_line_edit->setFocusPolicy(Qt::StrongFocus);

	AeroButton * save_preset_button = new AeroButton();
	QColor save_preset_highlight = QColor(Qt::red);
	save_preset_button->setHighlight(save_preset_highlight);
	save_preset_button->setText(tr("save"));
	save_preset_button->setRoundness(10);
	save_preset_button->setMinimumWidth(110);
	save_preset_button->setOpacity(0.5);
	connect(save_preset_button, SIGNAL(clicked()), this, SLOT(savePreset()));

	QHBoxLayout * preset_enter_layout = new QHBoxLayout();
	preset_enter_layout->addStretch(1);
	preset_enter_layout->addWidget(create_list_label);
	preset_enter_layout->addWidget(m_preset_line_edit);
	preset_enter_layout->addWidget(save_preset_button);
	preset_enter_layout->addStretch(1);

	AeroButton * preset_bg_button = new AeroButton();
	//preset_bg_button->setGlassOpacity(0.0);
	preset_bg_button->setRoundness(2);
	preset_bg_button->setOpacity(0.15);
	//preset_bg_button->setDecoration(true);
	preset_bg_button->setMinimumHeight(50);

	preset_bg_button->setLayout(preset_enter_layout);

	presets_layout->addLayout(list_layout);
	presets_layout->addWidget(preset_bg_button);

	m_visuals_widget = new DrawerWidget(this);
	m_visuals_widget->resize(800, 500);
	m_visuals_widget->move(0, 50);
	m_visuals_widget->raise();
	m_visuals_widget->setDrawerArea(DrawerWidget::Top);
	m_visuals_widget->hide();

	drawer_layout = new QVBoxLayout(m_visuals_widget->label());

	QTranscluentWidget * visuals_background = new QTranscluentWidget();
	drawer_layout->addWidget(visuals_background);
	visuals_background->setOpacity(0.96);

	QVBoxLayout * visuals_layout = new QVBoxLayout();
	visuals_background->setLayout(visuals_layout);

	FsImageModel model("images/backgrounds/thumbnails");

	m_bg_flow = new QxtFlowView();
	m_bg_flow->setSlideSize(QSize(250, 188));
	m_bg_flow->setModel(&model);
	m_bg_flow->setMinimumHeight(275);
	QColor m_bg_flow_background = QColor(Qt::black);
	m_bg_flow->setBackgroundColor(m_bg_flow_background);
	connect(m_bg_flow, SIGNAL(newImageChosen(QString)), this, SLOT(newBackgroundInfo(QString)));

	AeroButton * visuals_bg_button = new AeroButton();
	//visuals_bg_button->setGlassOpacity(0.0);
	visuals_bg_button->setRoundness(2);
	visuals_bg_button->setOpacity(0.15);
	//visuals_bg_button->setDecoration(true);
	visuals_bg_button->setMinimumHeight(50);

	m_bg_label = new QLabel();

	QDir backgrounds("images/backgrounds/thumbnails");
	QFileInfoList info = backgrounds.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

	m_bg_label->setText(info.at(0).baseName());
	newBackgroundInfo(info.at(0).fileName());

	m_bg_set_button = new AeroButton();
	QColor color_yellow = QColor(Qt::yellow);
	m_bg_set_button->setHighlight(color_yellow);
	m_bg_set_button->setText(tr("set background"));
	m_bg_set_button->setMinimumWidth(110);
	m_bg_set_button->setRoundness(10);
	m_bg_set_button->setOpacity(0.5);
	connect(m_bg_set_button, SIGNAL(clicked()), this, SLOT(setNewBackground()));

	QHBoxLayout * bg_choose_layout = new QHBoxLayout();
	bg_choose_layout->addWidget(m_bg_label);
	bg_choose_layout->addStretch(1);
	bg_choose_layout->addWidget(m_bg_set_button);

	visuals_bg_button->setLayout(bg_choose_layout);

	visuals_layout->addWidget(m_bg_flow);
	visuals_layout->addWidget(visuals_bg_button);

	//help/about
	m_help_widget = new DrawerWidget(this);
	m_help_widget->resize(800, 500);
	m_help_widget->move(0, 50);
	m_help_widget->raise();
	m_help_widget->setDrawerArea(DrawerWidget::Top);
	m_help_widget->hide();

	drawer_layout = new QVBoxLayout(m_help_widget->label());

	QTranscluentWidget * help_background = new QTranscluentWidget();
	drawer_layout->addWidget(help_background);
	help_background->setOpacity(0.96);

	AeroButton * help_bg_button = new AeroButton();
	//help_bg_button->setGlassOpacity(0.0);
	help_bg_button->setRoundness(2);
	help_bg_button->setOpacity(0.15);
	//help_bg_button->setDecoration(true);
	help_bg_button->setMinimumHeight(50);

	m_reso_about_label = new QLabel();
	m_reso_about_label->setAlignment(Qt::AlignCenter);
	m_reso_about_label->setPixmap(QPixmap("images/resologo.png"));
	m_reso_about_label->setScaledContents(false);
	m_reso_about_label->setFixedSize(275, 62);

	QHBoxLayout * reso_logo_layout = new QHBoxLayout();
	reso_logo_layout->addStretch(1);
	reso_logo_layout->addWidget(m_reso_about_label);
	reso_logo_layout->addStretch(1);

	QLabel * m_reso_spectrum = new QLabel();
	m_reso_spectrum->setAlignment(Qt::AlignCenter);
	m_reso_spectrum->setPixmap(QPixmap("images/spectrumbar.jpg"));
	m_reso_spectrum->setScaledContents(true);
	m_reso_spectrum->setFixedSize(200, 1);

	QHBoxLayout * reso_spectrum_layout = new QHBoxLayout();
	reso_spectrum_layout->addStretch(1);
	reso_spectrum_layout->addWidget(m_reso_spectrum);
	reso_spectrum_layout->addStretch(1);

	QLabel * m_reso_version = new QLabel();
	m_reso_version->setText("v1.0");

	QHBoxLayout * reso_version_layout = new QHBoxLayout();
	reso_version_layout->addStretch(1);
	reso_version_layout->addWidget(m_reso_version);
	reso_version_layout->addStretch(1);

	m_about_widget = new QWidget();
	
	QVBoxLayout * about_layout = new QVBoxLayout();
	about_layout->addLayout(reso_logo_layout);
	about_layout->addLayout(reso_version_layout);
	about_layout->addSpacing(10);
	about_layout->addLayout(reso_spectrum_layout);

	m_about_widget->setLayout(about_layout);

	//tutorial
	AeroButton * tutorial_button = new AeroButton(tr("tutorial"));
	//tutorial_button->setGlassOpacity(0.0);
	tutorial_button->setRoundness(2);
	tutorial_button->setOpacity(0.15);
	tutorial_button->setCheckable(true);
	tutorial_button->setMinimumHeight(25);
	tutorial_button->setChecked(true);

	AeroButton * binaural_button = new AeroButton(tr("binaural"));
	//binaural_button->setGlassOpacity(0.0);
	binaural_button->setRoundness(2);
	binaural_button->setOpacity(0.15);
	binaural_button->setCheckable(true);
	binaural_button->setMinimumHeight(25);

	QButtonGroup * guide_group = new QButtonGroup();
	guide_group->addButton(tutorial_button, 0);
	guide_group->addButton(binaural_button, 1);
	guide_group->setExclusive(true);
	connect(guide_group, SIGNAL(buttonClicked(int)), this, SLOT(toggleTutorial(int)));

	QHBoxLayout * guide_button_layout = new QHBoxLayout();
	guide_button_layout->addStretch(1);
	guide_button_layout->addWidget(tutorial_button);
	guide_button_layout->addWidget(binaural_button);
	guide_button_layout->addStretch(1);

	m_tutorial_widget = new QWidget();
	m_tutorial_layout = new QVBoxLayout();
	m_tutorial_widget->setLayout(m_tutorial_layout);
	m_tutorial_widget->hide();

	m_guide_widget = new QTextEdit();
	m_guide_widget->setContextMenuPolicy(Qt::NoContextMenu);
	m_guide_widget->setReadOnly(true);
	m_guide_widget->setMinimumHeight(400);
	QFile html;
	html.setFileName("guide/index.html");
	html.open(QIODevice::ReadOnly);
	QString text = html.readAll();
	html.close();
	m_guide_widget->setHtml(text);

	m_tutorial_layout->addLayout(guide_button_layout);
	m_tutorial_layout->addWidget(m_guide_widget);

	m_help_layout = new QVBoxLayout();
	m_help_layout->setAlignment(Qt::AlignCenter);
	m_help_layout->addStretch(1);
	m_help_layout->addWidget(m_about_widget);
	m_help_layout->addStretch(1);
	m_help_layout->addWidget(help_bg_button);

	AeroButton * about_button = new AeroButton();
	QColor about_highlight = QColor(2, 135, 237);
	about_button->setHighlight(about_highlight);
	about_button->setText(tr("about"));
	about_button->setMinimumWidth(110);
	about_button->setRoundness(10);
	about_button->setOpacity(0.5);
	about_button->setCheckable(true);
	about_button->setChecked(true);

	AeroButton * guide_button = new AeroButton();
	QColor guide_highlight = QColor(2, 135, 237);
	guide_button->setHighlight(guide_highlight);
	guide_button->setText(tr("guide"));
	guide_button->setMinimumWidth(110);
	guide_button->setRoundness(10);
	guide_button->setOpacity(0.5);
	guide_button->setCheckable(true);

	AeroButton * www_button = new AeroButton();
	QColor www_highlight = QColor(175, 0, 175);
	www_button->setHighlight(www_highlight);
	www_button->setText(tr("web"));
	www_button->setMinimumWidth(110);
	www_button->setRoundness(10);
	www_button->setOpacity(0.5);
	connect(www_button, SIGNAL(clicked()), this, SLOT(gotoWebSite()));

	QButtonGroup * help_button_group = new QButtonGroup();
	help_button_group->addButton(about_button, 0);
	help_button_group->addButton(guide_button, 1);
	help_button_group->setExclusive(true);
	connect(help_button_group, SIGNAL(buttonClicked(int)), this, SLOT(changeHelpMenu(int)));

	QHBoxLayout * help_buttons_layout = new QHBoxLayout();
	help_buttons_layout->addStretch(1);
	help_buttons_layout->addWidget(about_button);
	help_buttons_layout->addWidget(guide_button);
	help_buttons_layout->addWidget(www_button);
	help_buttons_layout->addStretch(1);

	help_bg_button->setLayout(help_buttons_layout);

	help_background->setLayout(m_help_layout);

	QColor color_white = QColor(Qt::white);
	QColor color_green = QColor(Qt::green);

	m_help_button = new AeroButton();
	QColor help_highlight = QColor(2, 135, 237);
	m_help_button->setHighlight(help_highlight);
	m_help_button->setIcon(QIcon("images/help.png"));
	m_help_button->setIconSize(QSize(15, 14));
	m_help_button->setRoundness(10);
	m_help_button->setFixedSize(27, 27);
	m_help_button->setOpacity(0.5);
	m_help_button->setToolTip("help/about");
	m_help_button->setCheckable(true);

	m_pause_button = new AeroButton();
	m_pause_button->setHighlight(color_green);
	m_pause_button->setIcon(QIcon("images/play.png"));
	m_pause_button->setIconSize(QSize(15, 15));
	m_pause_button->setRoundness(10);
	m_pause_button->setFixedSize(27, 27);
	m_pause_button->setOpacity(0.5);
	m_pause_button->setToolTip("play");
	connect(m_pause_button, SIGNAL(clicked()), this, SLOT(togglePlay()));

	m_module_button = new AeroButton();
	m_module_button->setHighlight(color_white);
	m_module_button->setText("modules");
	m_module_button->setCheckable(true);
	m_module_button->setRoundness(10);
	m_module_button->setMaximumHeight(27);
	m_module_button->setFixedWidth(60);
	m_module_button->setOpacity(0.5);
	m_module_button->setCheckable(true);
	m_module_button->setChecked(true);

	m_presets_button = new AeroButton();
	m_presets_button->setHighlight(color_white);
	m_presets_button->setText("presets");
	m_presets_button->setCheckable(true);
	m_presets_button->setRoundness(10);
	m_presets_button->setFixedWidth(60);
	m_presets_button->setMaximumHeight(27);
	m_presets_button->setOpacity(0.5);
	m_presets_button->setCheckable(true);

	m_visual_button = new AeroButton();
	m_visual_button->setHighlight(color_white);
	m_visual_button->setText("visual");
	m_visual_button->setCheckable(true);
	m_visual_button->setRoundness(10);
	m_visual_button->setFixedWidth(60);
	m_visual_button->setMaximumHeight(27);
	m_visual_button->setOpacity(0.5);
	m_visual_button->setCheckable(true);

	m_window_buttons = new QButtonGroup();
	m_window_buttons->setExclusive(false);
	m_window_buttons->addButton(m_module_button, 0);
	m_window_buttons->addButton(m_presets_button, 1);
	m_window_buttons->addButton(m_visual_button, 2);
	m_window_buttons->addButton(m_help_button, 3);
	connect(m_window_buttons, SIGNAL(buttonClicked(int)), this, SLOT(toggleMainWindows(int)));

	m_timer_button = new AeroButton();
	m_timer_button->setHighlight(color_green);
	m_timer_button->setIcon(QIcon("images/timer_two.png"));
	m_timer_button->setIconSize(QSize(20, 20));
	m_timer_button->setRoundness(10);
	m_timer_button->setCheckable(true);
	m_timer_button->setFixedSize(27, 27);
	m_timer_button->setOpacity(0.5);
	m_timer_button->setToolTip("start timer");
	connect(m_timer_button, SIGNAL(toggled(bool)), this, SLOT(toggleTimer(bool)));

	m_timer_spin = new QSpinBox();
	m_timer_spin->setRange(1, 1440);
	m_timer_spin->setSuffix(" :min");
	m_timer_spin->setContextMenuPolicy(Qt::NoContextMenu);
	m_timer_spin->hide();

	m_go_button = new AeroButton();
	m_go_button->setHighlight(color_green);
	m_go_button->setText(">>");
	m_go_button->setRoundness(10);
	m_go_button->setFixedSize(27, 27);
	m_go_button->setOpacity(0.5);
	m_go_button->setToolTip("begin");
	connect(m_go_button, SIGNAL(clicked()), this, SLOT(startSoundTimeout()));
	m_go_button->hide();

	m_countdown_label = new QLabel();
	m_countdown_label->hide();
	m_countdown_label->setStyle(new ResoStyle());

	m_master_slider = new QSlider();
	m_master_slider->setOrientation(Qt::Horizontal);
	m_master_slider->setMinimumWidth(145);
	m_master_slider->setRange(0, 100);
	connect(m_master_slider, SIGNAL(valueChanged(int)), this, SIGNAL(masterVolumeChanged(int)));

	QLabel * master_label = new QLabel();
	master_label->setText("master volume ::");
	master_label->setStyle(new ResoStyle());

	QHBoxLayout * play_layout = new QHBoxLayout();
	play_layout->setSpacing(1);
	play_layout->addWidget(m_help_button);
	play_layout->addWidget(m_module_button);
	play_layout->addWidget(m_presets_button);
	play_layout->addWidget(m_visual_button);
	play_layout->addWidget(m_timer_button);
	play_layout->addWidget(m_timer_spin);
	play_layout->addWidget(m_go_button);
	play_layout->addWidget(m_countdown_label);
	play_layout->addStretch(1);
	play_layout->addWidget(master_label);
	play_layout->addWidget(m_master_slider);
	play_layout->addWidget(m_pause_button);

	QVBoxLayout * reso_layout = new QVBoxLayout();
	reso_layout->addStretch(100);
	reso_layout->addLayout(play_layout);

	connect(m_module_button, SIGNAL(clicked()), this, SLOT(toggleModules()));

	setLayout(reso_layout);

	m_presets_widget->hide();

	QSettings settings("resonance", "general");
	restoreGeometry(settings.value("geometry").toByteArray());

	m_bg_filename = settings.value("background", "resonance.png").toString();
	setNewBackground();

	m_bg_filename = "images/backgrounds/" + QString(info.at(0).fileName());
}

void Resonance::setupStyleSheets()
{
	QString menu_ss = "QMenu { color: white; background-color: black; margin: 0px; } " +
		QString("QMenu::item:selected { color: grey; }");

	QString textedit_ss = "QTextEdit { font-size: 13px; font-family: \"Arial\"; color: white; border: 1px solid gray; margin-right: 7px; margin-top: 7px; margin-left: 7px; padding-left: 5px; background: black; selection-color: black; selection-background-color: gray; }";

	QString scrollbar_ss = "QScrollBar:vertical { border: 0px solid grey; background: #000000; width: 14px; margin: 0px 0 0px 0; }" +
		QString("QScrollBar::handle:vertical { border: 1px solid grey; background: black; min-height: 15px; }") +
		QString("QScrollBar::add-line:vertical { border: 1px solid grey; background: #000000; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }") +
		QString("QScrollBar::sub-line:vertical { border: 1px solid grey; background: #000000; height: 10px; subcontrol-position: top; subcontrol-origin: margin; }") +
		QString("QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { border: 0px solid grey; width: 3px; height: 3px; background: black; }") +
		QString("QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: black; }");

	QString h_scrollbar_ss = "QScrollBar:horizontal { border: 0px solid grey; background: #000000; width: 14px; margin: 0px 0 0px 0; }" +
		QString("QScrollBar::handle:horizontal { border: 1px solid grey; background: black; min-height: 15px; }") +
		QString("QScrollBar::add-line:horizontal { border: 1px solid grey; background: #000000; height: 10px; subcontrol-position: bottom; subcontrol-origin: margin; }") +
		QString("QScrollBar::sub-line:horizontal { border: 1px solid grey; background: #000000; height: 10px; subcontrol-position: top; subcontrol-origin: margin; }") +
		QString("QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal { border: 0px solid grey; width: 3px; height: 3px; background: black; }") +
		QString("QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: black; }");

	QString listview_ss = "QListView { color: white; background: black; }" +
		QString("QListView { show-decoration-selected: 1; }") +
		QString("QListView::item:selected { border: 1px solid grey; }") +
		QString("QListView::item:selected:!active { color: white; border: 1px solid grey; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000000, stop: 1 grey); }") +
		QString("QListView::item:selected:active { border: 1px solid grey; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000000, stop: 1 grey); }") +
		QString("QListView::item:hover { border: 1px solid grey; }");

	QString treeview_ss = "QTreeView { color: white; background: black; }" +
		QString("QTreeView { show-decoration-selected: 0; }") +
		QString("QTreeView::item:selected { border: 1px solid grey; }") +
		QString("QTreeView::item:selected:!active { color: white; border: 1px solid grey; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000000, stop: 1 grey); }") +
		QString("QTreeView::item:selected:active { border: 1px solid grey; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #000000, stop: 1 grey); }") +
		QString("QTreeView::item:hover { border: 1px solid grey; }") +
		QString("QTreeView::branch:has-siblings:!adjoins-item { border-image: url(images/vline.png) 0; } QTreeView::branch:has-siblings:adjoins-item { border-image: url(images/branch-more.png) 0; }") +
		QString("QTreeView::branch:!has-children:!has-siblings:adjoins-item { border-image: url(images/branch-end.png) 0; }") +
		QString("QTreeView::branch:has-children:!has-siblings:closed, QTreeView::branch:closed:has-children:has-siblings { border-image: none; image: url(images/branch-closed.png); }") +
		QString("QTreeView::branch:open:has-children:!has-siblings, QTreeView::branch:open:has-children:has-siblings  { border-image: none; image: url(images/branch-open.png); }");

	QString tooltip_ss = "QToolTip { color: white; background: black; border: 1px solid grey; padding: 1px; border-radius: 1px; opacity: 200; }";

	QString check_box_ss = "QCheckBox::indicator { width: 14px; height: 14px; } " +
		QString("QCheckBox::indicator:unchecked { image: url(images/radio_uc.png); } ") +
		QString("QCheckBox::indicator:unchecked:hover { image: url(images/radio_h.png); } ") +
		QString("QCheckBox::indicator:unchecked:pressed { image: url(images/radio_hp.png); } ") +
		QString("QCheckBox::indicator:checked { image: url(images/radio_c.png); } ") +
		QString("QCheckBox::indicator:checked:pressed { image: url(images/radio_hp.png); } ") +
		QString("QCheckBox::indicator:checked:hover { image: url(images/radio_hc.png); } ");

	QString slider_ss = "QSlider::groove:horizontal { border: 0px solid #999999; height: 1px; background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #000000, stop:1 grey); margin-left: 1px; margin-right: 1px; } " +
		QString("QSlider::handle:horizontal { image: url(images/slider.png); width: 15px; margin-top: -9px; margin-bottom: -9px; }");

	QString v_slider_ss = "QSlider::groove:vertical { border: 0px solid #999999; width: 1px; background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 grey, stop:1 #000000); margin-left: 1px; margin-right: 1px; } " +
		QString("QSlider::handle:vertical { image: url(images/v_slider.png); height: 15px; margin-left: -9px; margin-right: -9px; }");

	QString label_ss = "QLabel { color: white; }";

	QString line_edit_ss = "QLineEdit { color: white; border: 1px solid gray; padding: 1 8px; background: black; selection-color: grey; selection-background-color: black; }";

	QString spinbox_ss = "QSpinBox { color: white; padding-right: 15px;  border: 1px solid gray; background: black; selection-color: white; selection-background-color: rgba(25, 25, 25, 255); }" +
		QString("QSpinBox::up-button { subcontrol-origin: border; subcontrol-position: top right;  width: 17px; border-image: url(images/spin_up_new.png) 1; border-width: 1px; }") +
		QString("QSpinBox::down-button { subcontrol-origin: border; subcontrol-position: bottom right; width: 17px; border-image: url(images/spin_down_new.png) 1; border-width: 1px; border-top-width: 0; }") +
		QString("QSpinBox::up-button:hover { border-image: url(images/spin_up_hover.png) 1; } QSpinBox::up-button:pressed { border-image: url(images/spin_up_pressed.png) 1; }") +
		QString("QSpinBox::down-button:hover { border-image: url(images/spin_down_hover.png) 1; } QSpinBox::down-button:pressed { border-image: url(images/spin_down_pressed.png) 1; }");

	QString header_ss = "QHeaderView::section { background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #222222, stop:1.0 #000000); /*qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 grey, stop:1 #000000);*/ color: white; padding-top: 5px; padding-bottom: 5px; padding-left: 5px; border: 0px solid #6c6c6c; }";

	QString app_ss = menu_ss + header_ss + scrollbar_ss + h_scrollbar_ss + slider_ss + listview_ss + treeview_ss + tooltip_ss + check_box_ss + v_slider_ss + label_ss + line_edit_ss + spinbox_ss + textedit_ss;

	setStyleSheet(app_ss);
}

void Resonance::readInPresets()
{
	QDir presets_dir("./presets/factory");
	QFileInfoList presets = presets_dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	m_factory_list->clear();
	for(int p = 0; p < presets.count(); p++)
	{
		QString name = presets.at(p).baseName();
		if(!name.contains("startup"))
		{
			name.prepend(":: ");
			QTreeWidgetItem * item = new QTreeWidgetItem(m_factory_list, QStringList() << name);
			item->setData(0, Qt::UserRole, presets.at(p).absoluteFilePath());
			m_factory_list->addTopLevelItem(item);
		}
	}

	QDir dir("./presets/user");
	presets = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	m_user_list->clear();
	for(int p = 0; p < presets.count(); p++)
	{
		QString name = presets.at(p).baseName();
		name.prepend(":: ");
		QTreeWidgetItem * item = new QTreeWidgetItem(m_user_list, QStringList() << name);
		item->setData(0, Qt::UserRole, presets.at(p).absoluteFilePath());
		m_user_list->addTopLevelItem(item);
	}
}

void Resonance::loadPersonalSound(const QString & file)
{
	QFileInfo file_info(file);
	QString app_path = QDir::currentPath();
	app_path += "/sounds/personal/";
	if(file_info.isFile())
	{
		if(isSoundFile(file))
		{
			saveSoundFile(app_path, file);
		}
	}
	else if(file_info.isDir())
	{
		saveSoundDirectory(app_path, file_info.absoluteFilePath());
	}
}

bool Resonance::isSoundFile(const QString & name)
{
	if(name.endsWith(".wav", Qt::CaseInsensitive) ||
	   name.endsWith(".mp3", Qt::CaseInsensitive) ||
	   name.endsWith(".wma", Qt::CaseInsensitive) ||
	   name.endsWith(".ogg", Qt::CaseInsensitive) ||
	   name.endsWith(".aif", Qt::CaseInsensitive) ||
	   name.endsWith(".wmv", Qt::CaseInsensitive) ||
	   name.endsWith(".tone", Qt::CaseInsensitive) ||
	    name.endsWith(".aiff", Qt::CaseInsensitive))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Resonance::saveSoundFile(const QString & dir, const QString & sound_file)
{
	QString file_path = dir + QFileInfo(sound_file).baseName() + ".rs";
	QFile file(file_path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}
	QTextStream out(&file);
	out << sound_file;
	file.close();
}

void Resonance::saveSoundDirectory(const QString & dir, const QString & sound_dir)
{
	QDir sound_d(sound_dir);
	QString new_dir = dir + sound_d.dirName() + "/";
	bool worked = sound_d.mkdir(new_dir);

	//First save any sound files in the current directory
	QFileInfoList files = sound_d.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
	for(int file = 0; file < files.count(); file++)
	{
		if(isSoundFile(files.at(file).fileName()))
		{
			saveSoundFile(new_dir, files.at(file).absoluteFilePath());
		}
	}

	//Now recursively save any child directories
	QFileInfoList dirs = sound_d.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
	for(int dir = 0; dir < dirs.count(); dir++)
	{
		saveSoundDirectory(new_dir, dirs.at(dir).absoluteFilePath());
	}
}
