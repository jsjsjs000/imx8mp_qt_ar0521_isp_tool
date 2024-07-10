#ifndef PRESET_XML_H
#define PRESET_XML_H

#include <QStringList>

class PresetXML
{
	const QString XmlDirectory  = "/opt/imx8-isp/bin/xml/";
	const QString XmlFilename   = "VM-017-COL_AO062-C_";
	const QString XmlResolution = "1920x1080";
	const QString XmlExtension  = ".xml";

	const QStringList XmlSkipFilenames = { "", "_factory" };

	static const QString DefaultPresetConfig() { return "default_xml_preset.cfg"; }

public:
	PresetXML();
	bool load(QString name, QMap<QString, QString> *params);
	bool save(QString name, QString params);
	bool loadPresetsList(QStringList *list);
	bool deleteConfiguration(QString name);
	bool renameConfiguration(QString oldName, QString newName);
	static bool saveDefaultPreset(QString name);
	static bool loadDefaultPreset(QString *name);
};

#endif // PRESET_XML_H
