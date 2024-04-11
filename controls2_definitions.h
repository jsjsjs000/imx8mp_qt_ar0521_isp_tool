#ifndef CONTROLS2DEFINITIONS_H
#define CONTROLS2DEFINITIONS_H

#include <QVector>
#include <QString>
#include <QMap>
#include "controls2.h"
#include "isp_xml.h"

class Controls2Definitions
{
	IspXml xml;
public:
	QVector<Control2*> controls;

	void readXml();
	bool getArrayNode(QString node, QList<float> &array);

	void init()
	{
		controls.append(new ChartControl2("sensor/AWB/globals/cell/SVDMeanValue", "SVD mean value", -3.0f, 3.0f, 1.0f, false, "Mean of singular value decomposition"));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/PCAMatrix", "PCAMatrix", -3.0f, 3.0f, 1.0f, false, "Transformation matrix of the PCA domain."));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/CenterLine", "CenterLine", -3.0f, 3.0f, 1.0f, false, "Position the coordinates of first-order straight lines fitted from white points during AWB calibration. See the orange-dotted line in Figure 1."));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/afRg1", "AWB afRg1", -3.0f, 3.0f, 1.0f, false, "The horizontal coordinate of the projection of the sampling point to the centerline, along the lower edge of the black box during AWB calibration."));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/afRg2", "AWB afRg1", -3.0f, 3.0f, 1.0f, false, "Horizontal coordinate of the projection of the upper edge of the black box, along the sampling point to the centerline during AWB calibration."));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/afMaxDist1", "AWB afRg1", -3.0f, 3.0f, 1.0f, false, "Distance between the horizontal coordinate of the sampling point along the lower edge of the black box and afRg1 during AWB calibration. That is, the horizontal coordinate of the sampling point along the lower edge of the black box = afRg1 – afMaxDist1 Note: During AWB calibration, the distance between the sampling point on the lower edge of the black box and the centerline equals the distance between this sampling point and its projection point."));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/afMaxDist2", "AWB afRg1", -3.0f, 3.0f, 1.0f, false, "Distance between the horizontal coordinate of the sample point on the upper edge of the black box and afRg2 during AWB calibration. That is, the horizontal coordinate of the sampling point on the edge of the black box = afRg2 + afMaxDist2"));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/afGlobalFade2", "afGlobalFade2", -3.0f, 3.0f, 1.0f, false, "Horizontal coordinate of the projection of the upper edge of the orange box, along the sampling point to the centerline during AWB calibration."));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/afGlobalGainDistance2", "afGlobalGainDistance2", -3.0f, 3.0f, 1.0f, false, "Distance between the horizontal coordinate of the sampling point on the upper edge of the orange box and afGlobalFade2 during AWB calibration. That is, the horizontal coordinate of the sampling point on the upper edge of the orange box = afGlobalFade2 + afGlobalGainDistance2. Note: During AWB calibration, the distance between the sampling point on the upper edge of the orange box and the centerline equals the distance between the sampling point and its projection point."));
		controls.append(new ChartControl2("sensor/AWB/globals/cell/afGlobalFade1", "afGlobalFade1", -3.0f, 3.0f, 1.0f, false, "The horizontal coordinate of the projection of the sampling point to the centerline, along the lower edge of the orange box during the AWB calibration."));
		// controls.append(new ChartControl2("sensor/AWB/globals/cell/", "", -3.0f, 3.0f, 1.0f, false, ""));
	}
};

#endif // CONTROLS2DEFINITIONS_H
