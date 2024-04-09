#include "chart.h"
#include "qdebug.h"
#include "ui_chart.h"

#include <QMouseEvent>
#include <QPainter>

Chart::Chart(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::Chart)
{
	ui->setupUi(this);
}

Chart::~Chart()
{
	delete ui;
}

void Chart::initialize(float x1, float x2, float y1, float y2, float gridX, float gridY, QList<QPointF> points)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->gridX = gridX;
	this->gridY = gridY;
	this->points = points;
}

void Chart::paintEvent(QPaintEvent* /* event */)
{
	const int circleSize = 6;

	QPainter painter(this);
	QFont font = painter.font();

	painter.drawText(0, 10, "title");

	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, this->width() - 1, this->height() - 1);

	int w = this->width() - 1 - this->padLeft - this->padRight;
	int h = this->height() - 1 - this->padTop - this->padBottom;
	this->drawChartArea(painter, this->padLeft, this->padTop, w, h);

	painter.setFont(font);
	painter.setPen(Qt::blue);
	painter.setBrush(Qt::blue);

	float dx = w / (this->x2 - this->x1);
	float dy = h / (this->y2 - this->y1);

		/* Draw lines */
	for (int i = 0; i < this->points.count() - 1; i++)
	{
		float x1 = this->padLeft + (this->points[i].x() - this->x1) * dx;
		float y1 = this->padTop + h - (this->points[i].y() - this->y1) * dy;
		float x2 = this->padLeft + (this->points[i + 1].x() - this->x1) * dx;
		float y2 = this->padTop + h - (this->points[i + 1].y() - this->y1) * dy;
		painter.drawLine(x1, y1, x2, y2);
	}

		/* Draw points */
	for (int i = 0; i < this->points.count(); i++)
	{
		float x = this->padLeft + (this->points[i].x() - this->x1) * dx;
		float y = this->padTop + h - (this->points[i].y() - this->y1) * dy;
		painter.drawEllipse(x - circleSize / 2, y - circleSize / 2, circleSize, circleSize);
	}
}

void Chart::drawChartArea(QPainter &painter, int x, int y, int w, int h)
{
	const float epsilon = 0.00001f;
	const QPoint xAxisLabelsPad = QPoint(0, -1);
	const QPoint yAxisLabelsPad = QPoint(3, 0);

	float dx = w / (this->x2 - this->x1);
	float dy = h / (this->y2 - this->y1);

	painter.drawRect(x, y, w, h);

		/* Draw grid */
	painter.setPen(Qt::lightGray);

	float xi = this->x1;
	while (xi <= this->x2 + epsilon)
	{
		float fxi = x + (xi - this->x1) * dx;
		painter.drawLine(fxi, y, fxi, y + h);
		xi += this->gridX;
	}

	float yi = this->y1;
	while (yi <= this->y2 + epsilon)
	{
		float fyi = h + y - (yi - this->y1) * dy;
		painter.drawLine(x, fyi, x + w, fyi);
		yi += this->gridY;
	}

		/* Draw axis labels */
	painter.setPen(Qt::black);
	QFont font = painter.font();
	font.setPointSizeF(7.5f);
	painter.setFont(font);

	xi = this->x1;
	while (xi <= this->x2 + epsilon)
	{
		float fxi = x + (xi - this->x1) * dx;
		QString text;
		text = text.asprintf("%.0f", xi);
		QRect rect = painter.boundingRect(0, 0, 100, 100, 0, text);
		painter.drawText(fxi - rect.width() / 2 + xAxisLabelsPad.x(), y + h + rect.height() + xAxisLabelsPad.y(), text);
		xi += this->gridX;
	}

	yi = this->y1;
	while (yi <= this->y2 + epsilon)
	{
		float fyi = h + y - (yi - this->y1) * dy;
		QString text;
		text = text.asprintf("%.2f", yi);
		QRect rect = painter.boundingRect(0, 0, 100, 100, 0, text);
		painter.drawText(this->x1 + yAxisLabelsPad.x(), fyi + rect.height() / 2 + yAxisLabelsPad.y(), text);
		yi += this->gridY;
	}
}

QPointF Chart::localPosTo(QPointF localPos)
{
	int w = this->width() - 1 - this->padLeft - this->padRight;
	int h = this->height() - 1 - this->padTop - this->padBottom;
	float dx = w / (this->x2 - this->x1);
	float dy = h / (this->y2 - this->y1);
	return QPointF(
			(localPos.x() - this->padLeft) / dx + this->x1,
			-(localPos.y() - this->padTop - h) / dy + this->y1);
}

void Chart::mousePressEvent(QMouseEvent *event)
{
	// qDebug() << "press" << event->localPos().x() << event->localPos().y() << event->button();
	QPointF p = this->localPosTo(event->localPos());
	// qDebug() << p.x() << p.y();
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
	// qDebug() << "move" << event->localPos().x() << event->localPos().y() << event->button();
	QPointF p = this->localPosTo(event->localPos());
	this->points[0].setX(p.x());
	this->points[0].setY(p.y());
	this->repaint();
}

void Chart::mouseReleaseEvent(QMouseEvent *event)
{
	// qDebug() << "release" << event->localPos().x() << event->localPos().y() << event->button();
	QPointF p = this->localPosTo(event->localPos());
}

void Chart::resizeEvent(QResizeEvent *event)
{
	qDebug() << "resize" << event->size();
	// $$ globalne w, h, dx, dy
}
