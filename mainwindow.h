#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "form_options.h"
#include "spec_options.h"
#include "global_options.h"
#include "model_file_generator.h"
#include <QThread>
#include "bgif_generator.h"
#include <QComboBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    Form_Options f_opt;
    spec_options s_opt;
    global_options g_opt;
    model_file_generator file_gen;

    QTableWidgetItem *model_item0;
    QTableWidgetItem *model_item1;
    QTableWidgetItem *model_item2;
    QTableWidgetItem *model_item3;
    QTableWidgetItem *model_vh_item0;
    QTableWidgetItem *model_vh_item1;
    QTableWidgetItem *model_vh_item2;
    QTableWidgetItem *model_vh_item3;
    QComboBox *qcb_widget;

    const QString reg_F = "Частота, Гц";
    const QString reg_F0 = "Начальная частота, Гц";
    const QString reg_AF = "Скорость изменения частоты, Гц/с";
    const QString reg_C = "Скорость счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B9");
    const QString reg_C0 = "Начальная cкорость счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B9");
    const QString reg_AC = "Скорость изменения скорости счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B2");

    QString str_F;
    QString str_F0;
    QString str_A;

    double F0; //Начальная скорость счёта, либо постоянная скорость счёта
    double A; //Скорость изменения параметра для линейно изменяющихся сигналов
    double T0; //Начальный период, либо постоянный период для экспоненциально изменяющихся сигналов
    double K; //Коэффициент периода для экспоненциальных сигналов с линейно изменяющимся периодом
    double z; //Энерговыработка для 24-групповой модели
    double x; //Обогащение для 24-групповой модели
    double r; //Величина ступеньки реактивности
    double r0; //Начальная подкритичность
    double pu_u; //Соотношения ядер Pu к U в ядерном топливе для 6-групповой модели
    double cfreq;

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int model_index;
    int mode_index;
    QThread *thread;
    bgif_generator *bgif;

private slots:
     void change_model();
     void change_model_param();
     void on_options_triggered();
     void on_form_conf_triggered();
     void on_spec_conf_triggered();
     void on_start_gen_file_but_clicked();
     void update_freq(uint64_t);
     void on_start_generation_clicked();
     void on_stop_generation_clicked();
};

#endif // MAINWINDOW_H
