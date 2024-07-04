#include "presets.h"

Presets::Presets() {}

void Presets::save(QComboBox *comboBox, QString name)
{
	qDebug() << "ci" << comboBox->currentIndex();

		/* None preset exists */
	if (comboBox->currentIndex() < 0)
	{
		this->add(comboBox, name);
	}
	else
	{

	}

this->printDebug();
}

void Presets::add(QComboBox *comboBox, QString name)
{
	Preset *preset = new Preset();
	preset->name = name;
	presetsList.push_back(preset);

	comboBox->addItem(name, QVariant::fromValue(preset));
	comboBox->setCurrentIndex(comboBox->count() - 1);

this->printDebug();
}

void Presets::deleteCurrent(QComboBox *comboBox)
{
	if (comboBox->currentIndex() < 0)
		return;

	Preset *preset = presetsList[comboBox->currentIndex()];
	delete preset;
	presetsList.removeAt(comboBox->currentIndex());

	comboBox->removeItem(comboBox->currentIndex());

this->printDebug();
}

void Presets::printDebug()
{
	qDebug() << "Presets list:" << this->presetsList.count();
	int i = 0;
	for (const Preset *preset : this->presetsList)
	{
		qDebug() << i++;
		qDebug() << "  name:" << preset->name;
		qDebug() << "  default:" << preset->isDefault;
	}
	qDebug() << "";
}
