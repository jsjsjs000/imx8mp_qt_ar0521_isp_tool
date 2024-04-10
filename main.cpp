// #include "isp_xml.h"
#include "mainwindow.h"
#include "preview_window.h"

#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>

int main(int argc, char *argv[])
{
/*
	const QString XmlFileName = "/opt/imx8-isp/bin/xml/VM-017-COL_AO062-C_1920x1080.xml";
	IspXml xml;
	xml.openXmlFile(XmlFileName); // $$ if

	QDomNode n = xml.getElementByTagPath("sensor/AWB/globals/cell/afRg1");
	if (!n.isNull())
	{
		// qDebug() << "result" << n.nodeName() << n.toElement().attribute("size") << IspXml::stripText(n.toElement().text());
		qDebug() << IspXml::stripText(n.toElement().text());
		QList<float> array;
		if (IspXml::parseArrayFloat(n.toElement().text(), array))
		{
			for (const float &f : qAsConst(array))
				qDebug() << f;
		}

		// array.clear();
		// for (int i = 0; i < 16; i++)
		// 	array.push_back(1.23f);
		QString s = IspXml::arrayFloatToString(array);
		qDebug() << s;
	}

	QDomNode n = xml.getElementByTagPath("header/creator");
	if (!n.isNull())
	{
		qDebug() << "result" << n.nodeName() << n.toElement().attribute("type") << IspXml::stripText(n.toElement().text());
		IspXml::setValue(n, "jsjs aa", "header/creator");
	}

	xml.saveXmlFile(XmlFileName); // $$ if

	return 0;
*/

	qDebug() << "Debug: Start";
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
	const int MainWindowWidth = 350;
	const int WindowBorder = 5;
	const QSize desktopSize = QGuiApplication::primaryScreen()->size();

/**/
	PreviewWindow previewWindow;
	previewWindow.show();
	previewWindow.setGeometry(MainWindowWidth + 2 * WindowBorder, DesktopTaskBarHeight,
			desktopSize.width() - MainWindowWidth - 4 * WindowBorder,
			desktopSize.height() - 2 * DesktopTaskBarHeight);
	// previewWindow.resize(40, 40);
	// previewWindow.setGeometry(MainWindowWidth + 2 * WindowBorder, DesktopTaskBarHeight,200,200);
														// desktopSize.width() - MainWindowWidth - 4 * WindowBorder, //previewWindow.geometry().height());
														// desktopSize.height() - 2 * DesktopTaskBarHeight);

	MainWindow mainWindow;
	// mainWindow.setGeometry(0, DesktopTaskBarHeight,
			// MainWindowWidth + 2 * WindowBorder, desktopSize.height() - 2 * DesktopTaskBarHeight);
	mainWindow.showMaximized();

	return application.exec();
}
