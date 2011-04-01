/*
 *  fgx.cpp
 *  fgx
 *
 *  Created by Yves Sablonier, Zurich
 *  Copyright 2011 --- GPL2
 *
 */


#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <iostream>
#include <QTextStream>

#include <QXmlStreamReader>
#include <QSettings>
#include <QAbstractSocket>
#include <QHostInfo>
#include <QTimer>


#include <QtGui/QCloseEvent>
#include <QStyleFactory>

#include <QFileDialog>
//#include <QProgressDialog>


#include "fgx.h"
#include "settings/settingsdialog.h"


fgx::fgx(QMainWindow *parent) : QMainWindow(parent){

	setupUi(this);


	setProperty("settings_namespace", QVariant("launcher_window"));
	settings.restoreWindow(this);

	setWindowTitle(QCoreApplication::applicationName().append(" - ").append(QCoreApplication::applicationVersion()));
	fgx_logo->setText(QCoreApplication::applicationName());


	//** Setup Styles
	QApplication::setStyle( QStyleFactory::create(settings.value("gui_style","Cleanlooks").toString()) );
	actionGroupStyle = new QActionGroup(this);
	actionGroupStyle->setExclusive(true);
	//connect(actionGroupStyle, SIGNAL(triggered(QAction*)), this, SLOT(on_style(QAction*)) );
	QStringList styles =  QStyleFactory::keys();
	for(int idx=0; idx < styles.count(); idx++){
		QString sty = QString(styles.at(idx));
		QAction *act = menuStyle->addAction( sty );
		actionGroupStyle->addAction( act );
		act->setCheckable(true);
		if(QApplication::style()->objectName() == sty.toLower()){
			act->setChecked(true);
		}
	}


	//** Initialse Extra Widgets
	aircraftWidget = new AircraftWidget(this);
	tabs->insertTab(2, aircraftWidget, "Aircraft");

	airportsWidget = new AirportsWidget(this);
	tabs->insertTab(3, airportsWidget, "Airports");

	networkWidget = new NetworkWidget(this);
	tabs->insertTab(4, networkWidget, "Network");



	//** Restore Settings
	tabs->setCurrentIndex( settings.value("last_tab").toInt() );

	//centralWidget()->setDisabled(true);

	//***** Qt Has no Show event for a form, so we need to present Widgets first
	//** and then initialise. THis is achieved with a timer that triggers in a moment
	QTimer::singleShot(500, this, SLOT(initialize()));

}/* end constructor */

fgx::~fgx(){
}


//=======================================================================================================================
// Initial Setup
//=======================================================================================================================
void fgx::initialize(){


	load_settings();
	aircraftWidget->initialize();
	airportsWidget->initialize();

	if(!settings.paths_sane()){
		show_settings_dialog();
	}
	centralWidget()->setDisabled(false);


}
//=======================================================================================================================
// Process Related
//=======================================================================================================================
void fgx::kill_process(QString pid) {

	QString command("kill ");
	command.append(pid);
	QProcess::startDetached(command);
}

//=======================================================================================================================
// Start FlightGear
//=======================================================================================================================
void fgx::on_buttonStartFg_clicked() {
	txtPreview->setPlainText(fg_args().join("\n"));

	//** This process will always start on the shell as fgfs returns an error help if incorrect args
	bool start = QProcess::startDetached( settings.fgfs_path(), fg_args(), QString(), &pid_fg);
	Q_UNUSED(start);

}


//=======================================================================================================================
// Stop FlightGear
//=======================================================================================================================
void fgx::on_buttonStopFg_clicked() {
	kill_process(QString::number(pid_fg));
}


//=======================================================================================================================
// Start FGCom
//=======================================================================================================================
void fgx::start_fgcom(){

	QString command("nice");

	QStringList args;
	args << "fgcom" << networkWidget->txtFgComNo->text() << "-p" << networkWidget->txtFgComPort->text();

	QProcess::startDetached(command, args, QString(), &pid_fgcom);
}
//=======================================================================================================================
// Stop FGCom
//=======================================================================================================================
void fgx::stop_fgcom() {
	kill_process(QString::number(pid_fgcom));
}




//=======================================================================================================================
// Start Terrasync
void fgx::on_buttonStartTerraSync_clicked(){
	start_terrasync();
}

void fgx::start_terrasync(){

	QString command("nice");
	QStringList args;
	args << "terrasync" << "-p" << "5505" << "-S" << "-d" << "settings";

	int start = QProcess::startDetached(command, args, QString(), &pid_terra);
	Q_UNUSED(start);
}


//=======================================================================================================================
// Stop TerraSync
void fgx::on_buttonStopTerraSync_clicked(){
	stop_terrasync();
}

