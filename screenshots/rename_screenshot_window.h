#ifndef RENAME_SCREENSHOT_WINDOW_H
#define RENAME_SCREENSHOT_WINDOW_H

#include <QWidget>

namespace Ui {
class RenameScreenshotWindow;
}

class RenameScreenshotWindow : public QWidget
{
	Q_OBJECT

public:
	explicit RenameScreenshotWindow(QWidget *parent = nullptr);
	~RenameScreenshotWindow();
	void setParameters(QString filename);

private slots:
	void on_savePushButton_clicked();
	void on_deletePushButton_clicked();

signals:
	void signal_addFilenameToCachedFilesList(QString filename);

private:
	Ui::RenameScreenshotWindow *ui;
	QString originalFilename;
	QString folder;

protected:
	bool eventFilter(QObject *obj, QEvent *event);
};

#endif // RENAME_SCREENSHOT_WINDOW_H
