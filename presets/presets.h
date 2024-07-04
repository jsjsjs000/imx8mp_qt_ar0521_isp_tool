#ifndef PRESETS_H
#define PRESETS_H

#include "preset.h"
#include <QComboBox>
#include <QList>

class Presets
{
private:
	QList<Preset*> presetsList;
	int currentPresetIndex = -1;

public:
	Presets();
	void save(QComboBox *comboBox, QString name);
};

#endif // PRESETS_H
