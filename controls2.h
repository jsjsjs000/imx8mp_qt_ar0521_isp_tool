#ifndef CONTROLS2_H
#define CONTROLS2_H

#include <QString>
#include <QList>
#include <cmath>

class Control2
{
	virtual void v(void) = 0;  // pure virtual, need virtual method for dynamic_cast<>()

public:
	QString node;
	QString name;
	QString description;
	bool readonly;
};

class GroupControl2 : public Control2
{
	void v(void) override {}  // required

public:
	GroupControl2(QString name)
	{
		this->name = name;
	}
};

class SliderControl2 : public Control2
{
	void v(void) override {}  // required

public:
	int min;
	int max;
	int value;
	int precision;
	int multiple;

	SliderControl2();
	SliderControl2(QString node, QString name, int min, int max, int value,
			QString description, bool readonly = false)
	{
		this->node = node;
		this->name = name;
		this->description = description;
		this->readonly = readonly;
		this->min = min;
		this->max = max;
		this->value = value;
		this->precision = 0;
		this->multiple = 1;
	}
	SliderControl2(QString node, QString name, float min, float max, float value,
			int digits, QString description, bool readonly = false)
	{
		this->node = node;
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

class SliderArrayControl2 : public Control2
{
	void v(void) override {}  // required

public:
	QList<int> min;
	QList<int> max;
	QList<int> value;
	QList<int> precision;
	QList<int> multiple;

	SliderArrayControl2();
	SliderArrayControl2(QString node, QString name, QList<QList<int>> *min_max_value,
			QString description, bool readonly = false)
	{
		this->node = node;
		this->name = name;
		this->description = description;
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
	SliderArrayControl2(QString node, QString name, QList<QList<float>> *min_max_value_digits,
			QString description, bool readonly = false)
	{
		this->node = node;
		this->name = name;
		this->description = description;
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
			this->min.push_back(item[0]);
			this->max.push_back(item[1]);
			this->value.push_back(item[2] * multiple);
		}
	}
};

class ChartControl2 : public Control2
{
	void v(void) override {}  // required

public:
	float y1, y2, gridY;
	bool readonly;

	ChartControl2(QString node, QString name, float y1, float y2, float gridY, bool readonly, QString description)
	{
		this->node = node;
		this->name = name;
		this->y1 = y1;
		this->y2 = y2;
		this->gridY = gridY;
		this->readonly = readonly;
		this->description = description;
	}
};

#endif // CONTROLS2_H
