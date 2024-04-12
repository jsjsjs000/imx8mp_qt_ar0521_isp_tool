#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSpacerItem>
#include <QProcess>

#include "controls_definitions.h"
#include "widgets/slider_widget.h"
#include "widgets/group_widget.h"
#include "widgets/chart_widget.h"
#include "widgets/checkbox_widget.h"
#include "widgets/button_widget.h"
#include "widgets/label_widget.h"
#include <widgets/combobox_widget.h>
#include "controls2_definitions.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

IspControl ispControl;
ControlsDefinitions controlsDefinition;
Controls2Definitions controls2Definition;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	controlsDefinition.init();
	controls2Definition.init();
	this->createControls();
	this->createControls2();

	// PreviewWindow::setupCamera(ui->verticalLayout_2);
	// PreviewWindow::setupCamera2(ui->verticalLayout_2);

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

	this->killGStreamerProcess();
	this->createGStreamerProcess();

	ispControl.openVideo();

	controls2Definition.readXml();
	this->updateControls2fromXml();

	this->canUpdateControls = true;

	this->elapsedTimer.start();
	this->lastTime = this->elapsedTimer.elapsed();
	timerId = startTimer(1000); // $$ 500
}

MainWindow::~MainWindow()
{
	killTimer(timerId);
	delete ui;
}


void MainWindow::killGStreamerProcess()
{
	QProcess *process = new QProcess(this);
	QString program = "killall -s KILL gst-launch-1.0";
	process->start(program);
	process->waitForFinished(1000);
}

void MainWindow::createGStreamerProcess()
{
	QProcess *process = new QProcess(this);
	QString program = "gst-launch-1.0 v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=1920,height=1080 ! waylandsink window-width=1560 window-height=878";
	process->start(program);
	process->waitForFinished(1000);
}

void MainWindow::createControls()
{
	for (const auto *control : qAsConst(controlsDefinition.controls))
	{
		if (const GroupControl *scontrol = dynamic_cast<const GroupControl*>(control))
		{
			GroupWidget *group = new GroupWidget(this, scontrol->name);
			ui->verticalLayout_1->addWidget(group);
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = new CheckBoxWidget(this, this, scontrol, &MainWindow::onCheckBoxChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), checkBox);
			ui->verticalLayout_1->addWidget(checkBox);
			checkBox->setState(scontrol->checked);
		}
		else if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = new SliderWidget(this, this, scontrol, &MainWindow::onSliderValueChange);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), slider);
			ui->verticalLayout_1->addWidget(slider);
		}
		else if (const ButtonControl *scontrol = dynamic_cast<const ButtonControl*>(control))
		{
			ButtonWidget *button = new ButtonWidget(this, this, scontrol, &MainWindow::onButtonClicked);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), button);
			ui->verticalLayout_1->addWidget(button);
		}
		else if (const LabelControl *scontrol = dynamic_cast<const LabelControl*>(control))
		{
			LabelWidget *label = new LabelWidget(this, this, scontrol);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), label);
			ui->verticalLayout_1->addWidget(label);
		}
		else if (const ComboBoxControl *scontrol = dynamic_cast<const ComboBoxControl*>(control))
		{
			ComboBoxWidget *comboBox = new ComboBoxWidget(this, this, scontrol, &MainWindow::onComboBoxIndexChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), comboBox);
			ui->verticalLayout_1->addWidget(comboBox);
		}
		else if (const ChartControl *scontrol = dynamic_cast<const ChartControl*>(control))
		{
			ChartWidget *chart = new ChartWidget(this, this, scontrol, &MainWindow::onChartControlPointsChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), chart);
			ui->verticalLayout_1->addWidget(chart);
		}
	}
}

void MainWindow::onCheckBoxChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, bool checked)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << int(checked);
	ispControl.setParamBool(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), checked);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onSliderValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << ((float)value / divide);
	ispControl.setParam(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), value, divide);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onComboBoxIndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << key << "(" + value + ")";
	ispControl.setParam(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), key, 1);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
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

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onChartControlPointsChanged(MainWindow *mainWindow, QString /* getCmd */, QString /* setCmd */, QString /* parameter */)
{
	if (!mainWindow->canUpdateControls)
		return;

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}


