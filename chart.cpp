#include "chart.h"
#include "ui_chart.h"

#include <QPainter>

Chart::Chart(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::Chart)
{
	ui->setupUi(this);

	this->x1 = 0;
	this->x2 = 15;
	this->y1 = -1;
	this->y2 = 1;
	this->gridX = 50;
	this->gridY = 50;
}

Chart::~Chart()
{
	delete ui;
}

void Chart::paintEvent(QPaintEvent* /* event */)
{
	QPainter painter(this);

	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, this->width() - 1, this->height() - 1);
	this->drawChartArea(painter, 10, 10, this->width() - 1 - 2 * 10, this->height() - 1 - 2 * 10);

	// QFont font = painter.font();
	// font.setPixelSize(20);
	// painter.setFont(font);
	painter.drawText(0, 10, "title");
}

void Chart::drawChartArea(QPainter &painter, int x, int y, int w, int h)
{
	painter.drawRect(x, y, w, h);
	float (x2 - x1)
}
