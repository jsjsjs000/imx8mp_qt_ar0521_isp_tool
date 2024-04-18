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
	this->description = control->description;
	ui->label->setText(control->name + ":  " + control->value);
	ui->label->setToolTip(control->description);
}

LabelWidget::~LabelWidget()
{
	delete ui;
}

void LabelWidget::setText(QString s)
{
	if (this->value != s)
	{
		this->value = s;
		this->ui->label->setText(this->name + ":  " + s);
		if (this->description.length() == 0)
			this->ui->label->setToolTip(this->name + ":  " + s);
		else
			this->ui->label->setToolTip(this->description + "\n\n" + this->name + ":  " + s);
	}
}
