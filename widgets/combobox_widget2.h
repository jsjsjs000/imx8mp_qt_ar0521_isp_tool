#ifndef COMBOBOX_WIDGET2_H
#define COMBOBOX_WIDGET2_H

#include <QWidget>
#include "controls_definitions.h"
#include "mainwindow.h"

namespace Ui {
class ComboBoxWidget2;
}

class ComboBoxWidget2 : public QWidget
{
	Q_OBJECT

public:
	explicit ComboBoxWidget2(QWidget *parent = nullptr);
	ComboBoxWidget2(QWidget *parent, MainWindow *mainWindow, const ComboBoxControl *control,
			void (*onComboBoxIndexChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value));
	~ComboBoxWidget2();
	void setItemIndex(int index);

private slots:
	void on_comboBox_currentIndexChanged(int index);

private:
	Ui::ComboBoxWidget2 *ui;
	MainWindow *mainWindow;
	QString getCmd;
	QString setCmd;
	QString parameter;
	QMap<int, QString> *map;
	QList<int> mapKeys;								// keys list
	QMap<int, int> mapKeysIndexes;		// keys index
	bool initialized = false;
	void (*onComboBoxIndexChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, int key, QString value) = nullptr;
};

#endif // COMBOBOX_WIDGET2_H
