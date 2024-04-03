#ifndef CONTROLS_DEFINITIONS_H
#define CONTROLS_DEFINITIONS_H

#include <cmath>
#include <QVector>
#include <QString>
#include "ioctl_cmds.h"
#include "viv_video_kevent.h"
#include "cam_device_api.hpp"

class Control
{
public:
	QString setCmd;
	QString getCmd;
	QString parameter;
	QString value;
	QString name;
	QString description;
	bool readonly;
	virtual void v(void) = 0;  // pure virtual, need virtual method for dynamic_cast<>()
};

class GroupControl : public Control
{
public:
	GroupControl(QString name)
	{
		this->name = name;
	}
	void v(void) override {}  // required
};

class SliderControl : public Control
{
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
	void v(void) override {}  // required
};

class CheckBoxControl : public Control
{
public:
	bool checked;
	void v(void) override {}  // required

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
public:
	void v(void) override {}  // required

	ButtonControl(QString getCmd, QString setCmd, QString parameter, QString name, QString description)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
	}
};

// template <typename T>

class LabelControl : public Control
{
public:
	QString label;
	const std::type_info *type;

	void v(void) override {}  // required

	LabelControl(QString getCmd, QString parameter, QString name, QString description, const std::type_info *type)
	{
		this->getCmd = getCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->type = type;
	}
};

class ControlsDefinitions
{
public:
	QVector<Control*> controls; //= QVector<Control&>
	void init(void)
	{
		controls.append(new GroupControl("AE - Auto Exposure"));
		controls.append(new CheckBoxControl(IF_AE_G_EN, IF_AE_S_EN, AE_ENABLE_PARAMS,        "Enabled",                    true,          ""));
		controls.append(new SliderControl(IF_AE_G_CFG, IF_AE_S_CFG, AE_MODE_PARAMS,          "Configuration mode",   1, 3, 1,             "Configuration mode\n1: Disabled evaluation\n2: Fix evaluation\n3: Adaptive evaluation"));
		controls.append(new SliderControl(IF_AE_G_CFG, IF_AE_S_CFG, AE_DAMP_OVER_PARAMS,     "Damping upper limit",  0.0f, 1.0f, 0.0f, 3, "Damping upper limit for luminance over set point. The larger the value, the smoother the convergence"));
		controls.append(new SliderControl(IF_AE_G_CFG, IF_AE_S_CFG, AE_DAMP_UNDER_PARAMS,    "Damping lower limit",  0.0f, 1.0f, 0.0f, 3, "Damping lower limit for luminance under set point. The larger the value, the smoother the convergence"));
		controls.append(new SliderControl(IF_AE_G_CFG, IF_AE_S_CFG, AE_SET_POINT_PARAMS,     "Set point",            0, 255, 0,           "Target luminance point"));
		controls.append(new SliderControl(IF_AE_G_CFG, IF_AE_S_CFG, AE_CLM_TOLERANCE_PARAMS, "Calculation accuracy", 0, 100, 0,           "Calculation accuracy; AE will make adjustments when the difference ratio between set.point and actual point over the clm.tolerance"));
		controls.append(new LabelControl( IF_AE_G_CFG,              AE_WEIGHT_PARAMS,        "Weights of 5x5 blocks ",                    "", &typeid(int[])));
		controls.append(new ButtonControl(NULL,           IF_AE_RESET, NULL,                 "Resets the Auto Exposure control",          ""));
		controls.append(new LabelControl( IF_AE_G_STATUS, AE_HIST_PARAMS_BASE64,             "Current histogram of image", "", &typeid(int[])));
		controls.append(new LabelControl( IF_AE_G_STATUS, AE_LUMA_PARAMS_BASE64,             "Mean luminance measured",    "", &typeid(int[])));
		controls.append(new LabelControl( IF_AE_G_STATUS, AE_OBJECT_REGION_PARAMS_BASE64,    "Measurement windows block",  "", &typeid(int[])));
		controls.append(new SliderControl(IF_AE_G_ISO,    IF_AE_S_ISO, AE_SENSITIVITY_PARAMS, "Sensitivity (ISO) of Auto Exposure control", 100, 1600, 100, "Can be changed when AE is disabled"));
		controls.append(new LabelControl( IF_AE_G_ISO,    AE_SENSITIVITY_MIN_PARAMS,          "Minimum Sensitivity", "", &typeid(int)));
		controls.append(new LabelControl( IF_AE_G_ISO,    AE_SENSITIVITY_MAX_PARAMS,          "Maximum Sensitivity", "", &typeid(int)));

		controls.append(new GroupControl("AE / ECM - Exposure Control Module"));
		controls.append(new SliderControl(  IF_AE_G_ECM, IF_AE_S_ECM, AE_FLICKER_PERIOD_PARAMS, "The flag of Auto Exposure flicker period", 0, 2, 0, "0: Flicker Period off\n1: 100 Hz\n2: 120 Hz"));
		controls.append(new CheckBoxControl(IF_AE_G_ECM, IF_AE_S_ECM, AE_AFPS_PARAMS,           "Auto FPS control value ",                  false,   ""));

		controls.append(new GroupControl("AF - Auto Focus (not implemented)"));

		controls.append(new GroupControl("AWB - Auto White Balance"));
// IF_AWB_G_CFG




		controls.append(new GroupControl("CPROC - Color Processing"));
		controls.append(new CheckBoxControl(IF_CPROC_G_EN,  IF_CPROC_S_EN,    CPROC_ENABLE_PARAMS,     "Enabled",                              true,    "The state of the CPROC"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_BRIGHTNESS_PARAMS, "Brightness",              -128,   127,    0,    "Brightness value"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_CHROMA_OUT_PARAMS, "Chrominance",                1,     2,    1,    "CPROC chrominance pixel clipping range at output\n1: CbCr_out clipping range [16 … 240] according to ITU-R BT.601 standard\n2: full UV_out clipping range [0 ... 255]"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_CONTRAST_PARAMS,   "Contrast",                0.0f, 1.99f, 1.0f, 3, "Contrast value"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_HUE_PARAMS,        "Hue",                      -90,    89,    0,    "Hue value"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_LUMA_IN_PARAMS,    "Luminance input",            1,     2,    1,    "CPROC luminance input range (offset processing)\n1: Y_in range [64 … 940] according to ITU-R BT.601 standard; offset of 64 is subtracted from Y_in\n2: Y_in full range [0 ... 1023]; no offset is subtracted from Y_in"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_LUMA_OUT_PARAMS,   "Luminance output",           1,     2,    1,    "CPROC luminance output clipping range\n1: Y_out clipping range [16 … 235]; offset of 16 is added to Y_out according to ITU-R BT.601 standard\n2: Y_out clipping range [0 ... 255]; no offset is added to Y_out"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_SATURATION_PARAMS, "Saturation",              0.0f, 1.99f, 1.0f, 3, "Saturation value"));
		controls.append(new SliderControl(  NULL,           IF_CPROC_S_COEFF, CPROC_INDEX_PARAMS,      "Input coefficient index",    1,     2,    1,    "index 1: 0.257812, 0.5,      0.101562, -0.148438, -0.289062, 0.4375, 0.4375, -0.367188, -0.070312\nindex 2: 0.296875, 0.585938, 0.117188, -0.171875, -0.328125, 0.5,    0.5,    -0.421875, -0.078125"));
	}
};

#endif // CONTROLS_DEFINITIONS_H
