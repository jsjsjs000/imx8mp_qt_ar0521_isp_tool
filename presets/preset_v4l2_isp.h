#ifndef PRESET_V4L2_ISP_H
#define PRESET_V4L2_ISP_H

#include "preset.h"

class PresetV4l2Isp
{
	const QString Directory = "/presets/";
	const QString Extension = ".cfg";

public:
	PresetV4l2Isp();
	bool save(QString name, QString params);
	bool load(QList<QString> *list);
};

#endif // PRESET_V4L2_ISP_H
