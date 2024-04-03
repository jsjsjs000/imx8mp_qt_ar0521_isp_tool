#include <sys/ioctl.h>
#include <fcntl.h>

#include "cam_device_api.hpp"
#include "viv_video_kevent.h"
#include "ioctl_cmds.h"
#include "isp_control.h"

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
		printf("can't open video file %s", szFile);
		return 1;
	}

	v4l2_capability caps;
	int result = ::ioctl(this->fd, VIDIOC_QUERYCAP, &caps);
	if (result < 0)
	{
		printf("failed to get device caps for %s (%d = %s)", szFile, errno, strerror(errno));
		return 1;
	}

#ifdef DEBUG
	printf("Open: %s \n", szFile);
	printf("Open: (fd=%d)\n", fd);
	printf("Open Device: %s (fd=%d)\n", szFile, fd);
	printf("  Driver: %s\n", caps.driver);
#endif

	if (strcmp((const char*)caps.driver, "viv_v4l2_device") == 0)
	{
		// printf("found viv video dev %s\n", szFile);
		int streamid = -1;
		::ioctl(this->fd, VIV_VIDIOC_S_STREAMID, &streamid);
	}
	else
	{
		printf("Open wrong type of viv video dev\n");
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
		printf("cmd should not be null!");
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
	printf("DEBUG out: %s\n", ec.string);
	// std::string inputString;
	// std::getline(std::cin, inputString);
#endif
	int ret = ::ioctl(this->fd, VIDIOC_S_EXT_CTRLS, &ecs);
	if (ret != 0)
	{
		printf("failed to set ext ctrl\n");
		goto end;
	}
	else
	{
		::ioctl(this->fd, VIDIOC_G_EXT_CTRLS, &ecs);
#ifdef DEBUG
		printf("DEBUG in: %s\n", ec.string);
#endif
		Json::Reader reader;
		reader.parse(ec.string, jsonResponse, true);
		delete[] ec.string;
		ec.string = NULL;
		return jsonResponse["result"].asInt() == 0;
		// return jsonResponse["MC_RET"].asInt(); // $$ - oryginalnie, ale nie działa
	}

end:
	delete ec.string;
	ec.string = NULL;
	return false;
}

bool IspControl::setParam(const char *getCmd, const char *setCmd, const char *parameter, int value, int divide)
{
	Json::Value jRequest, jResponse;
	if (!vivIoctl(getCmd, jRequest, jResponse))
		return false;

	jRequest = jResponse;
	if (divide == 1)
		jRequest[parameter] = value;
	else
		jRequest[parameter] = (float)value / divide;
	return vivIoctl(setCmd, jRequest, jResponse);
}

bool IspControl::setParamBool(const char *getCmd, const char *setCmd, const char *parameter, bool value)
{
	Json::Value jRequest, jResponse;
	if (!vivIoctl(getCmd, jRequest, jResponse))
		return false;

	jRequest = jResponse;
	jRequest[parameter] = value;
	return vivIoctl(setCmd, jRequest, jResponse);
}

bool IspControl::sendCmd(const char *setCmd, const char *parameter, const char *value)
{
	Json::Value jRequest, jResponse;
	if (parameter)
		jRequest[parameter] = value;
	return vivIoctl(setCmd, jRequest, jResponse);
}

Json::Value IspControl::getParam(const char *getCmd)
{
	Json::Value jRequest, jResponse;
	if (!vivIoctl(getCmd, jRequest, jResponse))
		return nullptr;

	return jResponse;
}
