#ifndef CONTROLS_H
#define CONTROLS_H

#include <cmath>
#include <QVector>
#include <QString>
#include <QMap>
#include <QDebug>

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

class SliderArrayControl : public Control
{
	void v(void) override {}  // required

public:
	QList<int> min;
	QList<int> max;
	QList<int> value;
	QList<int> precision;
	QList<int> multiple;
	QList<QString> descriptions;

	SliderArrayControl();
	SliderArrayControl(QString getCmd, QString setCmd, QString parameter, QString name, QList<QList<int>> *min_max_value,
			QString description, QList<QString> descriptions, bool readonly = false)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->descriptions = descriptions;
		this->readonly = readonly;
		for (const QList<int> &item : *min_max_value)
		{
			if (item.length() != 3)
			{
				qFatal("SliderArrayControl - initialization parameter min_max_value wrong length");
				return;
			}

			this->min.push_back(item[0]);
			this->max.push_back(item[1]);
			this->value.push_back(item[2]);
			this->precision.push_back(0);
			this->multiple.push_back(1);
		}
	}
	SliderArrayControl(QString getCmd, QString setCmd, QString parameter, QString name, QList<QList<float>> *min_max_value_digits,
			QString description, QList<QString> descriptions, bool readonly = false)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->descriptions = descriptions;
		this->readonly = readonly;
		for (const QList<float> &item : *min_max_value_digits)
		{
			if (item.length() != 4)
			{
				qFatal("SliderArrayControl - initialization parameter min_max_value wrong length");
				return;
			}

			int digits = item[3];
			this->precision.push_back(digits);
			int multiple = std::pow(10, digits);
			this->multiple.push_back(multiple);
			this->min.push_back(item[0] * multiple);
			this->max.push_back(item[1] * multiple);
			this->value.push_back(item[2] * multiple);
		}
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

class ComboBoxControl2 : public Control
{
	void v(void) override {}  // required

public:
	QMap<QString, QString> *map;

	ComboBoxControl2();
	ComboBoxControl2(QString getCmd, QString setCmd, QString parameter, QString name, QMap<QString, QString> *map,
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

class ChartControl : public Control
{
	void v(void) override {}  // required

public:
	float y1, y2, gridY;
	const std::type_info *type;
	bool readonly;
	bool gammaCurve;

	ChartControl(QString getCmd, QString setCmd, QString parameter, QString name, QString description, float y1, float y2, float gridY, const std::type_info *type, bool readonly = false, bool gammaCurve = false)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->y1 = y1;
		this->y2 = y2;
		this->gridY = gridY;
		this->type = type;
		this->readonly = readonly;
		this->gammaCurve = gammaCurve;
	}
};

class MatrixViewControl : public Control
{
	void v(void) override {}  // required

public:
	int columnsCount;
	int rowsCount;

	MatrixViewControl(QString getCmd, QString parameter, QString name, int columnsCount, int rowsCount, QString description)
	{
		this->getCmd = getCmd;
		this->parameter = parameter;
		this->name = name;
		this->columnsCount = columnsCount;
		this->rowsCount = rowsCount;
		this->description = description;
	}
};

class SaveControl
{
public:
	static const int MinPriority = -5;
	static const int MaxPriority = 5;

	QString getCmd;
	QString setCmd;
	QString parameter;
	int priority;
	std::optional<bool> enableDisable;

		/// priority: (-5 - 5) - default 0 - execution from -5 to 5
	SaveControl(QString getCmd, QString setCmd, QString parameter, int priority = 0,
			std::optional<bool> enableDisable = std::nullopt)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->priority = priority;
		this->enableDisable = enableDisable;
	}
};

#endif // CONTROLS_H
