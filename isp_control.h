#ifndef ISPCONTROL_H
#define ISPCONTROL_H

#include "json_helper.h"
#include "cam_device_api.hpp"
#include "viv_video_kevent.h"
#include "ioctl_cmds.h"

class IspControl
{
	int fd;
	int streamid = 0;
public:
	IspControl();
	int OpenVideo(void);
	bool viv_private_ioctl(const char *cmd, Json::Value& jsonRequest, Json::Value& jsonResponse);
	bool set_cproc_brightness(int brightness);
	void get_cproc();
};

#endif // ISPCONTROL_H
