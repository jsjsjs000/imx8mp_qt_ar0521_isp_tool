QT += core gui
QT += multimediawidgets
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    isp_control.cpp \
    main.cpp \
		mainwindow.cpp \
		lib/jsoncpp/src/lib_json/json_reader.cpp \
		lib/jsoncpp/src/lib_json/json_value.cpp \
		lib/jsoncpp/src/lib_json/json_writer.cpp \
  preview_window.cpp \
  slider_widget.cpp

HEADERS += \
  controls_definitions.h \
  isp_control.h \
    mainwindow.h \
		lib/json_helper/json_helper.h \
  preview_window.h \
  slider_widget.h

FORMS += \
    mainwindow.ui \
    preview_window.ui \
    slider_widget.ui

INCLUDEPATH += \
  lib/jsoncpp/include \
  lib/json_helper \
  lib/cam_device/include \
	lib/cam_device/vvcam/common \
	lib/mediacontrol/include \
	lib/mediacontrol/include_api

# lib/cam_device/vvcam/common \
# lib/cam_device/utils/base64
# lib/jsoncpp/include/json \
# lib/jsoncpp/src/lib_json \

# LIBS +=
    # -L"$$_PRO_FILE_PWD_/lib/cam_device/" -l
		# -L$$OUT_PWDlib/cam_device/cam_device_api.hpp -l

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
