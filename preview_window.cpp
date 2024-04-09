#include "preview_window.h"
#include "ui_preview_window.h"

// #include <glib.h>
#include <gst/gst.h>
#include <gst/gsttaskpool.h>
#include <gst/video/video.h>
#include <gst/video/videooverlay.h>
// /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/gstreamer1.0-libav/1.20.7-r0/recipe-sysroot/usr/include/gstreamer-1.0/gst/video
// /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/qtmultimedia/5.15.7+gitAUTOINC+eeb34aae03-r0/git/src/gsttools/
// /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/qtmultimedia/5.15.7+gitAUTOINC+eeb34aae03-r0/recipe-sysroot/usr/include/gstreamer-1.0/gst/video

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QCamera>
#include <QCameraInfo>
#include <QLabel>
#include <QSizePolicy>
#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QScreen>

PreviewWindow::PreviewWindow(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::PreviewWindow)
{
	ui->setupUi(this);
}

void PreviewWindow::setupCamera(void)
{
	const QString AllowedCameraDevice = QString("/dev/video0");
	const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	QCamera *camera = nullptr;
	// for (const QCameraInfo &info : cameras)
	// 	if (info.deviceName().compare(AllowedCameraDevice) == 0)
	// 	{
	// 		camera = new QCamera(info);
	// 		qDebug() << info.deviceName() << Qt::endl;
	// 	}

	camera = new QCamera(QCameraInfo::defaultCamera());
	if (camera == nullptr)
	{
		qInfo() << "Any camera not found.\n";
		return;
	}

	int w = 1700;
	int h = w * 1080 / 1920;
	qDebug() << w << "x" << h;

	this->setGeometry(0, 0, w, h);
	// this->setGeometry(0, 0, 100, 100);

	videoWidget = new QVideoWidget;
	// videoWidget->setGeometry(0, 0, 200, 100);
	// videoWidget->show();
	ui->horizontalLayout->addWidget(videoWidget);
	// ui->verticalLayout->setGeometry(QRect(0, 0, 400, 400));

	camera->setViewfinder(videoWidget);
	camera->setCaptureMode(QCamera::CaptureMode::CaptureViewfinder); // lower CPU usage

	QCameraViewfinderSettings viewfinderSettings;
	viewfinderSettings.setResolution(1920, 1080);
//	viewfinderSettings.setResolution(1280, 768);
	viewfinderSettings.setPixelAspectRatio(1, 1);
	viewfinderSettings.setMinimumFrameRate(8.0f);
	viewfinderSettings.setMaximumFrameRate(30.0f);
	// viewfinderSettings.setPixelFormat(QVideoFrame::Format_RGB24); // Format_YUYV
	camera->setViewfinderSettings(viewfinderSettings);
	camera->start();

	// videoWidget->setGeometry(0, 0, 200, 100);

/*	const int DesktopTaskBarHeight = 32;
	const int MainWindowWidth = 340;
	const int WindowBorder = 5;
	const QSize desktopSize = QGuiApplication::primaryScreen()->size(); */

	// this->setGeometry(MainWindowWidth + 2 * WindowBorder, DesktopTaskBarHeight,
	// 		desktopSize.width() - MainWindowWidth - 4 * WindowBorder, //previewWindow.geometry().height());
	// 		desktopSize.height() - 2 * DesktopTaskBarHeight);

	// this->setGeometry(MainWindowWidth + 2 * WindowBorder, DesktopTaskBarHeight, 500, 500);
	// this->setGeometry(MainWindowWidth + 2 * WindowBorder, DesktopTaskBarHeight, 500, 500);

	// this->move(50, 50);
	// this->setFixedSize(300, 300);
}

PreviewWindow::~PreviewWindow()
{
	delete ui;
}

// void PreviewWindow::on_pushButton_clicked()
// {
// 	// this->move(100, 0);
// 	this->setGeometry(100, 100, 200, 200);
// 	this->move(50, 50);
// 	this->setFixedSize(300, 300);
// 	qDebug() << this->geometry().left() << "x" << this->geometry().top();
// }

void PreviewWindow::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	this->setupCamera();
	qDebug() << "shown";
	return;

/*
	videoWidget = new QVideoWidget;
	// ui->horizontalLayout->addWidget(videoWidget);
	// videoWidget->show();
	// videoWidget->setGeometry(20, 20, 1000, 800);

	QMediaPlayer *player = new QMediaPlayer;
	player->setVideoOutput(videoWidget);
	player->setParent(videoWidget);
	player->setMedia(QUrl("gst-pipeline: v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1 ! glimagesink")); // rotate-method=0 render-rectangle='<0,0,1920,1080>'
	// player->setMedia(QUrl("gst-pipeline: videotestsrc ! xvimagesink name=\"qtvideosink\""));
	// player->setMedia(QUrl("gst-pipeline: videotestsrc ! glimagesink"));

	// QVideoWidget *videoWidget = new QVideoWidget;
	// player->setVideoOutput(videoWidget);
	player->play();
	// WId id = QWidget::winId();
*/

	gst_init(0, NULL);


