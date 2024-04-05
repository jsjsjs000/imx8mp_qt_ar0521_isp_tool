#ifndef CONTROLS_H
#define CONTROLS_H

#include <cmath>
#include <QVector>
#include <QString>
#include <QMap>

class Control
{
	virtual void v(void) = 0;  // pure virtual, need virtual method for dynamic_cast<>()

public:
	QString setCmd;
	QString getCmd;
	QString parameter;
	QString value;
	QString name;
	QString description;
	bool readonly;
};

class GroupControl : public Control
{
	void v(void) override {}  // required

public:
	GroupControl(QString name)
	{
		this->name = name;
	}
};

class SliderControl : public Control
{
	void v(void) override {}  // required

public:
	int min;
	int max;
	int value;
	int precision;
	int multiple;

	SliderControl();
	SliderControl(QString getCmd, QString setCmd, QString parameter, QString name, int min, int max, int value,
			QString description, bool readonly = false)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->readonly = readonly;
		this->min = min;
		this->max = max;
		this->value = value;
		this->precision = 0;
		this->multiple = 1;
	}
	SliderControl(QString getCmd, QString setCmd, QString parameter, QString name, float min, float max, float value,
			int digits, QString description, bool readonly = false)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->readonly = readonly;
		this->precision = digits;
		this->multiple = std::pow(10, digits);
		this->min = min * this->multiple;
		this->max = max * this->multiple;
		this->value = value * this->multiple;
	}
};

class ComboBoxControl : public Control
{
	void v(void) override {}  // required

public:
	QMap<int, QString> *map;

	ComboBoxControl();
	ComboBoxControl(QString getCmd, QString setCmd, QString parameter, QString name, QMap<int, QString> *map,
			QString description, bool readonly = false)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->map = map;
		this->readonly = readonly;
	}
};

class CheckBoxControl : public Control
{
	void v(void) override {}  // required

public:
	bool checked;

	CheckBoxControl(QString getCmd, QString setCmd, QString parameter, QString name, bool checked, QString description)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->checked = checked;
		this->description = description;
	}
};

class ButtonControl : public Control
{
	void v(void) override {}  // required

public:
	ButtonControl(QString getCmd, QString setCmd, QString parameter, QString value, QString name, QString description)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->value = value;
		this->name = name;
		this->description = description;
	}
};

class LabelControl : public Control
{
	void v(void) override {}  // required

public:
	QString label;
	const std::type_info *type;

	LabelControl(QString getCmd, QString parameter, QString name, QString description, const std::type_info *type)
	{
		this->getCmd = getCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->type = type;
	}
};

#endif // CONTROLS_H
