#include "mainwindow.h"
#include "preview_window.h"

// #include <glib.h>
// #include <gst/gst.h>
// #include <gst/interfaces/xoverlay.h>

#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>

// #include <QVideoWidget>
// #include <QMediaPlayer>
// #include <QMediaPlaylist>
// #include <QCamera>
// #include <QCameraInfo>
// #include <QLabel>

// QMediaPlayer *player;
// QMediaPlaylist *playlist;
// QVideoWidget *videoWidget;

int main(int argc, char *argv[])
{
	qDebug() << "Debug: Start";
	QApplication a(argc, argv);

	/* player = new QMediaPlayer;

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

	const QSize desktopSize = QGuiApplication::primaryScreen()->size();

	const int MainWindowWidth = 340;

	PreviewWindow previewWindow;
	previewWindow.setGeometry(MainWindowWidth, QGuiApplication::primaryScreen()->geometry().top(),
														desktopSize.width() - MainWindowWidth, previewWindow.geometry().height());
	previewWindow.show();

	MainWindow w;
	w.setGeometry(2, 100, w.geometry().width(), w.geometry().height());
	w.show();

	return a.exec();
}
