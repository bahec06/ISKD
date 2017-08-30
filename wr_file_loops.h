#ifndef WR_FILE_LOOPS_H
#define WR_FILE_LOOPS_H
#define BUF_SIZE 16384

#include <random>
#include <QFile>
#include <QDataStream>
#include <QString>
#include "qmath.h"
#include "math.h"
#include "react_constants.h"
#include <QObject>

class wr_file_loops : public QObject
{
    Q_OBJECT
public:
    explicit wr_file_loops(QObject *parent = nullptr);
    double time; //Время теста
    double F0; //Начальная частота (скорость счёта)
    double A; //Скорость изменения сигнала
    double T0; //Начальный период
    double r0; //Начальная реактивность
    double r; //Величина ступеньки реактивности
    double Kt; //Коэффициент периода
    double S; //Мощность источника нейтронов
    double Ktt; //Период изменения периода
    double a_r; //Скорость изменения реактивности

    r_models rmod; //Выбор модели реактора
    react_constants r_const;
    bool test;
    uint64_t rolls;
    double k0, k1, k2, k3, mean_q;

    void gen_rand_const();
    void gen_rand_lin();
    void gen_rand_step(QVector<double> time_array, QVector<double> count_rate_array);
    void gen_rand_exp();
    void gen_rand_exp_var();
    void gen_rand_react();

    void gen_reg_const();
    void gen_reg_lin();
    void gen_reg_step(QVector<double> time_array, QVector<double> count_rate_array);
    void gen_reg_exp();
    void gen_reg_exp_var();
    void gen_reg_react();

    int16_t lin_current(double F);
private:
    double lb;
    double bet;
    double F_p;
    double F_n;

    QFile mFile;
    QFile nFile;
    QDataStream stream;
    QDataStream low_stream;
    std::mt19937 generator;
    int16_t number[BUF_SIZE];
    int16_t *val;
    double dt;
    double F;
    double T;
    double int_F;
    double omeg_n;
    double omeg_p;
    uint64_t i, j, k;
signals:
    loop_index(int);
public slots:
};

#endif // WR_FILE_LOOPS_H
