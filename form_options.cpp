#include "form_options.h"
#include "ui_form_options.h"

Form_Options::Form_Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Form_Options)
{
    ui->setupUi(this);
    connect(ui->pulse_form_box, SIGNAL(currentIndexChanged(int)),this,SLOT(replot_form()));
    connect(ui->pulse_duration_ledit, SIGNAL(textChanged(QString)),this,SLOT(replot_form()));

    ui->pform_plot->addGraph();
    ui->pform_plot->xAxis->setLabel("t, с");
    ui->pform_plot->yAxis->setLabel("y");
    ui->pform_plot->xAxis->setRange(0, 1e-6);
    ui->pform_plot->yAxis->setRange(0, 1e7);

    ui->pulse_duration_ledit->setText(QString::number(2e-7));
    ui->pulse_form_box->setCurrentIndex(1);
}

Form_Options::~Form_Options()
{
    delete ui;
}

void Form_Options::replot_form() {
    int index = ui->pulse_form_box->currentIndex();
    QString s_dur = ui->pulse_duration_ledit->text();
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

    ui->pform_plot->yAxis->setRange(0, 1.1*get_peak_amp(form.form_struct.norm_pulse_form));
    ui->pform_plot->graph(0)->setData(form.t,form.form_struct.norm_pulse_form);
    ui->pform_plot->replot();
}

double Form_Options::get_peak_amp(QVector<double> array) {
    double max = 0;

    for(int i = 0; i < array.size(); i++) {
        if(array[i] > max) max = array[i];
    }

    return max;
}
