#ifndef CONTROLS_DEFINITIONS_H
#define CONTROLS_DEFINITIONS_H

#include <cmath>
#include <QVector>
#include <QString>
#include <QMap>
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

class ComboBoxControl : public Control
{
public:
	QMap<int, QString> *map;

	ComboBoxControl();
	ComboBoxControl(QString getCmd, QString setCmd, QString parameter, QString name, QMap<int, QString> *map, QString description, bool readonly = false)
	{
		this->getCmd = getCmd;
		this->setCmd = setCmd;
		this->parameter = parameter;
		this->name = name;
		this->description = description;
		this->map = map;
		this->readonly = readonly;
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
			QMap<int, QString> *isoMap = new QMap<int, QString>;
			isoMap->insert({{100, "100"}, {200, "200"}, {400, "400"}, {800, "800"}, {1600, "1600"}});
		controls.append(new ComboBoxControl(IF_AE_G_ISO,    IF_AE_S_ISO, AE_SENSITIVITY_PARAMS, "Sensitivity (ISO) of Auto Exposure control", isoMap, "Can be changed when AE is disabled"));
		controls.append(new LabelControl( IF_AE_G_ISO,    AE_SENSITIVITY_MIN_PARAMS,          "Minimum Sensitivity", "", &typeid(int)));
		controls.append(new LabelControl( IF_AE_G_ISO,    AE_SENSITIVITY_MAX_PARAMS,          "Maximum Sensitivity", "", &typeid(int)));

		controls.append(new GroupControl("AE / ECM - Exposure Control Module"));
			QMap<int, QString> *expMap = new QMap<int, QString>;
			expMap->insert({{0, "0: Flicker Period off"}, {1, "1: 100 Hz"}, {2, "2: 120 Hz"}});
		controls.append(new ComboBoxControl(  IF_AE_G_ECM, IF_AE_S_ECM, AE_FLICKER_PERIOD_PARAMS, "The flag of Auto Exposure flicker period", expMap, ""));
		controls.append(new CheckBoxControl(IF_AE_G_ECM, IF_AE_S_ECM, AE_AFPS_PARAMS,           "Auto FPS control value ",                  false,   ""));

		controls.append(new GroupControl("AF - Auto Focus (not implemented)"));

		controls.append(new GroupControl("AWB - Auto White Balance"));
		controls.append(new CheckBoxControl(IF_AWB_G_EN, IF_AWB_S_EN, AWB_ENABLE_PARAMS,        "Enabled",                    true,          "The state of the AWB control"));
		controls.append(new SliderControl(IF_AWB_G_CFG, IF_AWB_S_CFG, AWB_MODE_PARAMS, "AWB mode", 1, 2, 1, "AWB mode; auto mode automatically calculates the appropriate illumination profile.\n1: Manual\n2: Auto"));
		controls.append(new SliderControl(IF_AWB_G_CFG, IF_AWB_S_CFG, AWB_INDEX_PARAMS, "Index of illumination profile", 0, 4, 0, "The index of the illumination profile; it will affect the AWB adjustment effect:\n0: A\n1: D50\n2: D65\n3: F2 (CWF)\n4: F11 (TL84)"));
		controls.append(new CheckBoxControl(IF_AWB_G_CFG, IF_AWB_S_CFG, AWB_DAMPING_PARAMS, "Damping", false, "Changes white balance smoothly through temporal damping"));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_LEFT,  "Measuring window left start position", 0, 3840, 0, ""));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_TOP,  "Measuring window top start position", 0, 2160, 0, ""));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_WIDTH,  "Measuring window width", 10, 3840, 1920, ""));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_HEIGHT,  "Measuring window height", 10, 2160, 1080, ""));
		controls.append(new ButtonControl(NULL,           IF_AWB_RESET, NULL,                 "Resets the Auto White Balance control",          ""));

		controls.append(new GroupControl("BLS - Black Level Subtraction"));
		controls.append(new SliderControl(IF_BLS_G_CFG, IF_BLS_S_CFG, BLS_RED_PARAMS,  "The red data information", 0, 255, 0, ""));
		controls.append(new SliderControl(IF_BLS_G_CFG, IF_BLS_S_CFG, BLS_GREEN_R_PARAMS,  "The Gr data information", 0, 255, 0, ""));
		controls.append(new SliderControl(IF_BLS_G_CFG, IF_BLS_S_CFG, BLS_GREEN_B_PARAMS,  "The Gb data information", 0, 255, 0, ""));
		controls.append(new SliderControl(IF_BLS_G_CFG, IF_BLS_S_CFG, BLS_BLUE_PARAMS,  "The blue data information", 0, 255, 0, ""));

		controls.append(new GroupControl("CAC - Chromatic Aberration Correction"));
		controls.append(new CheckBoxControl(IF_CAC_G_EN, IF_CAC_S_EN, CAC_ENABLE_PARAMS,        "Enabled",                    true,          "The state of the Chromatic Aberration Correction"));

		controls.append(new GroupControl("CNR - Chroma Noise Reduction"));
		controls.append(new CheckBoxControl(IF_CNR_G_EN, IF_CNR_S_EN, CNR_ENABLE_PARAMS,        "Enabled",                    true,          "The state of the Chroma Noise Reduction control"));
		controls.append(new SliderControl(IF_CNR_G_CFG, IF_CNR_S_CFG, CNR_TC1_PARAMS,  "CNR threshold Cb", 0, 32767, 0, "The CNR threshold value of the Cb channel. The larger the value, the stronger the noise reduction"));
		controls.append(new SliderControl(IF_CNR_G_CFG, IF_CNR_S_CFG, CNR_TC2_PARAMS,  "CNR threshold Cr", 0, 32767, 0, "The CNR threshold value of the Cr channel. The larger the value, the stronger the noise reduction"));

		controls.append(new GroupControl("CPROC - Color Processing"));
		controls.append(new CheckBoxControl(IF_CPROC_G_EN,  IF_CPROC_S_EN,    CPROC_ENABLE_PARAMS,     "Enabled",                              true,    "The state of the CPROC"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_BRIGHTNESS_PARAMS, "Brightness",              -128,   127,    0,    "Brightness value"));
			QMap<int, QString> *chromaMap = new QMap<int, QString>;
			chromaMap->insert({{1, "1: CbCr_out clipping range [16 … 240] according to ITU-R BT.601 standard"}, {2, "2: full UV_out clipping range [0 ... 255]"}});
		controls.append(new ComboBoxControl(IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_CHROMA_OUT_PARAMS, "Chrominance", chromaMap, "CPROC chrominance pixel clipping range at output"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_CONTRAST_PARAMS,   "Contrast",                0.0f, 1.99f, 1.0f, 3, "Contrast value"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_HUE_PARAMS,        "Hue",                      -90,    89,    0,    "Hue value"));
			QMap<int, QString> *lumainMap = new QMap<int, QString>;
			lumainMap->insert({{1, "1: Y_in range [64 … 940] according to ITU-R BT.601 standard; offset of 64 is subtracted from Y_in"}, {2, "2: Y_in full range [0 ... 1023]; no offset is subtracted from Y_in"}});
		controls.append(new ComboBoxControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_LUMA_IN_PARAMS,    "Luminance input", lumainMap, "CPROC luminance input range (offset processing)"));
			QMap<int, QString> *lumaoutMap = new QMap<int, QString>;
			lumaoutMap->insert({{1, "1: Y_out clipping range [16 … 235]; offset of 16 is added to Y_out according to ITU-R BT.601 standard"}, {2, "2: Y_out clipping range [0 ... 255]; no offset is added to Y_out"}});
		controls.append(new ComboBoxControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_LUMA_OUT_PARAMS,   "Luminance output", lumaoutMap, "CPROC luminance output clipping range"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_SATURATION_PARAMS, "Saturation",              0.0f, 1.99f, 1.0f, 3, "Saturation value"));
			QMap<int, QString> *coeffMap = new QMap<int, QString>;
			coeffMap->insert({{1, "index 1"}, {2, "index 2"}});
		controls.append(new ComboBoxControl(  NULL,           IF_CPROC_S_COEFF, CPROC_INDEX_PARAMS,      "Input coefficient index",    coeffMap,    "index 1: 0.257812, 0.5,      0.101562, -0.148438, -0.289062, 0.4375, 0.4375, -0.367188, -0.070312\nindex 2: 0.296875, 0.585938, 0.117188, -0.171875, -0.328125, 0.5,    0.5,    -0.421875, -0.078125"));
	}
};

#endif // CONTROLS_DEFINITIONS_H
