#ifndef CONTROLS2DEFINITIONS_H
#define CONTROLS2DEFINITIONS_H

#include <QVector>
#include <QString>
#include <QMap>
#include <QPointF>
#include "controls2.h"
#include "v4l2_isp/isp_xml.h"

class Controls2Definitions
{
	const QString XmlFileName =        "/opt/imx8-isp/bin/xml/VM-017-COL_AO062-C_1920x1080.xml";
	const QString XmlFactoryFileName = "/opt/imx8-isp/bin/xml/VM-017-COL_AO062-C_1920x1080_factory.xml";
	// const QString XmlFileName =        "/opt/imx8-isp/bin/xml/VM-017-COL_AO062-C_2592x1944.xml";
	// const QString XmlFactoryFileName = "/opt/imx8-isp/bin/xml/VM-017-COL_AO062-C_2592x1944_factory.xml";

	IspXml xml;
	IspXml xmlDefault;
	IspXml xmlFactory;

public:
	QVector<Control2*> controls;

	bool readXml();
	bool saveXml();
	bool getIntNode(QString node, int &i);
	bool getArrayNode(QString node, QList<float> &array);
	bool getFactoryArrayNode(QString node, QList<float> &array);
	bool setArrayNode(QString node, QList<float> array);
	bool setArrayNode(QString node, QList<QPointF> array);

