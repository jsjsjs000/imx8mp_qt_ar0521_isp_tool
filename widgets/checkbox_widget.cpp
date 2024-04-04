#include "checkbox_widget.h"
#include "controls_definitions.h"
#include "ui_checkbox_widget.h"

CheckBoxWidget::CheckBoxWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::CheckBoxWidget)
{
	ui->setupUi(this);
}

CheckBoxWidget::CheckBoxWidget(QWidget *parent, MainWindow *mainWindow, const CheckBoxControl *control,
		void (*onCheckBoxChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, bool checked))
		: QWidget(parent)
		, ui(new Ui::CheckBoxWidget)
{
	ui->setupUi(this);

	this->mainWindow = mainWindow;
	this->getCmd = control->getCmd;
	this->setCmd = control->setCmd;
	this->parameter = control->parameter;
	ui->checkBox->setText(control->name);
	ui->checkBox->setToolTip(control->description);
	this->onCheckBoxChanged = onCheckBoxChanged;
}

CheckBoxWidget::~CheckBoxWidget()
{
	delete ui;
}

void CheckBoxWidget::setState(bool state)
{
	if (this->ui->checkBox->isChecked() != state)
		this->ui->checkBox->setChecked(state);
}

void CheckBoxWidget::on_checkBox_stateChanged(int arg1)
{
	if (this->onCheckBoxChanged)
		(*this->onCheckBoxChanged)(this->mainWindow, this->getCmd, this->setCmd, this->parameter, arg1 != 0);
}
