#ifndef MATRIX_VIEW_WIDGET_H
#define MATRIX_VIEW_WIDGET_H

#include <QWidget>

namespace Ui {
class MatrixViewWidget;
}

class MatrixViewWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MatrixViewWidget(QWidget *parent = nullptr);
	~MatrixViewWidget();

private:
	Ui::MatrixViewWidget *ui;
};

#endif // MATRIX_VIEW_WIDGET_H
