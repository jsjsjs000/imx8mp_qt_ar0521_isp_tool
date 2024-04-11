#ifndef CONTROLS2_H
#define CONTROLS2_H

#include <QString>

class Control2
{
	virtual void v(void) = 0;  // pure virtual, need virtual method for dynamic_cast<>()

public:
	QString node;
	QString name;
	QString description;
	bool readonly;
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
