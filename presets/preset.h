#ifndef PRESET_H
#define PRESET_H

#include <QString>

class Preset
{
public:
	bool isDefault = false;
	QString name = "";

	Preset();
};

#endif // PRESET_H
