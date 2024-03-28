#ifndef CONTROLS_DEFINITIONS_H
#define CONTROLS_DEFINITIONS_H

#include <QVector>
#include <QString>

class Control
{
public:
	QString name;
	QString description;
};

class SliderControl : public Control
{
public:
	int min;
	int max;
	int value;

	SliderControl();
	SliderControl(QString name, int min, int max, int value, QString description)
	{
		this->name = name;
		this->description = description;
		this->min = min;
		this->max = max;
		this->value = value;
	}
};

class CheckBoxControl : public Control
{
public:
	bool defaultChecked;
	bool checked;
};

class ControlsDefinitions
{
public:
	QVector<Control*> controls; //= QVector<Control&>
	void init(void)
	{
		controls.append(new SliderControl("Brightness", -127, 127, 0,
																			"Brightness value"));
		controls.append(new SliderControl("Chrominance", -127, 127, 0,
																			"CPROC chrominance pixel clipping range at output"));
		controls.append(new SliderControl("Contrast", -127, 127, 0,
																			"Contrast value"));
		controls.append(new SliderControl("Hue", -127, 127, 0,
																			"Hue value"));
		controls.append(new SliderControl("Luminance input", -127, 127, 0,
																			"CPROC luminance input range (offset processing)"));
		controls.append(new SliderControl("Luminance output", -127, 127, 0,
																			"CPROC luminance output clipping range"));
		controls.append(new SliderControl("Saturation", -127, 127, 0,
																			"Saturation value"));
	}
	// {
	// 	SliderControl("Brightness", "description...", -127, 127, 0),
	// 	SliderControl("Sharpness", "description...", -127, 127, 0)
	// };
};

#endif // CONTROLS_DEFINITIONS_H
