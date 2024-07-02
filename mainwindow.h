#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "isp_proc_thread.h"

#include <QList>
#include <QPointF>
#include <QMainWindow>
#include <QVideoWidget>
#include <QMediaPlayer>
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
	void slot_update_slider_control_int(SliderWidget *slider, int value);
	void slot_update_slider_control_float(SliderWidget *slider, float value);
	void slot_update_slider_array_control_int(SliderArrayWidget *slider, QList<int> values);
	void slot_update_slider_array_control_float(SliderArrayWidget *slider, QList<float> values);
	void slot_update_comboBox_item_index(ComboBoxWidget *comboBox, int index);
	void slot_update_comboBox2_item_index(ComboBoxWidget2 *comboBox, QString index);
	void slot_update_checkBox_set_state(CheckBoxWidget *checkBox, bool state);
	void slot_update_label_set_text(LabelWidget *label, QString text);
	void slot_update_chart(ChartWidget *chart, float x1, float x2, float y1, float y2, float gridX, float gridY, QListQPointF points);
	void slot_update_matrix_view(MatrixViewWidget *matrixView, QList<QPointF> points);

	void on_tabWidget_currentChanged(int index);

	void slot_show_rename_screenshot_window(QString filename);

private:
	const int InitialFps = 30;
	const int Width = 1920;  // 1.777
	const int Height = 1080;
	// const int Width = 2592;  // 1.333
	// const int Height = 1944;
	// const int PreviewWidth = 1540;
	// const int PreviewHeight = 1050;
	const int PreviewWidth = 1920;
	const int PreviewHeight = 1080;

	Ui::MainWindow *ui;
	//QMediaPlayer *player;
	//QMediaPlaylist *playlist;
	QVideoWidget *videoWidget;
	QMap<QString, QWidget*> widgets;
	QMap<QString, QWidget*> widgets2;
	bool canUpdateControls = false;
	int timerId;
	clock_t lastTime = 0;
	bool isActivated = false;
	QElapsedTimer elapsedTimer;
	int lastSetFps = this->InitialFps;
	clock_t setFpsTime = 0;

	int getFps();
	void setFps(int fps);
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
	void displayRenameScreenshotWindow(QString filename);

	static void onCheckBoxChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, bool value);
	static void onSliderValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide);
	static void onSliderArrayValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<float> values);
	static void onComboBoxIndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value);
	static void onComboBox2IndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString key, QString value);
	static void onButtonClicked(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value);
	static void onChartControlPointsChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<QPointF> points);

	static void onSlider2ValueChange(MainWindow *mainWindow, QString node, int value, int divide);
	static void onChartControl2PointsChanged(MainWindow *mainWindow, QString node, QList<QPointF> points);
	void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
