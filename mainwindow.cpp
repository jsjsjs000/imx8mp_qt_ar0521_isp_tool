#include <QVideoWidget>
#include <QMediaPlayer>
#include <QSpacerItem>
#include <QProcess>
#include <QThread>

#include "command_item.h"
#include "controls_definitions.h"
#include <widgets/chart_widget.h>
#include <widgets/checkbox_widget.h>
#include <widgets/combobox_widget.h>
#include <widgets/combobox_widget2.h>
#include <widgets/label_widget.h>
#include <widgets/slider_widget.h>
#include <widgets/slider_array_widget.h>
#include <widgets/button_widget.h>
#include <widgets/group_widget.h>
#include <widgets/matrix_view_widget.h>
#include "controls2_definitions.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "isp_proc_thread.h"

IspControl ispControl;
IspProcThread *ispProcThread;
ControlsDefinitions controlsDefinition;
Controls2Definitions controls2Definition;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->ui->saveButton->setVisible(false);

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
	ispProcThread = new IspProcThread(this, ispControl, controlsDefinition, this->widgets);
	connect(ispProcThread, &IspProcThread::signal_update_slider_control_int,         this, &MainWindow::signal_update_slider_control_int);
	connect(ispProcThread, &IspProcThread::signal_update_slider_control_float,       this, &MainWindow::signal_update_slider_control_float);
	connect(ispProcThread, &IspProcThread::signal_update_slider_array_control_int,   this, &MainWindow::signal_update_slider_array_control_int);
	connect(ispProcThread, &IspProcThread::signal_update_slider_array_control_float, this, &MainWindow::signal_update_slider_array_control_float);
	connect(ispProcThread, &IspProcThread::signal_update_comboBox_item_index,        this, &MainWindow::signal_update_comboBox_item_index);
	connect(ispProcThread, &IspProcThread::signal_update_comboBox2_item_index,       this, &MainWindow::signal_update_comboBox2_item_index);
	connect(ispProcThread, &IspProcThread::signal_update_checkBox_set_state,         this, &MainWindow::signal_update_checkBox_set_state);
	connect(ispProcThread, &IspProcThread::signal_update_label_set_text,             this, &MainWindow::signal_update_label_set_text);
	connect(ispProcThread, &IspProcThread::signal_update_chart,                      this, &MainWindow::signal_update_chart);
	connect(ispProcThread, &IspProcThread::signal_update_matrix_view,                this, &MainWindow::signal_update_matrix_view);
	ispProcThread->start();
}

void MainWindow::stopProcFsThread()
{
	ispProcThread->Stop = true;
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
	int PreviewWidth = 1540;
	int PreviewHeight = 1050;
	// qDebug() << this->Width / (float)this->Height << PreviewWidth / (float)PreviewHeight;
	if (this->Width / (float)this->Height > PreviewWidth / (float)PreviewHeight)
		PreviewHeight = PreviewWidth * this->Height / this->Width;
	else
		PreviewWidth = PreviewHeight * this->Width / this->Height;
	// qDebug() << PreviewWidth << PreviewHeight;

	QProcess *process = new QProcess(this);
	QString program = "gst-launch-1.0";
	QString caps = "video/x-raw,format=YUY2,width=" + QString::number(this->Width) +
			",height=" + QString::number(this->Height) +
			",framerate=" + QString::number(this->InitialFps) + "/1";
	process->start(program, QStringList({
			"v4l2src", "device=/dev/video0", "!",
			caps, "!", "queue", "!",
			"waylandsink", "window-width=" + QString::number(PreviewWidth), "window-height=" + QString::number(PreviewHeight)}));
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
		else if (const SliderArrayControl *scontrol = dynamic_cast<const SliderArrayControl*>(control))
		{
			SliderArrayWidget *slider = new SliderArrayWidget(this, this, scontrol, &MainWindow::onSliderArrayValueChange);
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
		else if (const ComboBoxControl2 *scontrol = dynamic_cast<const ComboBoxControl2*>(control))
		{
			ComboBoxWidget2 *comboBox = new ComboBoxWidget2(this, this, scontrol, &MainWindow::onComboBox2IndexChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), comboBox);
			ui->verticalLayout_1->addWidget(comboBox);
		}
		else if (const ChartControl *scontrol = dynamic_cast<const ChartControl*>(control))
		{
			ChartWidget *chart = new ChartWidget(this, this, scontrol, &MainWindow::onChartControlPointsChanged);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), chart);
			ui->verticalLayout_1->addWidget(chart);
		}
		else if (const MatrixViewControl *scontrol = dynamic_cast<const MatrixViewControl*>(control))
		{
			MatrixViewWidget *matrix = new MatrixViewWidget(this, this, scontrol);
			this->widgets.insert(QString(scontrol->setCmd + "/" + scontrol->parameter), matrix);
			ui->verticalLayout_1->addWidget(matrix);
		}
	}
}

