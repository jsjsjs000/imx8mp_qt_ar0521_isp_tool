#include "label_widget.h"
#include "ui_label_widget.h"

LabelWidget::LabelWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::LabelWidget)
{
	ui->setupUi(this);
}

LabelWidget::LabelWidget(QWidget *parent, MainWindow *mainWindow, const LabelControl *control)
		: QWidget(parent)
		, ui(new Ui::LabelWidget)
{
	ui->setupUi(this);

	this->mainWindow = mainWindow;
	this->label = control->label;
	this->name = control->name;
	this->value = control->value;
	ui->label->setText(control->name + ": " + control->value);
	ui->label->setToolTip(control->description);
}

LabelWidget::~LabelWidget()
{
	delete ui;
}

void LabelWidget::setText(QString s)
{
	this->value = s;
	this->ui->label->setText(this->name + ":" + s);
}