void fgx::stop_terrasync() {

	/* This is a really nasty hack cos I dont know what Im doing!! (said pete)
	   Gets a list of processes, and find terrasync arg
	   Then kills it - is there a better way ?
	*/
	//** Get a list of ALL process
	QStringList args;
	args << "-ef";
	QProcess process;
	process.start("ps", args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		//QString error_result = process.readAllStandardError(); Unused atmo

		//* take result and split into parts
		QStringList entries = ok_result.split("\n");
		for(int i=0; i < entries.count(); i++){
			if(entries.at(i).contains("terrasync -p 5505")){
				//* found a terrasync  so murder it
				QStringList parts = entries.at(i).split(" ", QString::SkipEmptyParts);
				QStringList killargs;
				killargs << "-9" << parts.at(1);
				int start = QProcess::startDetached("kill", killargs);
				Q_UNUSED(start);
			}
		}
	}
}



//=======================================================================================================================
//* Validate
//=======================================================================================================================
bool fgx::validate(){
	int TIMEOUT = 5000;
	QString v;

	v = aircraftWidget->validate();
	if(v != ""){
		tabs->setCurrentIndex( tabs->indexOf(aircraftWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	v = airportsWidget->validate();
	if(v != ""){
		tabs->setCurrentIndex( tabs->indexOf(airportsWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	v = networkWidget->validate();
	if(v != ""){
		tabs->setCurrentIndex( tabs->indexOf(networkWidget));
		statusBar()->showMessage(v, TIMEOUT);
		return false;
	}

	if(groupBoxTerraSync->isChecked() and txtTerraSyncPath->text().length() == 0){
		tabs->setCurrentIndex(0);
		txtTerraSyncPath->setFocus();
		statusBar()->showMessage("Need a Terrasync directory", TIMEOUT);
		return false;
	}
	return true;
}

//=======================================================================================================================
// FlightGear Command Args
//=======================================================================================================================
QStringList fgx::fg_args(){

	QString argtime;
	argtime.append(year->text());
	argtime.append(":");
	argtime.append(month->text());
	argtime.append(":");
	argtime.append(day->text());
	argtime.append(":");
	argtime.append(hour->text());
	argtime.append(":");
	argtime.append(minute->text());
	argtime.append(":");
	argtime.append(second->text());
	

	//++ WARNING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//** Please make sure there are no spaces in the args.
	//** Any args with spaces are quoted eg ' --foo=bar'
	//-- end Warning <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	QStringList args;

	//** fg_root
	args << QString("--fg-root=").append(settings.fg_root());


	//** Startup , Spalsh, Geometry
	args << QString("--geometry=").append(comboScreenSize->currentText());
	if (checkboxDisableSplash->isChecked()) {
		args << QString("--disable-splash-screen");
	}
	if (checkboxFullScreen->isChecked()) {
		args << QString("--enable-fullscreen");
	}

	//** Controls
	args << QString(checkBoxEnableAutoCordination->isChecked() ? "--enable-auto-coordination" : "--disable-auto-coordination");


	//** Terrasync
	if (groupBoxTerraSync->isChecked()) {
		args << QString("--atlas=socket,out,5,localhost,5505,udp");
	}

	//** Scenery
	args << QString("--fg-scenery=%1").arg(settings.scenery_path());



	//** Time Of Day
	if (groupBoxSetTime->isChecked()) {
		//content.append("--start-date-lat=");
		//content.append(argtime);
	} else {
		args << QString("--timeofday=").append( buttonGroupTime->checkedButton()->text().toLower().replace(" ","") );
	}


	//* Weather/Metar fetch
	if(radioButtonWeatherNone->isChecked()) {
		args << QString("--disable-real-weather-fetch");

	}else if(radioButtonWeatherLive->isChecked()) {
		args << QString("--enable-real-weather-fetch");

	}else if(radioButtonWeatherMetar->isChecked()){
		args << QString("--metar=").append("\"").append(metarText->toPlainText()).append("\"");
	}


	//** Aircraft
	args << aircraftWidget->get_args();

	//** Airport, Runway Start pos
	args << airportsWidget->get_args();

	//** Network
	args << networkWidget->get_args();

	//* Ai Traffic TODO
	/*
	if (enableAITraffic->isChecked()) {
		args << QString("--enable-ai-traffic");
	}else{
		args << QString("--disable-ai-traffic");
	}
	*/
	//** Enable AI models ???
	args << QString("--enable-ai-models");

	args.sort();

	//*  Additonal args
	QString extra = lineEditExtraArgs->toPlainText().trimmed();
	if (extra.length() > 0) {
		QStringList parts = extra.split("\n");
		if(parts.count() > 0){
			for(int i=0; i < parts.count(); i++){
				QString part = parts.at(i).trimmed();
				if(part.length() > 0){
					args << part;
				}
			}
		}
	}

	//* Log Level - Redirect stdout and stderr to logfile
	if(checkBoxLogEnabled->isChecked()){
		//
		QString argwritelogdir = QDir::currentPath();
		QString argwritelog;
		argwritelog.append(argwritelogdir);
		argwritelog = " &> fgfslog.log";

		args << QString("--log-level=").append( buttonGroupLog->button(buttonGroupLog->checkedId())->text().toLower() );
		args << QString(argwritelog);
	}

	return args;
}


//================================================================================
// Save Settings
//================================================================================
void fgx::save_settings()
{
	//## NB: fgfs path and FG_ROOT are saves in SettingsDialog ##
	//qDebug() << "saves settings";
	aircraftWidget->save_settings();
	airportsWidget->save_settings();
	networkWidget->save_settings();


	settings.setValue("use_terrasync", groupBoxTerraSync->isChecked());
	settings.setValue("terrasync_path", txtTerraSyncPath->text());


	settings.setValue("screen_size", comboScreenSize->currentText());
	settings.setValue("screen_full", checkboxFullScreen->isChecked());
	settings.setValue("screen_splash", checkboxDisableSplash->isChecked());

	
	settings.setValue("year", year->text());
	settings.setValue("month", month->text());
	settings.setValue("day", day->text());
	settings.setValue("hour", hour->text());
	settings.setValue("minute", minute->text());
	settings.setValue("second", second->text());
	
	//* Time
	settings.setValue("timeofday", buttonGroupTime->checkedButton()->text());
	settings.setValue("set_time", groupBoxSetTime->isChecked());

	
	//* Weather
	settings.setValue("weather", buttonGroupWeather->checkedButton()->text());
	settings.setValue("metar", metarText->toPlainText());
	
	//* Advanced
	settings.setValue("extra_args", lineEditExtraArgs->toPlainText());
	settings.setValue("log_enabled", checkBoxLogEnabled->isChecked());
	settings.setValue("log_level", buttonGroupLog->checkedButton()->text());
	qDebug() << buttonGroupLog->checkedButton()->text();
	
	
}

//================================================================================
// Load Settings
//================================================================================
void fgx::load_settings()
{

	fgdataPath->setText(settings.fg_root());
	fgfsPath->setText(settings.fgfs_path());

	aircraftWidget->load_settings();
	airportsWidget->load_settings();
	networkWidget->load_settings();
	
	groupBoxTerraSync->setChecked(settings.value("use_terrasync").toBool());
	txtTerraSyncPath->setText( settings.value("terrasync_path").toString() );

	//** Sartup sxreens	
	comboScreenSize->setCurrentIndex( comboScreenSize->findText(settings.value("screen_size").toString()) );
	checkboxFullScreen->setChecked(settings.value("screen_full").toBool());
	checkboxDisableSplash->setChecked(settings.value("screen_splash").toBool());

	

		



	//** Time Of Day
	bool setTime = settings.value("set_time").toBool();
	groupBoxSetTime->setChecked(setTime);

	QString yearSet = settings.value("year").toString();
	year->setText(yearSet);
	QString monthSet = settings.value("month").toString();
	month->setText(monthSet);
	QString daySet = settings.value("day").toString();
	day->setText(daySet);
	QString hourSet = settings.value("hour").toString();
	hour->setText(hourSet);
	QString minuteSet = settings.value("minute").toString();
	minute->setText(minuteSet);
	QString secondSet = settings.value("second").toString();
	second->setText(secondSet);

	QString tod = settings.value("timeofday", "Real Time").toString();
	QList<QAbstractButton *> todButtons = buttonGroupTime->buttons();
	for (int i = 0; i < todButtons.size(); ++i) {
		if(todButtons.at(i)->text() == tod){
			todButtons.at(i)->setChecked(true);
		}
		todButtons.at(i)->setEnabled(!setTime);
	 }




	//** Weather
	QString weather = settings.value("weather").toString();
	if(weather == radioButtonWeatherMetar->text()){
		radioButtonWeatherMetar->setChecked(true);

	}else if(weather == radioButtonWeatherLive->text()){
		radioButtonWeatherLive->setChecked(true);

	}else{
		radioButtonWeatherNone->setChecked(true);
	}
	metarText->setPlainText(settings.value("metar").toString());
	metarText->setEnabled(weather == radioButtonWeatherMetar->text());
	

		
	lineEditExtraArgs->setPlainText(settings.value("extra_args").toString());

	checkBoxLogEnabled->setChecked(settings.value("log_enabled").toBool());
	radioButtonLogWarn->setChecked(true);
	QString log_level = settings.value("log_level").toString();
	QList<QAbstractButton *> logButtons = buttonGroupLog->buttons();
	for (int i = 0; i < logButtons.size(); ++i) {
		if(logButtons.at(i)->text() == log_level){
			logButtons.at(i)->setChecked(true);
			break;
		}
	 }

}

//===============================================================
// Terrasync
//===============================================================
//** Path buttons Clicked
void fgx::on_buttonTerraSyncPath_clicked(){

	QString filePath = QFileDialog::getExistingDirectory(this,
														 tr("Select Terrasync Directory"),
														 txtTerraSyncPath->text(),
														 QFileDialog::ShowDirsOnly);
	if(filePath.length() > 0){
		txtTerraSyncPath->setText(filePath);
	}

	//* Need to write out the terrasync dir as its used other places ;-(
	settings.setValue("terrasync_path", txtTerraSyncPath->text());
	settings.sync();
}

//** Group Box Checked
void fgx::on_groupBoxTerraSync_clicked(){
	settings.setValue("use_terrasync", groupBoxTerraSync->isChecked());
	settings.sync();
}

//===============================================================
// Misc Events
//===============================================================
void fgx::on_style(QAction *action){
	settings.setValue("gui_style", action->text());
	QApplication::setStyle(QStyleFactory::create(action->text()));
}

void fgx::on_actionAboutFGX_triggered(){
	QString txt;
	txt.append("<html><body><p><b>Launcher for OSX</b></p>");
	txt.append("<p>© 2011 Gral aka Yves Sablonier, Zurich</p>");
	txt.append("<p><a href='http://www.gnu.org/licenses/gpl-2.0.txt'>GPL2</a></p>");
	txt.append("<p><a href='http://wiki.flightgear.org'>FlightGear</a></p>");
	txt.append("</body></html>");
	QMessageBox::about(this, "About FGX", txt);
}

void fgx::on_actionAboutQT_triggered(){
	QMessageBox::aboutQt(this, "About Qt");
}

// Set Time checked
void fgx::on_groupBoxSetTime_clicked() {
	
	bool enabled  = groupBoxSetTime->isChecked();
	year->setEnabled(enabled);
	month->setEnabled(enabled);
	day->setEnabled(enabled);
	hour->setEnabled(enabled);
	minute->setEnabled(enabled);
	second->setEnabled(enabled);

	QList<QAbstractButton *> buttons = buttonGroupTime->buttons();
	for(int i=0; i < buttons.count(); i++){
		QAbstractButton *butt = buttons.at(i);
		butt->setEnabled(!enabled);
	}
}

// Metar checked
void fgx::on_buttonGroupWeather_buttonClicked(int id) {
	Q_UNUSED(id);
	qDebug() << id;
	metarText->setEnabled(radioButtonWeatherMetar->isChecked());
	if(radioButtonWeatherMetar->isChecked()){
		metarText->setFocus();
	}
}



//===============================================================
// Write settings on close
void fgx::closeEvent(QCloseEvent *event)
{
	save_settings();
	settings.saveWindow(this);
	settings.sync();
	event->accept();
}

//===============================================================
// Settings Dialog
void fgx::show_settings_dialog(){
	SettingsDialog *settingsDialog = new SettingsDialog();
	if(settingsDialog->exec()){
		fgfsPath->setText(settings.fgfs_path());
		fgdataPath->setText(settings.fg_root());
	}
}
void fgx::on_buttonFgFsPath_clicked(){
	show_settings_dialog();
}
void fgx::on_buttonFgRootPath_clicked(){
	show_settings_dialog();
}





void fgx::on_tabs_currentChanged(int index){
	settings.setValue("last_tab", index);
	if(index == 6){
		on_buttonViewCommand_clicked();
	}
}

//==============================================
//** View Buttons
void fgx::on_buttonViewCommand_clicked(){
	if(!validate()){
		return;
	}
	QString str = QString(settings.fgfs_path()).append(" \\\n");
	str.append( fg_args().join(" \\\n"));
	txtPreview->setPlainText(str);
}

void fgx::on_buttonViewHelp_clicked(){
	QProcess process;
	QStringList args;
	args << "-h" << "-v" << QString("--fg-root=").append(settings.fg_root());
	process.start(settings.fgfs_path(), args, QIODevice::ReadOnly);
	if(process.waitForStarted()){
		process.waitForFinished();
		QString ok_result = process.readAllStandardOutput();
		QString error_result = process.readAllStandardError();
		txtPreview->setPlainText(ok_result);
	}
}




void fgx::on_buttonLoadSettings_clicked(){
	load_settings();
	qDebug() << fg_args().join("\n");
}
void fgx::on_buttonSaveSettings_clicked(){
	save_settings();
	qDebug() << fg_args().join("\n");
}
