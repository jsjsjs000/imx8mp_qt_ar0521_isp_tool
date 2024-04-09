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

protected:
	void paintEvent(QPaintEvent *event);

private:
	Ui::Chart *ui;
	float x1, x2, y1, y2;
	float gridX, gridY;
	void drawChartArea(QPainter &painter, int x, int y, int w, int h);
};

#endif // CHART_H
