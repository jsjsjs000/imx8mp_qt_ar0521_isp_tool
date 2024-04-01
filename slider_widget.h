#ifndef SLIDER_WIDGET_H
#define SLIDER_WIDGET_H

#include "controls_definitions.h"
#include "ui_slider_widget.h"
#include "mainwindow.h"
#include <QWidget>

namespace Ui {
class SliderWidget;
}

class SliderWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SliderWidget(QWidget *parent = nullptr);
	void initialize(MainWindow *mainWindow, const SliderControl *control, void (*f)(MainWindow *mainWindow, QString type, QString parameter, int value, int divide));
	~SliderWidget();

private slots:
	void on_horizontalSlider_valueChanged(int value);

private:
	Ui::SliderWidget *ui;
	MainWindow *mainWindow;
	QString type;
	QString parameter;
	int precision;
	int multiple;
	void (*onSliderValueChange)(MainWindow *mainWindow, QString type, QString parameter, int value, int divide);
};

#endif // SLIDER_WIDGET_H
