#include "presets_v4l2_isp.h"

PresetsV4L2Isp::PresetsV4L2Isp() {}

bool PresetsV4L2Isp::loadPresetsList(QComboBox *comboBox)
{
	comboBox->clear();

	QList<QString> list;
	if (!presetV4l2Isp.loadPresetsList(&list))
		return false;

	for (const QString &s : qAsConst(list))
		comboBox->addItem(s);

	return true;
}

bool PresetsV4L2Isp::load(QString name, QMap<QString, QString> *params)
{
	if (!presetV4l2Isp.load(name, params))
		return false;

	return true;
}

void PresetsV4L2Isp::save(QComboBox *comboBox, QString name, QString params)
{
		/* None preset exists */
	if (comboBox->currentIndex() < 0)
		this->add(comboBox, name, params);
	else
		presetV4l2Isp.save(name, params);
}

void PresetsV4L2Isp::rename(QComboBox *comboBox, QString name)
{
	if (comboBox->currentIndex() < 0)
		return;

	QString oldName = comboBox->currentText();
	comboBox->setItemText(comboBox->currentIndex(), name);

	presetV4l2Isp.renameConfiguration(oldName, name);
}

void PresetsV4L2Isp::add(QComboBox *comboBox, QString name, QString params)
{
	comboBox->addItem(name);
	comboBox->setCurrentIndex(comboBox->count() - 1);

	presetV4l2Isp.save(name, params);
}

void PresetsV4L2Isp::deleteCurrent(QComboBox *comboBox)
{
	if (comboBox->currentIndex() < 0)
		return;

	QString name = comboBox->currentText();
	comboBox->removeItem(comboBox->currentIndex());

	presetV4l2Isp.deleteConfiguration(name);
}
