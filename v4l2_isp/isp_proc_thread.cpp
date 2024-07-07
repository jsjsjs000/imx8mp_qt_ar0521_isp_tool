#include "isp_proc_thread.h"
#include <QDebug>
#include <QStringBuilder>
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

void IspProcThread::saveParam(const Control *control, QString value)
{
	QString key = control->getCmd + "/" + control->parameter;
	if (!this->defaultParams.contains(key))
	{
		this->defaultParams[key] = value;
		this->lastReadParams[key] = value;

		// qDebug() << "new key:" << key << value;
	}
	else if (this->lastReadParams[key] != value)
	{
		this->lastReadParams[key] = value;

		// qDebug() << "update key:" << key << value;
	}
}

QString IspProcThread::slot_getParams()
{
	QString s = "";
	for (auto it = this->lastReadParams.keyValueBegin(); it != this->lastReadParams.keyValueEnd(); ++it)
	{
		if (s.length() > 0)
			s = s % "\n";
		s = s % it->first % "=" % it->second;
	}
	return s;
}

QString IspProcThread::slot_getParamsDiff()
{
	QString s = "";
	for (auto it = this->lastReadParams.keyValueBegin(); it != this->lastReadParams.keyValueEnd(); ++it)
	{
		if (it->second != this->defaultParams[it->first])
		{
			if (s.length() > 0)
				s = s % "\n";
			s = s % it->first % "=" % it->second;
		}
	}
	return s;
}

