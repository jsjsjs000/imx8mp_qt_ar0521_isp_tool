#ifndef PREVIEW_WINDOW_H
#define PREVIEW_WINDOW_H

#include <QWidget>
#include <QVideoWidget>

namespace Ui {
class PreviewWindow;
}

class PreviewWindow : public QWidget
{
	Q_OBJECT

public:
	explicit PreviewWindow(QWidget *parent = nullptr);
	~PreviewWindow();

private slots:

private:
	Ui::PreviewWindow *ui;
	QVideoWidget *videoWidget;
};

#endif // PREVIEW_WINDOW_H
