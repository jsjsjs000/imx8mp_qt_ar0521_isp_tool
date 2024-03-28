#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSpacerItem>

#include "controls_definitions.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->createControls();

	this->ispControl.OpenVideo();
}

void MainWindow::createControls(void)
{
	ControlsDefinitions controls;
	controls.init(); // $$

	for (const Control *control : qAsConst(controls.controls))
	{
		const SliderControl *scontrol = static_cast<const SliderControl*>(control);
		SliderWidget *slider = new SliderWidget();
		slider->initialize(control->name, control->description, scontrol->min, scontrol->max, scontrol->value);
		ui->verticalLayout->addWidget(slider, 1);
	}

	ui->verticalLayout->addStretch(2);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	this->setGeometry(100, 100, 200, 200);
	this->move(50, 50);
	this->setFixedSize(300, 300);
	// qDebug() << this->geometry().left() << "x" << this->geometry().top();
}
