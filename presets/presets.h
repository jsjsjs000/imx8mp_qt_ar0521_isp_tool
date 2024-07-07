#ifndef PRESETS_H
#define PRESETS_H

#include "preset.h"
#include "preset_v4l2_isp.h"
#include <QComboBox>
#include <QList>

class Presets
{
private:
	// QList<Preset*> presetsList;
	PresetV4l2Isp presetV4l2Isp;

	void printDebug();

public:
	Presets();
	bool loadPresetsList(QComboBox *comboBox);
	bool load(QString name, QMap<QString, QString> *params);
	void save(QComboBox *comboBox, QString name, QString params);
	void rename(QComboBox *comboBox, QString name);
	void add(QComboBox *comboBox, QString name, QString params);
	void deleteCurrent(QComboBox *comboBox);
};

#endif // PRESETS_H
