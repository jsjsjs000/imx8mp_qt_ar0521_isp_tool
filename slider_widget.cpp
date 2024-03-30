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

void SliderWidget::initialize(const SliderControl *control,
		void (*onSliderValueChange)(QString type, QString parameter, int value))
{
	this->type = control->type;
	this->parameter = control->parameter;
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
	ui->value->setText(QString::number(value));
	
	if (this->onSliderValueChange)
		(*this->onSliderValueChange)(this->type, this->parameter, value);
}
