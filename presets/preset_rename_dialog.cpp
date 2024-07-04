#include "preset_rename_dialog.h"
#include "ui_preset_rename_dialog.h"

#include <QKeyEvent>

PresetRenameDialog::PresetRenameDialog(QDialog *parent)
		: QDialog(parent)
		, ui(new Ui::preset_rename_dialog)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() & (~Qt::WindowMaximizeButtonHint));  /// remove maximize box

	QApplication::instance()->installEventFilter(this);
}

PresetRenameDialog::~PresetRenameDialog()
{
	delete ui;
}

bool PresetRenameDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == ui->edit && event->type() == QEvent::KeyPress)
	{
		QKeyEvent *key = static_cast<QKeyEvent *>(event);
		if (key->key() == Qt::Key_Return)
			on_okButton_clicked();
		else if (key->key() == Qt::Key_Escape)
			on_cancelButton_clicked();

		// qDebug() << "pressed" << key->key();
	}
	return QObject::eventFilter(obj, event);
}

void PresetRenameDialog::setParameters(QString windowTitle, QString labelText)
{
	this->setWindowTitle(windowTitle);
	this->ui->label->setText(labelText);
}

QString PresetRenameDialog::getName()
{
	return this->ui->edit->text();
}

void PresetRenameDialog::on_okButton_clicked()
{
	close();
	setResult(QDialog::Accepted);
}

void PresetRenameDialog::on_cancelButton_clicked()
{
	close();
	setResult(QDialog::Rejected);
}
