#ifndef PREVIEW_WINDOW_H
#define PREVIEW_WINDOW_H

#include <QWidget>
#include <QVideoWidget>
#include <QHBoxLayout>

namespace Ui {
class PreviewWindow;
}

class PreviewWindow : public QWidget
{
	Q_OBJECT

public:
	explicit PreviewWindow(QWidget *parent = nullptr);
	~PreviewWindow();
	static void setupCamera(QBoxLayout *horizontalLayout);
	static void setupCamera2(QBoxLayout *horizontalLayout);

private slots:
	// void on_pushButton_clicked();

protected:
	void showEvent(QShowEvent *event);

private:
	Ui::PreviewWindow *ui;
	QVideoWidget *videoWidget;
};

// class GstPlayer2 : public GstPlayer
// {
// 	void setVideoOutput(QWidget *widget);
// };

#endif // PREVIEW_WINDOW_H
