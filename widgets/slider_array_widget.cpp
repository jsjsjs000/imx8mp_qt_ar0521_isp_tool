#include "slider_array_widget.h"
#include "ui_slider_array_widget.h"

#include <QMenu>
#include <QSlider>

SliderArrayWidget::SliderArrayWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::SliderArrayWidget)
{
	ui->setupUi(this);
}

SliderArrayWidget::SliderArrayWidget(QWidget *parent, MainWindow *mainWindow, const SliderArrayControl *control,
		void (*onSliderArrayValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide))
		: QWidget(parent)
		, ui(new Ui::SliderArrayWidget)
{
	ui->setupUi(this);

	if (!control->readonly)
	{
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &SliderArrayWidget::customContextMenuRequested, this, &SliderArrayWidget::slotCustomMenuRequested);
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
	//$$ui->value->setText(QString::number(control->value));
	for (int i = 0; i < control->min.count(); i++)
	{
		QSlider *slider = new QSlider(Qt::Orientation::Horizontal, this);
		ui->horizontalLayout2->addWidget(slider);
		slider->setEnabled(!control->readonly);
	}
	this->setRange();
	//$$this->setValue(control->value);
	this->onSliderArrayValueChange = onSliderArrayValueChange;
}

SliderArrayWidget::SliderArrayWidget(QWidget *parent, MainWindow *mainWindow, const SliderArrayControl2 *control,
		void (*onSliderArray2ValueChange)(MainWindow *mainWindow, QString node, int value, int divide))
		: QWidget(parent)
		, ui(new Ui::SliderArrayWidget)
{
	ui->setupUi(this);

	if (!control->readonly)
	{
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &SliderArrayWidget::customContextMenuRequested, this, &SliderArrayWidget::slotCustomMenuRequested);
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
	//$$ui->value->setText(QString::number(control->value));
	for (int i = 0; i < control->min.count(); i++)
	{
		QSlider *slider = new QSlider(Qt::Orientation::Horizontal, this);
		ui->horizontalLayout2->addWidget(slider);
		slider->setEnabled(!control->readonly);
	}
	this->setRange();
	//$$this->setValue(control->value);
	this->onSliderArray2ValueChange = onSliderArray2ValueChange;
}

SliderArrayWidget::~SliderArrayWidget()
{
	delete ui;
}

void SliderArrayWidget::slotCustomMenuRequested(QPoint pos)
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

void SliderArrayWidget::actionResetDefaultSlot()
{
	//$$this->setValue(this->defaultValue);
}

void SliderArrayWidget::actionResetFactorySlot()
{
	//$$this->setValue(this->factoryValue);
}

void SliderArrayWidget::setRange()
{
	//$$ this->ui->horizontalSlider->setMinimum(this->min);
	//$$ this->ui->horizontalSlider->setMaximum(this->max);
}

void SliderArrayWidget::setDefaultAndFactoryValue(int defaultValue, int factoryValue)
{
	//$$this->defaultValue = defaultValue;
	//$$this->factoryValue = factoryValue;
}

void SliderArrayWidget::setDefaultAndFactoryValueFloat(float defaultValue, float factoryValue)
{
	//$$this->defaultValue = defaultValue * this->multiple;
	//$$this->factoryValue = factoryValue * this->multiple;
}

void SliderArrayWidget::setValue(int value)
{
	//$$ if (ui->horizontalSlider->value() != value)
	// {
	// 	ui->value->setText(QString::number(value));
	// 	ui->horizontalSlider->setValue(value);
	// }
}

void SliderArrayWidget::setValueFloat(float value)
{
	//$$value *= this->multiple;
	//$$ui->value->setText(QString::number((float)value / this->multiple, 'f', this->precision));
	//$$ ui->horizontalSlider->setValue((int)value);
}

int SliderArrayWidget::getValue()
{
	return 1; //$$ ui->horizontalSlider->value();
}

void SliderArrayWidget::on_horizontalSlider_valueChanged(int value)
{
	//$$if (this->precision == 0)
		//$$ui->value->setText(QString::number(value));
	//$$else
		//$$ui->value->setText(QString::number((float)value / this->multiple, 'f', this->precision));

	//$$if (this->onSliderArrayValueChange != nullptr)
	// 	(*this->onSliderArrayValueChange)(this->mainWindow, this->getCmd, this->setCmd, this->parameter, value, this->multiple);
	// if (this->onSliderArray2ValueChange != nullptr)
	// 	(*this->onSliderArray2ValueChange)(this->mainWindow, this->node, value, this->multiple);
}
