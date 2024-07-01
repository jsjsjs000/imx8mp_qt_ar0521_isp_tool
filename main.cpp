#include "mainwindow.h"
// #include "preview_window.h"

#include <QApplication>
#include <QGuiApplication>
//#include <QDesktopWidget>
#include <QScreen>

void playVideo()
{
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
￼

	myImage.load("/opt/qt12/bin/test.jpeg");
	QLabel myLabel;
	myLabel.setPixmap(QPixmap::fromImage(myImage));
	myLabel.show(); */
}

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	// qDebug() << argc << argv[1];

	const int DesktopTaskBarHeight = 32;
	const int MainWindowWidth = 360;
	const int WindowBorder = 5;
	const QSize desktopSize = QGuiApplication::primaryScreen()->size();

	MainWindow mainWindow;
	mainWindow.setGeometry(0, DesktopTaskBarHeight,
			MainWindowWidth + 2 * WindowBorder, desktopSize.height() - 2 * DesktopTaskBarHeight);
	mainWindow.show();

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

	Gamma correction
gamma correction equation: y = 1024 * (x / (16 + 1))) ^ 0.7
https://desmos.com/calculator y=1024\cdot\left(\frac{x}{16+1}\right)^{0.7}

	Fix camera Phytec VM-017 colors (black color on low luminacance)
nano /opt/imx8-isp/bin/xml/VM-017-COL_AO062-C_1920x1080.xml
# ----------------------------------------
<sensor/CC/cell[index=2]/ccMatrix>:
	[2.47656 -0.90625 0.0546875 -0.492188 2.125 -0.609375 -0.03125 -0.671875 1.95146] // option 1
	[2.47656 -0.90625 -0.0582523 -0.492188 2.125 -0.46875 -0.03125 -0.679688 1.89063] // option 2
	[2.46875 -0.90625 0.203125 -0.320313 2.125 -0.523438 0.0546875 -0.695313 2.03884] // option 3 - newest
<sensor/CC/cell[index=2]/ccOffsets>:
	[-59 -150 -59]

<sensor/DEGAMMA/cell/degamma_dx>: 0.75
	[0.000000 537.261288 903.562145 1224.690783 1519.604274 1796.441185 2059.676093 2312.112773 2555.659468 2791.691152 3021.241781 3245.114244 3463.948344 3678.264729 3888.494524 4095.000000]
# ----------------------------------------

*/
