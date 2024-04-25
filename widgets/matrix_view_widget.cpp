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
	{
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		// connect(this, &MatrixViewWidget::customContextMenuRequested, this, &MatrixViewWidget::slotCustomMenuRequested);
	}

	this->mainWindow = mainWindow;
	this->matrixViewControl = (MatrixViewControl*)matrixViewControl;
	this->getCmd = matrixViewControl->getCmd;
	this->parameter = matrixViewControl->parameter;

	// this->recalculateSize();

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

	int w = this->width() - this->padLeft - this->padRight;
	int h = this->height() - this->padTop - this->padBottom;
	int rectWidth = w / this->matrixViewControl->columnsCount;
	int rectHeight = h / this->matrixViewControl->rowsCount;

		/* Draw rectangles */
	for (int x = 0; x < this->matrixViewControl->columnsCount; x++)
		for (int y = 0; y < this->matrixViewControl->rowsCount; y++)
		{
			painter.setPen(Qt::white);
			painter.setBrush(Qt::gray);
			painter.drawRect(this->padLeft + x * rectWidth, this->padTop + y * rectHeight, rectWidth, rectHeight);
		}

#ifdef DEBUG_TIME
	double diff = double(clock() - startTime) / CLOCKS_PER_SEC * 1000;
	qDebug() << diff << "ms";
#endif
}
