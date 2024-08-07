#include "preset_xml.h"

#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

PresetXML::PresetXML() {}

bool PresetXML::loadPresetsList(QStringList *list)
{
	try
	{
		QStringList filter = QStringList(XmlFilename + XmlResolution + "*" + XmlExtension);
		QDirIterator it(XmlDirectory, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		while (it.hasNext())
		{
			QString name = it.next();
			if (!name.startsWith(XmlDirectory))
				continue;

			name = name.mid((XmlDirectory).length());
			if (!name.endsWith(XmlExtension))
				continue;

			name = name.mid(0, name.length() - XmlExtension.length());
			if (!name.startsWith(XmlFilename + XmlResolution))
				continue;

			name = name.mid(XmlFilename.length() +  + XmlResolution.length());
			if (!XmlSkipFilenames.contains(name))
				list->push_back(name);
		}

		return true;
	}
	catch (...)
	{
		qDebug() << "Can't list preset folder" << XmlDirectory;
		return false;
	}
}

bool PresetXML::save(QString name)
{
	try
	{
		return QFile().copy(
				XmlDirectory + XmlFilename + XmlResolution + XmlExtension,
				XmlDirectory + XmlFilename + XmlResolution + "_" + name + XmlExtension);
	}
	catch (...)
	{
		qDebug() << "Can't save file" << XmlDirectory + name + XmlExtension;
		return false;
	}
}

bool PresetXML::load(QString name)
{
	try
	{
		return QFile().copy(
				XmlDirectory + XmlFilename + XmlResolution + "_" + name + XmlExtension,
				XmlDirectory + XmlFilename + XmlResolution + XmlExtension);
	}
	catch (...)
	{
		qDebug() << "Can't read file" << XmlDirectory + name + XmlExtension;
		return false;
	}
}

bool PresetXML::deleteConfiguration(QString name)
{
	try
	{
		return QFile().remove(XmlDirectory + XmlFilename + XmlResolution + "_" + name + XmlExtension);
	}
	catch (...)
	{
		qDebug() << "Can't delete preset" << XmlDirectory + XmlFilename + XmlResolution + "_" + name + XmlExtension;
		return false;
	}
}

bool PresetXML::renameConfiguration(QString oldName, QString newName)
{
	try
	{
		return QFile().rename(
				XmlDirectory + XmlFilename + XmlResolution + "_" + oldName + XmlExtension,
				XmlDirectory + XmlFilename + XmlResolution + "_" + newName + XmlExtension);
	}
	catch (...)
	{
		qDebug() << "Can't rename preset" << XmlDirectory;
		return false;
	}
}

bool PresetXML::saveDefaultPreset(QString name)
{
	try
	{
		QFile file(QCoreApplication::applicationDirPath() + "/" + DefaultPresetConfig());
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
			return false;

		QTextStream out(&file);
		out << name;

		file.close();
		return true;
	}
	catch (...)
	{
		qDebug() << "Can't save file" << QCoreApplication::applicationDirPath() + "/" + DefaultPresetConfig();
		return false;
	}
}

bool PresetXML::loadDefaultPreset(QString *name)
{
	try
	{
		QFile file(QCoreApplication::applicationDirPath() + "/" + DefaultPresetConfig());
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return false;

		QTextStream in(&file);
		while (!in.atEnd())
			*name = in.readLine();

		file.close();
		return true;
	}
	catch (...)
	{
		qDebug() << "Can't read file" << QCoreApplication::applicationDirPath() + "/" + DefaultPresetConfig();
		return false;
	}
}
