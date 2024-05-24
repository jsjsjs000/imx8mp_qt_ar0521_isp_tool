#ifndef COMMANDITEM_H
#define COMMANDITEM_H

#include <QList>
#include <QString>

class CommandItem
{
public:
	enum CommandItemType { Bool, Number, Array, String };

	CommandItem();
	CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, int value, int divide);
	CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, bool boolValue);
	CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, QList<float> array);
	CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, QString string);

	CommandItemType commandItemType;
	QString getCmd;
	QString setCmd;
	QString parameter;
	int value;
	int divide;
	bool boolValue;
	QList<float> array;
	QString string;
};

#endif // COMMANDITEM_H
