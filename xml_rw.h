#ifndef XML_RW_H
#define XML_RW_H

#include <QXmlStreamReader>
#include <QMap>
#include <QFile>

class xml_rw
{
public:
    xml_rw();

    QMap<QString, QString> read_opt_file(QString fname, QString opt_name);
    QMap<QString, QString> read_file(QString fname, QString opt_name);
    void write_file(QString fname, QString opt_name, QVector<QString> tag_name, QVector<QString> tag_value);
};

#endif // XML_RW_H
