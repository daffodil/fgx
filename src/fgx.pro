# #####################################################################
# Automatically generated by qmake (2.01a) Do. Mrz 3 00:45:52 2011
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT += network \
    xml \
    xmlpatterns \
    sql

# Input
HEADERS +=  xobjects/xsettings.h \
    settings/settingsdialog.h \
    network/networkwidget.h \
    network/mptelnet.h \
    aircraft/aircraftwidget.h \
    aircraft/aerotools.h \
    airports/airportswidget.h \
    airports/importairportswidget.h \
    airports/aptdatparser.h \
    xwidgets/execontrols.h \
    panes/timeweatherwidget.h \
    panes/advancedoptionswidget.h \
    panes/outputpreviewwidget.h \
    panes/coresettingswidget.h \
    xwidgets/xgroupboxes.h \
    xobjects/mainobject.h \
    launcher/launcherwindow.h
SOURCES += main.cpp \
    xobjects/xsettings.cpp \
    settings/settingsdialog.cpp \
    network/networkwidget.cpp \
    network/mptelnet.cpp \
    aircraft/aircraftwidget.cpp \
    aircraft/aerotools.cpp \
    airports/airportswidget.cpp \
    airports/aptdatparser.cpp \
    airports/importairportswidget.cpp \
    xwidgets/execontrols.cpp \
    panes/timeweatherwidget.cpp \
    panes/advancedoptionswidget.cpp \
    panes/outputpreviewwidget.cpp \
    panes/coresettingswidget.cpp \
    xwidgets/xgroupboxes.cpp \
    xobjects/mainobject.cpp \
    launcher/launcherwindow.cpp
RESOURCES += resources/icons.qrc \
    resources/artwork.qrc
