#include <ctime>
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
#include "button_widget.h"
#include "label_widget.h"

IspControl ispControl;
ControlsDefinitions ControlsDefinition;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ControlsDefinition.init();
	this->createControls();

	ispControl.openVideo();

	this->canUpdateControls = true;

	timerId = startTimer(200);
}

MainWindow::~MainWindow()
{
	killTimer(timerId);
	delete ui;
}

void MainWindow::createControls()
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		if (const GroupControl *scontrol = dynamic_cast<const GroupControl*>(control))
		{
			GroupWidget *group = new GroupWidget(this, scontrol->name);
			ui->verticalLayout->addWidget(group, 1);
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = new CheckBoxWidget(this, this, scontrol, &MainWindow::onCheckBoxChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), checkBox);
			ui->verticalLayout->addWidget(checkBox, 1);
			checkBox->setState(scontrol->checked);
		}
		else if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = new SliderWidget(this, this, scontrol, &MainWindow::onSliderValueChange);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), slider);
			ui->verticalLayout->addWidget(slider, 1);
		}
		else if (const ButtonControl *scontrol = dynamic_cast<const ButtonControl*>(control))
		{
			ButtonWidget *button = new ButtonWidget(this, this, scontrol, &MainWindow::onButtonClicked);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), button);
			ui->verticalLayout->addWidget(button, 1);
		}
		else if (const LabelControl *scontrol = dynamic_cast<const LabelControl*>(control))
		{
			LabelWidget *label = new LabelWidget(this, this, scontrol);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), label);
			ui->verticalLayout->addWidget(label, 1);
		}
	}

	ui->verticalLayout->addStretch(2);
}

void MainWindow::onCheckBoxChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, bool checked)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << int(checked);
	ispControl.setParamBool(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), checked);

	mainWindow->lastTime = clock();
}

void MainWindow::onSliderValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << ((float)value / divide);
	ispControl.setParam(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), value, divide);

	// if (type == IF_CPROC_S_CFG)
	// 	ispControl.setCprocCfg(parameter, value, divide);
	// else if (type == IF_CPROC_S_COEFF)
	// 	ispControl.setCprocCoeff(parameter, value, divide);

	mainWindow->lastTime = clock();
}

void MainWindow::onButtonClicked(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter;
	if (getCmd == NULL)
		ispControl.sendCmd(setCmd.toStdString().c_str(), parameter.toStdString().c_str(), value.toStdString().c_str());
	// else
	// 	ispControl.setParam(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str());
}

void MainWindow::on_pushButton_clicked()
{
	// this->setGeometry(100, 100, 200, 200);
	// this->move(50, 50);
	// this->setFixedSize(300, 300);
	// qDebug() << this->geometry().left() << "x" << this->geometry().top();

	// this->readParameters();
}

void MainWindow::readParameters()
{
	this->canUpdateControls = false;
	Json::Value params;

	params = ispControl.getParam(IF_AE_G_EN);
	if (params)
		this->updateControlsFromJson(params, IF_AE_S_EN);

	params = ispControl.getParam(IF_AE_G_CFG);
	if (params)
		this->updateControlsFromJson(params, IF_AE_S_CFG);

	params = ispControl.getParam(IF_AE_G_ECM);
	if (params)
		this->updateControlsFromJson(params, IF_AE_S_ECM);




	params = ispControl.getParam(IF_CPROC_G_EN);
	if (params)
		this->updateControlsFromJson(params, IF_CPROC_S_EN);

	params = ispControl.getParam(IF_CPROC_G_CFG);
	if (params)
		this->updateControlsFromJson(params, IF_CPROC_S_CFG);

	if (!this->notReadableControlsInitialized)
	{
		this->initializeControlsNotReadable(IF_CPROC_S_COEFF);
		this->initializeControlsNotReadable(IF_AE_G_STATUS);
		this->notReadableControlsInitialized = true;
	}

	this->canUpdateControls = true;
}

void MainWindow::initializeControlsNotReadable(QString type)
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == type)
			{
				slider->setRange();
				slider->setValue(scontrol->value);
			}
		}
}

void MainWindow::updateControlsFromJson(Json::Value json, QString type)
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == type)
			{
				slider->setRange();
				if (scontrol->precision == 0)
				{
					int value = json[scontrol->parameter.toStdString()].asInt();
					slider->setValue(value);
					// qDebug() << scontrol->parameter << value;
				}
				else
				{
					float value = json[scontrol->parameter.toStdString()].asFloat();
					slider->setValueFloat(value);
					// qDebug() << scontrol->parameter << value;
				}
			}
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = (CheckBoxWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (checkBox == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == type)
			{
				int state = json[scontrol->parameter.toStdString()].asInt();
				checkBox->setState((bool)state);
				// qDebug() << scontrol->parameter << state;
			}
		}
	}
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	double diff = double(clock() - this->lastTime) / CLOCKS_PER_SEC * 1000;
	if (diff >= 300)
		this->readParameters();
}
