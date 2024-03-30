#ifndef CHECKBOX_WIDGET_H
#define CHECKBOX_WIDGET_H

#include "controls_definitions.h"

#include <QWidget>

namespace Ui {
class CheckBoxWidget;
}

class CheckBoxWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CheckBoxWidget(QWidget *parent = nullptr);
	~CheckBoxWidget();
	void initialize(const CheckBoxControl *control,
			void (*f)(QString type, QString parameter, bool value));
	void setState(bool state);

private slots:
	void on_checkBox_stateChanged(int arg1);

private:
	Ui::CheckBoxWidget *ui;
	QString type;
	QString parameter;
	void (*onCheckBoxChanged)(QString type, QString parameter, bool value);
};

#endif // CHECKBOX_WIDGET_H
