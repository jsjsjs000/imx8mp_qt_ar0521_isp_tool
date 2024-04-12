#include "chart_widget.h"
#include "ui_chart_widget.h"

#include <QMouseEvent>
#include <QPainter>

// #define DEBUG_TIME

ChartWidget::ChartWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::Chart)
{
	ui->setupUi(this);
}

ChartWidget::ChartWidget(QWidget *parent, MainWindow *mainWindow, const ChartControl *chartControl,
		void (*onChartPointsChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter))
		: QWidget(parent)
		, ui(new Ui::Chart)
{
	ui->setupUi(this);
	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_Hover);

	this->mainWindow = mainWindow;
	this->chartControl = (ChartControl*)chartControl;
	this->getCmd = chartControl->getCmd;
	this->setCmd = chartControl->setCmd;
	this->parameter = chartControl->parameter;
	this->readonly = chartControl->readonly;
	this->onChartPointsChanged = onChartPointsChanged;

	this->recalculateSize();

	this->setToolTip(chartControl->description);
}

ChartWidget::ChartWidget(QWidget *parent, MainWindow *mainWindow, const ChartControl2 *chartControl2,
		void (*onChartPointsChanged2)(MainWindow *mainWindow, QString node))
		: QWidget(parent)
		, ui(new Ui::Chart)
{
	ui->setupUi(this);
	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_Hover);

	this->mainWindow = mainWindow;
	this->chartControl2 = (ChartControl2*)chartControl2;
	this->node = chartControl2->node;
	this->readonly = chartControl2->readonly;
	this->onChartPointsChanged2 = onChartPointsChanged2;

	this->recalculateSize();

	this->setToolTip(chartControl2->description);
}

ChartWidget::~ChartWidget()
{
	delete ui;
}

void ChartWidget::initialize(float x1, float x2, float y1, float y2, float gridX, float gridY, QList<QPointF> points)
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

bool ChartWidget::event(QEvent *event)
{
	switch(event->type())
	{
		case QEvent::HoverEnter:
			hooverEnterEvent(static_cast<QHoverEvent*>(event));
			return true;
			break;
		case QEvent::HoverLeave:
			hooverLeaveEvent(static_cast<QHoverEvent*>(event));
			return true;
			break;
		// case QEvent::HoverMove:
			// return true;
			// break;
		default:
			break;
	}
	return QWidget::event(event);
}

void ChartWidget::paintEvent(QPaintEvent* /* event */)
{
	const int circleSize = 6;

#ifdef DEBUG_TIME
	double startTime = clock();
#endif

	QPainter painter(this);
	QFont font = painter.font();

		/* Title */
	if (this->chartControl != nullptr)
		painter.drawText(0, 3, this->width(), this->padTop - 3, Qt::AlignHCenter, this->chartControl->name);
	else if (this->chartControl2 != nullptr)
		painter.drawText(0, 3, this->width(), this->padTop - 3, Qt::AlignHCenter, this->chartControl2->name);

		/* Outline rect */
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

void ChartWidget::drawChartArea(QPainter &painter, int x, int y, int w, int h)
{
	const float epsilon = 0.00001f;
	const QPoint xAxisLabelsPad = QPoint(0, -1);
	const QPoint yAxisLabelsPad = QPoint(3, 0);

	painter.setPen(Qt::blue);
	if (this->showMousePosition)
	{
		QString s;
		s = s.asprintf("(%d, %0.2f)", (int)this->mousePosition.x(), this->mousePosition.y());
		painter.drawText(x, y + 0, w, 20, Qt::AlignRight, s);
		// QString::number(this->points[0].y(), 'f', 3)
	}

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
		if (this->y2 - this->y1 >= 10.0f)
			text = text.asprintf("%.0f", yi);
		else
			text = text.asprintf("%.2f", yi);
		QRect rect = painter.boundingRect(0, 0, 100, 100, 0, text);
		painter.drawText(this->x1 + yAxisLabelsPad.x(), fyi + rect.height() / 2 + yAxisLabelsPad.y(), text);
		yi += this->gridY;
	}
}

QPointF ChartWidget::localPosTo(QPointF localPos)
{
	return QPointF(
			(localPos.x() - this->padLeft) / this->dx + this->x1,
			-(localPos.y() - this->padTop - h) / this->dy + this->y1);
}

void ChartWidget::mousePressEvent(QMouseEvent *event)
{
	// qDebug() << "press" << event->localPos().x() << event->localPos().y() << event->button();

	QPointF p = this->localPosTo(event->localPos());
	if (!this->readonly && event->button() == Qt::LeftButton)
	{
		int x = round(p.x());
		if (x >= 0 && x < this->points.count())
		{
			this->points[x].setY(p.y());
			this->repaint();
		}
	}
}

void ChartWidget::mouseMoveEvent(QMouseEvent *event)
{
	// qDebug() << "move" << event->localPos().x() << event->localPos().y() << event->buttons();

	QPointF p = this->localPosTo(event->localPos());

	this->mousePosition = QPointF(event->localPos().x(), event->localPos().y());
	this->showMousePosition = true;

	if (!this->readonly && event->buttons() == Qt::LeftButton)
	{
		int x = round(p.x());
		if (x >= 0 && x < this->points.count())
			this->points[x].setY(p.y());
	}

	this->repaint();
}

void ChartWidget::mouseReleaseEvent(QMouseEvent* /* event */)
{
	// qDebug() << "release" << event->localPos().x() << event->localPos().y() << event->button();
}

void ChartWidget::hooverEnterEvent(QHoverEvent* /* event */)
{
}

void ChartWidget::hooverLeaveEvent(QHoverEvent* /* event */)
{
	qDebug() << "leave";
	this->showMousePosition = false;
	this->repaint();
}

void ChartWidget::resizeEvent(QResizeEvent* /* event */)
{
	// qDebug() << "resize" << event->size() << this->width() << this->height();
	this->recalculateSize();
}

void ChartWidget::recalculateSize()
{
	this->w = this->width() - 1 - this->padLeft - this->padRight;
	this->h = this->height() - 1 - this->padTop - this->padBottom;
	this->dx = this->w / (this->x2 - this->x1);
	this->dy = this->h / (this->y2 - this->y1);
}
