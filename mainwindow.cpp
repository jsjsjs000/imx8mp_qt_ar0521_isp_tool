#include <QVideoWidget>
#include <QMediaPlayer>
#include <QSpacerItem>
#include <QProcess>
#include <QThread>
#include <QStringBuilder>

#include "command_item.h"
#include "controls_definitions.h"
#include "qscreen.h"
#include "screenshots/screenshot_checker.h"
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
#include "v4l2_isp/isp_proc_thread.h"
#include "screenshots//rename_screenshot_window.h"
#include "presets/preset_rename_dialog.h"

IspControl ispControl;
IspProcThread *ispProcThread;
ControlsDefinitions controlsDefinition;
Controls2Definitions controls2Definition;
ScreenshotChecker *screenshotChecker;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & (~Qt::WindowMaximizeButtonHint));  /// remove maximize box

	ui->saveButton->setVisible(false);

	controlsDefinition.init();
	controlsDefinition.initSaveControls();
	controlsDefinition.getControlsCount();
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

	int video = ispControl.openVideo();

	controls2Definition.readXml();
	this->updateControls2fromXml();

	if (!presetsV4L2.loadPresetsList(ui->presetComboBox))
		qDebug() << "Can't load presets from disk.";

	QString defaultPreset = "";
	if (PresetV4l2Isp::loadDefaultPreset(&defaultPreset))
	{
		for (int i = 0; i < ui->presetComboBox->count(); i++)
			if (ui->presetComboBox->itemText(i) == defaultPreset)
			{
				ui->presetComboBox->setCurrentIndex(i);
				break;
			}
	}
	else if (ui->presetComboBox->count() > 0)
		ui->presetComboBox->setCurrentIndex(0);

	this->canUpdateControls = true;

	if (!video)
	{
		this->elapsedTimer.start();
		this->lastTime = this->elapsedTimer.elapsed();
		timerId = startTimer(500);

		this->runProcFsThread();
	}

	screenshotChecker = new ScreenshotChecker(this);
	connect(screenshotChecker, &ScreenshotChecker::signal_show_rename_screenshot_window,
			this, &MainWindow::slot_show_rename_screenshot_window);
	screenshotChecker->start();

	if (ui->presetComboBox->count() > 0)
		on_presetComboBox_currentIndexChanged(ui->presetComboBox->currentIndex());
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
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "|" + scontrol->parameter];
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
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "|" + scontrol->parameter];
			if (slider != nullptr && scontrol->setCmd == IF_S_FPS)
				slider->setValue(fps);
		}
}

