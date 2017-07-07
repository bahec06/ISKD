#ifndef REACT_CONSTANTS_H
#define REACT_CONSTANTS_H

#include <service_types.h>
#include <qmath.h>

class react_constants
{
public:
    react_constants();

    double dt; //Шаг по времени
    double N; //Число групп ядер-эмиттеров запаздывающих нейтронов

    //**************24-групповая модель РБМК*********************//
    //Константы
    const double lam_rbmk[24] = {0.0124, 0.0305, 0.111, 0.301, 1.14, 3.01,
                     0.0132, 0.0321, 0.139, 0.358, 1.41, 4.02,
                     0.0129, 0.0311, 0.134, 0.331, 1.26, 3.21,
                     0.0128, 0.0299, 0.124, 0.352, 1.61, 3.5};
    const double bi[4] = {0.00682, 0.0157, 0.00219, 0.00521};
    const double bb1[6] = {0.033, 0.219, 0.196, 0.395, 0.115, 0.042};
    const double bb2[6] = {0.013, 0.137, 0.162, 0.388, 0.225, 0.075};
    const double bb3[6] = {0.038, 0.280, 0.216, 0.328, 0.103, 0.035};
    const double bb4[6] = {0.010, 0.229, 0.173, 0.390, 0.182, 0.016};
    const double l_rbmk = 0.001;
    //Переменные
    double bb_rbmk[24];
    double bet_rbmk;
    yield_enum energy_yield; //Энерговыработка для 24-групповой модели (0 5 10 15 20 МВт*сут/кг)
    enrichment_enum enrichment; //Обогащение для 24-групповой модели (2.0 2.4 2.6 2.8 %)

    //**************8-групповая модель БН*********************//
    //Константы
    const double lam_bn[24] = {0.0125, 0.0283, 0.0425, 0.1330, 0.2925, 0.6665, 1.6350, 3.5550, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_bn[24] = {0.0252458, 0.1442657, 0.0743784, 0.1781303, 0.3186853, 0.1211844, 0.0947100, 0.0434001, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double l_bn = 0.00001;
    const double bet_bn = 0.0065;

    //**************6-групповая модель ВВЭР*********************//
    //Константы
    const double lam_bwr[24] = {0.0124, 0.0305, 0.111, 0.301, 1.14, 3.01, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bet_0_000 = 7.3854e-3;
    const double bet_0_100 = 6.6300e-3;
    const double bet_0_193 = 5.9311e-3;
    const double bet_0_402 = 5.1985e-3;
    const double bet_0_672 = 4.7923e-3;
    const double bet_1_051 = 4.4873e-3;
    const double bet_1_604 = 4.3023e-3;
    const double bet_24_950 = 3.8345e-3;
    const double bb_0_000[24] = {2.8868e-2, 2.0446e-1, 1.8834e-1, 3.9524e-1, 1.3516e-1, 4.7932e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_0_100[24] = {2.8355e-2, 2.0664e-1, 1.8703e-1, 3.9216e-1, 1.3725e-1, 4.8565e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_0_193[24] = {2.7362e-2, 2.0688e-1, 1.8563e-1, 3.8273e-1, 1.4854e-1, 4.8858e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_0_402[24] = {2.6158e-2, 2.1160e-1, 1.8652e-1, 3.8300e-1, 1.4300e-1, 4.9722e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_0_672[24] = {2.4520e-2, 2.1305e-1, 1.8488e-1, 3.7998e-1, 1.4803e-1, 4.9540e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_1_051[24] = {2.3086e-2, 2.1483e-1, 1.8414e-1, 3.7952e-1, 1.4902e-1, 4.9404e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_1_604[24] = {2.1640e-2, 2.1430e-1, 1.8179e-1, 3.7585e-1, 1.5761e-1, 4.8810e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double bb_24_950[24] = {2.1124e-2, 2.2684e-1, 1.8339e-1, 3.6537e-1, 1.5584e-1, 4.7436e-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    const double l_bwr = 0.001;
    //Переменные
    pu_u_enum pu_u; //Соотношение ядер плутония к урану для 6-групповой модели

    //Глобальные переменные
    double lam[24];
    double bb[24];
    double bet;
    double d[4];
    double e[24], a[24], b[24], x[24];
    double sum_xe, sum_a, sum_b;
    double l;
    int i,j,k;

    void init_react_consts_rbmk(double initial_F);
    void init_react_consts_bwr(double initial_F);
    void init_react_consts_bn(double initial_F);
    void init_react_consts(r_models rmod, double initial_F);
    void preinit_react_consts(double initial_F);
    void react_d_rbmk(yield_enum z, enrichment_enum x);
};

#endif // REACT_CONSTANTS_H
