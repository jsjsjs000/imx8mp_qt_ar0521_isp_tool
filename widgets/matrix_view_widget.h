#ifndef MATRIX_VIEW_H
#define MATRIX_VIEW_H

#include <QWidget>
#include "controls.h"
#include "mainwindow.h"

namespace Ui {
class MatrixView;
}

class MatrixViewWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MatrixViewWidget(QWidget *parent = nullptr);
	MatrixViewWidget(QWidget *parent, MainWindow *mainWindow, const MatrixViewControl *matrixViewControl);
	~MatrixViewWidget();
	void setPoints(QList<QPointF> points);

protected:
	void paintEvent(QPaintEvent *event);

private:
	const int padLeft = 10;
	const int padTop = 18;
	const int padRight = 10;
	const int padBottom = 10;

	QString title;
	QString description;

	QList<QPointF> points;

	MainWindow *mainWindow;
	Ui::MatrixView *ui;
	MatrixViewControl *matrixViewControl = nullptr;
	QString getCmd;
	QString parameter;
};

#endif // MATRIX_VIEW_H
