#include "preview_window.h"
#include "ui_preview_window.h"

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QCamera>
#include <QCameraInfo>
#include <QLabel>
#include <QSizePolicy>

PreviewWindow::PreviewWindow(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::PreviewWindow)
{
	ui->setupUi(this);

	// const QString AllowedCameraDevice = QString("/dev/video0");
	// const QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
	// QCamera *camera = nullptr;
	// for (const QCameraInfo &info : cameras)
	// 	if (info.deviceName().compare(AllowedCameraDevice) == 0)
	// 	{
	// 		camera = new QCamera(info);
	// 		qDebug() << info.deviceName() << Qt::endl;
	// 	}

	// if (camera == nullptr)
	// {
	// 	qInfo() << "Any camera not found.\n";
	// 	return;
	// }

	// int w = 1700;
	// int h = w * 1080 / 1920;
	// qDebug() << w << "x" << h << Qt::endl;

	// //this->setGeometry(0, 0, w, h);

	// videoWidget = new QVideoWidget;
	// videoWidget->setGeometry(0, 0, 200, 100);
	// // videoWidget->show();
	// ui->verticalLayout->addWidget(videoWidget);

	// camera->setViewfinder(videoWidget);

	// QCameraViewfinderSettings viewfinderSettings;
	// viewfinderSettings.setResolution(1920, 1080);
	// viewfinderSettings.setMinimumFrameRate(5.0);
	// viewfinderSettings.setMaximumFrameRate(60.0);
	// camera->setViewfinderSettings(viewfinderSettings);
	// camera->start();
}

PreviewWindow::~PreviewWindow()
{
	delete ui;
}
