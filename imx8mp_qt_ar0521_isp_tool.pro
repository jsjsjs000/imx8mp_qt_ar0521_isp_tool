QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimediawidgets
QT += multimedia
QT += xml

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
  command_item.cpp \
  controls2_definitions.cpp \
  internal_isp/internal_isp_afps.cpp \
	v4l2_isp/isp_control.cpp \
	v4l2_isp/isp_proc_thread.cpp \
	v4l2_isp/isp_xml.cpp \
  presets/preset.cpp \
  presets/preset_rename_dialog.cpp \
  presets/presets.cpp \
	screenshots/rename_screenshot_window.cpp \
	screenshots/screenshot_checker.cpp \
	main.cpp \
	mainwindow.cpp \
	lib/jsoncpp/src/lib_json/json_reader.cpp \
	lib/jsoncpp/src/lib_json/json_value.cpp \
	lib/jsoncpp/src/lib_json/json_writer.cpp \
	widgets/button_widget.cpp \
  widgets/chart_widget.cpp \
	widgets/checkbox_widget.cpp \
  widgets/combobox_widget.cpp \
  widgets/combobox_widget2.cpp \
	widgets/group_widget.cpp \
	widgets/label_widget.cpp \
	widgets/matrix_view_widget.cpp \
  widgets/slider_array_widget.cpp \
	widgets/slider_widget.cpp

HEADERS += \
  command_item.h \
	controls.h \
  controls2.h \
  controls2_definitions.h \
  controls_definitions.h \
  internal_isp/internal_isp_afps.h \
	v4l2_isp/isp_control.h \
	v4l2_isp/isp_proc_thread.h \
	v4l2_isp/isp_xml.h \
  presets/preset.h \
  presets/preset_rename_dialog.h \
  presets/presets.h \
	screenshots/rename_screenshot_window.h \
	screenshots/screenshot_checker.h \
	mainwindow.h \
	lib/json_helper/json_helper.h \
	widgets/button_widget.h \
  widgets/chart_widget.h \
	widgets/checkbox_widget.h \
  widgets/combobox_widget.h \
  widgets/combobox_widget2.h \
	widgets/group_widget.h \
	widgets/label_widget.h \
	widgets/matrix_view_widget.h \
  widgets/slider_array_widget.h \
	widgets/slider_widget.h

FORMS += \
	mainwindow.ui \
	presets/preset_rename_dialog.ui \
	screenshots/rename_screenshot_window.ui \
	widgets/button_widget.ui \
	widgets/chart_widget.ui \
	widgets/checkbox_widget.ui \
	widgets/combobox_widget.ui \
	widgets/combobox_widget2.ui \
	widgets/group_widget.ui \
	widgets/label_widget.ui \
	widgets/matrix_view_widget.ui \
	widgets/slider_array_widget.ui \
	widgets/slider_widget.ui

INCLUDEPATH += \
  lib/jsoncpp/include \
  lib/json_helper \
  lib/cam_device/include \
	lib/cam_device/vvcam/common \
	lib/mediacontrol/include \
	lib/mediacontrol/include_api \
	/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/include/gstreamer-1.0 \
	/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/include/glib-2.0 \
	/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/lib/glib-2.0/include

	# /opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/include/orc-0.4 \
	# /opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/lib/libffi-3.2.1/include \
	# /usr/include/gstreamer-1.0 \

	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/gstreamer1.0/1.20.3.imx-r0/git \
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/gstreamer1.0/1.20.3.imx-r0/build \
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/glib-2.0/1_2.72.3-r0/glib-2.72.3/glib \
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/glib-2.0/1_2.72.3-r0/glib-2.72.3 \
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/glib-2.0/1_2.72.3-r0/build/glib
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/gobject-introspection/1.72.0-r0/recipe-sysroot/usr/include/glib-2.0
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-mx8mp-phytec-linux/gstreamer1.0-plugins-base/1.20.3.imx-r0/git/gst-libs \
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-mx8mp-phytec-linux/gstreamer1.0-plugins-base/1.20.3.imx-r0/build/gst-libs \
	# /opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/lib
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-mx8mp-phytec-linux/gstreamer1.0-plugins-base/1.20.3.imx-r0/recipe-sysroot/usr/include/gstreamer-1.0 \
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/gstreamer1.0-libav/1.20.7-r0/recipe-sysroot/usr/include/gstreamer-1.0
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/qtmultimedia/5.15.7+gitAUTOINC+eeb34aae03-r0/git
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-phytec-linux/qtmultimedia/5.15.7+gitAUTOINC+eeb34aae03-r0/recipe-sysroot/usr/include/gstreamer-1.0
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-mx8mp-phytec-linux/gstreamer1.0-plugins-base/1.20.3.imx-r0/recipe-sysroot/usr/lib

# lib/cam_device/utils/base64

LIBS += -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lgstvideo-1.0 -lgstbase-1.0
	# -pthread
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-mx8mp-phytec-linux/gstreamer1.0-plugins-base/1.20.3.imx-r0/build/gst-libs/gst/video/libgstvideo-1.0.so
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-mx8mp-phytec-linux/gstreamer1.0-plugins-base/1.20.3.imx-r0/build/gst-libs/gst/video/libgstvideo-1.0.so
	# /opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/lib/libgstbase-1.0.so
	# -L/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD22.1.1/sysroots/cortexa53-crypto-phytec-linux/usr/lib
	# /home/jarsulk/phyLinux/build/tmp/work/cortexa53-crypto-mx8mp-phytec-linux/gstreamer1.0-plugins-base/1.20.3.imx-r0/recipe-sysroot/usr/lib/libgstbase-1.0.so
	# -llibgstbase-1.0
# LIBS += -lOpengl
    # -L"$$_PRO_FILE_PWD_/lib/cam_device/" -l
		# -L$$OUT_PWDlib/cam_device/cam_device_api.hpp -l

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
  README.txt
