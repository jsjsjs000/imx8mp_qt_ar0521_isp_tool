#ifndef CONTROLS_DEFINITIONS_H
#define CONTROLS_DEFINITIONS_H

#include <QVector>
#include <QString>
#include <QMap>
#include "cam_device_module_ids.h"
#include "ioctl_cmds.h"
#include "controls.h"

class ControlsDefinitions
{
public:
	QVector<Control*> controls;
	QList<QString> readParams;

	void init(void)
	{
		controls.append(new GroupControl("AE - Auto Exposure"));
		controls.append(new CheckBoxControl(IF_AE_G_EN, IF_AE_S_EN, AE_ENABLE_PARAMS,        "Enabled",                    true,          ""));
			QMap<int, QString> *aemodeMap = new QMap<int, QString>;
			aemodeMap->insert({{1, "Disabled evaluation"}, {2, "Fix evaluation"}, {3, "Adaptive evaluation"}});
		controls.append(new ComboBoxControl(IF_AE_G_CFG,    IF_AE_S_CFG, AE_MODE_PARAMS,          "Configuration mode", aemodeMap,             "Configuration mode"));
		controls.append(new SliderControl(  IF_AE_G_CFG,    IF_AE_S_CFG, AE_DAMP_OVER_PARAMS,     "Damping upper limit",  0.0f, 1.0f, 0.0f, 3, "Damping upper limit for luminance over set point. The larger the value, the smoother the convergence"));
		controls.append(new SliderControl(  IF_AE_G_CFG,    IF_AE_S_CFG, AE_DAMP_UNDER_PARAMS,    "Damping lower limit",  0.0f, 1.0f, 0.0f, 3, "Damping lower limit for luminance under set point. The larger the value, the smoother the convergence"));
		controls.append(new SliderControl(  IF_AE_G_CFG,    IF_AE_S_CFG, AE_SET_POINT_PARAMS,     "Set point",               0,  255,    0,    "Target luminance point"));
		controls.append(new SliderControl(  IF_AE_G_CFG,    IF_AE_S_CFG, AE_CLM_TOLERANCE_PARAMS, "Calculation accuracy",    0,  100,    0,    "Calculation accuracy; AE will make adjustments when the difference ratio between set.point and actual point over the clm.tolerance"));
		controls.append(new LabelControl(   IF_AE_G_CFG,                 AE_WEIGHT_PARAMS,        "Weights of 5x5 blocks",                     "", &typeid(int[])));
		controls.append(new ButtonControl(  NULL,           IF_AE_RESET, NULL, "",                "Resets the Auto Exposure control",          ""));
		controls.append(new ChartControl(   IF_AE_G_STATUS, NULL,        AE_HIST_PARAMS_BASE64,   "Current histogram of image",                "", 0, 30000, 5000, true));
		controls.append(new ChartControl(   IF_AE_G_STATUS, NULL,        AE_LUMA_PARAMS_BASE64,   "Mean luminance measured",                   "", 0,   255,   50, true));
		controls.append(new LabelControl(   IF_AE_G_STATUS, AE_OBJECT_REGION_PARAMS_BASE64,       "Measurement windows block",                 "", &typeid(int[])));
			QMap<int, QString> *isoMap = new QMap<int, QString>;
			isoMap->insert({{100, "100"}, {200, "200"}, {400, "400"}, {800, "800"}, {1600, "1600"}});
		controls.append(new ComboBoxControl(IF_AE_G_ISO,    IF_AE_S_ISO, AE_SENSITIVITY_PARAMS, "Sensitivity (ISO) of Auto Exposure control", isoMap, "Can be changed when AE is disabled"));
		controls.append(new LabelControl(   IF_AE_G_ISO,    AE_SENSITIVITY_MIN_PARAMS,          "Minimum Sensitivity", "", &typeid(int)));
		controls.append(new LabelControl(   IF_AE_G_ISO,    AE_SENSITIVITY_MAX_PARAMS,          "Maximum Sensitivity", "", &typeid(int)));

		controls.append(new GroupControl("AE / ECM - Exposure Control Module"));
			QMap<int, QString> *expMap = new QMap<int, QString>;
			expMap->insert({{0, "Flicker Period off"}, {1, "100 Hz"}, {2, "120 Hz"}});
		controls.append(new ComboBoxControl(IF_AE_G_ECM, IF_AE_S_ECM, AE_FLICKER_PERIOD_PARAMS, "The flag of Auto Exposure flicker period", expMap, ""));
		controls.append(new CheckBoxControl(IF_AE_G_ECM, IF_AE_S_ECM, AE_AFPS_PARAMS,           "Auto FPS control value", false,             "Set manual FPS to disable it (Sensor > FPS)"));

		controls.append(new GroupControl("AF - Auto Focus (configuration not implemented)"));
		controls.append(new CheckBoxControl(IF_AF_G_EN,   IF_AF_S_EN,      AF_ENABLE_PARAMS,    "Enabled",                true,              ""));
		controls.append(new CheckBoxControl(IF_AF_G_AVI,  NULL,            AF_AVAILABEL_PARAMS, "Available",              true,              ""));

		controls.append(new GroupControl("AWB - Auto White Balance"));
		controls.append(new CheckBoxControl(IF_AWB_G_EN,  IF_AWB_S_EN,      AWB_ENABLE_PARAMS,  "Enabled",                true,              "The state of the AWB control"));
			QMap<int, QString> *awbmodeMap = new QMap<int, QString>;
			awbmodeMap->insert({{1, "Manual"}, {2, "Auto"}});
		controls.append(new ComboBoxControl(IF_AWB_G_CFG, IF_AWB_S_CFG,     AWB_MODE_PARAMS,    "AWB mode", awbmodeMap, "Auto mode automatically calculates the appropriate illumination profile."));
			QMap<int, QString> *awbprofileMap = new QMap<int, QString>;
			awbprofileMap->insert({{0, "A"}, {1, "D50"}, {2, "D65"}, {3, "F2 (CWF)"}, {4, "F11 (TL84)"}});
		controls.append(new ComboBoxControl(IF_AWB_G_CFG, IF_AWB_S_CFG,     AWB_INDEX_PARAMS,   "Index of illumination profile", awbprofileMap, "The index of the illumination profile; it will affect the AWB adjustment effect"));
		controls.append(new CheckBoxControl(IF_AWB_G_CFG, IF_AWB_S_CFG,     AWB_DAMPING_PARAMS, "Damping", false, "Changes white balance smoothly through temporal damping"));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_LEFT,          "Measuring window left start position",  0, 3840,    0, "Write only"));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_TOP,           "Measuring window top start position",   0, 2160,    0, "Write only"));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_WIDTH,         "Measuring window width",               10, 3840, 1920, "Write only"));
		controls.append(new SliderControl(  NULL,         IF_AWB_S_MEASWIN, RECT_HEIGHT,        "Measuring window height",              10, 2160, 1080, "Write only"));
		controls.append(new ButtonControl(  NULL,         IF_AWB_RESET,     NULL, "",           "Resets the Auto White Balance control",                ""));

		controls.append(new GroupControl("BLS - Black Level Subtraction"));
		controls.append(new SliderControl(  IF_BLS_G_CFG, IF_BLS_S_CFG,     BLS_RED_PARAMS,     "The red data information",  0, 255, 0, ""));
		controls.append(new SliderControl(  IF_BLS_G_CFG, IF_BLS_S_CFG,     BLS_GREEN_R_PARAMS, "The Gr data information",   0, 255, 0, ""));
		controls.append(new SliderControl(  IF_BLS_G_CFG, IF_BLS_S_CFG,     BLS_GREEN_B_PARAMS, "The Gb data information",   0, 255, 0, ""));
		controls.append(new SliderControl(  IF_BLS_G_CFG, IF_BLS_S_CFG,     BLS_BLUE_PARAMS,    "The blue data information", 0, 255, 0, ""));

		controls.append(new GroupControl("CAC - Chromatic Aberration Correction"));
		controls.append(new CheckBoxControl(IF_CAC_G_EN,  IF_CAC_S_EN,  CAC_ENABLE_PARAMS,      "Enabled",                    true,          "The state of the Chromatic Aberration Correction"));

		controls.append(new GroupControl("CNR - Chroma Noise Reduction"));
		controls.append(new CheckBoxControl(IF_CNR_G_EN,  IF_CNR_S_EN,  CNR_ENABLE_PARAMS,      "Enabled",                    true,          "The state of the Chroma Noise Reduction control"));
		controls.append(new SliderControl(  IF_CNR_G_CFG, IF_CNR_S_CFG, CNR_TC1_PARAMS,  "CNR threshold Cb", 0, 32767, 0, "The CNR threshold value of the Cb channel. The larger the value, the stronger the noise reduction"));
		controls.append(new SliderControl(  IF_CNR_G_CFG, IF_CNR_S_CFG, CNR_TC2_PARAMS,  "CNR threshold Cr", 0, 32767, 0, "The CNR threshold value of the Cr channel. The larger the value, the stronger the noise reduction"));

		controls.append(new GroupControl("CPROC - Color Processing"));
		controls.append(new CheckBoxControl(IF_CPROC_G_EN,  IF_CPROC_S_EN,    CPROC_ENABLE_PARAMS,     "Enabled",                              true,    "The state of the CPROC"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_BRIGHTNESS_PARAMS, "Brightness",              -128,   127,    0,    "Brightness value"));
			QMap<int, QString> *chromaMap = new QMap<int, QString>;
			chromaMap->insert({{1, "CbCr_out clipping range [16 … 240] according to ITU-R BT.601 standard"}, {2, "full UV_out clipping range [0 ... 255]"}});
		controls.append(new ComboBoxControl(IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_CHROMA_OUT_PARAMS, "Chrominance", chromaMap,                        "CPROC chrominance pixel clipping range at output"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_CONTRAST_PARAMS,   "Contrast",                0.0f, 1.99f, 1.0f, 3, "Contrast value"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_HUE_PARAMS,        "Hue",                      -90,    89,    0,    "Hue value"));
			QMap<int, QString> *lumainMap = new QMap<int, QString>;
			lumainMap->insert({{1, "Y_in range [64 … 940] according to ITU-R BT.601 standard; offset of 64 is subtracted from Y_in"}, {2, "Y_in full range [0 ... 1023]; no offset is subtracted from Y_in"}});
		controls.append(new ComboBoxControl(IF_CPROC_G_CFG, IF_CPROC_S_CFG, CPROC_LUMA_IN_PARAMS,    "Luminance input", lumainMap, "CPROC luminance input range (offset processing)"));
			QMap<int, QString> *lumaoutMap = new QMap<int, QString>;
			lumaoutMap->insert({{1, "Y_out clipping range [16 … 235]; offset of 16 is added to Y_out according to ITU-R BT.601 standard"}, {2, "Y_out clipping range [0 ... 255]; no offset is added to Y_out"}});
		controls.append(new ComboBoxControl(IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_LUMA_OUT_PARAMS,   "Luminance output", lumaoutMap, "CPROC luminance output clipping range"));
		controls.append(new SliderControl(  IF_CPROC_G_CFG, IF_CPROC_S_CFG,   CPROC_SATURATION_PARAMS, "Saturation",              0.0f, 1.99f, 1.0f, 3, "Saturation value"));
			QMap<int, QString> *coeffMap = new QMap<int, QString>;
			coeffMap->insert({{0, "index 1"}, {1, "index 2"}});
		controls.append(new ComboBoxControl(NULL,           IF_CPROC_S_COEFF, CPROC_INDEX_PARAMS,      "Input coefficient index",    coeffMap,    "index 1: 0.257812, 0.5,      0.101562, -0.148438, -0.289062, 0.4375, 0.4375, -0.367188, -0.070312\nindex 2: 0.296875, 0.585938, 0.117188, -0.171875, -0.328125, 0.5,    0.5,    -0.421875, -0.078125"));

		controls.append(new GroupControl("DEMOSAIC"));
		controls.append(new CheckBoxControl(IF_DEMOSAIC_G_EN,  IF_DEMOSAIC_S_EN,  DEMOSAIC_ENABLE_PARAMS,    "Enabled",                              true,    "The state of the Demosaic control"));
			QMap<int, QString> *demosaicModeMap = new QMap<int, QString>;
			demosaicModeMap->insert({{1, "Normal"}, {2, "Bypass"}});
		controls.append(new ComboBoxControl(IF_DEMOSAIC_G_CFG, IF_DEMOSAIC_S_CFG, DEMOSAIC_MODE_PARAMS,      "Demosaic mode", demosaicModeMap, ""));
		controls.append(new SliderControl(  IF_DEMOSAIC_G_CFG, IF_DEMOSAIC_S_CFG, DEMOSAIC_THRESHOLD_PARAMS, "Demosaic threshold", 0, 255, 0, "0: Maximum edge sensitivity\n255: No texture detection"));

		controls.append(new GroupControl("DPCC -  Defect Pixel Cluster Correction"));
		controls.append(new CheckBoxControl(IF_DPCC_G_EN, IF_DPCC_S_EN, DPF_ENABLE_PARAMS,         "Enabled",                              true,    "The state of the Defect Pixel Cluster Correction"));

		controls.append(new GroupControl("DPF - De-noising Pre-Filter"));
		controls.append(new CheckBoxControl(IF_DPF_G_EN,  IF_DPF_S_EN,  DPF_ENABLE_PARAMS,         "Enabled",                              true,    "The state of the De-noising Pre-Filter"));
		controls.append(new SliderControl(  IF_DPF_G_CFG, IF_DPF_S_CFG, DPF_GRADIENT_PARAMS,       "Gradient value for dynamic strength calculation",        0, 128, 0, ""));
		controls.append(new SliderControl(  IF_DPF_G_CFG, IF_DPF_S_CFG, DPF_OFFSET_PARAMS,         "Offset value for dynamic strength calculation",       -128, 127, 0, ""));
		controls.append(new SliderControl(  IF_DPF_G_CFG, IF_DPF_S_CFG, DPF_MIN_PARAMS,            "Upper bound for dynamic strength calculation",           0, 128, 0, ""));
		controls.append(new SliderControl(  IF_DPF_G_CFG, IF_DPF_S_CFG, DPF_DIV_PARAMS,            "Division factor for dynamic strength calculation",       0,  64, 0, ""));
		controls.append(new SliderControl(  IF_DPF_G_CFG, IF_DPF_S_CFG, DPF_SIGMA_GREEN_PARAMS,    "The spatial filter’s sigma of the green channel",        1, 255, 0, ""));
		controls.append(new SliderControl(  IF_DPF_G_CFG, IF_DPF_S_CFG, DPF_SIGMA_RED_BLUE_PARAMS, "The spatial filter’s sigma of the Red/Blue channel",     1, 255, 0, ""));

		controls.append(new GroupControl("EC - Exposure Control"));
		controls.append(new SliderControl(  IF_EC_G_CFG, IF_EC_S_CFG, EC_GAIN_PARAMS,            "Exposure gain",         1.0f, 110.0f, 1.0f, 3, "Diasable AE (Auto Exposure) first"));
		controls.append(new LabelControl(   IF_EC_G_CFG,              EC_GAIN_MIN_PARAMS,        "Minimum gain",                                 "", &typeid(float)));
		controls.append(new LabelControl(   IF_EC_G_CFG,              EC_GAIN_MAX_PARAMS,        "Maximum gain",                                 "", &typeid(float)));
		controls.append(new SliderControl(  IF_EC_G_CFG, IF_EC_S_CFG, EC_TIME_PARAMS,            "Exposure time",         0.0f,  0.15f, 0.0f, 5, "Diasable AE (Auto Exposure) first"));
		controls.append(new LabelControl(   IF_EC_G_CFG,              EC_INTEGRATION_MIN_PARAMS, "Minimum exposure time",                        "", &typeid(float)));
		controls.append(new LabelControl(   IF_EC_G_CFG,              EC_INTEGRATION_MAX_PARAMS, "Maximum exposure time",                        "", &typeid(float)));

