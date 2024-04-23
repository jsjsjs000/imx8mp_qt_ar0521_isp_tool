#include "combobox_widget2.h"
#include "ui_combobox_widget2.h"

ComboBoxWidget2::ComboBoxWidget2(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::ComboBoxWidget2)
{
	ui->setupUi(this);
}

ComboBoxWidget2::ComboBoxWidget2(QWidget *parent, MainWindow *mainWindow, const ComboBoxControl2 *control,
		void (*onComboBoxIndexChanged)(MainWindow *mainWindow, QString getCmd, QString setCmd, QString parameter, QString key, QString value))
		: QWidget(parent)
		, ui(new Ui::ComboBoxWidget2)
{
	ui->setupUi(this);

	this->mainWindow = mainWindow;
	this->getCmd = control->getCmd;
	this->setCmd = control->setCmd;
	this->parameter = control->parameter;
	this->map = control->map;

	ui->label->setText(control->name);
	ui->label->setToolTip(control->description);

	int i = 0;
	for (auto a = this->map->cbegin(), end = this->map->cend(); a != end; a++)
	{
		this->mapKeys.append(a.key());
		this->mapKeysIndexes[a.key()] = i++;
		ui->comboBox->addItem(QString(a.value()));
	}

	this->onComboBoxIndexChanged = onComboBoxIndexChanged;
	this->initialized = true;
}

ComboBoxWidget2::~ComboBoxWidget2()
{
	delete ui;
}

void ComboBoxWidget2::setItemIndex(int key)
{
	// int index = this->mapKeysIndexes[key];

	// if (index == -1)
	// 	qDebug() << "Can't find key" << key << "in combobox";
	// else
	// {
	// 	// qDebug() << "map" << this->map->keys() << this->map->values();
	// 	// qDebug() << "key" << key;
	// 	// qDebug() << "new index" << this->parameter << index;
	// 	if (this->ui->comboBox->currentIndex() != index)
	// 		this->ui->comboBox->setCurrentIndex(index);
	// }
}

void ComboBoxWidget2::on_comboBox_currentIndexChanged(int index)
{
	if (!this->initialized)
		return;

	const QString key = this->mapKeys[index];
	QString value = this->map->value(key);
	qDebug() << "on cb index changed" << index << key << value;
	if (this->onComboBoxIndexChanged)
		(*this->onComboBoxIndexChanged)(this->mainWindow, this->getCmd, this->setCmd, this->parameter, key, value);
}
