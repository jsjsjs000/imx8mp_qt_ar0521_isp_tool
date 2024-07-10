#include <QString>
#include "controls_definitions.h"

SaveControl* ControlsDefinitions::getSaveControls(QString name)
{
	QStringList words = name.split("|");
	if (words.count() != 2)
		return nullptr;

	QString cmd = words[0];
	QString param = words[1];
	for (int i = 0; i < saveControls.count(); i++)
		if ((saveControls[i]->getCmd == cmd || saveControls[i]->getCmd == nullptr) && saveControls[i]->parameter == param)
			return saveControls[i];

	return nullptr;
}

SaveControl* ControlsDefinitions::getSaveControls(QString name, int priority)
{
	QStringList words = name.split("|");
	if (words.count() != 2)
		return nullptr;

	QString cmd = words[0];
	QString param = words[1];
	for (int i = 0; i < saveControls.count(); i++)
		if ((saveControls[i]->getCmd == cmd || saveControls[i]->getCmd == nullptr) && saveControls[i]->parameter == param &&
				saveControls[i]->priority == priority)
			return saveControls[i];

	return nullptr;
}
