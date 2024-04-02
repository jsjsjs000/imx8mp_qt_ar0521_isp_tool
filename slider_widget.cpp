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
		void (*onSliderValueChange)(MainWindow *mainWindow, QString &type, QString &parameter, int value, int divide))
{
	this->mainWindow = mainWindow;
	qDebug() << control->type << control->parameter;
	this->type = QString(control->type);
	this->parameter = QString(control->parameter);
	this->precision = control->precision;
	this->multiple = control->multiple;
	this->ui->name->setText(QString(control->name));
	this->ui->name->setToolTip(QString(control->description));
	this->ui->value->setText(QString::number(control->value));
	qDebug() << this->type << this->parameter << this->ui->name->text() << this->ui->name->text() << this->ui->name->toolTip();
	qDebug() << control->min << control->max << control->value << this->ui->horizontalSlider->minimum() << this->ui->horizontalSlider->maximum() << this->ui->horizontalSlider->value();
	this->ui->horizontalSlider->setRange(control->min, control->max);
	// this->ui->horizontalSlider->setPageStep(1);
	// this->ui->horizontalSlider->setSingleStep(1);
	// this->ui->horizontalSlider->setValue(control->value);
	//qDebug() << "a:" << this->ui->horizontalSlider->minimum() << this->ui->horizontalSlider->maximum() << this->ui->horizontalSlider->value();
	// this->onSliderValueChange = onSliderValueChange;
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
