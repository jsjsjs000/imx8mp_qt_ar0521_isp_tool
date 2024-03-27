#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->ispControl.OpenVideo();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
	this->ispControl.set_cproc_brightness(value);
	this->ui->label_2->setText(QString::number(value));
}
