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
	ComboBoxWidget2(QWidget *parent, MainWindow *mainWindow, const ComboBoxControl2 *control,
			void (*onComboBoxIndexChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString key, QString value));
	~ComboBoxWidget2();
	void setItemIndex(QString index);

private slots:
	void on_comboBox_currentIndexChanged(int index);

private:
	Ui::ComboBoxWidget2 *ui;
	MainWindow *mainWindow;
	QString getCmd;
	QString setCmd;
	QString parameter;
	QMap<QString, QString> *map;
	QList<QString> mapKeys;								// keys list
	QMap<QString, int> mapKeysIndexes;		// keys index
	bool initialized = false;
	void (*onComboBoxIndexChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString key, QString value) = nullptr;
};

#endif // COMBOBOX_WIDGET2_H
