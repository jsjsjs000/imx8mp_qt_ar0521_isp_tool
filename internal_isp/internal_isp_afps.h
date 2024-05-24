#ifndef INTERNALISPAFPS_H
#define INTERNALISPAFPS_H

#include <QElapsedTimer>
#include <QList>
#include <QPointF>

class IspProcThread;    // forward declaration - fix "main file cannot be included recursively"

class InternalIspAfps
{
public:
	InternalIspAfps();
	void Initialize(IspProcThread *ispProcThread);
	void SetMeanLuminanceMeasured(QList<QPointF> meanLuminance);

private:
	QElapsedTimer elapsedTimer;
	clock_t lastTime = 0;
	IspProcThread *ispProcThread = nullptr;

		/* allowed FPS list for AR0521 1920x1080 */
	const QList<uint8_t> AllowedFPS = { 7, 8, 9, 11, 12, 14, 16, 20, 25, 33, 50, 100 };
	int fpsIndex = 9;  // 33 fps
	int fps = 33;
	bool grayMode = false;

	bool isInitialized();
	void setLowerFps();
	void setHigherFps();
	void setFps(int fps);
	void setGrayMode(bool grayMode);
};

#endif // INTERNALISPAFPS_H