//	controls.append(new LabelControl(  IF_EC_G_STATUS,            EC_GAIN_PARAMS,            "Exposure gain range",                          "", &typeid(int)));  // duplicate data as above
//	controls.append(new LabelControl(  IF_EC_G_STATUS,            EC_GAIN_MIN_PARAMS,        "Sub-node parameter, Minimum gain",             "", &typeid(int)));  // gain.min
//	controls.append(new LabelControl(  IF_EC_G_STATUS,            EC_GAIN_MAX_PARAMS,        "Sub-node parameter, Maximum gain",             "", &typeid(int)));
//	controls.append(new LabelControl(  IF_EC_G_STATUS,            EC_TIME_PARAMS,            "Exposure time range",                          "", &typeid(float)));
//	controls.append(new LabelControl(  IF_EC_G_STATUS,            EC_INTEGRATION_MIN_PARAMS, "Sub-node parameter, Minimum integration time", "", &typeid(float)));
//	controls.append(new LabelControl(  IF_EC_G_STATUS,            EC_INTEGRATION_MAX_PARAMS, "Sub-node parameter, Maximum integration time", "", &typeid(float)));

		controls.append(new GroupControl("FILTER"));
		controls.append(new CheckBoxControl(IF_FILTER_G_EN,  IF_FILTER_S_EN,   FILTER_ENABLE_PARAMS,   "Enabled",                        true,     "The state of the Filter control"));
		controls.append(new CheckBoxControl(IF_FILTER_G_CFG, IF_FILTER_S_CFG,  FILTER_AUTO_PARAMS,     "Auto control",                   true,     "Auto control; automatically adjusts denoise and sharpen values."));
		controls.append(new SliderControl(  IF_FILTER_G_CFG, IF_FILTER_S_CFG,  FILTER_DENOISE_PARAMS,  "Denoise level",                  0, 10, 0, ""));
		controls.append(new SliderControl(  IF_FILTER_G_CFG, IF_FILTER_S_CFG,  FILTER_SHARPEN_PARAMS,  "Sharpen level",                  0, 10, 0, ""));
		controls.append(new SliderControl(  IF_FILTER_G_CFG, IF_FILTER_S_CFG,  FILTER_CHRHMODE_PARAMS, "Chroma filter horizontal mode",  0,  3, 0, ""));
		controls.append(new SliderControl(  IF_FILTER_G_CFG, IF_FILTER_S_CFG,  FILTER_CHRVMODE_PARAMS, "Chroma filter vertical mode",    0,  3, 0, ""));
