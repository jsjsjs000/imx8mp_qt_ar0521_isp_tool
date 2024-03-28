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

void SliderWidget::on_horizontalSlider_valueChanged(int value)
{
	ui->value->setText(QString::number(value));
}
