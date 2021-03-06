#include "pulseform.h"
#include "qmath.h"

PulseForm::PulseForm()
{
    dt = 1e-8;
    dU = 2.5/(qPow(2,16)-1);
    dQ = MAX_CHARGE/(SPEC_SIZE-1);
    k = (pow(2,31)-1)/(30*(SPEC_SIZE-1)*100);
    R = 100000;

    t.resize(P_FORM_SIZE);
    norm_pulse_form.resize(P_FORM_SIZE);
    for(int i = 0; i < P_FORM_SIZE; i++) {
        t[i] = i*dt;
    }
}

void PulseForm::get_square_pulse(double Tu) {
    QVector<double> pulse(P_FORM_SIZE);

    int bound = (int)(Tu/dt);

    for(int i = 0; i < bound + 1; i++) {
        pulse[i] = 1;
    }

    fpga_compatible_pulse(pulse);
}

void PulseForm::get_gauss_pulse(double Tu) {
    QVector<double> pulse(P_FORM_SIZE);

    double m = 5e-7;
    double sig = Tu/(2*qSqrt(2*qLn(2)));

    for(size_t i = 0; i < P_FORM_SIZE; i++) {
        pulse[i] = qExp(-(qPow((i*dt-m),2))/(2*qPow(sig,2)));
    }

    fpga_compatible_pulse(pulse);
}

void PulseForm::get_exp_pulse(double Tu) {
    QVector<double> pulse(P_FORM_SIZE);

    for(size_t i = 0; i < P_FORM_SIZE; i++) {
        pulse[i] = qExp(-(i*dt)/Tu);
    }

    fpga_compatible_pulse(pulse);
}

void PulseForm::get_parabolic_pulse(double Tu) {
    QVector<double> pulse(P_FORM_SIZE);

    double a = -4/qPow(Tu,2);
    double b = -a*1e-6;
    double c = 1 - (1e-12/qPow(Tu,2));

    for(size_t i = 0; i < P_FORM_SIZE; i++) {
        double time = i*dt;
        if((time >= (5e-7 - (Tu/2))) && (time <= (5e-7 + (Tu/2)))) {
            pulse[i] = a*qPow(time,2) + b*time + c;
        }
        else {
            pulse[i] = 0;
        }
    }

    fpga_compatible_pulse(pulse);
}

void PulseForm::fpga_compatible_pulse(QVector<double> pulse) {
    //Форма импульса, совместимая с FPGA
    for(int i = 0; i < P_FORM_SIZE; i++) {
        pulse_form[i] = (uint16_t)(pulse[i]*k);
    }
    //Форма импульса с единичной площадью
    square = 0;

    for(int i = 0; i < P_FORM_SIZE; i++) {
        square += dt*((double)pulse_form[i]);
    }

    for(int i = 0; i < P_FORM_SIZE; i++) {
        norm_pulse_form[i] = ((double)pulse_form[i])/square;
    }

   pulse_coeff = (R*dQ/dU)/(square*2); //Двойка из-за 50 Ом, предположительно
}

double PulseForm::squared_pulse_area() {
    double square = 0;

    for(int i = 0; i < P_FORM_SIZE; i++) {
        square += norm_pulse_form[i]*norm_pulse_form[i]*dt;
    }

    return square;
}