//	controls.append(new LabelControl(   IF_FILTER_G_TBL,                   FILTER_TABLE_PARAMS,    "Filter auto table",                        "", &typeid(std::string)));  // empty response
//	controls.append(new LabelControl(   IF_FILTER_G_TBL, IF_FILTER_S_TBL,  "columns",              "Table's column property",                  "", &typeid(std::string)));  // empty response
//	controls.append(new LabelControl(   IF_FILTER_G_TBL, IF_FILTER_S_TBL,  "rows",                 "Values of properties in the table",        "", &typeid(std::string)));  // empty response
		controls.append(new LabelControl(   IF_FILTER_G_STATUS,                FILTER_GAIN_PARAMS,             "Sensor gain",                      "", &typeid(float)));
		controls.append(new LabelControl(   IF_FILTER_G_STATUS,                FILTER_INTEGRATION_TIME_PARAMS, "Sensor integration time",          "", &typeid(float)));

		controls.append(new GroupControl("GC - Gamma control"));
		controls.append(new CheckBoxControl(IF_GC_G_EN,  IF_GC_S_EN,       GC_ENABLE_PARAMS,        "Enabled",                   true,     "The state of the Gamma Control"));
		// IF_GC_G_CURVE - the same as IF_GC_G_CFG
		controls.append(new ChartControl(   IF_GC_G_CFG, IF_GC_S_CFG,      GC_CURVE_PARAMS,         "Gamma curve",               "Gamma curve; data array length is 17", 0, 1024, 100, false));
			QMap<int, QString> *gcmodeMap = new QMap<int, QString>;
			gcmodeMap->insert({{1, "Logarithmic mode"}, {2, "Equidistant mode"}});
		controls.append(new ComboBoxControl(IF_GC_G_CFG, IF_GC_S_CFG,      GC_MODE_PARAMS,          "Gamma segmentation mode", gcmodeMap, "Selects the gamma segmentation mode. Logarithmic: logarithmic segmentation from 0 to 4095, (64,64,64,64,128,128,128,128,256,256,256,512,512,512,512,512)\nEquidistant: equidistant segmentation from 0 to 4095, (256, 256, ... ); all 16 segments are 256."));

		controls.append(new GroupControl("HDR - High Dynamic Range"));
		controls.append(new CheckBoxControl(IF_HDR_G_EN,  IF_HDR_S_EN,   HDR_ENABLE_PARAMS,         "Enabled",         true,     "The state of High Dynamic Range"));
		controls.append(new SliderControl(  IF_HDR_G_CFG, IF_HDR_S_CFG,  HDR_EXTENSION_BIT_PARAMS,  "Extension bit",   0,  4, 0, ""));
		controls.append(new SliderControl(  IF_HDR_G_CFG, IF_HDR_S_CFG,  HDR_EXPOSURE_RATIO_PARAMS, "Exposure ration", 0, 16, 0, ""));

		controls.append(new GroupControl("LSC - Lens Shade Correction"));
		controls.append(new CheckBoxControl(IF_LSC_G_EN,  IF_LSC_S_EN,   LSC_ENABLE_PARAMS,  "Enabled",                true,      "The state of Lens Shade Correction"));
