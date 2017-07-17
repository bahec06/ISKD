#ifndef GLOBAL_OPTIONS_H
#define GLOBAL_OPTIONS_H

#include <QDialog>
#include <QMessageBox>
#include "xml_rw.h"

namespace Ui {
class global_options;
}

class global_options : public QDialog
{
    Q_OBJECT

public:
    explicit global_options(QWidget *parent = 0);
    QString fpga_name;
    QString fgen_name;
    ~global_options();

private slots:
    void on_save_button_clicked();

private:
    Ui::global_options *ui;
    xml_rw conf_xml;
    const QVector<QString> tag_name = {"fpga_name",
                                       "fgen_name"};
    QString xml_fname = "D:/Qt_repo/options.xml";
    QString opt_name = "options";
    void read_xml();
};

#endif // GLOBAL_OPTIONS_H
