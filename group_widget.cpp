#include "group_widget.h"
#include "ui_group_widget.h"

GroupWidget::GroupWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::GroupWidget)
{
	ui->setupUi(this);
}

GroupWidget::~GroupWidget()
{
	delete ui;
}
