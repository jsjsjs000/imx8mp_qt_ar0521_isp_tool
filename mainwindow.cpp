#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSpacerItem>
#include <QProcess>
#include <QThread>

#include "controls_definitions.h"
#include <widgets/chart_widget.h>
#include <widgets/checkbox_widget.h>
#include <widgets/combobox_widget.h>
#include <widgets/label_widget.h>
#include <widgets/slider_widget.h>
#include "controls2_definitions.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "isp_proc_thread.h"

#include <widgets/button_widget.h>
#include <widgets/group_widget.h>

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

	this->setFps(this->InitialFps);

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
	timerId = startTimer(500);

	this->runProcFsThread();
}

MainWindow::~MainWindow()
{
	killTimer(timerId);
	delete ui;
}

int MainWindow::getFps()
{
	for (const auto *control : qAsConst(controlsDefinition.controls))
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider != nullptr && scontrol->setCmd == IF_S_FPS)
				return slider->getValue();
		}

	return -1;
}

void MainWindow::setFps(int fps)
{
	for (const auto *control : qAsConst(controlsDefinition.controls))
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider != nullptr && scontrol->setCmd == IF_S_FPS)
				slider->setValue(fps);
		}
}

void MainWindow::runProcFsThread()
{
	thread = new IspProcThread(this, ispControl, controlsDefinition, this->widgets);
	connect(thread, &IspProcThread::signal_update_slider_control_int,
			this, &MainWindow::signal_update_slider_control_int);
	connect(thread, &IspProcThread::signal_update_slider_control_float,
			this, &MainWindow::signal_update_slider_control_float);
	connect(thread, &IspProcThread::signal_update_comboBox_item_index,
			this, &MainWindow::signal_update_comboBox_item_index);
	connect(thread, &IspProcThread::signal_update_checkBox_set_state,
			this, &MainWindow::signal_update_checkBox_set_state);
	connect(thread, &IspProcThread::signal_update_label_set_text,
			this, &MainWindow::signal_update_label_set_text);
	connect(thread, &IspProcThread::signal_update_chart,
			this, &MainWindow::signal_update_chart);
	thread->start();
}

void MainWindow::stopProcFsThread()
{
	this->thread->Stop = true;
	QThread::msleep(100);
}

void MainWindow::killGStreamerProcess()
{
	QProcess *process = new QProcess(this);
	QString program = "killall";
	process->start(program, QStringList({"-s", "KILL", "gst-launch-1.0"}));
	process->waitForFinished(1000);
}

void MainWindow::createGStreamerProcess()
{
	QProcess *process = new QProcess(this);
	QString program = "gst-launch-1.0";
	QString caps = "video/x-raw,format=YUY2,width=1920,height=1080,framerate=" + QString::number(this->InitialFps) + "/1";
	process->start(program, QStringList({
			"v4l2src", "device=/dev/video0", "!",
			caps, "!",
			"waylandsink", "window-width=1560", "window-height=878"}));
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

	if (setCmd == IF_S_FPS && parameter == "fps")
		mainWindow->lastSetFps = value;

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

void MainWindow::onChartControlPointsChanged(MainWindow *mainWindow, QString /* getCmd */, QString /* setCmd */, QString /* parameter */, QList<QPointF> /* points */)
{
	if (!mainWindow->canUpdateControls)
		return;

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}


void MainWindow::createControls2()
{
	for (const auto *control : qAsConst(controls2Definition.controls))
	{
		if (const GroupControl2 *scontrol = dynamic_cast<const GroupControl2*>(control))
		{
			GroupWidget *group = new GroupWidget(this, scontrol->name);
			ui->verticalLayout_2->addWidget(group);
		}
		else if (const SliderControl2 *scontrol = dynamic_cast<const SliderControl2*>(control))
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

	// qDebug() << node << ((float)value / divide);
	float f = (float)value / divide;
	QList<float> array;
	array.push_back(f);
	controls2Definition.setArrayNode(node, array);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onChartControl2PointsChanged(MainWindow *mainWindow, QString node, QList<QPointF> points)
{
	if (!mainWindow->canUpdateControls)
		return;

	// qDebug() << node << points;
	controls2Definition.setArrayNode(node, points);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
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
					QList<float> array2;
					if (controls2Definition.getFactoryArrayNode(scontrol->node, array2))
						if (array.count() == 1 && array2.count() == 1)
							slider->setDefaultAndFactoryValueFloat(array[0], array2[0]);
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
				QList<float> array2;
				QList<QPointF> points;
				QList<QPointF> defaultPoints;
				QList<QPointF> factoryPoints;
				if (controls2Definition.getArrayNode(scontrol->node, array))
					for (int i = 0; i < array.size(); i++)
					{
						points.push_back(QPointF(i, array[i]));
						defaultPoints.push_back(QPointF(i, array[i]));
					}
				if (controls2Definition.getFactoryArrayNode(scontrol->node, array2))
					for (int i = 0; i < array2.size(); i++)
						factoryPoints.push_back(QPointF(i, array2[i]));

				float gridX = 1.0f;
				if (array.size() >= 100)
					gridX = 20;
				chart->initialize(0, array.size() - 1, scontrol->y1, scontrol->y2, gridX, scontrol->gridY, points);
				chart->initializeDefaultAndFactoryPoints(defaultPoints, factoryPoints);
			}
		}
	}
}

