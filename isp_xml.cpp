#include "isp_xml.h"

#include <QFile>
#include <QDebug>

IspXml::IspXml() {}

bool IspXml::openXmlFile(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Failed to open the file for reading.";
		return false;
	}
	else
	{
		if (!document.setContent(&file))
		{
			qDebug() << "Failed to load the file for reading.";
			return false;
		}
		file.close();
	}

	return true;
}

bool IspXml::saveXmlFile(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "Failed to open the file for writing.";
		return false;
	}
	else
	{
		QTextStream stream(&file);
		stream << this->document.toString(2);
		file.close();
	}

	return true;
}

QDomNode IspXml::getElementByTagPath(QString tagPath)
{
	return this->getElementByTagPath_(this->document.firstChildElement(), tagPath);
}

QDomNode IspXml::getElementByTagPath_(QDomNode root, QString tagPath)
{
	int slashIndex = tagPath.indexOf('/');
	int bracketIndex = tagPath.indexOf('[');
	QString tag;
	QString param;
	QString value;
	if (slashIndex < 0)
	{
		tag = QString(tagPath);
		tagPath = "";
	}
	else
	{
		tag = tagPath.left(slashIndex);
		tagPath = tagPath.mid(slashIndex + 1, tagPath.length() - slashIndex - 1);
	}

	if (bracketIndex >= 0)
	{
		int equalIndex = tag.indexOf('=');
		if (equalIndex >= 0)
		{
			param = tag.mid(bracketIndex + 1, equalIndex - bracketIndex - 1);
			value = tag.mid(equalIndex + 1, tag.length() - equalIndex - 2);
		}
		tag = tag.left(bracketIndex);
	}

	QDomNode n = root.firstChild();
	while (!n.isNull())
	{
		bool ok = false;
		if (n.nodeName() == tag)
		{
			if (param.length() > 0 && value.length() > 0)
			{
				if (n.toElement().attribute(param) == value)
					ok = true;
			}
			else
				ok = true;
		}

		if (ok)
		{
			if (tagPath == "")
				return n;

			return this->getElementByTagPath_(n, tagPath);
		}

		n = n.nextSibling();
	}

	return QDomNode();
}

QString IspXml::stripText(QString text)
{
	return text.replace('\r', ' ').replace('\n', ' ').trimmed();
}

void IspXml::setValue(QDomNode n, QString text, QString forPath)
{
	if (n.isNull())
		return;

	QString newText = "\n    ";
	QString endingText = "  ";

	for (int i = 0; i < forPath.length(); i++)
		if (forPath[i] == '/')
		{
			newText += "  ";
			endingText += "  ";
		}

	newText += text + "\n" + endingText;

	n.firstChild().setNodeValue(newText);
}

bool IspXml::parseArrayFloat(QString text, QList<float> &array)
{
	text = IspXml::stripText(text);

	if (text.length() > 0 && text.startsWith('['))
		text = text.mid(1);
	if (text.length() > 0 && text.endsWith(']'))
		text = text.mid(0, text.length() - 1);

	bool ok;
	QStringList list = text.split(' ', Qt::SkipEmptyParts);
	for (const QString &item : list)
	{
		float f = item.toFloat(&ok);
		if (!ok)
			return false;

		array.push_back(f);
	}

	return true;
}

QString IspXml::arrayFloatToString(QList<float> array)
{
	QString s = "[";
	for (int i = 0; i < array.length(); i++)
	{
		if (i > 0)
			s += ", ";
		s += QString::number(array[i]);
	}
	return s + "]";
}

QString IspXml::arrayFloatToString(QList<QPointF> array)
{
	QString s = "[";
	for (int i = 0; i < array.length(); i++)
	{
		if (i > 0)
			s += " ";
		s += QString::number(array[i].y(), 'f');
	}
	return s + "]";
}

/*
	Xml examples:
https://www.bogotobogo.com/Qt/Qt5_QtXML_QDOM_Reading_XML.php
https://www.bogotobogo.com/Qt/Qt5_QtXML_QDomDocument_QDomElement.php
*/