void MainWindow::runProcFsThread()
{
	ispProcThread = new IspProcThread(this, ispControl, controlsDefinition, this->widgets);
	connect(ispProcThread, &IspProcThread::signal_update_slider_control_int,         this, &MainWindow::slot_update_slider_control_int);
	connect(ispProcThread, &IspProcThread::signal_update_slider_control_float,       this, &MainWindow::slot_update_slider_control_float);
	connect(ispProcThread, &IspProcThread::signal_update_slider_array_control_int,   this, &MainWindow::slot_update_slider_array_control_int);
	connect(ispProcThread, &IspProcThread::signal_update_slider_array_control_float, this, &MainWindow::slot_update_slider_array_control_float);
	connect(ispProcThread, &IspProcThread::signal_update_comboBox_item_index,        this, &MainWindow::slot_update_comboBox_item_index);
	connect(ispProcThread, &IspProcThread::signal_update_comboBox2_item_index,       this, &MainWindow::slot_update_comboBox2_item_index);
	connect(ispProcThread, &IspProcThread::signal_update_checkBox_set_state,         this, &MainWindow::slot_update_checkBox_set_state);
	connect(ispProcThread, &IspProcThread::signal_update_label_set_text,             this, &MainWindow::slot_update_label_set_text);
	connect(ispProcThread, &IspProcThread::signal_update_chart,                      this, &MainWindow::slot_update_chart);
	connect(ispProcThread, &IspProcThread::signal_update_matrix_view,                this, &MainWindow::slot_update_matrix_view);
	connect(this,          &MainWindow::signal_getParams,                   ispProcThread, &IspProcThread::slot_getParams);
	connect(this,          &MainWindow::signal_getParamsDiff,               ispProcThread, &IspProcThread::slot_getParamsDiff);
	connect(this,          &MainWindow::signal_setParams,                   ispProcThread, &IspProcThread::slot_setParams);
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
	int previewWidth = this->PreviewWidth;
	int previewHeight = this->PreviewHeight;
	// qDebug() << this->Width / (float)this->Height << previewWidth / (float)previewHeight;
	if (this->Width / (float)this->Height > this->PreviewWidth / (float)this->PreviewHeight)
		previewHeight = PreviewWidth * this->Height / this->Width;
	else
		previewWidth = PreviewHeight * this->Width / this->Height;
	// qDebug() << previewWidth << previewHeight;

	QProcess *process = new QProcess(this);
	QString program = "gst-launch-1.0";
	QString caps = "video/x-raw,format=YUY2,width=" + QString::number(this->Width) +
			",height=" + QString::number(this->Height) +
			",framerate=" + QString::number(this->InitialFps) + "/1";
	process->start(program, QStringList({
			"v4l2src", "device=/dev/video0", "!",
			caps, "!", "queue", "!",
			"waylandsink", "window-width=" + QString::number(previewWidth), "window-height=" + QString::number(previewHeight)}));
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
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), checkBox);
			ui->verticalLayout_1->addWidget(checkBox);
			checkBox->setState(scontrol->checked);
		}
		else if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = new SliderWidget(this, this, scontrol, &MainWindow::onSliderValueChange);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), slider);
			ui->verticalLayout_1->addWidget(slider);
		}
		else if (const SliderArrayControl *scontrol = dynamic_cast<const SliderArrayControl*>(control))
		{
			SliderArrayWidget *slider = new SliderArrayWidget(this, this, scontrol, &MainWindow::onSliderArrayValueChange);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), slider);
			ui->verticalLayout_1->addWidget(slider);
		}
		else if (const ButtonControl *scontrol = dynamic_cast<const ButtonControl*>(control))
		{
			ButtonWidget *button = new ButtonWidget(this, this, scontrol, &MainWindow::onButtonClicked);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), button);
			ui->verticalLayout_1->addWidget(button);
		}
		else if (const LabelControl *scontrol = dynamic_cast<const LabelControl*>(control))
		{
			LabelWidget *label = new LabelWidget(this, this, scontrol);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), label);
			ui->verticalLayout_1->addWidget(label);
		}
		else if (const ComboBoxControl *scontrol = dynamic_cast<const ComboBoxControl*>(control))
		{
			ComboBoxWidget *comboBox = new ComboBoxWidget(this, this, scontrol, &MainWindow::onComboBoxIndexChanged);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), comboBox);
			ui->verticalLayout_1->addWidget(comboBox);
		}
		else if (const ComboBoxControl2 *scontrol = dynamic_cast<const ComboBoxControl2*>(control))
		{
			ComboBoxWidget2 *comboBox = new ComboBoxWidget2(this, this, scontrol, &MainWindow::onComboBox2IndexChanged);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), comboBox);
			ui->verticalLayout_1->addWidget(comboBox);
		}
		else if (const ChartControl *scontrol = dynamic_cast<const ChartControl*>(control))
		{
			ChartWidget *chart = new ChartWidget(this, this, scontrol, &MainWindow::onChartControlPointsChanged);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), chart);
			ui->verticalLayout_1->addWidget(chart);
		}
		else if (const MatrixViewControl *scontrol = dynamic_cast<const MatrixViewControl*>(control))
		{
			MatrixViewWidget *matrix = new MatrixViewWidget(this, this, scontrol);
			this->widgets.insert(QString(scontrol->setCmd + "|" + scontrol->parameter), matrix);
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
	this->ui->fpsLabel->setText(
			"FPS: " + QString::number(ispProcThread->fps) + ", " +
			"ISO: " + QString::number(ispProcThread->iso) + ", " +
			"mean luminance: " + QString::number(ispProcThread->meanLuminance) + ",\n" +
			"exposure time: " + QString::number((int)(ispProcThread->exposureTime * 1000)) + " ms, " +
			"gain: " + QString::number(ispProcThread->exposureGain, 'f', 2) + ", " +
			"integration time: " + QString::number((int)(ispProcThread->integrationTime * 1000)) + " ms");
			// ispProcThread->GetStatus()

		/* set initial FPS at startup */
	if (this->setFpsTime > 0 && this->elapsedTimer.elapsed() - this->setFpsTime >= 1000)
	{
		this->setFpsTime = 0;
		ispControl.setParamNumber("", IF_S_FPS, "fps", this->lastSetFps, 1);

		this->lastTime = this->elapsedTimer.elapsed();
	}

	// QString s = emit this->signal_getParamsDiff();
	// qDebug().noquote() << s;
	// qDebug() << "";
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
	on_presetSaveButton_clicked();

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

void MainWindow::slot_update_slider_control_int(SliderWidget *slider, int value)
{
	this->canUpdateControls = false;
	slider->setValue(value);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_slider_control_float(SliderWidget *slider, float value)
{
	this->canUpdateControls = false;
	slider->setValueFloat(value);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_slider_array_control_int(SliderArrayWidget *slider, QList<int> values)
{
	this->canUpdateControls = false;
	slider->setValues(values);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_slider_array_control_float(SliderArrayWidget *slider, QList<float> values)
{
	this->canUpdateControls = false;
	slider->setValuesFloats(values);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_comboBox_item_index(ComboBoxWidget *comboBox, int index)
{
	this->canUpdateControls = false;
	comboBox->setItemIndex(index);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_comboBox2_item_index(ComboBoxWidget2 *comboBox, QString index)
{
	this->canUpdateControls = false;
	comboBox->setItemIndex(index);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_checkBox_set_state(CheckBoxWidget *checkBox, bool state)
{
	this->canUpdateControls = false;
	checkBox->setState((bool)state);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_label_set_text(LabelWidget *label, QString text)
{
	this->canUpdateControls = false;
	label->setText(text);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_chart(ChartWidget *chart, float x1, float x2, float y1, float y2, float gridX, float gridY, QList<QPointF> points)
{
	this->canUpdateControls = false;
	chart->initialize(x1, x2, y1, y2, gridX, gridY, points);
	this->canUpdateControls = true;
}

void MainWindow::slot_update_matrix_view(MatrixViewWidget *matrixView, QList<QPointF> points)
{
	this->canUpdateControls = false;
	matrixView->setPoints(points);
	this->canUpdateControls = true;
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
	this->ui->saveButton->setVisible(index == 1);

	qDebug() << "tab";

	canUpdateControls = false;
	if (index == 0)
	{
		ui->presetComboBox->clear();
		if (!presetsV4L2.loadPresetsList(ui->presetComboBox))
			qDebug() << "Can't load presets from disk.";
		QString defaultPreset = "";
		if (PresetV4l2Isp::loadDefaultPreset(&defaultPreset))
		{
			for (int i = 0; i < ui->presetComboBox->count(); i++)
				if (ui->presetComboBox->itemText(i) == defaultPreset)
				{
					ui->presetComboBox->setCurrentIndex(i);
					break;
				}
		}
	}
	else if (index == 1)
	{
		ui->presetComboBox->clear();
		if (!presetsXmls.loadPresetsList(ui->presetComboBox))
			qDebug() << "Can't load XML presets from disk.";
	}
	canUpdateControls = true;
}

void MainWindow::slot_show_rename_screenshot_window(QString filename)
{
	qDebug() << "screenshot" << filename;
	this->displayRenameScreenshotWindow(filename);
}

void MainWindow::displayRenameScreenshotWindow(QString filename)
{
	RenameScreenshotWindow *renameScreenshotWindow = new RenameScreenshotWindow;
	const QSize desktopSize = QGuiApplication::primaryScreen()->size();
	QRect screenGeometry = renameScreenshotWindow->geometry();
	renameScreenshotWindow->setGeometry(
			desktopSize.width() / 2 - screenGeometry.width() / 2,
			desktopSize.height() / 2 - screenGeometry.height() / 2,
			screenGeometry.width(), screenGeometry.height());
	renameScreenshotWindow->setParameters(filename);

	connect(renameScreenshotWindow, &RenameScreenshotWindow::signal_addFilenameToCachedFilesList,
			screenshotChecker, &ScreenshotChecker::slot_addFilenameToCachedFilesList);

	renameScreenshotWindow->show();
}

void MainWindow::on_presetComboBox_currentIndexChanged(int index)
{
	if (!canUpdateControls)
		return;

		/* V4L2 */
	if (this->ui->tabWidget->currentIndex() == 0)
	{
		QString name = this->ui->presetComboBox->itemText(index);
		qDebug() << "load preset" << name;
		QMap<QString, QString> params;
		if (!presetsV4L2.load(name, &params))
			return;

		emit signal_setParams(&params);

		canUpdateControls = false;
		QString keyFps = QString(IF_S_FPS) % QString("|fps");
		for (auto it = params.keyValueBegin(); it != params.keyValueEnd(); ++it)
			if (it->first == keyFps)
			{
				bool ok;
				int fps = it->second.toInt(&ok);
				if (ok)
				{
					if (this->widgets.contains(keyFps))
					{
						SliderWidget *slider = (SliderWidget*)this->widgets[keyFps];
						slider->setValue(fps);
					}
				}
			}
		canUpdateControls = true;

		PresetV4l2Isp::saveDefaultPreset(name);
	}
		/* XML */
	else if (this->ui->tabWidget->currentIndex() == 1)
	{
	}
}

void MainWindow::on_presetSaveButton_clicked()
{
	QString name = "";
		/* V4L2 */
	if (this->ui->tabWidget->currentIndex() == 0)
	{
			/* None preset exists */
		if (this->ui->presetComboBox->currentIndex() < 0)
		{
			name = this->showRenamePresetDialog("Save preset", "Preset name:", nullptr);
			if (name == nullptr)
				return;
		}
		else
			name = this->ui->presetComboBox->itemText(this->ui->presetComboBox->currentIndex());

		QString params = emit this->signal_getParams();

		QString keyFps = QString(IF_S_FPS) % QString("|fps");
		if (this->widgets.contains(keyFps))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[keyFps];
			params = params % "\n" % keyFps % QString("=") % QString::number(slider->getValue());
		}

		presetsV4L2.save(this->ui->presetComboBox, name, params);
	}
		/* XML */
	else if (this->ui->tabWidget->currentIndex() == 1)
	{
		/* None preset exists */
		if (this->ui->presetComboBox->currentIndex() < 0)
		{
			name = this->showRenamePresetDialog("Save preset", "Preset name:", nullptr);
			if (name == nullptr)
				return;
		}
		else
			name = this->ui->presetComboBox->itemText(this->ui->presetComboBox->currentIndex());

		presetsXmls.save(this->ui->presetComboBox, name);
	}
}

void MainWindow::on_presetRenameButton_clicked()
{
		/* V4L2 */
	if (this->ui->tabWidget->currentIndex() == 0)
	{
		if (this->ui->presetComboBox->currentIndex() >= 0)
		{
			QString oldName = this->ui->presetComboBox->currentText();
			QString name = showRenamePresetDialog("Rename preset", "New preset name:", oldName);
			if (name == nullptr)
				return;

			presetsV4L2.rename(this->ui->presetComboBox, name);
		}
	}
		/* XML */
	else if (this->ui->tabWidget->currentIndex() == 1)
	{
		QString oldName = this->ui->presetComboBox->currentText();
		QString name = showRenamePresetDialog("Rename preset", "New preset name:", oldName);
		if (name == nullptr)
			return;

		presetsXmls.rename(this->ui->presetComboBox, name);
	}
}

void MainWindow::on_presetNewButton_clicked()
{
		/* V4L2 */
	if (this->ui->tabWidget->currentIndex() == 0)
	{
		QString name = this->showRenamePresetDialog("Add preset", "Preset name:", nullptr);
		if (name == nullptr)
			return;

		QString params = emit this->signal_getParams();
		presetsV4L2.add(this->ui->presetComboBox, name, params);
	}
		/* XML */
	else if (this->ui->tabWidget->currentIndex() == 1)
	{
		QString name = this->showRenamePresetDialog("Add preset", "Preset name:", nullptr);
		if (name == nullptr)
			return;

		presetsXmls.add(this->ui->presetComboBox, name);
	}
}

void MainWindow::on_presetDeleteButton_clicked()
{
		/* V4L2 */
	if (this->ui->tabWidget->currentIndex() == 0)
	{
		presetsV4L2.deleteCurrent(this->ui->presetComboBox);
	}
		/* XML */
	else if (this->ui->tabWidget->currentIndex() == 1)
	{
		presetsXmls.deleteCurrent(this->ui->presetComboBox);
	}
}

QString MainWindow::getSafeFilename(QString s)
{
	return s.replace('/', '_');
}

QString MainWindow::showRenamePresetDialog(QString windowTitle, QString labelText, QString value)
{
	PresetRenameDialog *presetRenameForm = new PresetRenameDialog;
	const QSize desktopSize = QGuiApplication::primaryScreen()->size();
	QRect screenGeometry = presetRenameForm->geometry();
	presetRenameForm->setGeometry(
			desktopSize.width() / 2 - screenGeometry.width() / 2,
			desktopSize.height() / 2 - screenGeometry.height() / 2,
			screenGeometry.width(), screenGeometry.height());
	presetRenameForm->setParameters(windowTitle, labelText, value);
	presetRenameForm->setModal(true);
	if (presetRenameForm->exec() == 0)
		return nullptr;

	QString name = presetRenameForm->getName();
	name = getSafeFilename(name);
	return name;
}
