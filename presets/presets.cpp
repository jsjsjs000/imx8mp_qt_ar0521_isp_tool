#include "presets.h"

Presets::Presets() {}

bool Presets::loadPresetsList(QComboBox *comboBox)
{
	comboBox->clear();

	QList<QString> list;
	if (!presetV4l2Isp.loadPresetsList(&list))
		return false;

	for (const QString &s : qAsConst(list))
		comboBox->addItem(s);

	return true;
}

bool Presets::load(QString name, QMap<QString, QString> *params)
{
	if (!presetV4l2Isp.load(name, params))
		return false;

	return true;
}

void Presets::save(QComboBox *comboBox, QString name, QString params)
{
// qDebug() << "combobox item" << comboBox->currentIndex();

		/* None preset exists */
	if (comboBox->currentIndex() < 0)
		this->add(comboBox, name, params);
	else
		presetV4l2Isp.save(name, params);

this->printDebug();
}

void Presets::rename(QComboBox *comboBox, QString name)
{
	if (comboBox->currentIndex() < 0)
		return;

	// QString oldName = presetsList[comboBox->currentIndex()]->name;
	// presetsList[comboBox->currentIndex()]->name = name;

	QString oldName = comboBox->currentText();
	comboBox->setItemText(comboBox->currentIndex(), name);

	presetV4l2Isp.renameConfiguration(oldName, name);

this->printDebug();
}

void Presets::add(QComboBox *comboBox, QString name, QString params)
{
	Preset *preset = new Preset();
	preset->name = name;
	// presetsList.push_back(preset);

	comboBox->addItem(name, QVariant::fromValue(preset));
	comboBox->setCurrentIndex(comboBox->count() - 1);

	presetV4l2Isp.save(name, params);

this->printDebug();
}

void Presets::deleteCurrent(QComboBox *comboBox)
{
	if (comboBox->currentIndex() < 0)
		return;

	// Preset *preset = presetsList[comboBox->currentIndex()];
	// QString name = QString(preset->name);
	// delete preset;
	// presetsList.removeAt(comboBox->currentIndex());

	QString name = comboBox->currentText();
	comboBox->removeItem(comboBox->currentIndex());

	presetV4l2Isp.deleteConfiguration(name);

this->printDebug();
}

void Presets::printDebug()
{
	// qDebug() << "Presets list:" << this->presetsList.count();
	// int i = 0;
	// for (const Preset *preset : this->presetsList)
	// {
	// 	qDebug() << i++;
	// 	qDebug() << "  name:" << preset->name;
	// 	qDebug() << "  default:" << preset->isDefault;
	// }
	// qDebug() << "";
}
