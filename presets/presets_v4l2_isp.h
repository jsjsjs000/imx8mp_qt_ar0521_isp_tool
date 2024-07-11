#ifndef PRESETS_H
#define PRESETS_H

#include <QComboBox>
#include <QList>
#include "preset_v4l2_isp.h"

class PresetsV4L2Isp
{
private:
	PresetV4l2Isp presetV4l2Isp;

public:
	PresetsV4L2Isp();
	bool loadPresetsList(QComboBox *comboBox);
	bool load(QString name, QMap<QString, QString> *params);
	void save(QComboBox *comboBox, QString name, QString params);
	void rename(QComboBox *comboBox, QString name);
	void add(QComboBox *comboBox, QString name, QString params);
	void deleteCurrent(QComboBox *comboBox);
};

#endif // PRESETS_H
