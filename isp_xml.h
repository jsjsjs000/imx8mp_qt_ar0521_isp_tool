#ifndef ISP_XML_H
#define ISP_XML_H

#include <QString>
#include <QDomDocument>

class IspXml
{
	QDomNode retrievElements(QDomNode root, QString tagPath);
public:
	IspXml();
	bool openXmlFile(QString filename);
};

#endif // ISP_XML_H
