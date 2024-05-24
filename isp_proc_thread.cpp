#include "isp_proc_thread.h"
#include <QDebug>
#include <QThread>
#include <json/value.h>
#include <widgets/chart_widget.h>
#include <widgets/checkbox_widget.h>
#include <widgets/combobox_widget.h>
#include <widgets/combobox_widget2.h>
#include <widgets/label_widget.h>
#include <widgets/matrix_view_widget.h>
#include <widgets/slider_widget.h>
#include <widgets/slider_array_widget.h>

IspProcThread::IspProcThread(QObject *parent, IspControl &ispControl, ControlsDefinitions &controlsDefinition,
		QMap<QString, QWidget*> &widgets) : QThread(parent), ispControl(ispControl),
		controlsDefinition(controlsDefinition), widgets(widgets)
{
	qRegisterMetaType<QListQPointF>("QListQPointF");   // need for emit signal_update_chart( QList<QPointF> )
	qRegisterMetaType<CheckBoxWidget*>("CheckBoxWidget*");
	qRegisterMetaType<ComboBoxWidget*>("ComboBoxWidget*");
	qRegisterMetaType<ComboBoxWidget2*>("ComboBoxWidget2*");
	qRegisterMetaType<SliderWidget*>("SliderWidget*");
	qRegisterMetaType<SliderArrayWidget*>("SliderArrayWidget*");
	qRegisterMetaType<LabelWidget*>("LabelWidget*");
	qRegisterMetaType<ChartWidget*>("ChartWidget*");
	qRegisterMetaType<MatrixViewWidget*>("MatrixViewWidget*");

	this->internalIspAfps.Initialize(this);
}

void IspProcThread::AddCommandToQueue(const CommandItem &commandItem)
{
	this->commandsQueueMutex.lock();
	this->commandsQueue.enqueue(commandItem);
	this->commandsQueueMutex.unlock();
}

QString IspProcThread::GetStatus()
{
	return this->internalIspAfps.GetStatus();
}

bool IspProcThread::isCommandQueueNotEmpty()
{
	this->commandsQueueMutex.lock();
	bool notEmpty = !this->commandsQueue.empty();
	this->commandsQueueMutex.unlock();
	return notEmpty;
}

void IspProcThread::run()
{
	while (!this->Stop)
	{
			/* 300 ms delay */
		for (int i = 0; i < 10 && !this->Stop && !this->isCommandQueueNotEmpty(); i++)
			msleep(30);

		if (this->isCommandQueueNotEmpty())
			this->executeCommandsFromQueue();
		else
			this->readParameters();
	}
}

void IspProcThread::executeCommandsFromQueue()
{
	this->commandsQueueMutex.lock();
	if (this->commandsQueue.empty())
	{
		this->commandsQueueMutex.unlock();
		return;
	}

	CommandItem commandItem = this->commandsQueue.dequeue();
	this->commandsQueueMutex.unlock();

	if (commandItem.commandItemType == CommandItem::CommandItemType::Bool)
		ispControl.setParamBool(commandItem.getCmd.toStdString().c_str(), commandItem.setCmd.toStdString().c_str(),
				commandItem.parameter.toStdString().c_str(), commandItem.boolValue);
	else if (commandItem.commandItemType == CommandItem::CommandItemType::Number)
		ispControl.setParamNumber(commandItem.getCmd.toStdString().c_str(), commandItem.setCmd.toStdString().c_str(),
				commandItem.parameter.toStdString().c_str(), commandItem.value, commandItem.divide);
	else if (commandItem.commandItemType == CommandItem::CommandItemType::Array)
		ispControl.setParamArray(commandItem.getCmd.toStdString().c_str(), commandItem.setCmd.toStdString().c_str(),
				commandItem.parameter.toStdString().c_str(), commandItem.array);
	else if (commandItem.commandItemType == CommandItem::CommandItemType::String)
		ispControl.setParamString(commandItem.getCmd.toStdString().c_str(), commandItem.setCmd.toStdString().c_str(),
				commandItem.parameter.toStdString().c_str(), commandItem.string.toStdString().c_str());
}

void IspProcThread::readParameters()
{
	for (const QString &paramName : qAsConst(controlsDefinition.readParams))
	{
		if (this->Stop || this->isCommandQueueNotEmpty())
			break;

		Json::Value params = ispControl.getParam(paramName.toStdString().c_str());
		if (params)
			this->updateControlsFromJson(params, paramName);
	}
}

