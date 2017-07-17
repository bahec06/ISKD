#include "global_options.h"
#include "ui_global_options.h"

global_options::global_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::global_options)
{
    ui->setupUi(this);

    read_xml();
}

global_options::~global_options()
{
    delete ui;
}


void global_options::read_xml() {
    QMap<QString, QString> map = conf_xml.read_file(xml_fname, opt_name);

    fpga_name = map[tag_name[0]];
    ui->fpga_res_edit->setText(fpga_name);

    fgen_name = map[tag_name[1]];
    ui->fgen_res_edit->setText(fgen_name);
}

void global_options::on_save_button_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Оповещение", "Закрыть окно настроек после сохранения?",
                                    QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QVector<QString> tag_value = {ui->fpga_res_edit->text(),
                                      ui->fgen_res_edit->text()};

        conf_xml.write_file(xml_fname, opt_name, tag_name, tag_value);
        read_xml();

        this->close();
    }
}
