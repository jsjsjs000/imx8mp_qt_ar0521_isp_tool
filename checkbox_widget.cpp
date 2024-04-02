#include "checkbox_widget.h"
#include "controls_definitions.h"
#include "ui_checkbox_widget.h"

CheckBoxWidget::CheckBoxWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::CheckBoxWidget)
{
	ui->setupUi(this);
}

CheckBoxWidget::~CheckBoxWidget()
{
	delete ui;
}

void CheckBoxWidget::initialize(MainWindow *mainWindow, const CheckBoxControl *control,
		void (*onCheckBoxChanged)(MainWindow *mainWindow, QString &type, QString &parameter, bool checked))
{
	this->mainWindow = mainWindow;
	this->type = QString(control->type);
	this->parameter = QString(control->parameter);
	this->ui->checkBox->setText(QString(control->name));
	this->ui->checkBox->setToolTip(QString(control->description));
	// this->onCheckBoxChanged = onCheckBoxChanged;
}

void CheckBoxWidget::setState(bool state)
{
	this->ui->checkBox->setChecked(state);
}

void CheckBoxWidget::on_checkBox_stateChanged(int arg1)
{
	if (this->onCheckBoxChanged)
		(*this->onCheckBoxChanged)(this->mainWindow, this->type, this->parameter, arg1 != 0);
}
