#ifndef OUTPUTPREVIEWWIDGET_H
#define OUTPUTPREVIEWWIDGET_H

#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

#include "xobjects/mainobject.h"

class OutputPreviewWidget : public QWidget
{
Q_OBJECT
public:
	explicit OutputPreviewWidget(MainObject *mOb, QWidget *parent = 0);

	MainObject *mainObject;

	QPlainTextEdit *txtPreviewOutput;
	QPushButton *buttonCommandPreview;
	QPushButton *buttonCommandHelp;
signals:

public slots:

};

#endif // OUTPUTPREVIEWWIDGET_H
