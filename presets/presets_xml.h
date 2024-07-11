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
	bool load(QString name, QMap<QString, QString> *params);
	void save(QComboBox *comboBox, QString name, QString params);
	void rename(QComboBox *comboBox, QString name);
	void add(QComboBox *comboBox, QString name, QString params);
	void deleteCurrent(QComboBox *comboBox);
};

#endif // PRESETS_XML_H
