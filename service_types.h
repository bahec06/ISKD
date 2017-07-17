#ifndef SERVICE_TYPES_H
#define SERVICE_TYPES_H

#include <QVector>

//Параметры спектра
#define SPEC_SIZE 4096
#define MAX_CHARGE 1E-12
//Параметры формы импульа
#define P_FORM_SIZE 101
//Модели реакторов
enum r_models {
    RBMK = 0,
    VVR = 1,
    BN = 2
};
//Обогащение из ряда : 2.0 2.4 2.6 2.8 % для 24-групповой модели
enum enrichment_enum {
    en2_0 = 0,
    en2_4 = 1,
    en2_6 = 2,
    en2_8 = 3
};
//Энерговыработка из ряда : 0 5 10 15 20 МВт*сут/кг для 24-групповой модели
enum yield_enum {
    yld_0 = 0,
    yld_5 = 1,
    yld_10 = 2,
    yld_15 = 3,
    yld_20 = 4
};
//Соотношение ядер плутония к урана из ряда : 0.000 0.100 0.193 0.402 0.672 1.051 1.604 24.950 для 6-групповой модели
enum pu_u_enum {
    pu_u_0_000 = 0,
    pu_u_0_100 = 1,
    pu_u_0_193 = 2,
    pu_u_0_402 = 3,
    pu_u_0_672 = 4,
    pu_u_1_051 = 5,
    pu_u_1_604 = 6,
    pu_u_24_950 = 7
};
#endif // SERVICE_TYPES_H
