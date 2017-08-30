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
    uint16_t pulse_form[P_FORM_SIZE];
    double pulse_coeff = 0;
    QVector<double> norm_pulse_form;
    QVector<double> t;

    void get_square_pulse(double Tu);
    void get_gauss_pulse(double Tu);
    void get_exp_pulse(double Tu);
    void get_parabolic_pulse(double Tu);
    double squared_pulse_area();
};

#endif // PULSEFORM_H
