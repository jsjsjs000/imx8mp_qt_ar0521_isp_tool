#ifndef ISPCONTROL_H
#define ISPCONTROL_H

#include "json_helper.h"

class IspControl
{
	int fd;
	int streamid = 0;
public:
	IspControl();
	int openVideo(void);
	bool vivIoctl(const char *cmd, Json::Value& jsonRequest, Json::Value& jsonResponse);
	bool setCprocCfg(QString parameter, int value, int divide);
	bool setCprocCoeff(QString parameter, int value, int divide);
	bool setCprocEnable(QString parameter, int value);
	Json::Value getCprocEn();
	Json::Value getCprocCfg();
};

#endif // ISPCONTROL_H
