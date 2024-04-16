#include "slider_widget.h"
#include "ui_slider_widget.h"

#include <QMenu>

SliderWidget::SliderWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::SliderWidget)
{
	ui->setupUi(this);
}

SliderWidget::SliderWidget(QWidget *parent, MainWindow *mainWindow, const SliderControl *control,
		void (*onSliderValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide))
		: QWidget(parent)
		, ui(new Ui::SliderWidget)
{
	ui->setupUi(this);

	if (!control->readonly)
	{
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &SliderWidget::customContextMenuRequested, this, &SliderWidget::slotCustomMenuRequested);
	}

	this->mainWindow = mainWindow;
	this->getCmd = control->getCmd;
	this->setCmd = control->setCmd;
	this->parameter = control->parameter;
	this->min = control->min;
	this->max = control->max;
	this->precision = control->precision;
	this->multiple = control->multiple;
	this->defaultValue = control->value;
	ui->name->setText(control->name);
	ui->name->setToolTip(control->description);
	ui->value->setText(QString::number(control->value));
	ui->horizontalSlider->setEnabled(!control->readonly);
	this->setRange();
	this->setValue(control->value);
	this->onSliderValueChange = onSliderValueChange;
}

SliderWidget::SliderWidget(QWidget *parent, MainWindow *mainWindow, const SliderControl2 *control,
		void (*onSlider2ValueChange)(MainWindow *mainWindow, QString node, int value, int divide))
		: QWidget(parent)
		, ui(new Ui::SliderWidget)
{
	ui->setupUi(this);

	if (!control->readonly)
	{
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &SliderWidget::customContextMenuRequested, this, &SliderWidget::slotCustomMenuRequested);
	}

	this->mainWindow = mainWindow;
	this->node = control->node;
	this->min = control->min;
	this->max = control->max;
	this->precision = control->precision;
	this->multiple = control->multiple;
	this->defaultValue = control->value;
	ui->name->setText(control->name);
	ui->name->setToolTip(control->description);
	ui->value->setText(QString::number(control->value));
	ui->horizontalSlider->setEnabled(!control->readonly);
	this->setRange();
	this->setValue(control->value);
	this->onSlider2ValueChange = onSlider2ValueChange;
}

SliderWidget::~SliderWidget()
{
	delete ui;
}

void SliderWidget::slotCustomMenuRequested(QPoint pos)
{
	QMenu *menu = new QMenu(this);
	QAction *actionReset = new QAction("Reset to program start", this);
	connect(actionReset, SIGNAL(triggered()), this, SLOT(actionResetDefaultSlot()));
	menu->addAction(actionReset);
	QAction *actionFactory = new QAction("Reset to factory default", this);
	connect(actionFactory, SIGNAL(triggered()), this, SLOT(actionResetFactorySlot()));
	menu->addAction(actionFactory);
	menu->popup(this->mapToGlobal(pos));
}

void SliderWidget::actionResetDefaultSlot()
{
	this->setValue(this->defaultValue);
}

void SliderWidget::actionResetFactorySlot()
{
	this->setValue(this->factoryValue);
}

void SliderWidget::setRange()
{
	this->ui->horizontalSlider->setMinimum(this->min);
	this->ui->horizontalSlider->setMaximum(this->max);
}

void SliderWidget::setDefaultAndFactoryValue(int defaultValue, int factoryValue)
{
	this->defaultValue = defaultValue;
	this->factoryValue = factoryValue;
}

void SliderWidget::setDefaultAndFactoryValueFloat(float defaultValue, float factoryValue)
{
	this->defaultValue = defaultValue * this->multiple;
	this->factoryValue = factoryValue * this->multiple;
}

void SliderWidget::setValue(int value)
{
	if (ui->horizontalSlider->value() != value)
	{
		ui->value->setText(QString::number(value));
		ui->horizontalSlider->setValue(value);
	}
}

void SliderWidget::setValueFloat(float value)
{
	value *= this->multiple;
	ui->value->setText(QString::number((float)value / this->multiple, 'f', this->precision));
	ui->horizontalSlider->setValue((int)value);
}

int SliderWidget::getValue()
{
	return ui->horizontalSlider->value();
}

void SliderWidget::on_horizontalSlider_valueChanged(int value)
{
	if (this->precision == 0)
		ui->value->setText(QString::number(value));
	else
		ui->value->setText(QString::number((float)value / this->multiple, 'f', this->precision));

	if (this->onSliderValueChange != nullptr)
		(*this->onSliderValueChange)(this->mainWindow, this->getCmd, this->setCmd, this->parameter, value, this->multiple);
	 if (this->onSlider2ValueChange != nullptr)
		(*this->onSlider2ValueChange)(this->mainWindow, this->node, value, this->multiple);
}
