#include "rename_screenshot_window.h"
#include "ui_rename_screenshot_window.h"

#include <QFile>
#include <QKeyEvent>

RenameScreenshotWindow::RenameScreenshotWindow(QWidget *parent)
		: QWidget(parent)
		, ui(new Ui::RenameScreenshotWindow)
{
	ui->setupUi(this);

	QApplication::instance()->installEventFilter(this);
}

RenameScreenshotWindow::~RenameScreenshotWindow()
{
	delete ui;
}

void RenameScreenshotWindow::setParameters(QString filename)
{
	this->originalFilename = filename;

	if (filename.endsWith(".png"))
		filename = filename.replace(".png", "");

	int i = filename.lastIndexOf('/');
	if (i > 0)
	{
		this->folder = filename.left(i + 1);
		filename = filename.mid(i + 1, filename.length() - i - 1);
	}

	this->ui->filenameEdit->setText(filename);
	this->ui->locationLabel->setText("Saved in: " + folder);
}

bool RenameScreenshotWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->filenameEdit && event->type() == QEvent::KeyPress)
	{
		QKeyEvent *key = static_cast<QKeyEvent *>(event);
		if (key->key() == Qt::Key_Return)
			on_savePushButton_clicked();
		else if (key->key() == Qt::Key_Escape)
			on_deletePushButton_clicked();

		// qDebug() << "pressed" << key->key();
	}
	return QObject::eventFilter(obj, event);
}

void RenameScreenshotWindow::on_savePushButton_clicked()
{
	QString filename = this->ui->filenameEdit->text();
	QString newFilename = this->folder + filename + ".png";

	if (newFilename.compare(this->originalFilename) != 0)
	{
		emit signal_addFilenameToCachedFilesList(newFilename);

		QFile file(this->originalFilename);
		file.rename(newFilename);
		qDebug() << "rename" << newFilename << "to" << this->originalFilename;
	}

	this->close();
}

void RenameScreenshotWindow::on_deletePushButton_clicked()
{
	QFile file(this->originalFilename);
	if (!file.remove())
		qDebug() << "can't delete file: " << this->originalFilename;

	this->close();
}
