#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->model_param_table,SIGNAL(cellChanged(int,int)),this,SLOT(change_model_param()));
    connect(ui->time_model_box, SIGNAL(currentIndexChanged(int)),this,SLOT(change_model()));
    connect(ui->time_mode_box, SIGNAL(currentIndexChanged(int)),this,SLOT(change_model()));

    F0 = 1e6;
    A = 1e5;
    T0 = 20;
    K = 0.5;
    r = -1;
    z = 10;
    x = 2;

    model_item0 = new QTableWidgetItem("");
    model_item1 = new QTableWidgetItem("");
    model_item2 = new QTableWidgetItem("");
    model_item3 = new QTableWidgetItem("");
    model_vh_item0 = new QTableWidgetItem("");
    model_vh_item1 = new QTableWidgetItem("");
    model_vh_item2 = new QTableWidgetItem("");
    model_vh_item3 = new QTableWidgetItem("");
    //TODO
    //Сделать отдельно виджеты
    qcb_widget = new QComboBox();

    qcb_widget->addItem("РБМК");
    qcb_widget->addItem("БН");
    qcb_widget->addItem("ВВЭР");
    qcb_widget->addItem("Произвольная");

    ui->model_param_table->blockSignals(true);
    ui->model_param_table->setVerticalHeaderItem(0, model_vh_item0);
    ui->model_param_table->setVerticalHeaderItem(1, model_vh_item1);
    ui->model_param_table->setVerticalHeaderItem(2, model_vh_item2);
    ui->model_param_table->setVerticalHeaderItem(3, model_vh_item3);
    ui->model_param_table->setItem(0, 0, model_item0);
    ui->model_param_table->setItem(1, 0, model_item1);
    ui->model_param_table->setItem(2, 0, model_item2);
    ui->model_param_table->setItem(3, 0, model_item3);
    ui->model_param_table->blockSignals(false);

    ui->time_model_box->setCurrentIndex(1);

    thread = new QThread;
    bgif = new bgif_generator;

    bgif->moveToThread(thread);
    connect(thread, SIGNAL(started()), bgif, SLOT(start_generation())); //Запуск потока thread вызывает запуск генерации
    connect(bgif, SIGNAL(generation_finished()), thread, SLOT(terminate())); //По окончании генерации поток thread останавливается
    connect(bgif, SIGNAL(send_frequency(uint64_t)),this,SLOT(update_freq(uint64_t))); //Обновление текущего значения частоты на индикаторе по сигналу send_frequency
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_model() {
    ui->model_param_table->blockSignals(true);
    if(ui->time_mode_box->currentIndex() == 0) {
        str_F = reg_F;
        str_F0 = reg_F0;
        str_A = reg_AF;
    }
    else {
        str_F = reg_C;
        str_F0 = reg_C0;
        str_A = reg_AC;
    }
    switch(ui->time_model_box->currentIndex()){
        case 0:
            //ui->model_param_table->setCellWidget(0,0, Q);
            model_vh_item0->setText(str_F);
            model_vh_item1->setText("");
            model_vh_item2->setText("");
            model_vh_item3->setText("");
            model_item0->setText(QString::number(F0));
            model_item1->setText("");
            model_item2->setText("");
            model_item3->setText("");
            break;
        case 1:
            model_vh_item0->setText(str_F0);
            model_vh_item1->setText(str_A);
            model_vh_item2->setText("");
            model_vh_item3->setText("");
            model_item0->setText(QString::number(F0));
            model_item1->setText(QString::number(A));
            model_item2->setText("");
            model_item3->setText("");
            break;
        case 3:
            model_vh_item0->setText(str_F0);
            model_vh_item1->setText("Период, с");
            model_vh_item2->setText("");
            model_vh_item3->setText("");
            model_item0->setText(QString::number(F0));
            model_item1->setText(QString::number(T0));
            model_item2->setText("");
            model_item3->setText("");
            break;
        case 4:
            model_vh_item0->setText(str_F0);
            model_vh_item1->setText("Начальный период, с");
            model_vh_item2->setText("Коэффициент периода");
            model_vh_item3->setText("");
            model_item0->setText(QString::number(F0));
            model_item1->setText(QString::number(T0));
            model_item2->setText(QString::number(K));
            model_item3->setText("");
            break;
        case 5:
            model_vh_item0->setText(str_F0);
            model_vh_item1->setText("Реактивность, бета");
            model_vh_item2->setText("Энерговыработка, МВт*сут/кг");
            model_vh_item3->setText("Обогащение, %");
            model_item0->setText(QString::number(F0));
            model_item1->setText(QString::number(r));
            model_item2->setText(QString::number(z));
            model_item3->setText(QString::number(x));
            ui->model_param_table->setCellWidget(0,0, qcb_widget);
            break;
        default:
            break;
    }
    ui->model_param_table->blockSignals(false);
    emit change_model_param();
}

void MainWindow::change_model_param() {
    model_index = ui->time_model_box->currentIndex();

    switch(model_index){
        case 0:
            F0 = model_item0->text().toDouble();
            break;
        case 1:
            F0 = model_item0->text().toDouble();
            A = model_item1->text().toDouble();
            break;
        case 3:
            F0 = model_item0->text().toDouble();
            T0 = model_item1->text().toDouble();
            break;
        case 4:
            F0 = model_item0->text().toDouble();
            T0 = model_item1->text().toDouble();
            K = model_item2->text().toDouble();
            break;
        case 5:
            F0 = model_item0->text().toDouble();
            r = model_item1->text().toDouble();
            z = model_item2->text().toDouble();
            x = model_item3->text().toDouble();
            break;
        default:
            break;
    }
}

void MainWindow::on_options_triggered()
{
    g_opt.show();
}

void MainWindow::on_form_conf_triggered()
{
    f_opt.show();
}

void MainWindow::on_spec_conf_triggered()
{
    s_opt.show();
}

void MainWindow::on_start_gen_file_but_clicked() //В отдельный поток
{
    model_index = ui->time_model_box->currentIndex();
    mode_index = ui->time_mode_box->currentIndex();

    file_gen.F0 = F0;
    file_gen.A = A;
    file_gen.T0 = T0;
    file_gen.r0 = r;
//    file_gen.energy_yield = z;
//    file_gen.enrichment = x;
//    file_gen.K = K;

    if(mode_index == 0) {
        switch (model_index) {
        case 0:
            break;
        case 1:
            file_gen.gen_reg_lin();
            break;
        case 3:
            file_gen.gen_reg_exp();
            break;
        case 4:
            file_gen.gen_reg_exp_var();
            break;
        case 5:
            //file_gen.gen_reg_react();
            break;
        default:
            break;
        }
    }
    else {
        switch (model_index) {
        case 0:
            break;
        case 1:
            file_gen.gen_rand_lin();
            break;
        case 3:
            file_gen.gen_rand_exp();
            break;
        case 4:
            file_gen.gen_rand_exp_var();
            break;
        case 5:
            //file_gen.gen_rand_react();
            break;
        default:
            break;
        }
    }
}

void MainWindow::update_freq(uint64_t freq) {
    ui->freq_line->setText(QString::number(double(freq)*pow(2,-16)));
}

void MainWindow::on_start_generation_clicked()
{
    thread->start();
}

void MainWindow::on_stop_generation_clicked()
{
    bgif->stop_indicator = true;
    bgif->stop_generation();
}
