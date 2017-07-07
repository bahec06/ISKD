#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    F0 = 1e6;
    A = 1e5;
    T0 = 20;
    K = 0.5;
    r = -1;
    z = 10;
    x = 2;

    w_params = new wdg_params();
    w_params->t_widget = ui->model_param_table;

    connect(w_params, SIGNAL(update_table()), w_params, SLOT(slot_update()));
    connect(ui->time_mode_box, SIGNAL(currentIndexChanged(int)), w_params, SLOT(pulse_dist_change(int)));
    connect(ui->time_model_box, SIGNAL(currentIndexChanged(int)), w_params, SLOT(st_dist_change(int)));
    connect(w_params->t_widget, SIGNAL(cellChanged(int,int)), w_params, SLOT(read_params_from_table()));

    ui->time_model_box->setCurrentIndex(1);

    thread = new QThread;
    bgif = new bgif_generator;
    ui->file_gen_bar->setValue(0);

    bgif->moveToThread(thread);
    connect(thread, SIGNAL(started()), bgif, SLOT(start_generation())); //Запуск потока thread вызывает запуск генерации
    connect(bgif, SIGNAL(generation_finished()), thread, SLOT(terminate())); //По окончании генерации поток thread останавливается
    connect(bgif, SIGNAL(send_frequency(uint64_t)),this,SLOT(update_freq(uint64_t))); //Обновление текущего значения частоты на индикаторе по сигналу send_frequency
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_start_gen_file_but_clicked()
{
    f_thread = new file_gen_thread();

    f_thread->S = w_params->S;
    f_thread->A = w_params->A;
    f_thread->T = w_params->T;
    f_thread->R0 = w_params->R0;
    f_thread->R = w_params->R;
    f_thread->TT = w_params->TT;
    f_thread->Kt = w_params->Kt;
    f_thread->r_mod = w_params->r_mod;
    f_thread->En_Y = w_params->En_Y;
    f_thread->Enrch = w_params->Enrch;
    f_thread->Pu_U = w_params->Pu_U;
    f_thread->A_R = w_params->R/w_params->Rt;

    f_thread->model_index = ui->time_model_box->currentIndex();
    f_thread->mode_index = ui->time_mode_box->currentIndex();

    QThread* thread = new QThread();
    f_thread->moveToThread(thread);
    connect(f_thread, SIGNAL(send_bar(int)), this, SLOT(update_bar(int)));
    connect(thread, SIGNAL(started()), f_thread, SLOT(process()));
    connect(f_thread, SIGNAL(finished()), this, SLOT(wr_file_done()));
    connect(f_thread, SIGNAL(finished()), thread, SLOT(quit()));
    connect(f_thread, SIGNAL(finished()), f_thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}

void MainWindow::update_freq(uint64_t freq) {
    ui->freq_line->setText(QString::number(double(freq)*pow(2,-16)));
}

void MainWindow::update_bar(int a) {
    ui->file_gen_bar->setValue(a);
}

void MainWindow::wr_file_done() {
    ui->file_gen_bar->setValue(100);
    QMessageBox *box = new QMessageBox();
    box->setWindowTitle("Сообщение");
    box->setText("Запись файла завершена");
    box->exec();
    ui->file_gen_bar->setValue(0);
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
