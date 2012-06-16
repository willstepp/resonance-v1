#ifndef RESONANCE_H
#define RESONANCE_H

#include <QtGui>
#include <QtCore>
#include <QtXml>

class DrawerWidget;
class ResoModule;
class ResoSound;
class FMOD_Sound;
class QTranscluentWidget;
class AeroButton;
class ResoTree;
class QxtFlowView;
class QWebView;

class Resonance : public QWidget
{
	Q_OBJECT

public:
	Resonance(QWidget * parent = 0);
	virtual ~Resonance();

	enum SoundClass	{ Nature, Civilization, Tone, Music, Personal };
	enum Guide { Tutorial, Binaural };

signals:
	void masterVolumeChanged(int volume);

public slots:
	void updateModuleLayout(int module);
	void toggleModules();
	void togglePresets();
	void togglePlay();
	void toggleTimer(bool on);
	void toggleMainWindows(int window);
	void toggleTutorial(int tutorial);

	void startSoundTimeout();
	void stopSoundTimeout();

	void updateCountdownClock(int time_to_go);

	void maximizeWindow(QSystemTrayIcon::ActivationReason reason);

	void loadPreset(QTreeWidgetItem * preset, int column);
	void savePreset(QString preset = "preset");

	void setModulePaused(int module_id, bool paused);

	void removePreset(QTreeWidgetItem * preset);

	void newBackgroundInfo(QString index);
	void setNewBackground();

	void changeHelpMenu(int menu);

	void gotoWebSite();

protected:
	void timerEvent(QTimerEvent * te);
	void paintEvent(QPaintEvent * pe);
	void resizeEvent(QResizeEvent * re);
	void moveEvent(QMoveEvent * me);
	void changeEvent(QEvent * ce);
	void dragEnterEvent(QDragEnterEvent * de);
	void dropEvent(QDropEvent * de);
	void closeEvent(QCloseEvent * ce);

private:
	void setupGUI();
	void setupStyleSheets();

	void readInPresets();
	void loadPersonalSound(const QString & sound);
	bool isSoundFile(const QString & name);

	void saveSoundFile(const QString & dir, const QString & sound_file);
	void saveSoundDirectory(const QString & dir, const QString & sound_dir);

private:
	QLabel * m_bg;
	int m_bg_timer;
	
	int m_stop_sounds_timer;
	int m_countdown_timer_id;
	QTime m_countdown_timer;
	quint64 m_stop_sounds_ms;

	QLabel * m_countdown_label;

	qreal m_bg_opacity;

	QList<QString> m_bg_list;

	QPixmap m_bg_one;
	QPixmap m_bg_two;

	QWidget * m_main_widget;

	DrawerWidget * m_modules_widget;
	DrawerWidget * m_presets_widget;
	DrawerWidget * m_visuals_widget;
	DrawerWidget * m_help_widget;

	QButtonGroup * m_window_buttons;
	AeroButton * m_module_button;
	AeroButton * m_presets_button;
	AeroButton * m_visual_button;

	QList<ResoModule*> m_modules;

	ResoModule * m_module_one;
	ResoModule * m_module_two;
	ResoModule * m_module_three;
	ResoModule * m_module_four;
	ResoModule * m_module_five;

	int m_expanded_module;

	QVBoxLayout * m_module_layout;

	ResoSound * m_reso_sound;
	FMOD_Sound * m_fmod_sound;

	QSystemTrayIcon * m_system_tray_icon;
	QPoint m_current_screen_pos;

	static const int DO_NOT_UPDATE = -1;

	QTranscluentWidget * m_drawer_background;
	QTranscluentWidget * m_presets_background;

	ResoTree * m_user_list;
	ResoTree * m_factory_list;

	QLineEdit * m_preset_line_edit;

	AeroButton * m_pause_button;
	AeroButton * m_help_button;

	AeroButton * m_timer_button;
	QSpinBox * m_timer_spin;
	AeroButton * m_go_button;

	bool m_playing;

	QSlider * m_master_slider;

	QMenu * m_sys_tray_menu;
	QAction * m_sys_tray_action;
	QAction * m_sys_tray_pause;

	QxtFlowView * m_bg_flow;
	QLabel * m_bg_label;
	QString m_bg_filename;
	int m_bg_index;

	AeroButton * m_bg_set_button;

	bool m_default_bg;

	QLabel * m_reso_about_label;

	QWidget * m_about_widget;
	QWidget * m_tutorial_widget;

	QVBoxLayout * m_help_layout;

	QVBoxLayout * m_tutorial_layout;

	QTextEdit * m_guide_widget;

	int m_fade_out_id;
	int m_pre_fade_out_volume;
};

#endif // RESONANCE_H