//	controls.append(new ChartControl(   IF_LSC_G_STATUS, NULL,       LSC_RED_PARAMS,     "LSC gains red",                     "",  0, 3999, 500, true));  // too much useless data
//	controls.append(new ChartControl(   IF_LSC_G_STATUS, NULL,       LSC_GREEN_R_PARAMS, "LSC gains green.r",                 "",  0, 3999, 500, true));
//	controls.append(new ChartControl(   IF_LSC_G_STATUS, NULL,       LSC_GREEN_B_PARAMS, "LSC gains green.b",                 "",  0, 3999, 500, true));
//	controls.append(new ChartControl(   IF_LSC_G_STATUS, NULL,       LSC_BLUE_PARAMS,    "LSC gains blue",                    "",  0, 3999, 500, true));
//	controls.append(new ChartControl(   IF_LSC_G_STATUS, NULL,       LSC_XSIZE_PARAMS,   "Horizontal orientation block size", "", 10, 1024, 100, true));
//	controls.append(new ChartControl(   IF_LSC_G_STATUS, NULL,       LSC_YSIZE_PARAMS,   "Vertical orientation block size",   "",  8, 1024, 100, true));

		controls.append(new GroupControl("WDR - Wide Dynamic Range"));
		controls.append(new CheckBoxControl(IF_WDR_G_EN,  IF_WDR_S_EN,  WDR_ENABLE_PARAMS,          "Enables or disables WDR", false,              "The state of WDR"));
