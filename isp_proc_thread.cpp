#include "isp_proc_thread.h"
#include <QDebug>
#include <QThread>
#include <json/value.h>
#include <widgets/chart_widget.h>
#include <widgets/checkbox_widget.h>
#include <widgets/combobox_widget.h>
#include <widgets/label_widget.h>
#include <widgets/slider_widget.h>

IspProcThread::IspProcThread(QObject *parent, IspControl &ispControl, ControlsDefinitions &controlsDefinition,
		QMap<QString, QWidget*> &widgets) : QThread(parent), ispControl(ispControl),
		controlsDefinition(controlsDefinition), widgets(widgets)
{
	qRegisterMetaType<QListQPointF>("QListQPointF");   // need for emit signal_update_chart( QList<QPointF> )
}

void IspProcThread::run()
{
	while (!this->Stop)
	{
		msleep(300);

		this->readParameters();
	}
}

void IspProcThread::readParameters()
{
	for (const QString &paramName : qAsConst(controlsDefinition.readParams))
	{
		Json::Value params = ispControl.getParam(paramName.toStdString().c_str());
		if (params)
			this->updateControlsFromJson(params, paramName);
	}
}

void IspProcThread::updateControlsFromJson(Json::Value json, QString cmd)
{
	for (const auto *control : qAsConst(controlsDefinition.controls))
	{
		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				if (scontrol->precision == 0)
				{
					int value = json[scontrol->parameter.toStdString()].asInt();
					emit signal_update_slider_control_int(slider, value);
					// qDebug() << scontrol->parameter << value;
				}
				else
				{
					float value = json[scontrol->parameter.toStdString()].asFloat();
					emit signal_update_slider_control_float(slider, value);
					// qDebug() << scontrol->parameter << value;

					// if (strncmp(scontrol->parameter.toStdString().c_str(), EC_TIME_PARAMS, strlen(EC_TIME_PARAMS)) == 0)
					// 	qDebug("%f", 1.0 / value);
				}
			}
		}
		else if (const ComboBoxControl *scontrol = dynamic_cast<const ComboBoxControl*>(control))
		{
			ComboBoxWidget *comboBox = (ComboBoxWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (comboBox == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				int index;
				if (json[scontrol->parameter.toStdString()].isBool())
					index = json[scontrol->parameter.toStdString()].asBool();
				else
					index = json[scontrol->parameter.toStdString()].asInt();
				emit signal_update_comboBox_item_index(comboBox, index);
			}
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = (CheckBoxWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (checkBox == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				int state = json[scontrol->parameter.toStdString()].asInt();
				emit signal_update_checkBox_set_state(checkBox, state);
				// qDebug() << scontrol->parameter << state;
			}
		}
		else if (const LabelControl *scontrol = dynamic_cast<const LabelControl*>(control))
		{
			LabelWidget *label = (LabelWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (label == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				Json::Value value = json[scontrol->parameter.toStdString()];
				QString text;
				if (scontrol->type == &typeid(int[]))
				{
					for (uint i = 0; i < value.size(); i++)
					{
						if (i > 0)
							text += ",";
						text += QString::number(value[i].asInt());
					}
				}
				else if (scontrol->type == &typeid(float[]))
				{
					for (uint i = 0; i < value.size(); i++)
					{
						if (i > 0)
							text += ", ";
						text += QString::number(value[i].asFloat(), 'f', 3);
					}
				}
				else if (scontrol->type == &typeid(int))
					text = QString::number(value.asInt());
				else if (scontrol->type == &typeid(float))
					text = QString::number(value.asFloat(), 'f', 3);
				else if (scontrol->type == &typeid(std::string))
					text = QString(value.asCString());
				else if (scontrol->type == &typeid(std::string[]))
				{
					for (uint i = 0; i < value.size(); i++)
					{
						if (i > 0)
							text += ", ";
						text += QString(value[i].asCString());
					}
				}
				else
				{
					text = "(" + scontrol->parameter + " not decoded type)";
					qDebug() << scontrol->parameter << "not decoded in LabelControl";
				}

				emit signal_update_label_set_text(label, text);
				// qDebug() << scontrol->parameter << state;
			}
		}
		else if (const ChartControl *scontrol = dynamic_cast<const ChartControl*>(control))
		{
			ChartWidget *chart = (ChartWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (chart == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				Json::Value value = json[scontrol->parameter.toStdString()];
				// if (scontrol->type == &typeid(int[]))
				// else if (scontrol->type == &typeid(float[]))
				{
					QList<QPointF> points;
					for (uint i = 0; i < value.size(); i++)
						points.push_back(QPointF(i, value[i].asFloat()));
					emit signal_update_chart(chart, 0, value.size() - 1, scontrol->y1, scontrol->y2, 1.0f, scontrol->gridY, points);
				}
			}
		}
	}
}