void MainWindow::createControls2()
{
	for (const auto *control : qAsConst(controls2Definition.controls))
	{
		if (const SliderControl2 *scontrol = dynamic_cast<const SliderControl2*>(control))
		{
			SliderWidget *slider = new SliderWidget(this, this, scontrol, &MainWindow::onSlider2ValueChange);
			this->widgets2.insert(QString(scontrol->node), slider);
			ui->verticalLayout_2->addWidget(slider);
		}
		if (const ChartControl2 *scontrol = dynamic_cast<const ChartControl2*>(control))
		{
			ChartWidget *chart = new ChartWidget(this, this, scontrol, &MainWindow::onChartControl2PointsChanged);
			this->widgets2.insert(QString(scontrol->node), chart);
			ui->verticalLayout_2->addWidget(chart);
		}
	}
}

void MainWindow::onSlider2ValueChange(MainWindow *mainWindow, QString node, int value, int divide)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << node << ((float)value / divide);
	// ispControl.setParam(getCmd.toStdString().c_str(), setCmd.toStdString().c_str(), parameter.toStdString().c_str(), value, divide);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onChartControl2PointsChanged(MainWindow *mainWindow, QString /* node */)
{
	if (!mainWindow->canUpdateControls)
		return;

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}


void MainWindow::readParameters()
{
	this->canUpdateControls = false;
	Json::Value params;

	if (!this->notReadableControlsInitialized)
	{
		for (const QString &paramName : qAsConst(controlsDefinition.initializeNotReadableControls))
			this->initializeControlsNotReadable(paramName);

		this->notReadableControlsInitialized = true;
	}

	for (const QString &paramName : qAsConst(controlsDefinition.readParams))
	{
		params = ispControl.getParam(paramName.toStdString().c_str());
		if (params)
			this->updateControlsFromJson(params, paramName);
	}

	this->canUpdateControls = true;
}

void MainWindow::initializeControlsNotReadable(QString cmd)
{
	for (const auto *control : qAsConst(controlsDefinition.controls))
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
	for (const auto *control : qAsConst(controlsDefinition.controls))
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

void MainWindow::updateControls2fromXml()
{
	for (const auto *control : qAsConst(controls2Definition.controls))
	{
		if (const SliderControl2 *scontrol = dynamic_cast<const SliderControl2*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets2[scontrol->node];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->node << " not found";
			else
			{
				slider->setRange();
				// if (scontrol->precision == 0)
				// {
				// 	// QList<float> array;
				// 	// if (controls2Definition.getArrayNode(scontrol->node, array))
				// 	// 	if (array.count() == 1)
				// 	// 		slider->setValueFloat(array[0]);
				// }
				// else
				// {
					QList<float> array;
					if (controls2Definition.getArrayNode(scontrol->node, array))
						if (array.count() == 1)
							slider->setValueFloat(array[0]);
				// }
			}
		}
		else if (const ChartControl2 *scontrol = dynamic_cast<const ChartControl2*>(control))
		{
			ChartWidget *chart = (ChartWidget*)this->widgets2[scontrol->node];
			if (chart == nullptr)
				qDebug() << "Widget " << scontrol->node << " not found";
			else
			{
				QList<float> array;
				if (controls2Definition.getArrayNode(scontrol->node, array))
				{
					QList<QPointF> points;
					for (int i = 0; i < array.size(); i++)
						points.push_back(QPointF(i, array[i]));
					chart->initialize(0, array.size() - 1, scontrol->y1, scontrol->y2, 1.0f, scontrol->gridY, points);
				}
			}
		}
	}
}

void MainWindow::timerEvent(QTimerEvent* /* event */)
{
	int diff = this->elapsedTimer.elapsed() - this->lastTime;
	if (!this->readyForReadJson && diff < 1000)
		return;

	if (!this->readyForReadJson)
		this->readyForReadJson = true;

	if (diff >= 300)
	{
		if (this->ui->tabWidget->currentIndex() == 0)
			this->readParameters();

		// ui->fpsLabel->setText(QString::number(ispControl.fps));
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

void MainWindow::on_saveButton_clicked()
{
	// this->setGeometry(100, 100, 200, 200);
	// this->move(50, 50);
	// this->setFixedSize(300, 300);
	// qDebug() << this->geometry().left() << "x" << this->geometry().top();
}
