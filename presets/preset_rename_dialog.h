#ifndef PRESET_RENAME_DIALOG_H
#define PRESET_RENAME_DIALOG_H

#include <QDialog>

namespace Ui {
class preset_rename_dialog;
}

class PresetRenameDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PresetRenameDialog(QDialog *parent = nullptr);
	~PresetRenameDialog();
	void setParameters(QString windowTitle, QString labelText, QString value);
	QString getName();

private slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();

private:
	Ui::preset_rename_dialog *ui;
	bool eventFilter(QObject *obj, QEvent *event);
};

#endif // PRESET_RENAME_DIALOG_H
