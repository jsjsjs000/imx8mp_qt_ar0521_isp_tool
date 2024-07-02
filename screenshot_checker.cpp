#include "qscreen.h"
#include "rename_screenshot_window.h"
#include "screenshot_checker.h"
#include <QDebug>
#include <QDirIterator>
#include <QGuiApplication>
#include <QThread>

ScreenshotChecker::ScreenshotChecker(QObject *parent) : QThread(parent)
{
}

void ScreenshotChecker::run()
{
	this->check(true);

	while (!this->Stop)
	{
			/* 300 ms delay */
		for (int i = 0; i < 10 && !this->Stop; i++)
			msleep(30);

		this->check(false);
	}
}

void ScreenshotChecker::slot_addFilenameToCachedFilesList(QString filename)
{
	cachedFilesList << filename;
}

void ScreenshotChecker::check(bool initial)
{
	const QString path = "/home/weston/";
	QStringList filter = QStringList("wayland-screenshot-*.png");
	QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	if (initial)
	{
		while (it.hasNext())
			cachedFilesList << it.next();
		return;
	}

	while (it.hasNext())
	{
		QString filename = it.next();
		if (!cachedFilesList.contains(filename) && this->checkIsImageCorrect(filename))
		{
			cachedFilesList << filename;
			emit signal_show_rename_screenshot_window(filename);
		}
	}
}

bool ScreenshotChecker::checkIsImageCorrect(QString filename)
{
	try
	{
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly))
			return false;

		const int ReadLength = 12;
		int size = file.size();
		if (size < ReadLength)
			return false;

		file.seek(size - ReadLength);

		char buffer[ReadLength] = {0};
		if (file.read(buffer, ReadLength) != ReadLength)
			return false;

		return buffer[0] == 0 && buffer[1] == 0 && buffer[2] == 0 && buffer[3] == 0 &&
				buffer[4] == 'I' && buffer[5] == 'E' && buffer[6] == 'N' && buffer[7] == 'D';
	}
	catch (...)
	{
		return false;
	}
}
