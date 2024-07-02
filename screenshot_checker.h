#ifndef SCREENSHOT_CHECKER_H
#define SCREENSHOT_CHECKER_H

#include <QDebug>
#include <QThread>

class ScreenshotChecker : public QThread
{
	Q_OBJECT

private:
	QStringList cachedFilesList;
	void run() override;
	bool checkIsImageCorrect(QString filename);

public:
	bool Stop = false;

	explicit ScreenshotChecker(QObject *parent);
	virtual ~ScreenshotChecker() {};
	void check(bool initial);

signals:
	void signal_show_rename_screenshot_window(QString filename);

public slots:
	void signal_addFilenameToCachedFilesList(QString filename);
};

#endif // SCREENSHOT_CHECKER_H
