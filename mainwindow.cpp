#include <ctime>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSpacerItem>

#include "controls_definitions.h"
#include "widgets/slider_widget.h"
#include "widgets/group_widget.h"
#include "widgets/chart_widget.h"
#include "widgets/checkbox_widget.h"
#include "widgets/button_widget.h"
#include "widgets/label_widget.h"
#include <widgets/combobox_widget.h>
#include "mainwindow.h"
#include "preview_window.h"
#include "ui_mainwindow.h"

IspControl ispControl;
ControlsDefinitions ControlsDefinition;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ControlsDefinition.init();
	this->createControls();

	PreviewWindow::setupCamera(ui->verticalLayout_2);

/*
	Chart *chart = new Chart();
	QList<QPointF> points = QList<QPointF>();
	points.push_back(QPointF(1, -0.9f));
	points.push_back(QPointF(3, -0.3f));
	points.push_back(QPointF(5, -0.5f));
	chart->initialize(0, 15, -1, 1, 1.0f, 0.1f, points);
	ui->verticalLayout_2->addWidget(chart, 1);
	// ui->verticalLayout_2->addStretch(0);
*/

	ispControl.openVideo();

	this->canUpdateControls = true;

	this->lastTime = clock();
	timerId = startTimer(500); // $$ 500
}

MainWindow::~MainWindow()
{
	killTimer(timerId);
	delete ui;
}

void MainWindow::createControls()
{
	const int stretch = 1;
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		if (const GroupControl *scontrol = dynamic_cast<const GroupControl*>(control))
		{
			GroupWidget *group = new GroupWidget(this, scontrol->name);
			ui->verticalLayout->addWidget(group, stretch);
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = new CheckBoxWidget(this, this, scontrol, &MainWindow::onCheckBoxChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), checkBox);
			ui->verticalLayout->addWidget(checkBox, stretch);
			checkBox->setState(scontrol->checked);
		}
		else if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = new SliderWidget(this, this, scontrol, &MainWindow::onSliderValueChange);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), slider);
			ui->verticalLayout->addWidget(slider, stretch);
		}
		else if (const ButtonControl *scontrol = dynamic_cast<const ButtonControl*>(control))
		{
			ButtonWidget *button = new ButtonWidget(this, this, scontrol, &MainWindow::onButtonClicked);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), button);
			ui->verticalLayout->addWidget(button, stretch);
		}
		else if (const LabelControl *scontrol = dynamic_cast<const LabelControl*>(control))
		{
			LabelWidget *label = new LabelWidget(this, this, scontrol);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), label);
			ui->verticalLayout->addWidget(label, stretch);
		}
		else if (const ComboBoxControl *scontrol = dynamic_cast<const ComboBoxControl*>(control))
		{
			ComboBoxWidget *comboBox = new ComboBoxWidget(this, this, scontrol, &MainWindow::onComboBoxIndexChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), comboBox);
			ui->verticalLayout->addWidget(comboBox, stretch);
		}
		else if (const ChartControl *scontrol = dynamic_cast<const ChartControl*>(control))
		{
			ChartWidget *chart = new ChartWidget(this, this, scontrol, &MainWindow::onChartControlPointsChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), chart);
			ui->verticalLayout->addWidget(chart, stretch);
		}
	}
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

	mainWindow->lastTime = clock();
}

void MainWindow::onComboBoxIndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << key << "(" + value + ")";
	ispControl.setParam(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), key, 1);

	mainWindow->lastTime = clock();
}

void MainWindow::onButtonClicked(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter;
	if (strncmp(value.toStdString().c_str(), "true", strlen("true")) == 0)
		ispControl.setParamBool(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), true);
	else if (strncmp(value.toStdString().c_str(), "false", strlen("false")) == 0)
		ispControl.setParamBool(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), false);
	else
		ispControl.setParamString(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), value.toStdString().c_str());

	mainWindow->lastTime = clock();
}

void MainWindow::onChartControlPointsChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter)
{

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

	if (!this->notReadableControlsInitialized)
	{
		for (const QString &paramName : qAsConst(ControlsDefinition.initializeNotReadableControls))
			this->initializeControlsNotReadable(paramName);

		this->notReadableControlsInitialized = true;
	}

	for (const QString &paramName : qAsConst(ControlsDefinition.readParams))
	{
		params = ispControl.getParam(paramName.toStdString().c_str());
		if (params)
			this->updateControlsFromJson(params, paramName);
	}

	this->canUpdateControls = true;
}

