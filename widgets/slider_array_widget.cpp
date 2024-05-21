#include "slider_array_widget.h"
#include "ui_slider_array_widget.h"

#include <QMenu>
#include <QSlider>
#include <QDebug>

SliderArrayWidget::SliderArrayWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::SliderArrayWidget)
{
	ui->setupUi(this);
}

SliderArrayWidget::SliderArrayWidget(QWidget *parent, MainWindow *mainWindow, const SliderArrayControl *control,
		void (*onSliderArrayValueChange)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<float> values))
		: QWidget(parent)
		, ui(new Ui::SliderArrayWidget)
{
	ui->setupUi(this);

	if (!control->readonly)
	{
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &SliderArrayWidget::customContextMenuRequested, this, &SliderArrayWidget::slotCustomMenuRequested);
	}

	this->setMinimumHeight(20 + control->min.count() * 29);
	this->mainWindow = mainWindow;
	this->getCmd = control->getCmd;
	this->setCmd = control->setCmd;
	this->parameter = control->parameter;
	this->min = control->min;
	this->max = control->max;
	this->precision = control->precision;
	this->multiple = control->multiple;
	// this->defaultValue = control->value;
	ui->name->setText(control->name);
	ui->name->setToolTip(control->description);
	for (int i = 0; i < control->min.count(); i++)
	{
		QVBoxLayout *vBoxLayout1 = new QVBoxLayout();
		vBoxLayout1->setSpacing(0);
		ui->horizontalLayout2->addLayout(vBoxLayout1);

		QHBoxLayout *hBoxLayout = new QHBoxLayout();
		hBoxLayout->setSpacing(0);
		vBoxLayout1->addLayout(hBoxLayout);

		QLabel *label1 = new QLabel();
		label1->setText(control->descriptions[i]);
		hBoxLayout->addWidget(label1);

		QLabel *label2 = new QLabel();
		label2->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		// label2->setText(QString::number(this->defaultValue[i]));
		hBoxLayout->addWidget(label2);
		this->labels.push_back(label2);

		QVBoxLayout *vBoxLayout2 = new QVBoxLayout();
		vBoxLayout2->setSpacing(0);
		ui->horizontalLayout2->addLayout(vBoxLayout2);

		QSlider *slider = new QSlider(Qt::Orientation::Horizontal);
		vBoxLayout2->addWidget(slider);
		slider->setEnabled(!control->readonly);
		slider->setProperty("id", i);
		this->horizontalSlider.push_back(slider);
	}
	this->setRange();
	this->onSliderArrayValueChange = onSliderArrayValueChange;

	for (int i = 0; i < this->horizontalSlider.count(); i++)
		connect(this->horizontalSlider[i], SIGNAL(valueChanged(int)), this, SLOT(onHorizontalSliderValueChanged(int)));
}

SliderArrayWidget::SliderArrayWidget(QWidget *parent, MainWindow *mainWindow, const SliderArrayControl2 *control,
		void (*onSliderArray2ValueChange)(MainWindow *mainWindow, QString node, QList<float> values))
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
	// this->defaultValue = control->value;
	ui->name->setText(control->name);
	ui->name->setToolTip(control->description);
	for (int i = 0; i < control->min.count(); i++)
	{
		QSlider *slider = new QSlider(Qt::Orientation::Horizontal, this);
		ui->horizontalLayout2->addWidget(slider);
		slider->setEnabled(!control->readonly);
	}
	this->setRange();
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
	for (int i = 0; i < this->min.count(); i++)
	{
		this->horizontalSlider[i]->setMinimum(this->min[i]);
		this->horizontalSlider[i]->setMaximum(this->max[i]);
	}
}

void SliderArrayWidget::setDefaultAndFactoryValue(QList<float> defaultValue, QList<float> factoryValue)
{
	// this->defaultValue = defaultValue;
	// this->factoryValue = factoryValue;
}

void SliderArrayWidget::setValues(QList<int> values)
{
	for (int i = 0; i < values.count(); i++)
	{
		this->labels[i]->setText(QString::number(values[i]));
		this->horizontalSlider[i]->setValue(values[i]);
	}
	//$$ if (ui->horizontalSlider->value() != value)
	// {
	// 	ui->value->setText(QString::number(value));
	// 	ui->horizontalSlider->setValue(value);
	// }
}

void SliderArrayWidget::setValuesFloats(QList<float> values)
{
	for (int i = 0; i < values.count(); i++)
	{
		this->labels[i]->setText(QString::number((float)values[i], 'f', this->precision[i]));
		this->horizontalSlider[i]->setValue((int)(values[i] * this->multiple[i]));
	}
}

// QList<float>* SliderArrayWidget::getValues()
// {
// 	QList<float> *values;
// 	return values; //$$ ui->horizontalSlider->value();
// }

void SliderArrayWidget::onHorizontalSliderValueChanged(int /* value */)
{
	//$$if (this->precision == 0)
		//$$ui->value->setText(QString::number(value));
	//$$else
		//$$ui->value->setText(QString::number((float)value / this->multiple, 'f', this->precision));

	QList<float> values;
	for (int i = 0; i < this->horizontalSlider.count(); i++)
	{
		float fvalue = (float)this->horizontalSlider[i]->value() / this->multiple[i];
		this->labels[i]->setText(QString::number(fvalue, 'f', this->precision[i]));
		values.push_back(fvalue);
	}

	if (this->onSliderArrayValueChange != nullptr)
		(*this->onSliderArrayValueChange)(this->mainWindow, this->getCmd, this->setCmd, this->parameter, values);
	if (this->onSliderArray2ValueChange != nullptr)
		(*this->onSliderArray2ValueChange)(this->mainWindow, this->node, values);
}
