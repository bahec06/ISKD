#include "xml_rw.h"

xml_rw::xml_rw()
{

}

QMap<QString, QString> xml_rw::read_opt_file(QString fname, QString opt_name) {
    QMap<QString, QString> map;
    QFile* file = new QFile(fname);
    file->open(QIODevice::ReadOnly);
    QXmlStreamReader xml_strm(file);
    bool found = false;

    while(!xml_strm.atEnd()) {
        QXmlStreamReader::TokenType token = xml_strm.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if ((xml_strm.name() == opt_name) && (found == false)) {
                found = true;
                continue;
            }
            if(!found) {
                continue;
            }
            QString a = xml_strm.name().toString();
            xml_strm.readNext();
            QString b = xml_strm.text().toString();
            map.insert(a, b);
        }
        else if((token == QXmlStreamReader::EndElement) && (xml_strm.name() == opt_name)) {
            break;
        }
    }
    file->close();

    return map;
}

QMap<QString, QString> xml_rw::read_file(QString fname, QString opt_name) {
    QMap<QString, QString> map;
    QFile* file = new QFile(fname);
    file->open(QIODevice::ReadOnly);
    QXmlStreamReader xml_strm(file);

    while(!xml_strm.atEnd()) {
        QXmlStreamReader::TokenType token = xml_strm.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if (xml_strm.name() == opt_name) continue;
            QString a = xml_strm.name().toString();
            xml_strm.readNext();
            QString b = xml_strm.text().toString();
            map.insert(a, b);
        }
    }
    file->close();

    return map;
}

void xml_rw::write_file(QString fname, QString opt_name, QVector<QString> tag_name, QVector<QString> tag_value) {
    QFile* file = new QFile(fname);
    file->open(QIODevice::WriteOnly);
    QXmlStreamWriter xml_strm(file);

    xml_strm.setAutoFormatting(true);
    xml_strm.writeStartDocument();
    xml_strm.writeStartElement(opt_name);
    for(int i = 0; i < tag_name.size(); i ++) {
        xml_strm.writeTextElement(tag_name[i], tag_value[i]);
    }
    xml_strm.writeEndElement();
    xml_strm.writeEndDocument();
    file->close();
}
