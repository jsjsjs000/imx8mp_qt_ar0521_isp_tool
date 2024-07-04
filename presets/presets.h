#ifndef PRESETS_H
#define PRESETS_H

#include "preset.h"
#include <QComboBox>
#include <QList>

class Presets
{
private:
	QList<Preset*> presetsList;

	void printDebug();

public:
	Presets();
	void save(QComboBox *comboBox, QString name);
	void rename(QComboBox *comboBox, QString name);
	void add(QComboBox *comboBox, QString name);
	void deleteCurrent(QComboBox *comboBox);
};

#endif // PRESETS_H
