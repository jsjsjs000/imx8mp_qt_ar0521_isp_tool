#include "slider_widget.h"
#include "ui_slider_widget.h"

SliderWidget::SliderWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::SliderWidget)
{
	ui->setupUi(this);
}

SliderWidget::~SliderWidget()
{
	delete ui;
}

void SliderWidget::initialize(MainWindow *mainWindow, const SliderControl *control,
		void (*onSliderValueChange)(MainWindow *mainWindow, QString type, QString parameter, int value, int divide))
{
	this->mainWindow = mainWindow;
	this->type = control->type;
	this->parameter = control->parameter;
	this->precision = control->precision;
	this->multiple = control->multiple;
	this->ui->name->setText(control->name);
	this->ui->name->setToolTip(control->description);
	this->ui->value->setText(QString::number(control->value));
	this->ui->horizontalSlider->setMinimum(control->min);
	this->ui->horizontalSlider->setMaximum(control->max);
	this->ui->horizontalSlider->setValue(control->value);
	this->onSliderValueChange = onSliderValueChange;
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