//	controls.append(new LabelControl(   IF_WDR_G_CFG,               WDR_Y_M_PARAMS,             "WDR1 curve Ym value",                         "", &typeid(float[])));  // empty response
//	controls.append(new LabelControl(   IF_WDR_G_CFG,               WDR_D_Y_PARAMS,             "WDR1 curve dY value",                         "", &typeid(float[])));  // empty response
//	controls.append(new CheckBoxControl(IF_WDR_G_CFG, IF_WDR_S_CFG, WDR_AUTO_PARAMS,            "WDR3 running mode", false,                    ""));                    // empty response
			QMap<QString, QString> *wdrAutoMap = new QMap<QString, QString>;
			wdrAutoMap->insert({{"true", "true"}, {"false", "false"}});
		controls.append(new ComboBoxControl2(IF_WDR_G_CFG, IF_WDR_S_CFG, WDR_AUTO_PARAMS,            "WDR3 running mode", wdrAutoMap, ""));                                 // $$ dokończyć ComboBoxControl2
		controls.append(new SliderControl(   IF_WDR_G_CFG, IF_WDR_S_CFG, WDR_AUTO_LEVEL_PARAMS,      "WDR3 auto level",                             0, 100, 0, ""));
		controls.append(new SliderControl(   IF_WDR_G_CFG, IF_WDR_S_CFG, WDR_STRENGTH_PARAMS,        "WDR2 or WDR3 strench",                        0, 128, 0, ""));
		controls.append(new SliderControl(   IF_WDR_G_CFG, IF_WDR_S_CFG, WDR_GAIN_MAX_PARAMS,        "WDR3 gain max",                               0, 128, 0, ""));
		controls.append(new SliderControl(   IF_WDR_G_CFG, IF_WDR_S_CFG, WDR_STRENGTH_GLOBAL_PARAMS, "WDR3 global strength, image global contrast", 0, 128, 0, ""));
		controls.append(new ButtonControl(   NULL,         IF_WDR_RESET, WDR_GENERATION_PARAMS, "2", "Reset WDR", ""));
		controls.append(new LabelControl(    IF_WDR_G_STATUS, WDR_GAIN_PARAMS,                       "WDR gain",                                    "", &typeid(float)));
		controls.append(new LabelControl(    IF_WDR_G_STATUS, WDR_INTEGRATION_TIME_PARAMS,           "WDR integration time",                        "", &typeid(float)));
