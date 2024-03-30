#ifndef CONTROLS_DEFINITIONS_H
#define CONTROLS_DEFINITIONS_H

#include <QVector>
#include <QString>
#include "ioctl_cmds.h"
#include "viv_video_kevent.h"
#include "cam_device_api.hpp"

class Control
{
public:
	QString type;
	QString parameter;
	QString name;
	QString description;
	virtual void v(void) = 0;  // pure virtual, need virtual method for dynamic_cast<>()
};

class GroupControl : public Control
{
public:
	GroupControl(QString name)
	{
		this->name = name;
	}
	void v(void) override {}
};

class SliderControl : public Control
{
public:
	int min;
	int max;
	int value;

	SliderControl();
	SliderControl(QString type, QString parameter, QString name, int min, int max, int value, QString description)
	{
		this->type = type;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->min = min;
		this->max = max;
		this->value = value;
	}
	void v(void) override {}
};

class CheckBoxControl : public Control
{
public:
	bool checked;
	void v(void) override {}

	CheckBoxControl(QString type, QString parameter, QString name, bool checked, QString description)
	{
		this->type = type;
		this->parameter = parameter;
		this->name = name;
		this->checked = checked;
		this->description = description;
	}
};

class ControlsDefinitions
{
public:
	QVector<Control*> controls; //= QVector<Control&>
	void init(void)
	{
		controls.append(new GroupControl("CPROC - Color Processing"));
		controls.append(new CheckBoxControl(IF_CPROC_S_EN,    CPROC_ENABLE_PARAMS,     "Enabled",                 true,           "The state of the CPROC"));
		controls.append(new SliderControl(  IF_CPROC_S_CFG,   CPROC_BRIGHTNESS_PARAMS, "Brightness",              -127,   127, 0, "Brightness value"));
		controls.append(new SliderControl(  IF_CPROC_S_CFG,   CPROC_CHROMA_OUT_PARAMS, "Chrominance",             -127,   127, 0, "CPROC chrominance pixel clipping range at output"));
		controls.append(new SliderControl(  IF_CPROC_S_CFG,   CPROC_CONTRAST_PARAMS,   "Contrast",                0.0f, 1.99f, 0, "Contrast value"));
		controls.append(new SliderControl(  IF_CPROC_S_CFG,   CPROC_HUE_PARAMS,        "Hue",                     -127,   127, 0, "Hue value"));
		controls.append(new SliderControl(  IF_CPROC_S_CFG,   CPROC_LUMA_IN_PARAMS,    "Luminance input",         -127,   127, 0, "CPROC luminance input range (offset processing)"));
		controls.append(new SliderControl(  IF_CPROC_S_CFG,   CPROC_LUMA_OUT_PARAMS,   "Luminance output",        -127,   127, 0, "CPROC luminance output clipping range"));
		controls.append(new SliderControl(  IF_CPROC_S_CFG,   CPROC_SATURATION_PARAMS, "Saturation",              0.0f, 1.99f, 0, "Saturation value"));
		controls.append(new SliderControl(  IF_CPROC_S_COEFF, CPROC_INDEX_PARAMS,      "Input coefficient index",    1,     2, 1, "index 1: 0.257812, 0.5,      0.101562, -0.148438, -0.289062, 0.4375, 0.4375, -0.367188, -0.070312\nindex 2: 0.296875, 0.585938, 0.117188, -0.171875, -0.328125, 0.5,    0.5,    -0.421875, -0.078125"));
	}
};

#endif // CONTROLS_DEFINITIONS_H
