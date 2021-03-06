#ifndef LAUNCHERWINDOW_H
#define LAUNCHERWINDOW_H

#include <QtGui/QWidget>

#include <QtGui/QTabWidget>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QPushButton>

#include "panes/coresettingswidget.h"
#include "panes/timeweatherwidget.h"
#include "aircraft/aircraftwidget.h"
#include "airports/airportswidget.h"
#include "network/networkwidget.h"
#include "panes/advancedoptionswidget.h"
#include "panes/outputpreviewwidget.h"

#include "xwidgets/execontrols.h"
#include "xwidgets/xstatusbar.h"

#include "xobjects/mainobject.h"

class LauncherWindow : public QWidget
{
    Q_OBJECT

public:


    LauncherWindow(MainObject *mainOb, QWidget *parent = 0);
    ~LauncherWindow();

    MainObject *mainObject;

	QActionGroup *actionGroupStyle;
    QLabel *headerLabel;
    QTabWidget *tabWidget;

	CoreSettingsWidget *coreSettingsWidget;
	TimeWeatherWidget *timeWeatherWidget;
	AircraftWidget *aircraftWidget;
	AirportsWidget *airportsWidget;
	NetworkWidget *networkWidget;
	AdvancedOptionsWidget *advancedOptionsWidget;
	OutputPreviewWidget *outputPreviewWidget;

	QPushButton *buttonLoadSettings;
	QPushButton *buttonSaveSettings;

	ExeControls *exeFgfs;
	ExeControls *exeTerraSync;
	ExeControls *exeFgCom;

	XStatusBar *statusBar;

    void closeEvent(QCloseEvent *event);

	QString fg_args();


public slots:


	//* FlightGear
	void on_start_fgfs_clicked();

	//* FgCom
	void on_start_fgcom_clicked();

	//* Terrasync
	void on_start_terrasync_clicked();
	void update_pids();

	//* Actions
	void on_about_fgx();
	void on_about_qt();
	void on_quit();
	void on_action_style(QAction *act);
	void on_action_open_url(QAction *act);

	//* Functions
	void load_settings();
	void save_settings();
	void initialize();
	bool validate();

	//* Ouput Buttons
	void on_command_preview();
	void on_command_help();


	//8 Widget Events
	void on_tab_changed(int);
	void on_group_box_terrasync_clicked();
};

#endif // LAUNCHERWINDOW_H
