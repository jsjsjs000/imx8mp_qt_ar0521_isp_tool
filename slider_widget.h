#ifndef SLIDER_WIDGET_H
#define SLIDER_WIDGET_H

#include "controls_definitions.h"
#include "ui_slider_widget.h"
#include <QWidget>

namespace Ui {
class SliderWidget;
}

class SliderWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SliderWidget(QWidget *parent = nullptr);
	void initialize(const SliderControl *control, void (*f)(QString type, QString parameter, int value));
	~SliderWidget();

private slots:
	void on_horizontalSlider_valueChanged(int value);

private:
	Ui::SliderWidget *ui;
	QString type;
	QString parameter;
	void (*onSliderValueChange)(QString type, QString parameter, int value);
};

#endif // SLIDER_WIDGET_H
