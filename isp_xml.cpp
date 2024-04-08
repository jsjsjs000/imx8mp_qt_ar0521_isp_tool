#include "isp_xml.h"

#include <QFile>
#include <QDebug>

IspXml::IspXml() {}

bool IspXml::openXmlFile(QString filename)
{
	QDomDocument document;

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

	QDomElement root = document.firstChildElement();

	QDomNode n = this->retrievElements(root, "sensor/AWB/globals/cell/afRg1");
	if (!n.isNull())
		qDebug() << "result" << n.nodeName() << n.toElement().attribute("size") << n.toElement().text();

	// if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	// {
	// 	qDebug() << "Failed to open the file for reading.";
	// 	return false;
	// }
	// else
	// {
	// 	QTextStream stream(&file);
	// 	stream << document.toString();
	// 	file.close();
	// }

	qDebug() << "Reading finished";
	return true;
}

QDomNode IspXml::retrievElements(QDomNode root, QString tagPath)
{
	int i = tagPath.indexOf('/');
	QString tag;
	if (i < 0)
	{
		tag = QString(tagPath);
		tagPath = "";
	}
	else
	{
		tag = tagPath.left(i);
		tagPath = tagPath.mid(i + 1, tagPath.length() - i - 1);
	}

	QDomNode n = root.firstChild();
	while (!n.isNull())
	{
		// QDomElement *e = n.toElement();

		if (n.nodeName() == tag)
		{
			qDebug() << "in tag" << n.nodeName();
			if (tagPath == "")
				return n;

			return this->retrievElements(n, tagPath);
		}

		n = n.nextSibling();
	}

	return QDomNode();
}

/*
	Xml examples:
https://www.bogotobogo.com/Qt/Qt5_QtXML_QDOM_Reading_XML.php
https://www.bogotobogo.com/Qt/Qt5_QtXML_QDomDocument_QDomElement.php
*/
