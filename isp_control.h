#ifndef ISPCONTROL_H
#define ISPCONTROL_H

#include "json_helper.h"

class IspControl
{
	int fd;
	int streamid = 0;
public:
	IspControl();
	int OpenVideo(void);
	bool viv_private_ioctl(const char *cmd, Json::Value& jsonRequest, Json::Value& jsonResponse);
	bool set_cproc_value(QString parameter, int value, int divide);
	bool set_cproc_enable(QString parameter, int value);
	Json::Value get_cproc();
};

#endif // ISPCONTROL_H
