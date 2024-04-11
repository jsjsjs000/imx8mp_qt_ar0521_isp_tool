#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QElapsedTimer>
#include "isp_control.h"

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

private:
	Ui::MainWindow *ui;
	QMediaPlayer *player;
	QMediaPlaylist *playlist;
	QVideoWidget *videoWidget;
	QMap<QString, QWidget*> widgets;
	QMap<QString, QWidget*> widgets2;
	bool canUpdateControls = false;
	int timerId;
	clock_t lastTime = 0;
	bool notReadableControlsInitialized = false;
	bool readyForReadJson = false;
	bool isActivated = false;
	QElapsedTimer elapsedTimer;

	void createControls();
	void createControls2();
	void readParameters();
	void updateControlsFromJson(Json::Value json, QString type);
	void updateControls2fromXml();
	void initializeControlsNotReadable(QString type);
	void killGStreamerProcess();
	void createGStreamerProcess();
	void onActivated();

	static void onCheckBoxChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, bool value);
	static void onSliderValueChange(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int value, int divide);
	static void onComboBoxIndexChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value);
	static void onButtonClicked(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString value);
	static void onChartControlPointsChanged(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter);

	static void onSlider2ValueChange(MainWindow *mainWindow, QString node, int value, int divide);
	static void onChartControl2PointsChanged(MainWindow *mainWindow, QString node);
	void timerEvent(QTimerEvent *event);
};
#endif // MAINWINDOW_H
