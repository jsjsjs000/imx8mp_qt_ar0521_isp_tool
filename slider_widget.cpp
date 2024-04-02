#include "slider_widget.h"
#include "ui_slider_widget.h"

SliderWidget::SliderWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::SliderWidget)
{
	ui->setupUi(this);
}

SliderWidget::SliderWidget(MainWindow *mainWindow, const SliderControl *control,
		void (*onSliderValueChange)(MainWindow *mainWindow, QString type, QString parameter, int value, int divide))
		: QWidget()
		, ui(new Ui::SliderWidget)
{
	ui->setupUi(this);

	this->mainWindow = mainWindow;
	this->type = control->type;
	this->parameter = control->parameter;
	this->min = control->min;
	this->max = control->max;
	this->precision = control->precision;
	this->multiple = control->multiple;
	ui->name->setText(control->name);
	ui->name->setToolTip(control->description);
	ui->value->setText(QString::number(control->value));
	this->onSliderValueChange = onSliderValueChange;
}

SliderWidget::~SliderWidget()
{
	delete ui;
}

void SliderWidget::setRange()
{
	this->ui->horizontalSlider->setMinimum(this->min);
	this->ui->horizontalSlider->setMaximum(this->max);
}

void SliderWidget::setValue(int value)
{
	ui->value->setText(QString::number(value));
	ui->horizontalSlider->setValue(value);
}

void SliderWidget::setValueFloat(float value)
{
	value *= this->multiple;
	ui->value->setText(QString::number((float)value / this->multiple, 'g', this->precision));
	ui->horizontalSlider->setValue(value);
}

void SliderWidget::on_horizontalSlider_valueChanged(int value)
{
	if (this->precision == 0)
		ui->value->setText(QString::number(value));
	else
		ui->value->setText(QString::number((float)value / this->multiple, 'g', this->precision));
	
	if (this->onSliderValueChange)
		(*this->onSliderValueChange)(this->mainWindow, this->type, this->parameter, value, this->multiple);
}
