#ifndef CHART_H
#define CHART_H

#include <QWidget>

namespace Ui {
class Chart;
}

class Chart : public QWidget
{
	Q_OBJECT

public:
	explicit Chart(QWidget *parent = nullptr);
	~Chart();
	void initialize(float x1, float y1, float x2, float y2, float gridX, float gridY, QList<QPointF> points);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	const int padLeft = 27;
	const int padTop = 20;
	const int padRight = 10;
	const int padBottom = 15;
	float x1, x2, y1, y2;
	float gridX, gridY;
	Ui::Chart *ui;
	QList<QPointF> points;
	void drawChartArea(QPainter &painter, int x, int y, int w, int h);
	QPointF localPosTo(QPointF localPos);
};

#endif // CHART_H
