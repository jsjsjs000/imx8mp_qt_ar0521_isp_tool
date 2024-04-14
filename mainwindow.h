#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "isp_proc_thread.h"

#include <QList>
#include <QPointF>
#include <QMainWindow>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_saveButton_clicked();
	bool event(QEvent *e);
	void signal_update_slider_control_int(SliderWidget *slider, int value);
	void signal_update_slider_control_float(SliderWidget *slider, float value);
	void signal_update_comboBox_item_index(ComboBoxWidget *comboBox, int index);
	void signal_update_checkBox_set_state(CheckBoxWidget *checkBox, bool state);
	void signal_update_label_set_text(LabelWidget *label, QString text);
	void signal_update_chart(ChartWidget *chart, float x1, float x2, float y1, float y2, float gridX, float gridY, QListQPointF points);

private:
	Ui::MainWindow *ui;
	IspProcThread *thread;
	QMediaPlayer *player;
	QMediaPlaylist *playlist;
	QVideoWidget *videoWidget;
	QMap<QString, QWidget*> widgets;
	QMap<QString, QWidget*> widgets2;
	bool canUpdateControls = false;
	int timerId;
	clock_t lastTime = 0;
	bool readyForReadJson = false;
	bool isActivated = false;
	QElapsedTimer elapsedTimer;

	void runProcFsThread();
	void stopProcFsThread();
	void createControls();
	void createControls2();
	void readParameters();
	void updateControlsFromJson(Json::Value json, QString type);
	void updateControls2fromXml();
	void killGStreamerProcess();
	void createGStreamerProcess();
	void onActivated();
	void onClose();
	void reloadDriver();

	static void onCheckBoxChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, bool value);
	static void onSliderValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide);
	static void onComboBoxIndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value);
	static void onButtonClicked(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value);
	static void onChartControlPointsChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<QPointF> points);

	static void onSlider2ValueChange(MainWindow *mainWindow, QString node, int value, int divide);
	static void onChartControl2PointsChanged(MainWindow *mainWindow, QString node, QList<QPointF> points);
	void timerEvent(QTimerEvent *event);
};
#endif // MAINWINDOW_H
