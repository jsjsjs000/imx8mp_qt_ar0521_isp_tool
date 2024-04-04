#ifndef COMBOBOX_WIDGET_H
#define COMBOBOX_WIDGET_H

#include <QWidget>
#include "controls_definitions.h"
#include "mainwindow.h"

namespace Ui {
class ComboBoxWidget;
}

class ComboBoxWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ComboBoxWidget(QWidget *parent = nullptr);
	ComboBoxWidget(QWidget *parent, MainWindow *mainWindow, const ComboBoxControl *control,
			void (*onComboBoxIndexChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value));
	~ComboBoxWidget();
	void setItemIndex(int index);

private slots:
	void on_comboBox_currentIndexChanged(int index);

private:
	Ui::ComboBoxWidget *ui;
	MainWindow *mainWindow;
	QString getCmd;
	QString setCmd;
	QString parameter;
	QMap<int, QString> *map;
	QList<int> mapKeys;								// keys list
	QMap<int, int> mapKeysIndexes;		// keys index
	void (*onComboBoxIndexChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value);
};

#endif // COMBOBOX_WIDGET_H
