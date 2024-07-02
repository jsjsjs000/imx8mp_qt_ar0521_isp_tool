#include <sys/ioctl.h>
#include <QList>
#include <QDebug>
#include <fcntl.h>

#include "ioctl_cmds.h"
#include "viv_video_kevent.h"
#include "isp_control.h"
#include "cam_device_module_ids.h"

// #define DEBUG

IspControl::IspControl() {}

int IspControl::openVideo()
{
	int videoid = 0;
	char szFile[256] = {0};
	sprintf(szFile, "/dev/video%d", videoid);
	this->fd = ::open(szFile, O_RDWR | O_NONBLOCK);
	if (this->fd < 0)
	{
		qDebug("can't open video file %s", szFile);
		return 1;
	}

	v4l2_capability caps;
	int result = ::ioctl(this->fd, VIDIOC_QUERYCAP, &caps);
	if (result < 0)
	{
		qDebug("failed to get device caps for %s (%d = %s)", szFile, errno, strerror(errno));
		return 1;
	}

#ifdef DEBUG
	qDebug("Open: %s \n", szFile);
	qDebug("Open: (fd=%d)\n", fd);
	qDebug("Open Device: %s (fd=%d)\n", szFile, fd);
	qDebug("  Driver: %s\n", caps.driver);
#endif

	if (strcmp((const char*)caps.driver, "viv_v4l2_device") == 0)
	{
#ifdef DEBUG
		qDebug("found viv video dev %s\n", szFile);
#endif
		int streamid = -1;
		::ioctl(this->fd, VIV_VIDIOC_S_STREAMID, &streamid);
#ifdef DEBUG
		qDebug("streamid %d\n", streamid);
#endif
	}
	else
	{
		qDebug("Open wrong type of viv video dev\n");
		return 1;
	}

	return 0;
}

#define VIV_CUSTOM_CID_BASE (V4L2_CID_USER_BASE | 0xf000)
#define V4L2_CID_VIV_EXTCTRL (VIV_CUSTOM_CID_BASE + 1)

bool IspControl::vivIoctl(const char *cmd, Json::Value& jsonRequest, Json::Value& jsonResponse)
{
	if (!cmd)
	{
		qDebug("cmd should not be null!");
		return false;
	}
	jsonRequest["id"] = cmd;
	jsonRequest["streamid"] = streamid;

	struct v4l2_ext_controls ecs;
	struct v4l2_ext_control ec;
	memset(&ecs, 0, sizeof(ecs));
	memset(&ec, 0, sizeof(ec));
	ec.string = new char[VIV_JSON_BUFFER_SIZE];
	ec.id = V4L2_CID_VIV_EXTCTRL;
	ec.size = 0;
	ecs.controls = &ec;
	ecs.count = 1;

	::ioctl(this->fd, VIDIOC_G_EXT_CTRLS, &ecs);

	strncpy(ec.string, jsonRequest.toStyledString().c_str(), VIV_JSON_BUFFER_SIZE);
#ifdef DEBUG
	qDebug("DEBUG out: %s\n", ec.string);
	// std::string inputString;
	// std::getline(std::cin, inputString);
#endif
	int ret = ::ioctl(this->fd, VIDIOC_S_EXT_CTRLS, &ecs);
	if (ret != 0)
	{
		qDebug("failed to set ext ctrl\n");
		goto end;
	}
	else
	{
		::ioctl(this->fd, VIDIOC_G_EXT_CTRLS, &ecs);
#ifdef DEBUG
		qDebug("DEBUG in: %s\n", ec.string);
#endif
		Json::Reader reader;
		reader.parse(ec.string, jsonResponse, true);
		delete[] ec.string;
		ec.string = NULL;
		return jsonResponse["result"].asInt() == 0;
	}

end:
	delete ec.string;
	ec.string = NULL;
	return false;
}

// bool IspControl::getFps()
// {
// 	viv_caps_mode_info_s info;
// 	if (::ioctl(this->fd, VIV_VIDIOC_S_MODEINFO, &info) != 0)
// 		return false;
// 	qDebug("fps %d", info.fps);

// 	vvcam_mode_info_t
// 	uint32_t pfps;
// 	uint32_t ret = ::ioctl(this->fd, VVSENSORIOC_G_FPS, &pfps);
// 	if (ret != 0)
// 		qDebug() << pfps;

// 	IsiSensorAeInfo_t AeInfo;
// 	IsiGetAeInfoIsfs(&AeInfo);

// 	SensorOps fo;

// 	return true;
// }

