#include "spec_options.h"
#include "ui_spec_options.h"

spec_options::spec_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::spec_options)
{
    ui->setupUi(this);
    connect(ui->spectrum_box, SIGNAL(currentIndexChanged(int)),this,SLOT(change_spec(int)));
    connect(ui->charge_param_table,SIGNAL(cellChanged(int,int)),this,SLOT(replot_spec()));
    spec_item0 = new QTableWidgetItem("");
    spec_item1 = new QTableWidgetItem("");
    spec_item2 = new QTableWidgetItem("");
    spec_vh_item0 = new QTableWidgetItem("");
    spec_vh_item1 = new QTableWidgetItem("");
    spec_vh_item2 = new QTableWidgetItem("");

    ui->spec_plot->addGraph();
    ui->spec_plot->xAxis->setLabel("q, Кл");
    ui->spec_plot->yAxis->setLabel("n");
    ui->spec_plot->xAxis->setRange(0, 1e-12);
    ui->spec_plot->yAxis->setRange(0, 1e13);

    q_const = 2E-13;
    q_mean = 5E-13;
    q_min = 1E-13;
    q_max = 5E-13;
    q_sigma = 5E-14;
    l = 6;

    ui->charge_param_table->blockSignals(true);
    ui->charge_param_table->setVerticalHeaderItem(0, spec_vh_item0);
    ui->charge_param_table->setVerticalHeaderItem(1, spec_vh_item1);
    ui->charge_param_table->setVerticalHeaderItem(2, spec_vh_item2);
    ui->charge_param_table->setItem(0, 0, spec_item0);
    ui->charge_param_table->setItem(1, 0, spec_item1);
    ui->charge_param_table->setItem(2, 0, spec_item2);
    ui->charge_param_table->blockSignals(false);

    ui->spectrum_box->setCurrentIndex(1);
}

spec_options::~spec_options()
{
    delete ui;
}

void spec_options::change_spec(int index) {
    ui->charge_param_table->blockSignals(true);
    switch(index){
        case 0:
            spec_vh_item0->setText("Заряд, Кл");
            spec_vh_item1->setText("");
            spec_vh_item2->setText("");
            spec_item0->setText(QString::number(q_const));
            spec_item1->setText("");
            spec_item2->setText("");
            break;
        case 1:
            spec_vh_item0->setText("Начальный заряд, Кл");
            spec_vh_item1->setText("Конечный заряд, Кл");
            spec_vh_item2->setText("");
            spec_item0->setText(QString::number(q_min));
            spec_item1->setText(QString::number(q_max));
            spec_item2->setText("");
            break;
        case 2:
            spec_vh_item0->setText("Средний заряд, Кл");
            spec_vh_item1->setText("СКО, Кл");
            spec_vh_item2->setText("");
            spec_item0->setText(QString::number(q_mean));
            spec_item1->setText(QString::number(q_sigma));
            spec_item2->setText("");
            break;
        case 3:
            spec_vh_item0->setText("Начальный заряд, Кл");
            spec_vh_item1->setText("Конечный заряд, Кл");
            spec_vh_item2->setText("Количество пиков");
            spec_item0->setText(QString::number(q_min));
            spec_item1->setText(QString::number(q_max));
            spec_item2->setText(QString::number(l));
            break;
        case 4:
            spec_vh_item0->setText("Средний заряд, Кл");
            spec_vh_item1->setText("");
            spec_vh_item2->setText("");
            spec_item0->setText(QString::number(q_mean));
            spec_item1->setText("");
            spec_item2->setText("");
            break;
        default:
            break;
    }
    ui->charge_param_table->blockSignals(false);
    ui->spectrum_square->setText(QString::number(spec.get_square()));
    emit replot_spec();
}

void spec_options::replot_spec() {
    int index = ui->spectrum_box->currentIndex();

    switch(index){
        case 0:
            spec_item0 = ui->charge_param_table->item(0,0);
            q_const = spec_item0->text().toDouble();
            spec.get_const_spectrum(q_const);
            break;
        case 1:
            spec_item0 = ui->charge_param_table->item(0,0);
            spec_item1 = ui->charge_param_table->item(1,0);
            q_min = spec_item0->text().toDouble();
            q_max = spec_item1->text().toDouble();
            spec.get_uni_spectrum(q_min,q_max);
            break;
        case 2:
            spec_item0 = ui->charge_param_table->item(0,0);
            spec_item1 = ui->charge_param_table->item(1,0);
            q_mean = spec_item0->text().toDouble();
            q_sigma = spec_item1->text().toDouble();
            spec.get_gauss_spectrum(q_mean,q_sigma);
            break;
        case 3:
            spec_item0 = ui->charge_param_table->item(0,0);
            spec_item1 = ui->charge_param_table->item(1,0);
            spec_item2 = ui->charge_param_table->item(2,0);
            q_min = spec_item0->text().toDouble();
            q_max = spec_item1->text().toDouble();
            l = spec_item2->text().toDouble();
            spec.get_lin_spectrum(q_min,q_max,l);
            break;
        case 4:
            spec_item0 = ui->charge_param_table->item(0,0);
            q_mean = spec_item0->text().toDouble();
            spec.get_exp_spectrum(q_mean);
            break;
        default:
            break;
    }

    ui->spec_plot->yAxis->setRange(0, 1.1*get_peak_amp(spec.spectrum_array));
    ui->spec_plot->graph(0)->setData(spec.q,spec.spectrum_array);
    ui->spec_plot->replot();
    ui->spectrum_square->setText(QString::number(spec.get_square()));
}

double spec_options::get_peak_amp(QVector<double> array) {
    double max = 0;

    for(int i = 0; i < array.size(); i++) {
        if(array[i] > max) max = array[i];
    }

    return max;
}
