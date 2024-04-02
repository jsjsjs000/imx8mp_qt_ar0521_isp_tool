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

	this->canUpdateControls = true;
}

void MainWindow::createControls()
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		if (const GroupControl *scontrol = dynamic_cast<const GroupControl*>(control))
		{
			GroupWidget *group = new GroupWidget();
			group->initialize(scontrol->name);
			ui->verticalLayout->addWidget(group, 1);
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = new CheckBoxWidget();
			checkBox->initialize(this, scontrol, &MainWindow::onCheckBoxChanged);
			this->widgets.insert(QString(scontrol->type + "/" + scontrol->parameter), checkBox);
			ui->verticalLayout->addWidget(checkBox, 1);
			checkBox->setState(scontrol->checked);
		}
		else if (const SliderControl *scontrol = static_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = new SliderWidget();
			slider->initialize(this, scontrol, &MainWindow::onSliderValueChange);
			this->widgets.insert(QString(scontrol->type + "/" + scontrol->parameter), slider);
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

void MainWindow::onPushButtonClicked()
{
	// this->setGeometry(100, 100, 200, 200);
	// this->move(50, 50);
	// this->setFixedSize(300, 300);
	// qDebug() << this->geometry().left() << "x" << this->geometry().top();

	this->readParameters();
}

void MainWindow::readParameters()
{
	this->canUpdateControls = false;

	Json::Value cprocEn = ispControl.getCprocEn();
	if (cprocEn)
		this->updateControlsFromJson(cprocEn, IF_CPROC_S_EN);

	Json::Value cprocCfg = ispControl.getCprocCfg();
	if (cprocCfg)
		this->updateControlsFromJson(cprocCfg, IF_CPROC_S_CFG);

	this->canUpdateControls = true;
}

void MainWindow::updateControlsFromJson(Json::Value json, QString type)
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->type + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->type + "/" + scontrol->parameter << " not found";
			else if (scontrol->type == type)
			{
				if (scontrol->precision == 0)
				{
					int value = json[scontrol->parameter.toStdString()].asInt();
					slider->setValue(value);
					qDebug() << scontrol->parameter << " " << value;
				}
				else
				{
					float value = json[scontrol->parameter.toStdString()].asFloat();
					slider->setValueFloat(value);
					qDebug() << scontrol->parameter << " " << value;
				}
			}
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = (CheckBoxWidget*)this->widgets[scontrol->type + "/" + scontrol->parameter];
			if (checkBox == nullptr)
				qDebug() << "Widget " << scontrol->type + "/" + scontrol->parameter << " not found";
			else if (scontrol->type == type)
			{
				int state = json[scontrol->parameter.toStdString()].asInt();
				checkBox->setState((bool)state);
				qDebug() << scontrol->parameter << " " << state;
			}
		}
	}
}