void MainWindow::initializeControlsNotReadable(QString cmd)
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				slider->setRange();
				slider->setValue(scontrol->value);
			}
		}
}

void MainWindow::updateControlsFromJson(Json::Value json, QString cmd)
{
	for (const auto *control : qAsConst(ControlsDefinition.controls))
	{
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
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

					// if (strncmp(scontrol->parameter.toStdString().c_str(), EC_TIME_PARAMS, strlen(EC_TIME_PARAMS)) == 0)
					// 	qDebug("%f", 1.0 / value);
				}
			}
		}
		else if (const ComboBoxControl *scontrol = dynamic_cast<const ComboBoxControl*>(control))
		{
			ComboBoxWidget *comboBox = (ComboBoxWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (comboBox == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				int index;
				if (json[scontrol->parameter.toStdString()].isBool())
					index = json[scontrol->parameter.toStdString()].asBool();
				else
					index = json[scontrol->parameter.toStdString()].asInt();
				comboBox->setItemIndex(index);
			}
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = (CheckBoxWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (checkBox == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				int state = json[scontrol->parameter.toStdString()].asInt();
				checkBox->setState((bool)state);
				// qDebug() << scontrol->parameter << state;
			}
		}
		else if (const LabelControl *scontrol = dynamic_cast<const LabelControl*>(control))
		{
			LabelWidget *label = (LabelWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (label == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				Json::Value value = json[scontrol->parameter.toStdString()];
				QString text;
				if (scontrol->type == &typeid(int[]))
				{
					for (uint i = 0; i < value.size(); i++)
					{
						if (i > 0)
							text += ",";
						text += QString::number(value[i].asInt());
					}
				}
				else if (scontrol->type == &typeid(float[]))
				{
					for (uint i = 0; i < value.size(); i++)
					{
						if (i > 0)
							text += ", ";
						text += QString::number(value[i].asFloat(), 'f', 3);
					}
				}
				else if (scontrol->type == &typeid(int))
					text = QString::number(value.asInt());
				else if (scontrol->type == &typeid(float))
					text = QString::number(value.asFloat(), 'f', 3);
				else if (scontrol->type == &typeid(std::string))
					text = QString(value.asCString());
				else if (scontrol->type == &typeid(std::string[]))
				{
					for (uint i = 0; i < value.size(); i++)
					{
						if (i > 0)
							text += ", ";
						text += QString(value[i].asCString());
					}
				}
				else
				{
					text = "(" + scontrol->parameter + " not decoded type)";
					qDebug() << scontrol->parameter << "not decoded in LabelControl";
				}

				label->setText(text);
				// qDebug() << scontrol->parameter << state;
			}
		}
		else if (const ChartControl *scontrol = dynamic_cast<const ChartControl*>(control))
		{
			ChartWidget *chart = (ChartWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (chart == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				Json::Value value = json[scontrol->parameter.toStdString()];
				// if (scontrol->type == &typeid(int[]))
				// else if (scontrol->type == &typeid(float[]))
				{
					QList<QPointF> points;
					for (uint i = 0; i < value.size(); i++)
						points.push_back(QPointF(i, value[i].asFloat()));
					chart->initialize(0, value.size() - 1, scontrol->y1, scontrol->y2, 1.0f, scontrol->gridY, points);
				}
			}
		}
	}
}

void MainWindow::timerEvent(QTimerEvent* /* event */)
{
	double diff = double(clock() - this->lastTime) / CLOCKS_PER_SEC * 1000;
	if (!this->readyForReadJson && diff < 1000)
		return;

	this->readyForReadJson = true;
	if (diff >= 300)
	{
		this->readParameters();

		ui->fpsLabel->setText(QString::number(ispControl.fps));
	}

	// ispControl.getFps();
}

void MainWindow::onActivated()
{
	if (this->isActivated)
		return;

	this->isActivated = true;
}

bool MainWindow::event(QEvent *e)
{
	if (e->type() == QEvent::WindowActivate)
		this->onActivated();
	return QWidget::event(e);
}
