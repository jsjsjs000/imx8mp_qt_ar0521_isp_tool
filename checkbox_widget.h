#ifndef CHECKBOX_WIDGET_H
#define CHECKBOX_WIDGET_H

#include "controls_definitions.h"
#include "mainwindow.h"

#include <QWidget>

namespace Ui {
class CheckBoxWidget;
}

class CheckBoxWidget : public QWidget
{
	Q_OBJECT

public:
	explicit CheckBoxWidget(QWidget *parent = nullptr);
	CheckBoxWidget(QWidget *parent, MainWindow *mainWindow, const CheckBoxControl *control,
			void (*onCheckBoxChanged)(MainWindow *mainWindow, QString type, QString parameter, bool checked));
	~CheckBoxWidget();
	void setState(bool state);

private slots:
	void on_checkBox_stateChanged(int arg1);

private:
	Ui::CheckBoxWidget *ui;
	MainWindow *mainWindow;
	QString type;
	QString parameter;
	void (*onCheckBoxChanged)(MainWindow *mainWindow, QString type, QString parameter, bool value);
};

#endif // CHECKBOX_WIDGET_H
