#include "form_options.h"
#include "ui_form_options.h"

Form_Options::Form_Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Form_Options)
{
    ui->setupUi(this);
    connect(ui->pulse_form_box, SIGNAL(currentIndexChanged(int)),this,SLOT(replot_form()));
    connect(ui->pulse_duration_edit, SIGNAL(textChanged(QString)),this,SLOT(replot_form()));

    ui->pform_plot->addGraph();
    ui->pform_plot->xAxis->setLabel("t, с");
    ui->pform_plot->yAxis->setLabel("y");
    ui->pform_plot->xAxis->setRange(0, 1e-6);
    ui->pform_plot->yAxis->setRange(0, 1e7);

    xrw = new xml_rw();
    QMap<QString, QString> map = xrw->read_file(fname, opt_name);

    ui->pulse_form_box->setCurrentIndex(map[tag_name[0]].toDouble());
    ui->pulse_duration_edit->setText(map[tag_name[1]]);

    replot_form();
}

Form_Options::~Form_Options()
{
    delete ui;
}

void Form_Options::replot_form() {
    int index = ui->pulse_form_box->currentIndex();
    s_dur = ui->pulse_duration_edit->text();
    s_form = ui->pulse_form_box->itemText(index);
    double dur = s_dur.toDouble();
    switch(index){
        case 0:
            form.get_square_pulse(dur);
            break;
        case 1:
            form.get_gauss_pulse(dur);
            break;
        case 2:
            form.get_exp_pulse(dur);
            break;
        case 3:
            form.get_parabolic_pulse(dur);
            break;
        default:
            break;
    }

    QVector<QString> tag_value = {QString::number(index), QString::number(dur)};
    xrw->write_file(fname, opt_name, tag_name, tag_value);

    ui->pform_plot->yAxis->setRange(0, 1.1*get_peak_amp(form.norm_pulse_form));
    ui->pform_plot->graph(0)->setData(form.t,form.norm_pulse_form);
    ui->pform_plot->replot();

    emit update_form();
}

double Form_Options::get_peak_amp(QVector<double> array) {
    double max = 0;

    for(int i = 0; i < array.size(); i++) {
        if(array[i] > max) max = array[i];
    }

    return max;
}
