#include "presets_xml.h"

PresetsXML::PresetsXML() {}

bool PresetsXML::loadPresetsList(QComboBox *comboBox)
{
	comboBox->clear();

	QList<QString> list;
	if (!presetXml.loadPresetsList(&list))
		return false;

	for (const QString &s : qAsConst(list))
		comboBox->addItem(s);

	return true;
}

bool PresetsXML::load(QString name)
{
	if (!presetXml.load(name))
		return false;

	return true;
}

void PresetsXML::save(QComboBox *comboBox, QString name)
{
	qDebug() << "save - combobox item" << comboBox->currentIndex();

		/* None preset exists */
	if (comboBox->currentIndex() < 0)
		this->add(comboBox, name);
	else
		presetXml.save(name);
}

void PresetsXML::rename(QComboBox *comboBox, QString name)
{
	if (comboBox->currentIndex() < 0)
		return;

	QString oldName = comboBox->currentText();
	comboBox->setItemText(comboBox->currentIndex(), name);

	presetXml.renameConfiguration(oldName, name);
}

void PresetsXML::add(QComboBox *comboBox, QString name)
{
	comboBox->addItem(name);
	comboBox->setCurrentIndex(comboBox->count() - 1);

	presetXml.save(name);
}

void PresetsXML::deleteCurrent(QComboBox *comboBox)
{
	if (comboBox->currentIndex() < 0)
		return;

	QString name = comboBox->currentText();
	comboBox->removeItem(comboBox->currentIndex());

	presetXml.deleteConfiguration(name);
}
