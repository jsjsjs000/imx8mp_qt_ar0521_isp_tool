#ifndef ISPCONTROL_H
#define ISPCONTROL_H

#include <QString>
#include "json_helper.h"

class IspControl
{
	int fd;
	int streamid = 0;
public:
	IspControl();
	int openVideo(void);
	bool vivIoctl(const char *cmd, Json::Value& jsonRequest, Json::Value& jsonResponse);
	Json::Value getParam(const char *getCmd);
	bool setParam(const char *getCmd, const char *setType, const char *parameter, int value, int divide);
	bool setParamBool(const char *getCmd, const char *setCmd, const char *parameter, bool value);
	bool sendCmd(const char *setCmd, const char *parameter, const char *value);
};

#endif // ISPCONTROL_H
