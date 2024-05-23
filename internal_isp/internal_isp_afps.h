#ifndef INTERNALISPAFPS_H
#define INTERNALISPAFPS_H

#include <QElapsedTimer>
#include <QList>
#include <QPointF>

class InternalIspAfps
{
public:
	InternalIspAfps();

	void SetMeanLuminanceMeasured(QList<QPointF> meanLuminance);
private:
	QElapsedTimer elapsedTimer;
	clock_t lastTime = 0;

	// allowed FPS list for AR0521 1920x1080: 7, 8, 9, 11, 12, 14, 16, 20, 25, 33, 50, 100
	const QList<uint8_t> AllowedFPS;

	void setLowerFps();
	void setHigherFps();
};

#endif // INTERNALISPAFPS_H