void MainWindow::onCheckBoxChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, bool checked)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << int(checked);
	CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Bool, getCmd, setCmd, parameter, checked);
	ispProcThread->AddCommandToQueue(commandItem);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onSliderValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << ((float)value / divide);
	CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Number, getCmd, setCmd, parameter, value, divide);
	ispProcThread->AddCommandToQueue(commandItem);

	if (setCmd == IF_S_FPS && parameter == "fps")
		mainWindow->lastSetFps = value;

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onSliderArrayValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<float> values)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << values;
	CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Array, getCmd, setCmd, parameter, values);
	ispProcThread->AddCommandToQueue(commandItem);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onComboBoxIndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << key << "(" + value + ")";
	CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Number, getCmd, setCmd, parameter, key, 1);
	ispProcThread->AddCommandToQueue(commandItem);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onComboBox2IndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString key, QString value)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter << key << "(" + value + ")";
	CommandItem commandItem;
	if (key == "false")
		commandItem = CommandItem(CommandItem::CommandItemType::Bool, getCmd, setCmd, parameter, false);
	else if (key == "true")
		commandItem = CommandItem(CommandItem::CommandItemType::Bool, getCmd, setCmd, parameter, true);
	else
		commandItem = CommandItem(CommandItem::CommandItemType::String, getCmd, setCmd, parameter, key);
	ispProcThread->AddCommandToQueue(commandItem);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onButtonClicked(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value)
{
	if (!mainWindow->canUpdateControls)
		return;

	qDebug() << setCmd << parameter;
	CommandItem commandItem;
	if (strncmp(value.toStdString().c_str(), "true", strlen("true")) == 0)
		commandItem = CommandItem(CommandItem::CommandItemType::Bool, getCmd, setCmd, parameter, true);
	else if (strncmp(value.toStdString().c_str(), "false", strlen("false")) == 0)
		commandItem = CommandItem(CommandItem::CommandItemType::Bool, getCmd, setCmd, parameter, false);
	else
		commandItem = CommandItem(CommandItem::CommandItemType::String, getCmd, setCmd, parameter, value);
	ispProcThread->AddCommandToQueue(commandItem);

	mainWindow->lastTime = mainWindow->elapsedTimer.elapsed();
}

void MainWindow::onChartControlPointsChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<QPointF> points)
{
	if (!mainWindow->canUpdateControls)
		return;

	QList<float> array;
	for (int i = 0; i < points.count(); i++)
		array.push_back(points[i].y());
	qDebug() << setCmd << array;
	CommandItem commandItem;
	commandItem = CommandItem(CommandItem::CommandItemType::Array, getCmd, setCmd, parameter, array);
	ispProcThread->AddCommandToQueue(commandItem);

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
	this->ui->fpsLabel->setText("FPS: " + QString::number(ispProcThread->readFps) + ", " + ispProcThread->GetStatus());

		/* set initial FPS at startup */
	if (this->setFpsTime > 0 && this->elapsedTimer.elapsed() - this->setFpsTime >= 1000)
	{
		this->setFpsTime = 0;
		ispControl.setParamNumber("", IF_S_FPS, "fps", this->lastSetFps, 1);

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

void MainWindow::signal_update_slider_array_control_int(SliderArrayWidget *slider, QList<int> values)
{
	this->canUpdateControls = false;
	slider->setValues(values);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_slider_array_control_float(SliderArrayWidget *slider, QList<float> values)
{
	this->canUpdateControls = false;
	slider->setValuesFloats(values);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_comboBox_item_index(ComboBoxWidget *comboBox, int index)
{
	this->canUpdateControls = false;
	comboBox->setItemIndex(index);
	this->canUpdateControls = true;
}

void MainWindow::signal_update_comboBox2_item_index(ComboBoxWidget2 *comboBox, QString index)
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

void MainWindow::signal_update_matrix_view(MatrixViewWidget *matrixView, QList<QPointF> points)
{
	this->canUpdateControls = false;
	matrixView->setPoints(points);
	this->canUpdateControls = true;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	this->ui->saveButton->setVisible(index == 1);
}

/*

	to do:
+ thread
+ reset to initial/default
+ AE parameters
+ FPS
+ rest controls
- presets

*/
