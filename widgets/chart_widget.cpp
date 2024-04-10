#include "chart_widget.h"
#include "qdebug.h"
#include "ui_chart.h"

#include <QMouseEvent>
#include <QPainter>

// #define DEBUG_TIME

Chart::Chart(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::Chart)
{
	ui->setupUi(this);
}

Chart::Chart(QWidget *parent, MainWindow *mainWindow, const ChartControl *chartControl,
		void (*onChartPointsChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter))
{
	ui->setupUi(this);

	this->mainWindow = mainWindow;
	this->chartControl = (ChartControl*)chartControl;
	this->getCmd = chartControl->getCmd;
	this->setCmd = chartControl->setCmd;
	this->parameter = chartControl->parameter;
	this->onChartPointsChanged = onChartPointsChanged;

	this->recalculateSize();

	this->setToolTip(chartControl->description);
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

	this->recalculateSize();
	this->repaint();
}

void Chart::paintEvent(QPaintEvent* /* event */)
{
	const int circleSize = 6;

#ifdef DEBUG_TIME
	double startTime = clock();
#endif

	QPainter painter(this);
	QFont font = painter.font();

	QFont titleFont = painter.font();
	titleFont.setBold(true);
	painter.setFont(titleFont);
	painter.drawText(0, 3, this->width(), this->padTop - 3, Qt::AlignHCenter, this->chartControl->name);

	painter.setFont(font);
	painter.setPen(QColor(200, 200, 200));
	painter.drawRect(0, 0, this->width() - 1, this->height() - 1);

	this->drawChartArea(painter, this->padLeft, this->padTop, this->w, this->h);

	painter.setFont(font);
	painter.setPen(Qt::blue);
	painter.setBrush(Qt::blue);

		/* Draw lines */
	for (int i = 0; i < this->points.count() - 1; i++)
	{
		float x1 = this->padLeft + (this->points[i].x() - this->x1) * this->dx;
		float y1 = this->padTop + h - (this->points[i].y() - this->y1) * this->dy;
		float x2 = this->padLeft + (this->points[i + 1].x() - this->x1) * this->dx;
		float y2 = this->padTop + h - (this->points[i + 1].y() - this->y1) * this->dy;
		painter.drawLine(x1, y1, x2, y2);
	}

		/* Draw points */
	for (int i = 0; i < this->points.count(); i++)
	{
		float x = this->padLeft + (this->points[i].x() - this->x1) * this->dx;
		float y = this->padTop + h - (this->points[i].y() - this->y1) * this->dy;
		painter.drawEllipse(x - circleSize / 2, y - circleSize / 2, circleSize, circleSize);
	}

#ifdef DEBUG_TIME
	double diff = double(clock() - startTime) / CLOCKS_PER_SEC * 1000;
	qDebug() << diff << "ms";
#endif
}

void Chart::drawChartArea(QPainter &painter, int x, int y, int w, int h)
{
	const float epsilon = 0.00001f;
	const QPoint xAxisLabelsPad = QPoint(0, -1);
	const QPoint yAxisLabelsPad = QPoint(3, 0);

	painter.drawRect(x, y, w, h);

		/* Draw grid */
	painter.setPen(QColor(220, 220, 220));

	float xi = this->x1;
	while (xi <= this->x2 + epsilon && this->gridX > epsilon)
	{
		float fxi = x + (xi - this->x1) * this->dx;
		painter.drawLine(fxi, y, fxi, y + h);
		xi += this->gridX;
	}

	float yi = this->y1;
	while (yi <= this->y2 + epsilon && this->gridY > epsilon)
	{
		float fyi = h + y - (yi - this->y1) * this->dy;
		painter.drawLine(x, fyi, x + w, fyi);
		yi += this->gridY;
	}

		/* Draw axis labels */
	painter.setPen(Qt::black);
	QFont font = painter.font();
	font.setPointSizeF(7.5f);
	painter.setFont(font);

	xi = this->x1;
	while (xi <= this->x2 + epsilon && this->gridX > epsilon)
	{
		float fxi = x + (xi - this->x1) * this->dx;
		QString text;
		text = text.asprintf("%.0f", xi);
		QRect rect = painter.boundingRect(0, 0, 100, 100, 0, text);
		painter.drawText(fxi - rect.width() / 2 + xAxisLabelsPad.x(), y + h + rect.height() + xAxisLabelsPad.y(), text);
		xi += this->gridX;
	}

	yi = this->y1;
	while (yi <= this->y2 + epsilon && this->gridY > epsilon)
	{
		float fyi = h + y - (yi - this->y1) * this->dy;
		QString text;
		text = text.asprintf("%.2f", yi);
		QRect rect = painter.boundingRect(0, 0, 100, 100, 0, text);
		painter.drawText(this->x1 + yAxisLabelsPad.x(), fyi + rect.height() / 2 + yAxisLabelsPad.y(), text);
		yi += this->gridY;
	}
}

QPointF Chart::localPosTo(QPointF localPos)
{
	return QPointF(
			(localPos.x() - this->padLeft) / this->dx + this->x1,
			-(localPos.y() - this->padTop - h) / this->dy + this->y1);
}

void Chart::mousePressEvent(QMouseEvent *event)
{
	// qDebug() << "press" << event->localPos().x() << event->localPos().y() << event->button();
	if (this->chartControl->readonly)
		return;

	QPointF p = this->localPosTo(event->localPos());
	// qDebug() << p.x() << p.y();
	this->points[0].setX(p.x());
	this->points[0].setY(p.y());
	this->repaint();
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
	// qDebug() << "move" << event->localPos().x() << event->localPos().y() << event->button();
	if (this->chartControl->readonly)
		return;

	QPointF p = this->localPosTo(event->localPos());
	this->points[0].setX(p.x());
	this->points[0].setY(p.y());
	this->repaint();
}

void Chart::mouseReleaseEvent(QMouseEvent *event)
{
	// qDebug() << "release" << event->localPos().x() << event->localPos().y() << event->button();
	// QPointF p = this->localPosTo(event->localPos());

	// if (this->chartControl->readonly)
		// return;
}

void Chart::resizeEvent(QResizeEvent *event)
{
	// qDebug() << "resize" << event->size() << this->width() << this->height();
	this->recalculateSize();
}

void Chart::recalculateSize()
{
	this->w = this->width() - 1 - this->padLeft - this->padRight;
	this->h = this->height() - 1 - this->padTop - this->padBottom;
	this->dx = this->w / (this->x2 - this->x1);
	this->dy = this->h / (this->y2 - this->y1);
}
