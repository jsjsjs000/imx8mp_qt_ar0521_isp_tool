#ifndef INTERNALISPAFPS_H
#define INTERNALISPAFPS_H

#include <QElapsedTimer>
#include <QList>
#include <QPointF>
#include <isp_control.h>

class InternalIspAfps
{
public:
	InternalIspAfps();
	void Initialize(IspControl *ispControl);
	void SetMeanLuminanceMeasured(QList<QPointF> meanLuminance);

private:
	QElapsedTimer elapsedTimer;
	clock_t lastTime = 0;
	IspControl *ispControl = nullptr;

		/* allowed FPS list for AR0521 1920x1080 */
	const QList<uint8_t> AllowedFPS = { 7, 8, 9, 11, 12, 14, 16, 20, 25, 33, 50, 100 };
	int fpsIndex = 9;  // 33 fps
	int fps = 33;

	bool isInitialized();
	void setLowerFps();
	void setHigherFps();
	void setFps(int fps);
};

#endif // INTERNALISPAFPS_H