void MainWindow::timerEvent(QTimerEvent* /* event */)
{
	// int diff = this->elapsedTimer.elapsed() - this->lastTime;

	this->ui->fpsLabel->setText("FPS: " + QString::number(this->thread->readFps));

	if (this->setFpsTime > 0 && this->elapsedTimer.elapsed() - this->setFpsTime >= 1000)
	{
		this->setFpsTime = 0;
		// this->setFps(this->lastSetFps);
		ispControl.setParam("", IF_S_FPS, "fps", this->lastSetFps, 1);

		this->lastTime = this->elapsedTimer.elapsed();
	}
}

void MainWindow::onActivated()
{
	if (this->isActivated)
		return;

	this->isActivated = true;
}

void MainWindow::onClose()
{
	this->stopProcFsThread();

	this->killGStreamerProcess();
}

bool MainWindow::event(QEvent *e)
{
	if (e->type() == QEvent::WindowActivate)
		this->onActivated();
	else if (e->type() == QEvent::Close)
		this->onClose();
	return QWidget::event(e);
}

void MainWindow::on_saveButton_clicked()
{
	this->stopProcFsThread();

	controls2Definition.saveXml();

	this->killGStreamerProcess();
	this->reloadDriver();
	this->createGStreamerProcess();

	this->runProcFsThread();
}

void MainWindow::reloadDriver()
{
	QProcess *process = new QProcess(this);
	QString program = "systemctl";
	process->start(program, QStringList({"restart", "imx8-phycam-isp.service"}));
	process->waitForFinished(1000);

	this->setFpsTime = this->elapsedTimer.elapsed();
}

void MainWindow::signal_update_slider_control_int(SliderWidget *slider, int value)
{
	this->canUpdateControls = false;
	slider->setValue(value);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_slider_control_float(SliderWidget *slider, float value)
{
	this->canUpdateControls = false;
	slider->setValueFloat(value);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_comboBox_item_index(ComboBoxWidget *comboBox, int index)
{
	this->canUpdateControls = false;
	comboBox->setItemIndex(index);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_checkBox_set_state(CheckBoxWidget *checkBox, bool state)
{
	this->canUpdateControls = false;
	checkBox->setState((bool)state);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_label_set_text(LabelWidget *label, QString text)
{
	this->canUpdateControls = false;
	label->setText(text);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_chart(ChartWidget *chart, float x1, float x2, float y1, float y2, float gridX, float gridY, QList<QPointF> points)
{
	this->canUpdateControls = false;
	chart->initialize(x1, x2, y1, y2, gridX, gridY, points);
	this->canUpdateControls = true;
}

/*

	to do:
+ thread
+ reset to initial/default
+ AE parameters
+ FPS
- rest controls
- presets

*/
