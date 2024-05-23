#ifndef ISPPROCTHREAD_H
#define ISPPROCTHREAD_H

#include "isp_control.h"
#include "controls_definitions.h"
#include <QPointF>
#include <QThread>
#include <QList>
#include <internal_isp/internal_isp_afps.h>

typedef QList<QPointF> QListQPointF;   // need for emit signal_update_chart( QList<QPointF> )

class ButtonWidget;    // forward declaration - fix "main file cannot be included recursively"
class ChartWidget;
class CheckBoxWidget;
class ComboBoxWidget;
class ComboBoxWidget2;
class LabelWidget;
class SliderWidget;
class SliderArrayWidget;
class MatrixViewWidget;

class IspProcThread : public QThread
{
	Q_OBJECT;

private:
	IspControl &ispControl;
	ControlsDefinitions &controlsDefinition;
	QMap<QString, QWidget*> &widgets;
	InternalIspAfps internalIspAfps;

	void run() override;
	void readParameters();
	void updateControlsFromJson(Json::Value json, QString cmd);

public:
	bool Stop = false;
	int readFps = 0;
	explicit IspProcThread(QObject *parent, IspControl &ispControl, ControlsDefinitions &controlsDefinition, QMap<QString, QWidget*> &widgets);

signals:
	void signal_update_slider_control_int(SliderWidget *slider, int value);
	void signal_update_slider_control_float(SliderWidget *slider, float value);
	void signal_update_slider_array_control_int(SliderArrayWidget *slider, QList<int> values);
	void signal_update_slider_array_control_float(SliderArrayWidget *slider, QList<float> values);
	void signal_update_comboBox_item_index(ComboBoxWidget *comboBox, int index);
	void signal_update_comboBox2_item_index(ComboBoxWidget2 *comboBox, QString index);
	void signal_update_checkBox_set_state(CheckBoxWidget *checkBox, bool state);
	void signal_update_label_set_text(LabelWidget *label, QString text);
	void signal_update_chart(ChartWidget *chart, float x1, float x2, float y1, float y2, float gridX, float gridY, QList<QPointF> points);
	void signal_update_matrix_view(MatrixViewWidget *matrixView, QList<QPointF> points);
};

#endif // ISPPROCTHREAD_H
