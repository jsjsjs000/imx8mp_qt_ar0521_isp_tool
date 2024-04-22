#include "combobox_widget2.h"
#include "ui_combobox_widget2.h"

ComboBoxWidget2::ComboBoxWidget2(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::ComboBoxWidget2)
{
	ui->setupUi(this);
}

ComboBoxWidget2::~ComboBoxWidget2()
{
	delete ui;
}
