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
    w_params->read_params_from_file();

    emit w_params->update_table();

    connect(ui->time_mode_box, SIGNAL(currentIndexChanged(int)), w_params, SLOT(pulse_dist_change(int)));
    connect(ui->time_model_box, SIGNAL(currentIndexChanged(int)), w_params, SLOT(st_dist_change(int)));
    connect(w_params->t_widget, SIGNAL(cellChanged(int,int)), w_params, SLOT(read_params_from_table()));

    ui->time_model_box->setCurrentIndex(w_params->st_dist);

    connect(this, SIGNAL(destroyed(QObject*)), w_params, SLOT(read_params_from_table()));
    ui->file_gen_bar->setValue(0);

    connect(&f_opt, SIGNAL(update_form()), this, SLOT(update_param_table()));
    connect(&s_opt, SIGNAL(update_spectrum()), this, SLOT(update_param_table()));
    bgif = new bgif_generator;
    update_param_table();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
    case Qt::Key_Delete:
    if(w_params->st_dist == STEP) {
        if(w_params->t_widget->currentRow() != w_params->t_widget->rowCount()-1) {
            w_params->t_widget->removeRow(w_params->t_widget->currentRow());
            w_params->cnt--;
        }
    }
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

void MainWindow::update_freq(quint64 freq) {
    ui->freq_line->setText(QString::number(double(freq)*qPow(2, -16)));
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
    QThread* thread = new QThread;
    bgif = new bgif_generator;

    strcpy(bgif->fpga.ni_fpga_resource, g_opt.fpga_name.toUtf8());
    strcpy(bgif->fgen.ni_fgen_resource, g_opt.fgen_name.toUtf8());

    bgif->correction_gain = f_opt.form.pulse_coeff;
    bgif->mean_charge = f_opt.form.R*s_opt.mean_charge*s_opt.spec.get_max(f_opt.form.norm_pulse_form);
    if(ui->time_mode_box->currentIndex() == 1) {
        bgif->mode = 1;
    }
    else {
        if(ui->time_model_box->currentIndex() != 0) {
            bgif->mode = 2;
        }
        else {
            bgif->regular_freq = w_params->S;
            bgif->mode = 0;
        }
    }
    bgif->pulse_form = f_opt.form.pulse_form;
    bgif->correction_gain = f_opt.form.pulse_coeff;
    bgif->spectrum = s_opt.spec.reverse_array;

    bgif->moveToThread(thread);
    connect(bgif, SIGNAL(update_play(bool)), this, SLOT(update_play_label(bool)));
    connect(bgif, SIGNAL(fpga_error()), this, SLOT(fpga_error_msg()));
    connect(bgif, SIGNAL(fgen_error()), this, SLOT(fgen_error_msg()));
    connect(thread, SIGNAL(started()), bgif, SLOT(start_generation())); //Запуск потока thread вызывает запуск генерации
    connect(bgif, SIGNAL(generation_finished()), thread, SLOT(terminate())); //По окончании генерации поток thread останавливается
    connect(bgif, SIGNAL(send_frequency(quint64)),this,SLOT(update_freq(quint64))); //Обновление текущего значения частоты на индикаторе по сигналу send_frequency

    thread->start();
}

void MainWindow::on_stop_generation_clicked()
{
    bgif->stop_indicator = true;
}

void MainWindow::update_param_table() {
    ui->param_table->setItem(0, 0, new QTableWidgetItem(f_opt.s_form));
    ui->param_table->setItem(1, 0, new QTableWidgetItem(f_opt.s_dur));
    ui->param_table->setItem(2, 0, new QTableWidgetItem(s_opt.s_spec));
    ui->param_table->setItem(3, 0, new QTableWidgetItem(QString::number(s_opt.mean_charge)));
    ui->param_table->setItem(4, 0, new QTableWidgetItem(QString::number(s_opt.mean_charge*s_opt.spec.get_max(f_opt.form.norm_pulse_form))));
    ui->param_table->setItem(5, 0, new QTableWidgetItem(QString::number(f_opt.form.R*s_opt.mean_charge*s_opt.spec.get_max(f_opt.form.norm_pulse_form))));
    bgif->pulse_form = f_opt.form.pulse_form;
    bgif->correction_gain = f_opt.form.pulse_coeff;
    bgif->spectrum = s_opt.spec.reverse_array;
}

void MainWindow::update_play_label(bool stat) {
    QString txt;

    if(stat) {
        txt = "PLAY";
    }
    else {
        txt = "IDLE";
    }
    ui->play_label->setText(txt);
}

void MainWindow::fpga_error_msg() {
    QMessageBox box;
    box.setText(QString::number(bgif->fpga.fpga_stat));
    box.exec();
}

void MainWindow::fgen_error_msg() {
    QMessageBox box;
    box.setText(QString::number(bgif->fgen.error));
    box.exec();
}
