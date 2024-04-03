#ifndef LABEL_WIDGET_H
#define LABEL_WIDGET_H

#include <QWidget>

#include "controls_definitions.h"
#include "mainwindow.h"

namespace Ui {
class LabelWidget;
}

class LabelWidget : public QWidget
{
	Q_OBJECT

public:
	explicit LabelWidget(QWidget *parent = nullptr);
	LabelWidget(QWidget *parent, MainWindow *mainWindow, const LabelControl *Widget);
	~LabelWidget();

private:
	Ui::LabelWidget *ui;
	MainWindow *mainWindow;
	QString getCmd;
	QString parameter;
	QString value;
	QString label;
};

#endif // LABEL_WIDGET_H