	void init()
	{
		controls.append(new GroupControl2( "AWB globals"));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/SVDMeanValue",              "SVD mean value",            -3.0f,  3.0f, 1.0f, false, "Mean of singular value decomposition"));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/PCAMatrix",                 "PCAMatrix [3x2]",           -3.0f,  3.0f, 1.0f, false, "Transformation matrix of the PCA domain."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/CenterLine",                "CenterLine",                -3.0f,  3.0f, 1.0f, false, "Position the coordinates of first-order straight lines fitted from white points during AWB calibration. See the orange-dotted line in Figure 1."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afRg1",                     "afRg1",                     -3.0f,  3.0f, 1.0f, false, "The horizontal coordinate of the projection of the sampling point to the centerline, along the lower edge of the black box during AWB calibration."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afRg2",                     "afRg2",                     -3.0f,  3.0f, 1.0f, false, "Horizontal coordinate of the projection of the upper edge of the black box, along the sampling point to the centerline during AWB calibration."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afMaxDist1",                "afMaxDist1",                -3.0f,  3.0f, 1.0f, false, "Distance between the horizontal coordinate of the sampling point along the lower edge of the black box and afRg1 during AWB calibration. That is, the horizontal coordinate of the sampling point along the lower edge of the black box = afRg1 – afMaxDist1 Note: During AWB calibration, the distance between the sampling point on the lower edge of the black box and the centerline equals the distance between this sampling point and its projection point."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afMaxDist2",                "afMaxDist2",                -3.0f,  3.0f, 1.0f, false, "Distance between the horizontal coordinate of the sample point on the upper edge of the black box and afRg2 during AWB calibration. That is, the horizontal coordinate of the sampling point on the edge of the black box = afRg2 + afMaxDist2"));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afGlobalFade2",             "afGlobalFade2",             -3.0f,  3.0f, 1.0f, false, "Horizontal coordinate of the projection of the upper edge of the orange box, along the sampling point to the centerline during AWB calibration."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afGlobalGainDistance2",     "afGlobalGainDistance2",     -3.0f,  3.0f, 1.0f, false, "Distance between the horizontal coordinate of the sampling point on the upper edge of the orange box and afGlobalFade2 during AWB calibration. That is, the horizontal coordinate of the sampling point on the upper edge of the orange box = afGlobalFade2 + afGlobalGainDistance2. Note: During AWB calibration, the distance between the sampling point on the upper edge of the orange box and the centerline equals the distance between the sampling point and its projection point."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afGlobalFade1",             "afGlobalFade1",             -3.0f,  3.0f, 1.0f, false, "The horizontal coordinate of the projection of the sampling point to the centerline, along the lower edge of the orange box during the AWB calibration."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/fRgProjIndoorMin",          "fRgProjIndoorMin",           0.0f,  3.0f, 1.0f, 2,     "Minimum Rg value in indoor scenes when the AWB clip box is calibrated. For more details, see Figure 1."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/fRgProjMax",                "fRgProjMax",                 0.0f,  3.0f, 1.0f, 2,     "Maximum Rg boundary value of the black box when the AWB clip box is calibrated. For more details, see Figure 1."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/fRgProjMaxSky",             "fRgProjMaxSky",              0.0f,  3.0f, 1.0f, 2,     "Maximum Rg boundary value of the orange box when the AWB clip box is calibrated. For more details, see Figure 1."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/fRgProjOutdoorMin",         "fRgProjOutdoorMin",          0.0f,  3.0f, 1.0f, 2,     "Rg of indoor and outdoor dividing line when the AWB clip box is calibrated. For more details, see Figure 1."));
		// $$ combo     Awb_clip_outdoor    "The name of the light source, which is the dividing line between indoor and outdoor."
		controls.append(new SliderControl2("sensor/AWB/globals/cell/K_Factor",                  "K_Factor",                   0.0f,  6.0f, 1.0f, 2,     "The sensor sensitivity coefficient, which determines whether the\nscene is indoor or outdoor, is as follows:\nfGExp = SensorGain x InteprationTime x K_Factor\np_out = 0.9 × (– ln(fGExp) – ln(0.04)) + 0.5\n- p_out > 1: DoorType = OutDoor\n- p_out <= 0.5: DoorType = InDoor\n- 0.5 <p_out <= 1: Transition_Range"));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afFade2",                   "afFade2",                   -5.0f,  5.0f, 1.0f, false, "Six coordinates on the Rg-Bg coordinate system. These coordinates divide the five Rg regions. The scenes in different regions are selected using the white point of this region."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afCbMinRegionMax",          "afCbMinRegionMax",           0.0f,   255,   50, false, "White points are in the range specified by Cb and Cr.\nThese parameters indicate the six points of afCbMinRegionMax in\nfive ranges, corresponding to the five regions in afFade2.\nFor more details, see Figure 2.\nThe following formulas are used for calculating variables in Figure 2:\nf_CbMin = (ResgionSize x afCbMinRegionMax + (1 – RegionSize) x\nafCbMinRegionMin)\nf_CrMin = (RegionSize x afCrMinRegionMax + (1 – RegionSize) x af\nCrMinRegionMin)\nf_MaxCSum = (RegionSize x afMaxCSumRegionMax + (1 – Region\nSize) x afMaxCSumRegionMin);\nf_shift = – (f_CrMin + f_CbMin)/2\nRefCr = (f_CrMin + f_shift + 128)\nRefCb = (f_CbMin + f_shift + 128)\nMinC = RefCb – f_CbMin\nMaxCSum = f_MaxCSum"));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afCrMinRegionMax",          "afCrMinRegionMax",           0.0f,   255,   50, false, "White points are in the range specified by Cb and Cr. These parameters indicate the six points of afCrMinRegionMax in five ranges, corresponding to the five regions in afFade2. For more details, see Figure 2."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afMaxCSumRegionMax",        "afMaxCSumRegionMax",         0.0f,   255,   50, false, "White points are in the range specified by Cb and Cr. These parameters indicate the six points of afMaxCSumRegionMax in five ranges, corresponding to the five regions in afFade2. For more details, see Figure 2."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afCbMinRegionMin",          "afCbMinRegionMin",           0.0f,   255,   50, false, "White points are in the range specified by Cb and Cr. These parameters indicate the six points of afCbMinRegionMin in five ranges, corresponding to the five regions in afFade2. For more details, see Figure 2."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afCrMinRegionMin",          "afCrMinRegionMin",           0.0f,   255,   50, false, "White points are in the range specified by Cb and Cr. These parameters indicate the six points of afCrMinRegionMin in five ranges, corresponding to the five regions in afFade2. For more details, see Figure 2."));
		controls.append(new ChartControl2( "sensor/AWB/globals/cell/afMaxCSumRegionMin",        "afMaxCSumRegionMin",         0.0f, 10.0f, 1.0f, false, "White points are in the range specified by Cb and Cr. These parameters indicate the six points of afMaxCsumMin in five ranges, corresponding to the five regions in afFade2. For more details, see Figure 2."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/RegionSize",                "RegionSize",                 0.0f,  2.0f, 1.0f, 2,     "White points are in the range specified by Cb and Cr. This parameter indicates the value of RegionSize, which controls the proportional scaling of the white point area. 1.0 is the maximum size."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/RegionSizeInc",             "RegionSizeInc",              0.0f,  2.0f, 1.0f, 2,     "Increment used for adjusting the RegionSize in the driver."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/RegionSizeDec",             "RegionSizeDec",              0.0f,  3.0f, 1.0f, 2,     "Decrement used for adjusting the RegionSize in the driver."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/DampCoefAdd",           "IIR DampCoefAdd",            0.0f,  1.0f, 0.2f, 2,     "Increment of damping, which is an infinite impulse response filter (IIR) parameter. The larger the increment, the slower the convergence."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/DampCoefSub",           "IIR DampCoefSub",            0.0f,  1.0f, 0.2f, 2,     "Decrement of damping, which is an IIR parameter. The larger the decrement, the faster the convergence"));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/DampFilterThreshold",   "IIR DampFilterThreshold",    0.0f,  1.0f, 0.2f, 2,     "Threshold of indoor probability change, which is an IIR parameter"));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/DampingCoefMin",        "IIR DampingCoefMin",         0.0f,  1.0f, 0.2f, 2,     "Minimum value of damping, which is an IIR parameter. The larger the minimum value, the slower the convergence."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/DampingCoefMax",        "IIR DampingCoefMax",         0.0f,  1.0f, 0.2f, 2,     "Maximum value of damping, which is an IIR parameter. The greater the maximum value, the slower the convergence."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/DampingCoefInit",       "IIR DampingCoefInit",        0.0f,  1.0f, 0.2f, 2,     "Initial value of the ring-shaped buffer (multi-frame indoor probability before storage), which is an IIR parameter."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/ExpPriorFilterSizeMax", "IIR ExpPriorFilterSizeMax",  1.0f,    50, 1.0f, 1,     "Ring buffer size of the AWB damping queue, which is an IIR parameter."));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/ExpPriorFilterSizeMin", "IIR ExpPriorFilterSizeMin",  1.0f,    50, 1.0f, 1,     "IIR parameter- Not used"));
		controls.append(new SliderControl2("sensor/AWB/globals/cell/IIR/ExpPriorMiddle",        "IIR ExpPriorMiddle",         1.0f,    50, 1.0f, 1,     "IIR parameter- Not used"));

		controls.append(new GroupControl2( "AWB illumination D50"));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/GMM/invCovMatrix",          "Gaussian Mixture Model (GMM) - invCovMatrix",          0.0f,    4000,  500, false, "InvCovMatrix: 2 x 2 inverse covariance matrix"));
		controls.append(new SliderControl2("sensor/AWB/illumination/cell[index=2]/GMM/GaussianScalingFactor", "Gaussian Mixture Model (GMM) - GaussianScalingFactor", 0.0f,     400,    0,        "GaussianScalingFactor: 1 x 1 Gaussian scaling factor"));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/GMM/tau"             ,      "Gaussian Mixture Model (GMM) - tau",                   0.0f,       2, 0.2f, false, "tau: 1 x 2 matrix, corresponding to the interpolation strategies tau1 and tau2 adjusted in the PCA domain during AWB calibration"));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/GMM/GaussianMeanValue",     "Gaussian Mixture Model (GMM) - GaussianMeanValue",    -1.0f,    1.0f, 0.2f, false, "GaussianMeanValue: 1 x 2 matrix, Gaussian mean value"));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/manualWB",                  "manualWB",                                             0.0f,       2, 0.2f, false, "WB gain value calibrated for the light source."));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/manualccMatrix",            "manualccMatrix",                                       -3.0f,      3, 0.5f, false, "CC matrix value calibrated for the light source."));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/manualccOffsets",           "manualccOffsets",                                      -100.0f,  100,   20, false, "CC offset value calibrated for the light source. The bit width is 12 bits."));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/sat_CT/gains",              "sat_CT gains",                                         0.0f,      32,    5, false, "Saturation parameters, including:\n- Gains: a set of gains, 10 gains at most.\n- SAT: a set of saturation values corresponding to gain values, 10 values at most. Gains and the SAT arrays constitute an interpolation coordinate system, with gains on the horizontal coordinate and SAT arrays on the vertical coordinate. In the coordinate system, the driver obtains a vertical ordinate SAT value based on the horizontal coordinate range in which the actual scene gain falls. Then selects two calibrated CC data as interpolation."));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/sat_CT/sat",                "sat_CT sat",                                           0.0f,     200,   20, false, "Saturation parameters, including:\n- Gains: a set of gains, 10 gains at most.\n- SAT: a set of saturation values corresponding to gain values, 10 values at most. Gains and the SAT arrays constitute an interpolation coordinate system, with gains on the horizontal coordinate and SAT arrays on the vertical coordinate. In the coordinate system, the driver obtains a vertical ordinate SAT value based on the horizontal coordinate range in which the actual scene gain falls. Then selects two calibrated CC data as interpolation."));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/vig_CT/gains",              "vig_CT gains",                                         0.0f,      32,    5, false, "LSC compensation parameters, including:\n- Gains: a set of gains, five gains at most.\n- Vig: a set of LSC compensation proportional parameters corresponding to gain values, five parameters at most. Gains and Vig arrays constitute an interpolation coordinate system for driver calibration."));
		controls.append(new ChartControl2( "sensor/AWB/illumination/cell[index=2]/vig_CT/vig",                "vig_CT vig",                                           0.0f,     200,   20, false, "LSC compensation parameters, including:\n- Gains: a set of gains, five gains at most.\n- Vig: a set of LSC compensation proportional parameters corresponding to gain values, five parameters at most. Gains and Vig arrays constitute an interpolation coordinate system for driver calibration."));

		controls.append(new GroupControl2( "LSC - Lens Shading Correction - D50_100"));
		controls.append(new SliderControl2("sensor/LSC/cell[index=2]/LSC_sectors",        "LSC_sectors",         0.0f,   64, 0,          "Number of Lens sectors. Currently, the supported number is 32", true));
		controls.append(new SliderControl2("sensor/LSC/cell[index=2]/LSC_No",             "LSC_No",              0.0f,   64, 0,          "Corresponds to HW Repres in the Lens Shade Correction Tool, as shown in the figure below:"));
		controls.append(new SliderControl2("sensor/LSC/cell[index=2]/LSC_Xo",             "LSC_Xo",              0.0f,   64, 0,          "Corresponds to Horizontal multiplication-factors in the Lens Shade Correction Tool."));
		controls.append(new SliderControl2("sensor/LSC/cell[index=2]/LSC_Yo",             "LSC_Yo",              0.0f,   64, 0,          "Corresponds to Vertical multiplication-factors in the Lens Shade Correction Tool."));
		controls.append(new ChartControl2( "sensor/LSC/cell[index=2]/LSC_SECT_SIZE_X",    "LSC_SECT_SIZE_X",     0.0f,  255, 20, false,  "Horizontal spacing between nodes. Because of symmetry, only half of the data is recorded [1x8]. Value range: [0, horizontal_length]"));
		controls.append(new ChartControl2( "sensor/LSC/cell[index=2]/LSC_SECT_SIZE_Y",    "LSC_SECT_SIZE_Y",     0.0f,  255, 20, false,  "Vertical spacing between nodes. Because of symmetry, only half of the data is recorded [1x8]. Value range: [0, vertical_length]"));
		controls.append(new SliderControl2("sensor/LSC/cell[index=2]/vignetting",         "vignetting",          0.0f,  200, 0,          "Proportion of compensation."));
		controls.append(new ChartControl2( "sensor/LSC/cell[index=2]/LSC_SAMPLES_red",    "LSC_SAMPLES_red",     0.0f, 4096, 500, false, "Lens compensation coefficient for the red channel. The coefficient at the center is 1024. The maximum coefficient is 4096."));
		controls.append(new ChartControl2( "sensor/LSC/cell[index=2]/LSC_SAMPLES_greenR", "LSC_SAMPLES_greenR",  0.0f, 4096, 500, false, "LSC compensation coefficient of the green red channel. The coefficient at the center is 1024. The maximum coefficient is 4096."));
		controls.append(new ChartControl2( "sensor/LSC/cell[index=2]/LSC_SAMPLES_greenB", "LSC_SAMPLES_greenB",  0.0f, 4096, 500, false, "LSC compensation coefficient of the green blue channel. The coefficient at the center is 1024. The maximum coefficient is 4096."));
		controls.append(new ChartControl2( "sensor/LSC/cell[index=2]/LSC_SAMPLES_blue",   "LSC_SAMPLES_blue",    0.0f, 4096, 500, false, "LSC compensation coefficient of the blue channel. The coefficient at the center is 1024. The maximum coefficient is 4096."));

		controls.append(new GroupControl2( "CC - Color Correction - D50_100"));
		controls.append(new ChartControl2( "sensor/CC/cell[index=2]/saturation",                "saturation",                             0.0f, 200,   20, false, "Saturation value."));
		controls.append(new ChartControl2( "sensor/CC/cell[index=2]/ccMatrix",                  "ccMatrix",                              -3.0f,   3, 0.5f, false, "CC matrices of the light source."));
		controls.append(new ChartControl2( "sensor/CC/cell[index=2]/ccOffsets",                 "ccOffsets",                           -512.0f,  50,   50, false, "CC offset matrices of the light source."));
		controls.append(new ChartControl2( "sensor/CC/cell[index=2]/wb",                        "wb",                                     0.0f,   3, 0.2f, false, "Wb gains of the light source."));

		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 15 FPS - fast"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=1]/PrioritySchemes/cell[index=1]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=1]/PrioritySchemes/cell[index=1]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));
		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 15 FPS - normal"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=1]/PrioritySchemes/cell[index=2]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=1]/PrioritySchemes/cell[index=2]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));
		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 15 FPS - slow"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=1]/PrioritySchemes/cell[index=3]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=1]/PrioritySchemes/cell[index=3]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));

		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 10 FPS - fast"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=2]/PrioritySchemes/cell[index=1]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=2]/PrioritySchemes/cell[index=1]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));
		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 10 FPS - normal"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=2]/PrioritySchemes/cell[index=2]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=2]/PrioritySchemes/cell[index=2]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));
		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 10 FPS - slow"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=2]/PrioritySchemes/cell[index=3]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=2]/PrioritySchemes/cell[index=3]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));

		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 5 FPS - fast"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=3]/PrioritySchemes/cell[index=1]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=3]/PrioritySchemes/cell[index=1]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));
		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 5 FPS - normal"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=3]/PrioritySchemes/cell[index=2]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=3]/PrioritySchemes/cell[index=2]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));
		controls.append(new GroupControl2( "AEC - Auto Exposure Control - ECM 5 FPS - slow"));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=3]/PrioritySchemes/cell[index=3]/OffsetT0Fac",      "OffsetT0Fac",    0.0f,   5, 0, ""));
		controls.append(new SliderControl2("sensor/AEC/ECM/cell[index=3]/PrioritySchemes/cell[index=3]/SlopeA0",          "SlopeA0",        0.0f,   5, 0, ""));

		// $$ aFpsMaxGain

		controls.append(new GroupControl2( "Degamma"));
		controls.append(new ChartControl2( "sensor/DEGAMMA/cell/degamma_dx",     "degamma_dx",   0.0f, 4095, 500, false, "Brightness value before linearity correction.", true));
		controls.append(new ChartControl2( "sensor/DEGAMMA/cell/degamma_y",      "degamma_y",    0.0f, 4095, 500, false, "Brightness value after linearity correction.", true));
	}
};

#endif // CONTROLS2DEFINITIONS_H
