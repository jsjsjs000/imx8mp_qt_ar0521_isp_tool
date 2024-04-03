#include "button_widget.h"
#include "controls_definitions.h"
#include "ui_button_widget.h"

ButtonWidget::ButtonWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::ButtonWidget)
{
	ui->setupUi(this);
}

ButtonWidget::ButtonWidget(QWidget *parent, MainWindow *mainWindow, const ButtonControl *control,
		void (*onButtonClicked)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value))
		: QWidget(parent)
		, ui(new Ui::ButtonWidget)
{
	ui->setupUi(this);

	this->mainWindow = mainWindow;
	this->getCmd = control->getCmd;
	this->setCmd = control->setCmd;
	this->parameter = control->parameter;
	this->value = control->value;
	ui->pushButton->setText(control->name);
	ui->pushButton->resize(ui->pushButton->sizeHint().width(), ui->pushButton->height());
	ui->pushButton->setToolTip(control->description);
	this->onButtonClicked = onButtonClicked;
}

ButtonWidget::~ButtonWidget()
{
	delete ui;
}

void ButtonWidget::on_pushButton_clicked()
{
	if (this->onButtonClicked)
		(*this->onButtonClicked)(this->mainWindow, this->getCmd, this->setCmd, this->parameter, this->value);
}
