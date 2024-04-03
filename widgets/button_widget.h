#ifndef BUTTON_WIDGET_H
#define BUTTON_WIDGET_H

#include <QWidget>

#include "controls_definitions.h"
#include "mainwindow.h"

namespace Ui {
class ButtonWidget;
}

class ButtonWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ButtonWidget(QWidget *parent = nullptr);
	ButtonWidget(QWidget *parent, MainWindow *mainWindow, const ButtonControl *control,
			void (*onButtonClicked)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value));
	~ButtonWidget();

private slots:
	void on_pushButton_clicked();

private:
	Ui::ButtonWidget *ui;
	MainWindow *mainWindow;
	QString getCmd;
	QString setCmd;
	QString parameter;
	QString value;
	void (*onButtonClicked)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value);
};

#endif // BUTTON_WIDGET_H