//	controls.append(new LabelControl(    IF_WDR_G_TBL, WDR_TABLE_PARAMS, "WDR table", "", &typeid(std::string[])));      // empty response

		controls.append(new GroupControl("WB - White Balance"));
		controls.append(new LabelControl(   IF_WB_G_CFG, WB_MATRIX_PARAMS,                "Color correction Matrinx (X-Talk)", "", &typeid(float[])));    // $$ edit - koniec sprawdzenia
		controls.append(new LabelControl(   IF_WB_G_CFG, WB_OFFSET_PARAMS,                "[red, green, blue] offset ",        "", &typeid(int[])));      // $$ edit
		controls.append(new SliderControl(  IF_WB_G_CFG, IF_WB_S_CFG, WB_RED_PARAMS,      "WB gains red",             0.0f, 3.999f, 0.0f, 3, "Disable AWB (Auto White Balance) first"));
		controls.append(new SliderControl(  IF_WB_G_CFG, IF_WB_S_CFG, WB_GREEN_R_PARAMS,  "WB gains green.r",         0.0f, 3.999f, 0.0f, 3, "Disable AWB (Auto White Balance) first"));
		controls.append(new SliderControl(  IF_WB_G_CFG, IF_WB_S_CFG, WB_GREEN_B_PARAMS,  "WB gains green.b",         0.0f, 3.999f, 0.0f, 3, "Disable AWB (Auto White Balance) first"));
		controls.append(new SliderControl(  IF_WB_G_CFG, IF_WB_S_CFG, WB_BLUE_PARAMS,     "WB gains blue",            0.0f, 3.999f, 0.0f, 3, "Disable AWB (Auto White Balance) first"));
