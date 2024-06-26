#ifndef GROUP_WIDGET_H
#define GROUP_WIDGET_H

#include "ui_group_widget.h"
#include <QWidget>

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
	Q_OBJECT

public:
	explicit GroupWidget(QWidget *parent = nullptr);
	GroupWidget(QWidget *parent, QString name);
	~GroupWidget();

private:
	Ui::GroupWidget *ui;
};

#endif // GROUP_WIDGET_H
