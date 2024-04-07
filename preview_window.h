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
	// void on_pushButton_clicked();

protected:
	void showEvent(QShowEvent *event);

private:
	Ui::PreviewWindow *ui;
	QVideoWidget *videoWidget;
	void setupCamera(void);
};

// class GstPlayer2 : public GstPlayer
// {
// 	void setVideoOutput(QWidget *widget);
// };

#endif // PREVIEW_WINDOW_H
