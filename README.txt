	to do:

- presets
- AFPS checkbox
- AFPS in kernel module
? 8/10/12 bit

+ thread
+ reset to initial/default
+ AE parameters
+ FPS
+ rest controls
+ show fps, iso, gain, exposure time, integration time in status bar

-- fix defect pixels reading - problem in driver
-- max gain in .xml - parameter not exists in .xml

ln -s /opt/imx8mp_qt_ar0521_isp_tool/bin/presets/ i
ln -s /opt/imx8-isp/bin/xml/ x

cd -P x
sed -n '72,+1p' VM-017-COL_AO062-C_1920x1080.xml
