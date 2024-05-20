#ifndef SLIDER_ARRAY_WIDGET_H
#define SLIDER_ARRAY_WIDGET_H

#include <QWidget>
#include <QSlider>
#include <controls2.h>
#include "controls_definitions.h"
#include "ui_slider_array_widget.h"
#include "mainwindow.h"

namespace Ui {
class SliderArrayWidget;
}

class SliderArrayWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SliderArrayWidget(QWidget *parent = nullptr);
	SliderArrayWidget(QWidget *parent, MainWindow *mainWindow, const SliderArrayControl *control,
			void (*onSliderArrayValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide));
	SliderArrayWidget(QWidget *parent, MainWindow *mainWindow, const SliderArrayControl2 *control,
			void (*onSliderArray2ValueChange)(MainWindow *mainWindow, QString node, int value, int divide));
	void setRange();
	int getValue();
	void setValue(int value);
	void setValueFloat(float value);
	void setDefaultAndFactoryValue(int defaultValue, int factoryValue);
	void setDefaultAndFactoryValueFloat(float defaultValue, float factoryValue);
	~SliderArrayWidget();

private slots:
	void on_horizontalSlider_valueChanged(int value);
	void actionResetDefaultSlot();
	void actionResetFactorySlot();

private:
	Ui::SliderArrayWidget *ui;
	QList<QSlider> horizontalSlider;
	MainWindow *mainWindow;
	QString getCmd;
	QString setCmd;
	QString parameter;
	QString node;
	QList<int> min;
	QList<int> max;
	QList<int> precision;
	QList<int> multiple;
	bool isSetDefaultAndFactoryValue = false;
	QList<int> defaultValue;
	QList<int> factoryValue;
	void (*onSliderArrayValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide) = nullptr;
	void (*onSliderArray2ValueChange)(MainWindow *mainWindow, QString node, int value, int divide) = nullptr;
	void slotCustomMenuRequested(QPoint pos);
};

#endif // SLIDER_ARRAY_WIDGET_H
