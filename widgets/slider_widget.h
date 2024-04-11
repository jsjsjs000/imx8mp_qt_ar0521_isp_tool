#ifndef SLIDER_WIDGET_H
#define SLIDER_WIDGET_H

#include <QWidget>
#include <controls2.h>
#include "controls_definitions.h"
#include "ui_slider_widget.h"
#include "mainwindow.h"

namespace Ui {
class SliderWidget;
}

class SliderWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SliderWidget(QWidget *parent = nullptr);
	SliderWidget(QWidget *parent, MainWindow *mainWindow, const SliderControl *control,
			void (*onSliderValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide));
	SliderWidget(QWidget *parent, MainWindow *mainWindow, const SliderControl2 *control,
			void (*onSlider2ValueChange)(MainWindow *mainWindow, QString node, int value, int divide));
	void setRange();
	void setValue(int value);
	void setValueFloat(float value);
	~SliderWidget();

private slots:
	void on_horizontalSlider_valueChanged(int value);

private:
	Ui::SliderWidget *ui;
	MainWindow *mainWindow;
	QString getCmd;
	QString setCmd;
	QString parameter;
	QString node;
	int min;
	int max;
	int precision;
	int multiple;
	void (*onSliderValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide) = nullptr;
	void (*onSlider2ValueChange)(MainWindow *mainWindow, QString node, int value, int divide) = nullptr;
};

#endif // SLIDER_WIDGET_H
