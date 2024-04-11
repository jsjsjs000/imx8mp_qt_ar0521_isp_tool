#include "mainwindow.h"
// #include "preview_window.h"

#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	/* QMediaPlayer *player;
	QMediaPlaylist *playlist;
	QVideoWidget *videoWidget;

	player = new QMediaPlayer;

	// playlist = new QMediaPlaylist(player);
	//player->setMedia(QUrl("https://storage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4"));
	// playlist->addMedia(QUrl("https://storage.googleapis.com/gtv-videos-bucket/sample/ElephantsDream.mp4"));
	// player->setMedia(QUrl("/opt/SampleVideo_720x480_10mb.mp4"));
	// playlist->addMedia(QUrl("/opt/SampleVideo_720x480_10mb.mp4"));
	player->setMedia(QUrl("/opt/qt12/bin/file_example_MP4_640_3MG.mp4"));
	// player->setMedia(QUrl("file_example_MP4_640_3MG.mp4"));
	// player->setMedia(QUrl("/dev/video0"));

	videoWidget = new QVideoWidget;
	player->setVideoOutput(videoWidget);

	videoWidget->setGeometry(20, 20, 1000, 800);
	videoWidget->show();
	// playlist->setCurrentIndex(1);
	player->play(); */

	/* QImage myImage;
	myImage.load("/opt/qt12/bin/test.jpeg");
	QLabel myLabel;
	myLabel.setPixmap(QPixmap::fromImage(myImage));
	myLabel.show(); */

	const int DesktopTaskBarHeight = 32;
	const int MainWindowWidth = 360;
	const int WindowBorder = 5;
	const QSize desktopSize = QGuiApplication::primaryScreen()->size();

/*
	PreviewWindow previewWindow;
	previewWindow.show();
	previewWindow.setGeometry(MainWindowWidth + 2 * WindowBorder, DesktopTaskBarHeight,
			desktopSize.width() - MainWindowWidth - 4 * WindowBorder,
			desktopSize.height() - 2 * DesktopTaskBarHeight);
	// previewWindow.resize(40, 40);
	// previewWindow.setGeometry(MainWindowWidth + 2 * WindowBorder, DesktopTaskBarHeight,200,200);
														// desktopSize.width() - MainWindowWidth - 4 * WindowBorder, //previewWindow.geometry().height());
														// desktopSize.height() - 2 * DesktopTaskBarHeight);
*/

	MainWindow mainWindow;
	mainWindow.setGeometry(0, DesktopTaskBarHeight,
			MainWindowWidth + 2 * WindowBorder, desktopSize.height() - 2 * DesktopTaskBarHeight);
	mainWindow.show();

	// mainWindow.showMaximized();

	return application.exec();
}

/*

Projekty > ARM_PD_22.1.1 > Uruchamianie > Środowisko:
	DISPLAY=:0
	GST_V4L2_USE_LIBV4L2=1

Projekty > ARM_PD_22.1.1 > Uruchamianie > Środowisko:
	DISPLAY=:0
	GDK_BACKEND=X11
	GST_V4L2_USE_LIBV4L2=1
	QT_QPA_PLATFORM=xcb

Projekty > ARM_PD_22.1.1 > Uruchamianie > Środowisko: (nie działa wideo)
	DISPLAY=:0:0
	GST_V4L2_USE_LIBV4L2=1
	QT_QPA_PLATFORM=wayland

	run in console:
export DISPLAY=:0
export GST_V4L2_USE_LIBV4L2=1
/opt/imx8mp_qt_ar0521_isp_tool/bin/imx8mp_qt_ar0521_isp_tool

	run in console:
DISPLAY=:0
GDK_BACKEND=X11
GST_V4L2_USE_LIBV4L2=1
QT_QPA_PLATFORM=xcb
/opt/imx8mp_qt_ar0521_isp_tool/bin/imx8mp_qt_ar0521_isp_tool

	run in console (is video working?):
export DISPLAY=:0:0
export GST_V4L2_USE_LIBV4L2=1
export QT_QPA_PLATFORM=wayland
/opt/imx8mp_qt_ar0521_isp_tool/bin/imx8mp_qt_ar0521_isp_tool

	install:
https://gstreamer.freedesktop.org/documentation/installing/on-linux.html?gi-language=c
apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio

	get libraries list:
pd22.1.1
pkg-config --cflags --libs gstreamer-video-1.0


https://stackoverflow.com/questions/76201961/can-not-display-gstreamer-inside-widget-qt-wayland

https://www.nxp.com/docs/en/user-guide/IMX_LINUX_USERS_GUIDE.pdf
7.3.1.6.3 Multiple videos overlay

*/