void IspProcThread::updateControlsFromJson(Json::Value json, QString cmd)
{
	for (const auto *control : qAsConst(controlsDefinition.controls))
	{
		if (this->Stop)
			break;

		Json::Value *value = IspControl::getJsonValue(json, control->parameter.toStdString().c_str());

		if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
		{
			SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				if (scontrol->precision == 0)
				{
					int value_ = value->asInt();
					emit signal_update_slider_control_int(slider, value_);
					// qDebug() << scontrol->parameter << value;
				}
				else
				{
					float value_ = value->asFloat();
					emit signal_update_slider_control_float(slider, value_);
					// qDebug() << scontrol->parameter << value;

					if (strncmp(scontrol->parameter.toStdString().c_str(), EC_TIME_PARAMS, strlen(EC_TIME_PARAMS)) == 0)
					{
						if (value_ != 0)
							this->readFps = 1.0f / value_;
						else
							this->readFps = 0;
						// qDebug("%f", 1.0 / value);
					}
				}
			}
		}
		else if (const SliderArrayControl *scontrol = dynamic_cast<const SliderArrayControl*>(control))
		{
			SliderArrayWidget *slider = (SliderArrayWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (slider == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				Json::Value defaultValue = -1.0;
				QList<float> values;
				for (uint i = 0; i < value->size(); i++)
					values.push_back(value->get(i, defaultValue).asFloat());
				emit signal_update_slider_array_control_float(slider, values);
				// qDebug() << scontrol->parameter << values;
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
				if (value->isBool())
					index = value->asBool();
				else
					index = value->asInt();
				emit signal_update_comboBox_item_index(comboBox, index);
			}
		}
		else if (const ComboBoxControl2 *scontrol = dynamic_cast<const ComboBoxControl2*>(control))
		{
			ComboBoxWidget2 *comboBox = (ComboBoxWidget2*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (comboBox == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				QString index;
				if (value->isBool())
					index = value->asBool() ? "true" : "false";
				else
					index = QString(value->asCString());
				emit signal_update_comboBox2_item_index(comboBox, index);
			}
		}
		else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
		{
			CheckBoxWidget *checkBox = (CheckBoxWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (checkBox == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				int state = value->asInt();
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
				Json::Value value_ = *value;
				QString text;
				if (scontrol->type == &typeid(int[]))
				{
					for (uint i = 0; i < value_.size(); i++)
					{
						if (i > 0)
							text += ",";
						text += QString::number(value_[i].asInt());
					}
				}
				else if (scontrol->type == &typeid(float[]))
				{
					for (uint i = 0; i < value_.size(); i++)
					{
						if (i > 0)
							text += ", ";
						text += QString::number(value_[i].asFloat(), 'f', 3);
					}
				}
				else if (scontrol->type == &typeid(int))
					text = QString::number(value_.asInt());
				else if (scontrol->type == &typeid(float))
					text = QString::number(value_.asFloat(), 'f', 3);
				else if (scontrol->type == &typeid(std::string))
					text = QString(value_.asCString());
				else if (scontrol->type == &typeid(std::string[]))
				{
					for (uint i = 0; i < value_.size(); i++)
					{
						if (i > 0)
							text += ", ";
						text += QString(value_[i].asCString());
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
				// if (scontrol->type == &typeid(float[]))
				// else if (scontrol->type == &typeid(int[]))
				Json::Value defaultValue = -1.0;
				QList<QPointF> points;
				for (uint i = 0; i < value->size(); i++)
					points.push_back(QPointF(i, value->get(i, defaultValue).asFloat()));
				emit signal_update_chart(chart, 0, value->size() - 1, scontrol->y1, scontrol->y2, 1.0f, scontrol->gridY, points);
				// qDebug() << scontrol->parameter << points;
			}
		}
		else if (const MatrixViewControl *scontrol = dynamic_cast<const MatrixViewControl*>(control))
		{
			MatrixViewWidget *matrixView = (MatrixViewWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
			if (matrixView == nullptr)
				qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
			else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
			{
				// if (scontrol->type == &typeid(float[]))
				// else if (scontrol->type == &typeid(int[]))
				Json::Value defaultValue = -1.0;
				QList<QPointF> points;
				for (uint i = 0; i < value->size(); i++)
					points.push_back(QPointF(i, value->get(i, defaultValue).asFloat()));
				emit signal_update_matrix_view(matrixView, points);
				// qDebug() << scontrol->parameter << points;

				if (strncmp(scontrol->parameter.toStdString().c_str(), AE_LUMA_PARAMS_BASE64, strlen(AE_LUMA_PARAMS_BASE64)) == 0)
					internalIspAfps.SetMeanLuminanceMeasured(points);
			}
		}
	}
}
