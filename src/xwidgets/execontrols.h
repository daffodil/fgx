#ifndef EXECONTROLS_H
#define EXECONTROLS_H

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <QtGui/QWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QStatusBar>

class ExeControls : public QGroupBox
{
Q_OBJECT
public:
	explicit ExeControls(QString title, QString exeNam, QWidget *parent = 0);

	QString exe_name; //* This is the exe name we find the "pidof foo"
	QStatusBar *statusBar;
	QPushButton *buttonStart;
	QPushButton *buttonStop;
	QPushButton *buttonRefresh;

	//** Start is called from fgx
	void start(QString command_line);
	int get_pid();
	void update_pid();
	void kill_pid();


signals:

public slots:
	//* on_start() is connected in fgx
	void on_stop_clicked();
	void on_refresh_clicked();


};

#endif // EXECONTROLS_H
