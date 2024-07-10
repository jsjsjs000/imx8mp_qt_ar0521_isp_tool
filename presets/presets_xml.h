#ifndef PRESETS_XML_H
#define PRESETS_XML_H

#include "preset_xml.h"

#include <QComboBox>

class PresetsXML
{
	PresetXML presetXml;

public:
	PresetsXML();
	bool loadPresetsList(QComboBox *comboBox);
};

#endif // PRESETS_XML_H
