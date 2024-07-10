#include "preset_v4l2_isp.h"
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

PresetV4l2Isp::PresetV4l2Isp()
{
	if (!QDir(QCoreApplication::applicationDirPath() + Directory).exists())
		QDir().mkdir(QCoreApplication::applicationDirPath() + Directory);
}

bool PresetV4l2Isp::loadPresetsList(QStringList *list)
{
	try
	{
		QStringList filter = QStringList("*" + Extension);
		QDirIterator it(QCoreApplication::applicationDirPath() + Directory, filter,
										QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		while (it.hasNext())
		{
			QString name = it.next();
			if (!name.startsWith(QCoreApplication::applicationDirPath() + Directory))
				continue;

			name = name.mid((QCoreApplication::applicationDirPath() + Directory).length());
			if (!name.endsWith(Extension))
				continue;

			name = name.mid(0, name.length() - Extension.length());
			list->push_back(name);
		}

		return true;
	}
	catch (...)
	{
		qDebug() << "Can't list preset folder" << QCoreApplication::applicationDirPath() + Directory;
		return false;
	}
}

bool PresetV4l2Isp::save(QString name, QString params)
{
	try
	{
		QFile file(QCoreApplication::applicationDirPath() + Directory + name + Extension);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
			return false;

		QTextStream out(&file);
		out << params;

		file.close();
		return true;
	}
	catch (...)
	{
		qDebug() << "Can't save file" << QCoreApplication::applicationDirPath() + Directory + name + Extension;
		return false;
	}
}

bool PresetV4l2Isp::load(QString name, QMap<QString, QString> *params)
{
	try
	{
		QFile file(QCoreApplication::applicationDirPath() + Directory + name + Extension);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return false;

		QTextStream in(&file);
		while (!in.atEnd())
		{
			QString line = in.readLine();
			QStringList words = line.split("=");
			if (words.count() == 2)
				(*params)[words[0]] = words[1];
		}

		file.close();
		return true;
	}
	catch (...)
	{
		qDebug() << "Can't read file" << QCoreApplication::applicationDirPath() + Directory + name + Extension;
		return false;
	}
}

bool PresetV4l2Isp::deleteConfiguration(QString name)
{
	try
	{
		return QFile().remove(QCoreApplication::applicationDirPath() + Directory + name + Extension);
	}
	catch (...)
	{
		qDebug() << "Can't delete preset" << QCoreApplication::applicationDirPath() + Directory;
		return false;
	}
}

bool PresetV4l2Isp::renameConfiguration(QString oldName, QString newName)
{
	try
	{
		return QFile().rename(QCoreApplication::applicationDirPath() + Directory + oldName + Extension,
				QCoreApplication::applicationDirPath() + Directory + newName + Extension);
	}
	catch (...)
	{
		qDebug() << "Can't delete preset" << QCoreApplication::applicationDirPath() + Directory;
		return false;
	}
}

bool PresetV4l2Isp::saveDefaultPreset(QString name)
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

bool PresetV4l2Isp::loadDefaultPreset(QString *name)
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