/*
	GstElement *pipeline =
			gst_parse_launch(
			// "gst-pipeline: videotestsrc ! glimagesink",
			// "gst-pipeline: v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1 ! glimagesink render-rectangle='<0,0,1920,1080>'",
			"gst-pipeline: v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=1920,height=1080,framerate=30/1 ! ximagesink",
			 NULL);
*/

/**/
	qDebug() << "GST_V4L2_USE_LIBV4L2 =" << g_getenv("GST_V4L2_USE_LIBV4L2");

	GstElement *pipeline = gst_pipeline_new("xvoverlay");
	// GstElement *src = gst_element_factory_make("videotestsrc", NULL);
	// GstElement *src = gst_element_factory_make("v4l2src", "camera-input");
	GstElement *src = gst_element_factory_make("v4l2src", NULL);
	// g_object_set(G_OBJECT(src), "device", "/dev/video0", NULL);
	qDebug("src = %llu", (unsigned long long)src);

	GstElement *src_capsfilter = gst_element_factory_make("capsfilter", "source_capsfilter");
	qDebug("src_capsfilter = %llu", (unsigned long long)src_capsfilter);
	GstCaps *src_caps = gst_caps_new_simple(
			"video/x-raw",
			"format", G_TYPE_STRING, "YUY2",
			"width", G_TYPE_INT, 1920,
			"height", G_TYPE_INT, 1080,
			"framerate", GST_TYPE_FRACTION, 30, 1,
			NULL);
	qDebug("src_caps = %llu", (unsigned long long)src_caps);
	g_object_set(G_OBJECT(src_capsfilter), "caps", src_caps, NULL);

	GstElement *sink = gst_element_factory_make("ximagesink", NULL);
	qDebug("sink = %llu", (unsigned long long)sink);

	gst_bin_add_many(GST_BIN(pipeline), src, src_capsfilter, sink, NULL);
	bool b = gst_element_link_many(src, src_capsfilter, sink, NULL);
	// gst_bin_add_many(GST_BIN(pipeline), src, sink, NULL);
	// bool b = gst_element_link_many(src, sink, NULL);
	qDebug("link = %d", b);

// https://github.com/simonqin09/gstest/blob/master/gstest.c
// https://forums.developer.nvidia.com/t/using-x-raw-memory-nvmm-in-gstreamer-program/42654


	videoWidget = new QVideoWidget;
	ui->horizontalLayout->addWidget(videoWidget);
	// videoWidget->show();
	// videoWidget->setGeometry(20, 20, 1000, 800);
	WId winId = videoWidget->winId();
	gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), winId);

	GstStateChangeReturn sret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
	qDebug("sret = %llu", sret);

	/* Wait until error or EOS
	GstBus *bus = gst_element_get_bus (pipeline);
	GstMessage *msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, (GstMessageType)(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));
	qDebug() << msg->src->name; */

/*
	GstElement *pipeline = gst_pipeline_new ("xvoverlay");
	GstElement *src = gst_element_factory_make ("videotestsrc", NULL);
	GstElement *sink = gst_element_factory_make ("ximagesink", "sink");
	gst_bin_add_many (GST_BIN (pipeline), src, sink, NULL);
	gst_element_link (src, sink);

	QWidget window;
	window.setGeometry(0, 32, 320, 240);
	window.show();

	WId xwinid = window.winId();
	gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (sink), xwinid);

	GstStateChangeReturn sret = gst_element_set_state (pipeline,GST_STATE_PLAYING);
	if (sret == GST_STATE_CHANGE_FAILURE) {
		gst_element_set_state (pipeline, GST_STATE_NULL);
		gst_object_unref (pipeline);
		// Exit application
		// QTimer::singleShot(0, QApplication::activeWindow(), SLOT(quit()));
	}

	// int ret = app.exec();

	// window.hide();
	// gst_element_set_state (pipeline, GST_STATE_NULL);
	// gst_object_unref (pipeline);
*/

	qDebug() << "end";
}

/*
https://gstreamer.freedesktop.org/documentation/installing/on-linux.html?gi-language=c
apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio

pd22.1.1
pkg-config --cflags --libs gstreamer-video-1.0

*/



// void GstPlayer::setVideoOutput(QWidget *widget)
// {
// 	_hwndVideo = widget->winId();
// }
