#include "screenshot_checker.h"
#include <QDebug>
#include <QDirIterator>
#include <QThread>

ScreenshotChecker::ScreenshotChecker(QObject *parent) : QThread(parent)
{
}

void ScreenshotChecker::run()
{
	// msleep(30);
	// qDebug() << "aaaaaaa";
	this->check(true);

	while (!this->Stop)
	{
			/* 300 ms delay */
		for (int i = 0; i < 10 && !this->Stop; i++)
			msleep(30);

		// qDebug() << "------";
		this->check(false);
	}
}

void ScreenshotChecker::check(bool initial)
{
	const QString path = "/home/weston/";
	QStringList filter = QStringList("wayland-screenshot-*.png");
	QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	if (initial)
	{
		while (it.hasNext())
			cachedFiles << it.next();
		return;
	}

	while (it.hasNext())
	{
		QString filename = it.next();
		if (!cachedFiles.contains(filename))
		{
			cachedFiles << filename;
			qDebug() << filename;
		}
	}
}
