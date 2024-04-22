#include "matrix_view_widget.h"
#include "ui_matrix_view_widget.h"

MatrixViewWidget::MatrixViewWidget(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::MatrixViewWidget)
{
	ui->setupUi(this);
}

MatrixViewWidget::~MatrixViewWidget()
{
	delete ui;
}