void IspControl::fixGetParam(Json::Value *jRequest, const char *getCmd)
{
	if (strncmp(getCmd, IF_WDR_G_EN, strlen(IF_WDR_G_EN)) == 0 ||    // fix WDR generation = 2 (WDR3)
			strncmp(getCmd, IF_WDR_G_CFG, strlen(IF_WDR_G_CFG)) == 0 ||
			strncmp(getCmd, IF_WDR_G_STATUS, strlen(IF_WDR_G_STATUS)) == 0 ||
			strncmp(getCmd, IF_WDR_G_TBL, strlen(IF_WDR_G_TBL)) == 0)
		(*jRequest)[WDR_GENERATION_PARAMS] = 2;           // 2: WDR3
}

void IspControl::fixSetParam(Json::Value *jRequest, const char *setCmd)
{
	if (strncmp(setCmd, IF_WDR_S_EN, strlen(IF_WDR_S_EN)) == 0 ||    // fix WDR generation = 2 (WDR3)
			strncmp(setCmd, IF_WDR_S_CFG, strlen(IF_WDR_S_CFG)) == 0 ||
			strncmp(setCmd, IF_WDR_RESET, strlen(IF_WDR_RESET)) == 0)
		(*jRequest)[WDR_GENERATION_PARAMS] = 2;           // 2: WDR3
}

Json::Value *IspControl::getJsonValue(Json::Value &jRequest, const char *parameter)
{
	QString parameter_ = QString(parameter);
	int slashIndex = parameter_.indexOf('/');
	if (slashIndex < 0)
		return &jRequest[parameter];

	QString parameter1 = parameter_.left(slashIndex);
	QString parameter2 = parameter_.mid(slashIndex + 1, parameter_.length() - slashIndex - 1);
	return &jRequest[parameter1.toStdString()][parameter2.toStdString()];
}

bool IspControl::setParamNumber(const char *getCmd, const char *setCmd, const char *parameter, int value, int divide)
{
	Json::Value jRequest, jResponse;
	if (strlen(getCmd) > 0)
	{
		this->fixGetParam(&jRequest, getCmd);
		if (!vivIoctl(getCmd, jRequest, jResponse))
			return false;
	}

	jRequest = jResponse;
	if (strlen(parameter) > 0)
	{
		Json::Value *jRequest2 = this->getJsonValue(jRequest, parameter);
		if (divide == 1)
			*jRequest2 = value;
		else
			*jRequest2 = (float)value / divide;
	}

	this->fixSetParam(&jRequest, setCmd);

	return vivIoctl(setCmd, jRequest, jResponse);
}

bool IspControl::setParamArray(const char *getCmd, const char *setCmd, const char *parameter, const QList<float> array)
{
	Json::Value jRequest, jResponse;
	if (strlen(getCmd) > 0)
	{
		this->fixGetParam(&jRequest, getCmd);
		if (!vivIoctl(getCmd, jRequest, jResponse))
			return false;
	}

	jRequest = jResponse;
	if (strlen(parameter) > 0)
	{
		Json::Value *jRequest2 = this->getJsonValue(jRequest, parameter);
		for	(int i = 0; i < array.count(); i++)
			(*jRequest2)[i] = array[i];
	}

	this->fixSetParam(&jRequest, setCmd);

	return vivIoctl(setCmd, jRequest, jResponse);
}

bool IspControl::setParamString(const char *getCmd, const char *setCmd, const char *parameter, const char *value)
{
	Json::Value jRequest, jResponse;
	if (strlen(getCmd) > 0)
	{
		this->fixGetParam(&jRequest, getCmd);
		if (!vivIoctl(getCmd, jRequest, jResponse))
			return false;
	}

	jRequest = jResponse;
	if (strlen(parameter) > 0)
	{
		Json::Value *jRequest2 = this->getJsonValue(jRequest, parameter);
		*jRequest2 = value;
	}

	this->fixSetParam(&jRequest, setCmd);

	return vivIoctl(setCmd, jRequest, jResponse);
}

bool IspControl::setParamBool(const char *getCmd, const char *setCmd, const char *parameter, bool value)
{
	Json::Value jRequest, jResponse;
	if (strlen(getCmd) > 0)
	{
		this->fixGetParam(&jRequest, getCmd);
		if (!vivIoctl(getCmd, jRequest, jResponse))
			return false;
	}

	jRequest = jResponse;
	if (strlen(parameter) > 0)
	{
		Json::Value *jRequest2 = this->getJsonValue(jRequest, parameter);
		*jRequest2 = value;
	}

	this->fixSetParam(&jRequest, setCmd);

	return vivIoctl(setCmd, jRequest, jResponse);
}

Json::Value IspControl::getParam(const char *getCmd)
{
	// Json::Value jRequest, jResponse;
	// this->fixGetParam(&jRequest, getCmd);
	// if (!vivIoctl(getCmd, jRequest, jResponse))
		return nullptr;

	// return jResponse;
}
