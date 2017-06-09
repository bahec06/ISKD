#ifndef MODEL_FILE_GENERATOR_H
#define MODEL_FILE_GENERATOR_H

#define BUF_SIZE 16384

#include <random>
#include <QFile>
#include <QDataStream>
#include <QString>
#include "qmath.h"
#include "math.h"
#include "react_constants.h"

class model_file_generator
{
public:
    double time; //Время теста
    double F0; //Начальная частота (скорость счёта)
    double A; //Скорость изменения сигнала
    double T0; //Начальный период
    double r0; //Начальная реактивность
    double r1; //Величина ступеньки реактивности
    double K; //Коэффициент периода
    double S; //Мощность источника нейтронов
    model_file_generator();

    void gen_rand_const();
    void gen_rand_lin();
    void gen_rand_exp();
    void gen_rand_exp_var();
    void gen_rand_react();

    void gen_reg_lin();
    void gen_reg_exp();
    void gen_reg_exp_var();
    void gen_reg_react();

private:
    double a_r;
    double r;
    double lb;
    double bet;
    double F_p;
    double F_n;

    QFile mFile;
    QDataStream stream;
    std::mt19937 generator;
    react_constants r_const;
    int16_t number[BUF_SIZE];
    react_mod_enum rmod;
    double dt;
    double F;
    double T;
    double int_F;
    double omeg_n;
    double omeg_p;
    uint64_t i, j, k;
    uint64_t rolls;

    QString filename;
};

#endif // MODEL_FILE_GENERATOR_H
