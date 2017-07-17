#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "stdint.h"
#include "qmath.h"
#include <QVector>
#include "service_types.h"

class Spectrum
{
private:

    void clear_array();
public:
    Spectrum();

    QVector<double> spectrum_array; //Массив со спектром единичной площади
    uint16_t reverse_array[SPEC_SIZE]; //Массив со спектром (обратный), загружаемый в FPGA
    QVector<double> q; //Массив зарядов
    double dQ; //Дискретность заряда
    void get_uni_spectrum(double qmin, double qmax); //Равномерный спектр
    void get_gauss_spectrum(double qmean, double sigma); //Гауссовский спектр
    void get_lin_spectrum(double qmin, double qmax, int n_points); //Линейчатый спектр
    void get_exp_spectrum(double qmean); //Экспоненциальный спектр
    void get_const_spectrum(double q); //Постоянный заряд
    void get_int_spectrum(QVector<double> array); //Получение обратного спектра через прямой
    double get_max(QVector<double> array); //Получение максимального значения массива
    double get_square(); //Получение площади массива <spectrum_array>
    double get_mean_charge(); //Получение среднего значения заряда в спектре
};

#endif // SPECTRUM_H