//	controls.append(new SliderControl(  NULL, IF_WB_S_GAIN, WB_RED_PARAMS,      "WB gains red",             0.0f, 3.999f, 0.0f, 3, ""));      // not works or duplicated $$
//	controls.append(new SliderControl(  NULL, IF_WB_S_GAIN, WB_GREEN_R_PARAMS,  "WB gains green.r",         0.0f, 3.999f, 0.0f, 3, ""));
//	controls.append(new SliderControl(  NULL, IF_WB_S_GAIN, WB_GREEN_B_PARAMS,  "WB gains green.b",         0.0f, 3.999f, 0.0f, 3, ""));
//	controls.append(new SliderControl(  NULL, IF_WB_S_GAIN, WB_BLUE_PARAMS,     "WB gains blue",            0.0f, 3.999f, 0.0f, 3, ""));
//	controls.append(new SliderControl(  NULL, IF_WB_S_GAIN, WB_BLUE_PARAMS,     "[red, green, blue] offset",  -2048, 2047, ""));              // $$ edit array/matrix
		// "Color correction Matrix (X-Talk)" IF_WB_S_CCM     $$ edit array/matrix

		controls.append(new GroupControl("DWE - Dewarp"));
//	controls.append(new LabelControl(                    IF_DWE_G_PARAMS, "dwe",   "Get the dewarp node", "", &typeid(std::string[])));       // $$ edit subnode
//	controls.append(new CheckBoxControl(IF_DWE_G_PARAMS, IF_DWE_S_PARAMS, "hflip", "", false,             ""));
		// $$ rest submodes: hflip, vflip, mode, bypass, mat...
		// jRequest["dwe"]["mode"] = dweParams.mode;
		// dweParams.vflip = jResponse["dwe"]["vflip"].asBool();

		controls.append(new GroupControl("Crops the image"));
		// $$ subnodes - nudbox

		controls.append(new GroupControl("Scales the image"));
		// $$ subnodes - nudbox

		controls.append(new GroupControl("Sensor"));
		// $$ subnodes - IF_SENSOR_QUERY

		// $$ IF_SENSOR_G_MODE, IF_SENSOR_S_MODE - sensor get/set mode index

		controls.append(new LabelControl(                    IF_SENSOR_G_RESW,      "resw", "Sensor width",            "", &typeid(int)));
		controls.append(new LabelControl(                    IF_SENSOR_G_RESH,      "resh", "Sensor height",           "", &typeid(int)));
		controls.append(new SliderControl(  NULL,            IF_S_FPS,              "fps",  "FPS",           1, 50, 1, ""));
