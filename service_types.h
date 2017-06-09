#ifndef SERVICE_TYPES_H
#define SERVICE_TYPES_H

#include <QVector>

//Параметры спектра
#define SPEC_SIZE 4096
#define MAX_CHARGE 1E-12
//Параметры формы импульа
#define P_FORM_SIZE 101
struct p_form {
    uint16_t pulse_form[SPEC_SIZE];
    QVector<double> norm_pulse_form;
    double pulse_coeff = 0;
};
//Модели реакторов
enum react_mod_enum {
    rbmk,
    bwr,
    bn,
    randm
};
//Обогащение из ряда : 2.0 2.4 2.6 2.8 % для 24-групповой модели
enum enrichment_enum {
    en2_0,
    en2_4,
    en2_6,
    en2_8
};
//Энерговыработка из ряда : 0 5 10 15 20 МВт*сут/кг для 24-групповой модели
enum yield_enum {
    yld_0,
    yld_5,
    yld_10,
    yld_15,
    yld_20
};
//Соотношение ядер плутония к урана из ряда : 0.000 0.100 0.193 0.402 0.672 1.051 1.604 24.950 для 6-групповой модели
enum pu_u_enum {
    pu_u_0_000,
    pu_u_0_100,
    pu_u_0_193,
    pu_u_0_402,
    pu_u_0_672,
    pu_u_1_051,
    pu_u_1_604,
    pu_u_24_950
};
#endif // SERVICE_TYPES_H
