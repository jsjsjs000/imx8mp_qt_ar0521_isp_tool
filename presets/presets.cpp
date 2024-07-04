#include "presets.h"

Presets::Presets() {}

void Presets::save(QComboBox *comboBox, QString name)
{
	qDebug() << "ci" << comboBox->currentIndex();

		/* None preset exists */
	if (comboBox->currentIndex() < 0)
	{
		Preset *preset = new Preset();
		preset->name = name;
		presetsList.push_back(preset);

		comboBox->addItem(name, QVariant::fromValue(preset));
	}
	else
	{

	}
}
