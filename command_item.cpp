#include "command_item.h"

CommandItem::CommandItem() {}

CommandItem::CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, int value, int divide)
{
	this->commandItemType = commandItemType;
	this->getCmd = getCmd;
	this->setCmd = setCmd;
	this->parameter = parameter;
	this->value = value;
	this->divide = divide;
}

CommandItem::CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, bool boolValue)
{
	this->commandItemType = commandItemType;
	this->getCmd = getCmd;
	this->setCmd = setCmd;
	this->parameter = parameter;
	this->boolValue = boolValue;
}

CommandItem::CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, QList<float> array)
{
	this->commandItemType = commandItemType;
	this->getCmd = getCmd;
	this->setCmd = setCmd;
	this->parameter = parameter;
	this->array = array;
}

CommandItem::CommandItem(CommandItemType commandItemType, QString getCmd, QString setCmd, QString parameter, QString string)
{
	this->commandItemType = commandItemType;
	this->getCmd = getCmd;
	this->setCmd = setCmd;
	this->parameter = parameter;
	this->string = string;
}
