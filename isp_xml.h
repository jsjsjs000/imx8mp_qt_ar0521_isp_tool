#ifndef ISP_XML_H
#define ISP_XML_H

#include <QString>
#include <QDomDocument>

class IspXml
{
	QDomDocument document;
	QDomNode getElementByTagPath_(QDomNode root, QString tagPath);
public:
	IspXml();
	bool openXmlFile(QString filename);
	bool saveXmlFile(QString filename);
	QDomNode getElementByTagPath(QString tagPath);
	static QString stripText(QString text);
	static void setValue(QDomNode n, QString text, QString forPath);
	static bool parseArrayFloat(QString text, QList<float> &array);
	static QString arrayFloatToString(QList<float> array);
};

#endif // ISP_XML_H
