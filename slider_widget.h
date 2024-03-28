#ifndef SLIDER_WIDGET_H
#define SLIDER_WIDGET_H

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
	void initialize(QString name, QString description, int min, int max, int value)
	{
		this->ui->name->setText(name);
		this->ui->name->setToolTip(description);
		this->ui->value->setText(QString::number(value));
		this->ui->horizontalSlider->setMinimum(min);
		this->ui->horizontalSlider->setMaximum(max);
		this->ui->horizontalSlider->setValue(value);
	}
	~SliderWidget();

private slots:
	void on_horizontalSlider_valueChanged(int value);

private:
	Ui::SliderWidget *ui;
};

#endif // SLIDER_WIDGET_H
