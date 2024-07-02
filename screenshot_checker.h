#ifndef SCREENSHOT_CHECKER_H
#define SCREENSHOT_CHECKER_H

#include <QDebug>
#include <QThread>

class ScreenshotChecker : public QThread
{
	Q_OBJECT

private:
	QStringList cachedFiles;
	void run() override;

public:
	bool Stop = false;

	explicit ScreenshotChecker(QObject *parent);
	virtual ~ScreenshotChecker() {};
	void check(bool initial);
};

#endif // SCREENSHOT_CHECKER_H
