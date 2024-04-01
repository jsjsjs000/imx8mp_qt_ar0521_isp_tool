#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSpacerItem>

#include "controls_definitions.h"
#include "slider_widget.h"
#include "group_widget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "checkbox_widget.h"

IspControl ispControl;
ControlsDefinitions ControlsDefinition;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ControlsDefinition.init();
	this->createControls();

	ispControl.OpenVideo();

	Json::Value cproc = ispControl.get_cproc();
}

void MainWindow::createControls()
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		if (const GroupControl *scontrol = dynamic_cast<const GroupControl*>(control))
		{
			GroupWidget *gcontrol = new GroupWidget();
			gcontrol->initialize(scontrol->name);
			ui->verticalLayout->addWidget(gcontrol, 1);
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = new CheckBoxWidget();
			checkBox->initialize(this, scontrol, &MainWindow::onCheckBoxChanged);
			ui->verticalLayout->addWidget(checkBox, 1);
			checkBox->setState(scontrol->checked);
		}
		else if (const SliderControl *scontrol = static_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = new SliderWidget();
			slider->initialize(this, scontrol, &MainWindow::onSliderValueChange);
			ui->verticalLayout->addWidget(slider, 1);
		}
	}

	ui->verticalLayout->addStretch(2);
}

void MainWindow::onCheckBoxChanged(MainWindow *mainWindow, QString type, QString parameter, bool checked)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << type << " " << parameter << " " << int(checked);
	if (type == IF_CPROC_S_EN)
		ispControl.set_cproc_enable(parameter, checked);
}

void MainWindow::onSliderValueChange(MainWindow *mainWindow, QString type, QString parameter, int value, int divide)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << type << " " << parameter << " " << ((float)value / divide);
	if (type == IF_CPROC_S_CFG)
		ispControl.set_cproc_value(parameter, value, divide);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	this->setGeometry(100, 100, 200, 200);
	this->move(50, 50);
	this->setFixedSize(300, 300);
	// qDebug() << this->geometry().left() << "x" << this->geometry().top();
}
