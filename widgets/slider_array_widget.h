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
			void (*onSliderArrayValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<float> values));
	SliderArrayWidget(QWidget *parent, MainWindow *mainWindow, const SliderArrayControl2 *control,
			void (*onSliderArray2ValueChange)(MainWindow *mainWindow, QString node, QList<float> values));
	void setRange();
	// QList<float>* getValues();
	void setValues(QList<int> values);
	void setValuesFloats(QList<float> values);
	void setDefaultAndFactoryValue(QList<float> defaultValue, QList<float> factoryValue);
	~SliderArrayWidget();

private slots:
	void onHorizontalSliderValueChanged(int value);
	void actionResetDefaultSlot();
	void actionResetFactorySlot();

private:
	Ui::SliderArrayWidget *ui;
	QList<QSlider*> horizontalSlider;
	QList<QLabel*> labels;
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
	// QList<float> defaultValue;
	// QList<float> factoryValue;
	void (*onSliderArrayValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<float> values) = nullptr;
	void (*onSliderArray2ValueChange)(MainWindow *mainWindow, QString node, QList<float> values) = nullptr;
	void slotCustomMenuRequested(QPoint pos);
};

#endif // SLIDER_ARRAY_WIDGET_H
