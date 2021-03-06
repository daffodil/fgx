#ifndef AIRPORTSWIDGET_H
#define AIRPORTSWIDGET_H

#include <QtCore/QDirIterator>
#include <QtCore/QStringList>


#include <QtGui/QWidget>
#include <QtGui/QButtonGroup>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QProgressBar>
#include <QtGui/QStatusBar>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>


#include <QtGui/QStandardItemModel>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QTreeView>
#include <QtGui/QTreeWidget>

#include "xobjects/mainobject.h"

class AirportsWidget : public QWidget
{
Q_OBJECT
public:
	enum AIPORTS_TREE_COLS{
		CA_CODE = 0,
		CA_NAME = 1
    };
	enum AIRPORT_INFO_TREE_COLS{
		CI_NODE = 0,
		CI_LABEL = 1,
		CI_TYPE = 2,
		CI_SETTING_KEY = 3
	};

	enum STARTUP_POSTITION{
		USE_DEFAULT = 0,
		USE_AIRPORT = 1,
		USE_COORDINATES = 2
	};

	explicit AirportsWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QButtonGroup *buttonGroupUse;

	QGroupBox *groupBoxAirport;
	QGroupBox *groupBoxUseCoordinates;

	QButtonGroup *buttonGroupFilter;
    QLineEdit *txtAirportsFilter;

	QStandardItemModel *model;
	QSortFilterProxyModel *proxyModel;
	QTreeView *treeViewAirports;

	QPushButton *buttonRefreshTree;
	QStatusBar *statusBarAirports;

	QTreeWidget *treeWidgetAirportInfo;
	QStatusBar *statusBarAirportInfo;


	void scan_airports_xml();
	void show_progress(bool state);

	void load_airports_tree();

	void load_info_tree(QString airport_code);
	int load_runways_node(QString airport_code);
	int load_parking_node(QString airport_code);

	QLineEdit *txtLat;
	QLineEdit *txtLng;
	QLineEdit *txtAltitude;
	QLineEdit *txtHeading;
	QLineEdit *txtRoll;
	QLineEdit *txtPitch;
	QLineEdit *txtAirspeed;

	void save_settings();
	void load_settings();
	QString validate();
	QStringList get_args();



signals:
	void set_arg(QString action, QString arg, QString value);

public slots:

	void initialize();

	void on_update_airports_filter();

	void on_airport_tree_selected(QModelIndex currentIdx, QModelIndex previousIdx);

	void on_import_aptdat_clicked();
	void on_rescan_xml_clicked();

	void on_buttonGroupUse();
};

#endif // AIRPORTSWIDGET_H
