#include <QDebug>
#include "controls2_definitions.h"

void Controls2Definitions::readXml()
{
	const QString XmlFileName = "/opt/imx8-isp/bin/xml/VM-017-COL_AO062-C_1920x1080.xml";
	this->xml.openXmlFile(XmlFileName); // $$ if
}

bool Controls2Definitions::getArrayNode(QString node, QList<float> &array)
{
	QDomNode n = this->xml.getElementByTagPath(node);
	if (n.isNull())
		return false;

	// qDebug() << "result" << n.nodeName() << n.toElement().attribute("size") << IspXml::stripText(n.toElement().text());
	// qDebug() << IspXml::stripText(n.toElement().text());
	// QList<float> array;
	if (!IspXml::parseArrayFloat(n.toElement().text(), array))
		return false;

	// for (const float &f : qAsConst(array))
		// qDebug() << f;

	// array.clear();
	// for (int i = 0; i < 16; i++)
	// 	array.push_back(1.23f);
	// QString s = IspXml::arrayFloatToString(array);
	// qDebug() << s;

	return true;

	// QDomNode n = xml.getElementByTagPath("header/creator");
	// if (!n.isNull())
	// {
	// 	qDebug() << "result" << n.nodeName() << n.toElement().attribute("type") << IspXml::stripText(n.toElement().text());
	// 	IspXml::setValue(n, "jsjs aa", "header/creator");
	// }

	// xml.saveXmlFile(XmlFileName); // $$ if
}
