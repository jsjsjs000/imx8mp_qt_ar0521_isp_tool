#include "group_widget.h"
#include "ui_group_widget.h"

GroupWidget::GroupWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::GroupWidget)
{
	ui->setupUi(this);
}

GroupWidget::GroupWidget(QWidget *parent, QString name)
		: QWidget(parent)
		, ui(new Ui::GroupWidget)
{
	ui->setupUi(this);
	this->ui->label->setText(name);
}

GroupWidget::~GroupWidget()
{
	delete ui;
}
