#include <QDebug>
#include "controls2_definitions.h"

bool Controls2Definitions::readXml()
{
	bool ok1 = this->xml.openXmlFile(Controls2Definitions::XmlFileName);
	bool ok2 = this->xmlDefault.openXmlFile(Controls2Definitions::XmlFileName);
	bool ok3 = this->xmlFactory.openXmlFile(Controls2Definitions::XmlFactoryFileName);
	return ok1 && ok2 && ok3;
}

bool Controls2Definitions::getArrayNode(QString node, QList<float> &array)
{
	QDomNode n = this->xml.getElementByTagPath(node);
	if (n.isNull())
		return false;

	// qDebug() << "result" << n.nodeName() << n.toElement().attribute("size") << IspXml::stripText(n.toElement().text());
	// qDebug() << IspXml::stripText(n.toElement().text());

	if (!IspXml::parseArrayFloat(n.toElement().text(), array))
		return false;

	return true;
}

bool Controls2Definitions::getFactoryArrayNode(QString node, QList<float> &array)
{
	QDomNode n = this->xmlFactory.getElementByTagPath(node);
	if (n.isNull())
		return false;

	if (!IspXml::parseArrayFloat(n.toElement().text(), array))
		return false;

	return true;
}

bool Controls2Definitions::setArrayNode(QString node, QList<float> array)
{
	QDomNode n = this->xml.getElementByTagPath(node);
	if (n.isNull())
		return false;

	QString s = IspXml::arrayFloatToString(array);
	IspXml::setValue(n, s, node);
	return true;
}

bool Controls2Definitions::setArrayNode(QString node, QList<QPointF> array)
{
	QDomNode n = this->xml.getElementByTagPath(node);
	if (n.isNull())
		return false;

	QString s = IspXml::arrayFloatToString(array);
	IspXml::setValue(n, s, node);
	return true;
}

bool Controls2Definitions::saveXml()
{
	// QDomNode n = xml.getElementByTagPath("header/creator");
	// if (!n.isNull())
	// {
	// 	qDebug() << "result" << n.nodeName() << n.toElement().attribute("type") << IspXml::stripText(n.toElement().text());
	// 	IspXml::setValue(n, "jsjs aa", "header/creator");
	// }

	return xml.saveXmlFile(Controls2Definitions::XmlFileName);
}
