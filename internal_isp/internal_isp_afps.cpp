#include "internal_isp_afps.h"
#include <QDebug>
#include <command_item.h>
#include "ioctl_cmds.h"
#include "isp_proc_thread.h"

InternalIspAfps::InternalIspAfps()
{
	this->elapsedTimer.start();
	this->lastTime = this->elapsedTimer.elapsed();
}

void InternalIspAfps::Initialize(IspProcThread *ispProcThread)
{
	this->ispProcThread = ispProcThread;
}

bool InternalIspAfps::isInitialized()
{
	return this->ispProcThread != nullptr;
}

void InternalIspAfps::SetMeanLuminanceMeasured(QList<QPointF> meanLuminanceMeasured)
{
	if (!this->isInitialized())
		return;

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

	if (avgMeanLuminanceMeasured < 50)
		this->setLowerFps();
	else if (avgMeanLuminanceMeasured >= 70)
		this->setHigherFps();

	if (this->fps == 7 && !this->grayMode)
		this->setGrayMode(true);
	if (this->fps != 7 && this->grayMode)
		this->setGrayMode(false);
}

void InternalIspAfps::setLowerFps()
{
	if (!this->isInitialized() || this->fpsIndex == 0)
		return;

	this->fpsIndex--;
	this->fps = this->AllowedFPS[this->fpsIndex];
	qDebug() << "set lower FPS" << this->fps;

	this->setFps(this->fps);
}

void InternalIspAfps::setHigherFps()
{
	if (!this->isInitialized() || this->fpsIndex >= this->AllowedFPS.count() - 1)
		return;

	this->fpsIndex++;
	this->fps = this->AllowedFPS[this->fpsIndex];
	qDebug() << "set upper FPS" << this->fps;

	this->setFps(this->fps);
}

void InternalIspAfps::setFps(int fps)
{
	CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Number, NULL, IF_S_FPS, "fps", fps, 1);
	ispProcThread->AddCommandToQueue(commandItem);
}

void InternalIspAfps::setGrayMode(bool grayMode)
{
	this->grayMode = grayMode;

	CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Bool, IF_DEMOSAIC_G_EN,  IF_DEMOSAIC_S_EN,  DEMOSAIC_ENABLE_PARAMS, !grayMode);
	ispProcThread->AddCommandToQueue(commandItem);
}
