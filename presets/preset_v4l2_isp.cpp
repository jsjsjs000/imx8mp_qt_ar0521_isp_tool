#include "preset_v4l2_isp.h"
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>

PresetV4l2Isp::PresetV4l2Isp()
{
	if (!QDir(QCoreApplication::applicationDirPath() + Directory).exists())
		QDir().mkdir(QCoreApplication::applicationDirPath() + Directory);
}

bool PresetV4l2Isp::save(QString name, QString params)
{
	try
	{
qDebug() << "save" << QCoreApplication::applicationDirPath() + Directory + name + Extension;

		QFile file(QCoreApplication::applicationDirPath() + Directory + name + Extension);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
			return false;

qDebug() << "pos" << file.pos();

		QTextStream out(&file);
		out << params;

		file.close();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool PresetV4l2Isp::load(QList<QString> *list)
{
	QStringList filter = QStringList("*" + Extension);
	QDirIterator it(QCoreApplication::applicationDirPath() + Directory, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	while (it.hasNext())
		list->push_back(it.next());

	return true;
}
