#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include "controls.h"
#include "controls2.h"
#include "mainwindow.h"

namespace Ui {
class Chart;
}

class ChartWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ChartWidget(QWidget *parent = nullptr);
	ChartWidget(QWidget *parent, MainWindow *mainWindow, const ChartControl *control,
			void (*onChartPointsChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<QPointF> points));
	ChartWidget(QWidget *parent, MainWindow *mainWindow, const ChartControl2 *chartControl2,
			void (*onChartPointsChanged)(MainWindow *mainWindow, QString node, QList<QPointF> points));
	~ChartWidget();
	void initialize(float x1, float x2, float y1, float y2, float gridX, float gridY, QList<QPointF> points);

protected:
	bool event(QEvent *event);
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void hooverEnterEvent(QHoverEvent *event);
	void hooverLeaveEvent(QHoverEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	const int padLeft = 27;
	const int padTop = 18;
	const int padRight = 10;
	const int padBottom = 15;

	QString title;
	QString description;
	float x1 = 0.0f;
	float x2 = 1.0f;
	float y1 = 0.0f;
	float y2 = 1.0f;
	float gridX, gridY;
	int w = 1;
	int h = 1;
	float dx = 1.0f;
	float dy = 1.0f;

	MainWindow *mainWindow;
	ChartControl *chartControl;
	ChartControl2 *chartControl2;
	QString getCmd;
	QString setCmd;
	QString parameter;
	QString node;
	bool readonly;

	Ui::Chart *ui;
	QList<QPointF> points;
	void executeChangedEvent();
	bool showMousePosition = false;
	QPointF mousePosition;
	void drawChartArea(QPainter &painter, int x, int y, int w, int h);
	QPointF localPosTo(QPointF localPos);
	void recalculateSize();
	void (*onChartPointsChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QList<QPointF> points) = nullptr;
	void (*onChartPointsChanged2)(MainWindow *mainWindow, QString node, QList<QPointF> points) = nullptr;
};

#endif // CHART_H