void IspProcThread::slot_setParams(QMap<QString, QString> *params)
{
	for (auto it = params->keyValueBegin(); it != params->keyValueEnd(); ++it)
	{
		QStringList words = it->first.split("/");
		if (words.count() != 2)
			continue;

		QString cmd = words[0];
		QString param = words[1];
		QString value = it->second;

		for (const auto *control : qAsConst(controlsDefinition.controls))
		{
			if (this->Stop)
				break;

			if (control->getCmd != cmd || control->parameter != param || control->getCmd == "" || control->setCmd == "")
				continue;

			if (const SliderControl *scontrol = dynamic_cast<const SliderControl*>(control))
			{
				SliderWidget *slider = (SliderWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
				if (slider == nullptr)
					qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
				else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
				{
					// qDebug() << "restore:" << cmd << param << value;
					bool ok;
					if (scontrol->precision == 0)
					{
						int value_ = value.toInt(&ok);
						if (!ok)
							continue;
						// ispControl.setParamNumber(control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
						// 		control->parameter.toStdString().c_str(), value_, 1);
						CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Number,
								control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
								control->parameter.toStdString().c_str(), value_, 1);
						this->AddCommandToQueue(commandItem);
					}
					else
					{
						float value_ = value.toFloat(&ok);
						if (!ok)
							continue;
						// ispControl.setParamNumber(control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
						// 		control->parameter.toStdString().c_str(), value_ * scontrol->multiple, scontrol->multiple);
						CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Number,
								control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
								control->parameter.toStdString().c_str(), value_ * scontrol->multiple, scontrol->multiple);
						this->AddCommandToQueue(commandItem);
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
					// Json::Value defaultValue = -1.0;
					// QList<float> values;
					// QString values_ = "";
					// for (uint i = 0; i < value.size(); i++)
					// {
					// 	values.push_back(value.get(i, defaultValue).asFloat());
					// 	if (i > 0)
					// 		values_ += " ";
					// 	values_ += QString::number(value.get(i, defaultValue).asFloat());
					// }
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
					// qDebug() << "restore combobox:" << cmd << param << value;
					bool value_ = value == "1" || value == "true";
					// ispControl.setParamBool(control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
					// 		control->parameter.toStdString().c_str(), value_);
					CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Number,
							control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
							control->parameter.toStdString().c_str(), value_, 1);
					this->AddCommandToQueue(commandItem);
				}
			}
			else if (const ComboBoxControl2 *scontrol = dynamic_cast<const ComboBoxControl2*>(control))
			{
				ComboBoxWidget2 *comboBox = (ComboBoxWidget2*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
				if (comboBox == nullptr)
					qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
				else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
				{
					// qDebug() << "restore combobox2:" << cmd << param << value;
					// bool value_ = value == "1" || value == "true";
					// ispControl.setParamBool(control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
					// 		control->parameter.toStdString().c_str(), value_);
					CommandItem commandItem;
					if (value == "0" || value == "false")
						commandItem = CommandItem(CommandItem::CommandItemType::Bool,
								control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
								control->parameter.toStdString().c_str(), false);
					else if (value == "1" || value == "true")
						commandItem = CommandItem(CommandItem::CommandItemType::Bool,
								control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
								control->parameter.toStdString().c_str(), true);
					else
						commandItem = CommandItem(CommandItem::CommandItemType::String,
								control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
								control->parameter.toStdString().c_str(), value);
					this->AddCommandToQueue(commandItem);
				}
			}
			else if (const CheckBoxControl *scontrol = dynamic_cast<const CheckBoxControl*>(control))
			{
				CheckBoxWidget *checkBox = (CheckBoxWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
				if (checkBox == nullptr)
					qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
				else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
				{
					qDebug() << "restore checkbox:" << cmd << param << value;
					bool value_ = value == "1" || value == "true";
					// ispControl.setParamBool(control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
					// 		control->parameter.toStdString().c_str(), value_);
				}
				CommandItem commandItem = CommandItem(CommandItem::CommandItemType::Bool,
						control->getCmd.toStdString().c_str(), control->setCmd.toStdString().c_str(),
						control->parameter.toStdString().c_str(), value);
				this->AddCommandToQueue(commandItem);
			}
			// else if (const LabelControl *scontrol = dynamic_cast<const LabelControl*>(control))
			else if (const ChartControl *scontrol = dynamic_cast<const ChartControl*>(control))
			{
				ChartWidget *chart = (ChartWidget*)this->widgets[scontrol->setCmd + "/" + scontrol->parameter];
				if (chart == nullptr)
					qDebug() << "Widget " << scontrol->setCmd + "/" + scontrol->parameter << " not found";
				else if (scontrol->setCmd == cmd || scontrol->getCmd == cmd)
				{
					// if (scontrol->type == &typeid(float[]))
					// else if (scontrol->type == &typeid(int[]))
					// Json::Value defaultValue = -1.0;
					// QList<QPointF> points;
					// QString points_ = "";
					// for (uint i = 0; i < value->size(); i++)
					// {
					// 	points.push_back(QPointF(i, value->get(i, defaultValue).asFloat()));
					// 	if (i > 0)
					// 		points_ += " ";
					// 	points_ += QString::number(value->get(i, defaultValue).asFloat());
					// }
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
					// Json::Value defaultValue = -1.0;
					// QList<QPointF> points;
					// QString points_ = "";
					// for (uint i = 0; i < value->size(); i++)
					// {
					// 	points.push_back(QPointF(i, value->get(i, defaultValue).asFloat()));
					// 	if (i > 0)
					// 		points_ += " ";
					// 	points_ += QString::number(value->get(i, defaultValue).asFloat());
					// }
					// // qDebug() << scontrol->parameter << points;

					// if (scontrol->getCmd == IF_AE_G_STATUS && scontrol->parameter == AE_LUMA_PARAMS_BASE64)
					// {
					// 	int mean = 0;
					// 	for (uint i = 0; i < value->size(); i++)
					// 		mean += value->get(i, 0).asFloat();
					// 	this->meanLuminance = mean / value->size();
					// }
				}
			}
		}
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
					this->saveParam(control, QString::number(value_));
					emit signal_update_slider_control_int(slider, value_);
					// qDebug() << scontrol->parameter << value;
				}
				else
				{
					float value_ = value->asFloat();
					this->saveParam(control, QString::number(value_));
					emit signal_update_slider_control_float(slider, value_);
					// qDebug() << scontrol->parameter << value_;

					if (scontrol->getCmd == IF_EC_G_CFG && scontrol->parameter == EC_TIME_PARAMS)
					{
						this->exposureTime = value_;

						if (value_ != 0)
							this->fps = 1.0f / value_;
						else
							this->fps = 0;
						// qDebug("%f", 1.0 / value);
					}
					else if (scontrol->getCmd == IF_EC_G_CFG && scontrol->parameter == EC_GAIN_PARAMS)
						this->exposureGain = value_;
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
				QString values_ = "";
				for (uint i = 0; i < value->size(); i++)
				{
					values.push_back(value->get(i, defaultValue).asFloat());
					if (i > 0)
						values_ += " ";
					values_ += QString::number(value->get(i, defaultValue).asFloat());
				}
				this->saveParam(control, values_);
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
				this->saveParam(control, QString::number(index));
				emit signal_update_comboBox_item_index(comboBox, index);

				if (scontrol->getCmd == IF_AE_G_ISO && scontrol->parameter == AE_SENSITIVITY_PARAMS)
				{
					this->iso = index;
					internalIspAfps.SetIso(index);
				}
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
				this->saveParam(control, index);
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
				int state;
				if (value->isBool())
					state = value->asBool();
				else if (value->isString())
					state = value->asString().compare("false") != 0;
				else
					state = value->asInt();
				this->saveParam(control, QString::number(state));
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

				this->saveParam(control, text);
				emit signal_update_label_set_text(label, text);
				// qDebug() << scontrol->parameter << state;

				if (scontrol->getCmd == IF_FILTER_G_STATUS && scontrol->parameter == FILTER_INTEGRATION_TIME_PARAMS)
					this->integrationTime = value_.asFloat();
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
				QString points_ = "";
				for (uint i = 0; i < value->size(); i++)
				{
					points.push_back(QPointF(i, value->get(i, defaultValue).asFloat()));
					if (i > 0)
						points_ += " ";
					points_ += QString::number(value->get(i, defaultValue).asFloat());
				}
				this->saveParam(control, points_);
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
				QString points_ = "";
				for (uint i = 0; i < value->size(); i++)
				{
					points.push_back(QPointF(i, value->get(i, defaultValue).asFloat()));
					if (i > 0)
						points_ += " ";
					points_ += QString::number(value->get(i, defaultValue).asFloat());
				}
				this->saveParam(control, points_);
				emit signal_update_matrix_view(matrixView, points);
				// qDebug() << scontrol->parameter << points;

				if (scontrol->getCmd == IF_AE_G_STATUS && scontrol->parameter == AE_LUMA_PARAMS_BASE64)
				{
					int mean = 0;
					for (uint i = 0; i < value->size(); i++)
						mean += value->get(i, 0).asFloat();
					this->meanLuminance = mean / value->size();

					internalIspAfps.SetMeanLuminanceMeasured(points);
				}
			}
		}
	}
}
