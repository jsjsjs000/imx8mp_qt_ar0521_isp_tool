#ifndef PRESET_V4L2_ISP_H
#define PRESET_V4L2_ISP_H

#include "preset.h"

class PresetV4l2Isp
{
	const QString Directory = "/presets/";
	const QString Extension = ".cfg";
	static const QString DefaultPresetConfig() { return "default_preset.cfg"; }

public:
	PresetV4l2Isp();
	bool load(QString name, QMap<QString, QString> *params);
	bool save(QString name, QString params);
	bool loadPresetsList(QStringList *list);
	bool deleteConfiguration(QString name);
	bool renameConfiguration(QString oldName, QString newName);
	static bool saveDefaultPreset(QString name);
	static bool loadDefaultPreset(QString *name);
};

#endif // PRESET_V4L2_ISP_H
