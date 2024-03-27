#include "preview_window.h"
#include "ui_preview_window.h"

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
	for (const QCameraInfo &info : cameras)
		if (info.deviceName().compare(AllowedCameraDevice) == 0)
		{
			camera = new QCamera(info);
			qDebug() << info.deviceName() << Qt::endl;
		}

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

	QCameraViewfinderSettings viewfinderSettings;
	viewfinderSettings.setResolution(1920, 1080);
	viewfinderSettings.setMinimumFrameRate(5.0);
	viewfinderSettings.setMaximumFrameRate(60.0);
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

void PreviewWindow::on_pushButton_clicked()
{
	// this->move(100, 0);
	this->setGeometry(100, 100, 200, 200);
	this->move(50, 50);
	this->setFixedSize(300, 300);
	qDebug() << this->geometry().left() << "x" << this->geometry().top();
}

void PreviewWindow::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	this->setupCamera();
	qDebug() << "shown";
}
