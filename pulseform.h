#ifndef PULSEFORM_H
#define PULSEFORM_H

#include "stdint.h"
#include <QVector>
#include "service_types.h"

class PulseForm
{
private:
    double dt; //Шаг по времени, с
    double dU; //Шаг по напряжению, В
    double dQ; //Дискретность заряда, Кл
    double k; //Амплитуда импульса в кодах FPGA

    void fpga_compatible_pulse(QVector<double> pulse);
public:
    PulseForm();

    double R;
    double square;
    p_form form_struct;
    QVector<double> t;

    void get_square_pulse(double Tu);
    void get_gauss_pulse(double Tu);
    void get_exp_pulse(double Tu);
    void get_parabolic_pulse(double Tu);
};

#endif // PULSEFORM_H
