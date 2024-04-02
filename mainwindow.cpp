#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSpacerItem>
#include <QtWidgets>

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

	ispControl.OpenVideo();

	this->canUpdateControls = true;
}

void MainWindow::createControls()
{
	int i = 0;
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		qDebug() << i++;
		// if (const GroupControl *scontrol = dynamic_cast<const GroupControl*>(control))
		// {
		// 	qDebug() << "group";
		// 	GroupWidget *group = new GroupWidget;
		// 	group->initialize(scontrol->name);
		// 	ui->verticalLayout->addWidget(group, 1);
		// }
		// else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		// {
		// 	qDebug() << "check";
		// 	CheckBoxWidget *checkBox = new CheckBoxWidget;
		// 	checkBox->initialize(this, scontrol, nullptr);//&MainWindow::onCheckBoxChanged);

		// 	// QString *name = new QString(scontrol->type + "/" + scontrol->parameter);
		// 	// this->widgets.insert(name, checkBox);

		// 	ui->verticalLayout->addWidget(checkBox, 1);
		// 	qDebug() << "check2";
		// 	// checkBox->setState(scontrol->checked);
		// }
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			qDebug() << "slider";
			SliderWidget *slider = new SliderWidget;
			slider->initialize(this, scontrol, &MainWindow::onSliderValueChange);

			// QString *name = new QString(scontrol->type + "/" + scontrol->parameter);
			// this->widgets.insert(name, slider);


			ui->verticalLayout->addWidget(slider, 1);
		}
	}

	qDebug() << "end";
	ui->verticalLayout->addStretch(2);
	qDebug() << "end2";
}

void MainWindow::onCheckBoxChanged(MainWindow *mainWindow, QString &type, QString &parameter, bool checked)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << type << " " << parameter << " " << int(checked);
	if (type == IF_CPROC_S_EN)
		ispControl.set_cproc_enable(parameter, checked);
}

void MainWindow::onSliderValueChange(MainWindow *mainWindow, QString &type, QString &parameter, int value, int divide)
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
			QString name = QString(scontrol->type + "/" + scontrol->parameter);
			SliderWidget *slider = (SliderWidget*)this->widgets[&name];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->type + "/" + scontrol->parameter << " not found";
			else if (scontrol->type == type)
			{
				if (scontrol->precision == 0)
				{
					int value = json[(scontrol->parameter).toStdString()].asInt();
					slider->setValue(value);
					qDebug() << scontrol->type + "/" + scontrol->parameter << "=" << value;
				}
				else
				{
					float value = json[(scontrol->parameter).toStdString()].asFloat();
					slider->setValueFloat(value);
					qDebug() << scontrol->type + "/" + scontrol->parameter << "=" << value;
				}
			}
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			QString name = QString(scontrol->type + "/" + scontrol->parameter);
			CheckBoxWidget *checkBox = (CheckBoxWidget*)this->widgets[&name];
			if (checkBox == nullptr)
				qDebug() << "Widget " << scontrol->type + "/" + scontrol->parameter << " not found";
			else if (scontrol->type == type)
			{
				int state = json[(scontrol->parameter).toStdString()].asInt();
				checkBox->setState((bool)state);
				qDebug() << scontrol->type + "/" + scontrol->parameter << "=" << state;
			}
		}
	}
}

void MainWindow::on_pushButtonClicked()
{
	// this->setGeometry(100, 100, 200, 200);
	// this->move(50, 50);
	// this->setFixedSize(300, 300);
	// qDebug() << this->geometry().left() << "x" << this->geometry().top();

	this->readParameters();
}

void MainWindow::on_pushButton_clicked()
{
	this->canUpdateControls = false;
	this->createControls();
	this->canUpdateControls = true;

	this->readParameters();
}

// https://doc.qt.io/qt-5/qtwidgets-layouts-dynamiclayouts-example.html
// https://www.youtube.com/watch?v=fNTnU7mMgK4
// connect()
// set thread safety
