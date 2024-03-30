#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
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
	void on_pushButton_clicked();

private:
	Ui::MainWindow *ui;
	QMediaPlayer *player;
	QMediaPlaylist *playlist;
	QVideoWidget *videoWidget;
	// static IspControl ispControl;

	void createControls();
	static void onCheckBoxChanged(QString type, QString parameter, bool value);
	static void onSliderValueChange(QString type, QString parameter, int value, int divide);
};
#endif // MAINWINDOW_H