//	controls.append(new ButtonControl(  NULL,            IF_SENSOR_LIB_PRELOAD, "",     "",       "Reload sensor calibration file (hang up stream)", ""));     // not works
		controls.append(new SliderControl(  IF_SENSOR_G_SEC, IF_SENSOR_S_SEC,       "exposure",       "AE start exposure = IntegrationTime x Gain",         0.0f, 4.0f, 0.0f, 3, "Disable AE (Auto Exposure) first"));
//	controls.append(new SliderControl(  NULL,            IF_SENSOR_S_TESTPAT,   "test.pattern ",  "Sensor test pattern mode",         0, 10, 0, ""));  // not works

//	controls.append(new GroupControl("Pipline"));
//	controls.append(new ButtonControl(  NULL, IF_PIPELINE_S_WARM_UP, "enable", "true",  "Warms up pipeline", ""));    // not works
//	controls.append(new ButtonControl(  NULL, IF_PIPELINE_S_WARM_UP, "enable", "false", "Disable pipeline",  ""));    // not works
		// IF_PIPELINE_S_SMP_MODE (simple matrix processor)
		// IF_PIPELINE_S_DWE_ONOFF
		// IF_PIPELINE_S_TESTPAT
		// IF_PIPELINE_G_3A_LOCK / IF_PIPELINE_S_3A_LOCK  AF/AE/AWB lock

		readParams.append({
				IF_GC_G_CFG,
				IF_GC_G_CFG,

				// IF_AE_G_EN,
				// IF_AE_G_CFG,
				// IF_AE_G_ECM,
				// IF_AE_G_STATUS,
				// IF_AE_G_ISO,
				// IF_AWB_G_CFG,
				// IF_AWB_G_EN,
				// IF_BLS_G_CFG,
				// IF_CAC_G_EN,
				// IF_CNR_G_EN,
				// IF_CNR_G_CFG,
				// IF_CPROC_G_EN,
				// IF_CPROC_G_CFG,
				// IF_DEMOSAIC_G_EN,
				// IF_DEMOSAIC_G_CFG,
				// IF_DPCC_G_EN,
				// IF_DPF_G_EN,
				// IF_DPF_G_CFG,
				// IF_EC_G_CFG,
				// //- IF_EC_G_STATUS,   // duplicate
				// IF_FILTER_G_EN,
				// IF_FILTER_G_CFG,
				// // - IF_FILTER_G_TBL,  // empty response
				// IF_FILTER_G_STATUS,
				// IF_GC_G_CFG,
				// IF_GC_G_EN,
				// IF_HDR_G_CFG,
				// IF_HDR_G_EN,
				// IF_LSC_G_EN,
				// // - IF_LSC_G_STATUS,   // too much useless data
				// IF_WDR_G_EN,
				// IF_WDR_G_CFG,
				// IF_WDR_G_STATUS,
				// // - IF_WDR_G_TBL,     // empty response
				// IF_WB_G_CFG,
				// IF_DWE_G_PARAMS,
				// IF_SENSOR_QUERY,
				// IF_SENSOR_G_MODE,
				// IF_SENSOR_G_RESW,
				// IF_SENSOR_G_RESH,
				// IF_SENSOR_G_SEC,
		});
	}
};

#endif // CONTROLS_DEFINITIONS_H
