#include "matrix_view_widget.h"
#include "ui_matrix_view_widget.h"

#include <QPainter>

// #define DEBUG_TIME

MatrixViewWidget::MatrixViewWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::MatrixView)
{
	ui->setupUi(this);
}

MatrixViewWidget::MatrixViewWidget(QWidget *parent, MainWindow *mainWindow, const MatrixViewControl *matrixViewControl)
		: QWidget(parent)
		, ui(new Ui::MatrixView)
{
	ui->setupUi(this);
	this->setMouseTracking(true);
	this->setAttribute(Qt::WA_Hover);

	if (!matrixViewControl->readonly)
		this->setContextMenuPolicy(Qt::CustomContextMenu);

	this->mainWindow = mainWindow;
	this->matrixViewControl = (MatrixViewControl*)matrixViewControl;
	this->getCmd = matrixViewControl->getCmd;
	this->parameter = matrixViewControl->parameter;

	this->setToolTip(matrixViewControl->description);
}

MatrixViewWidget::~MatrixViewWidget()
{
	delete ui;
}

void MatrixViewWidget::setPoints(QList<QPointF> points)
{
	this->points = points;

	this->repaint();
}

void MatrixViewWidget::paintEvent(QPaintEvent* /* event */)
{
#ifdef DEBUG_TIME
	double startTime = clock();
#endif

	if (this->matrixViewControl == nullptr)
		return;

	QPainter painter(this);

		/* Title */
	if (this->matrixViewControl != nullptr)
		painter.drawText(0, 3, this->width(), this->padTop - 3, Qt::AlignHCenter, this->matrixViewControl->name);

		/* Outline rect */
	painter.setPen(QColor(210, 210, 210));
	painter.drawRect(0, 0, this->width() - 1, this->height() - 1);

	int w = this->width() - this->padLeft - this->padRight;
	int h = this->height() - this->padTop - this->padBottom;
	int rectWidth = w / this->matrixViewControl->columnsCount;
	int rectHeight = h / this->matrixViewControl->rowsCount;

	QFont font2 = painter.font();
	font2.setPointSizeF(7.5f);
	painter.setFont(font2);

		/* Draw rectangles */
	int i = 0;
	for (int y = 0; y < this->matrixViewControl->rowsCount; y++)
		for (int x = 0; x < this->matrixViewControl->columnsCount; x++)
			if (i < this->points.count())
			{
				int color = this->points[i++].y();
				int binInvColor = (color >= 255 / 2) ? 24 : 255 - 24;
				int x_ = this->padLeft + x * rectWidth;
				int y_ = this->padTop + y * rectHeight;
				painter.setPen(QColor(binInvColor, binInvColor, binInvColor));
				painter.setBrush(QColor(color, color, color));
				painter.drawRect(x_, y_, rectWidth, rectHeight);

				QString text = QString::number(color);
				QRect textRect = painter.boundingRect(0, 0, 100, 100, 0, text);
				painter.drawText(x_ + rectWidth / 2 - textRect.width() / 2, y_ + rectHeight / 2 + textRect.height() / 2, text);
			}

#ifdef DEBUG_TIME
	double diff = double(clock() - startTime) / CLOCKS_PER_SEC * 1000;
	qDebug() << diff << "ms";
#endif
}
