#include "internal_isp_afps.h"
#include <QDebug>

InternalIspAfps::InternalIspAfps()
{
	this->elapsedTimer.start();
	this->lastTime = this->elapsedTimer.elapsed();
}

void InternalIspAfps::SetMeanLuminanceMeasured(QList<QPointF> meanLuminanceMeasured)
{
	QList<uint8_t> sortedList;
	for (int i = 0; i < meanLuminanceMeasured.count(); i++)
		sortedList.push_back(meanLuminanceMeasured[i].y());
	std::sort(sortedList.begin(), sortedList.end());

		/* Calculate average mean luminance measured without few brightest items */
	const int SkipItemsCount = 7;
	int avgMeanLuminanceMeasured = 0;
	for (int i = 0; i < sortedList.count() - SkipItemsCount; i++)
		avgMeanLuminanceMeasured += sortedList[i];
	avgMeanLuminanceMeasured /= meanLuminanceMeasured.count() - SkipItemsCount;

	int diff = this->elapsedTimer.elapsed() - this->lastTime;
	this->lastTime = this->elapsedTimer.elapsed();

	qDebug() << avgMeanLuminanceMeasured << avgMeanLuminanceMeasured * 100.0 / 255.0 << "%" << diff << "ms";

	if (avgMeanLuminanceMeasured < 30)
		this->setLowerFps();
}

void InternalIspAfps::setLowerFps()
{

}

void InternalIspAfps::setHigherFps()
{

}
