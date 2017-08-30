#include "spectrum.h"

Spectrum::Spectrum()
{
    dQ = MAX_CHARGE/(SPEC_SIZE-1);

    spectrum_array.resize(SPEC_SIZE);
    q.resize(SPEC_SIZE);
    for(int i = 0; i < SPEC_SIZE; i++) {
        q[i] = i*dQ;
    }
}

void Spectrum::get_const_spectrum(double q){
    clear_array();

    int point = (int)(qFloor(q/dQ));
    spectrum_array[point] = 1/dQ;
    get_int_spectrum(spectrum_array);
}

void Spectrum::get_uni_spectrum(double qmin, double qmax) {
    clear_array();

    int first_point = (int)(qFloor(qmin/dQ));
    int second_point = (int)(qFloor(qmax/dQ));

    for(int i = first_point; i < second_point; i++) {
        spectrum_array[i] = 1/(qmax-qmin);
    }
    get_int_spectrum(spectrum_array);
}

void Spectrum::get_gauss_spectrum(double qmean, double sigma) {
    clear_array();

    for(int i = 0; i < SPEC_SIZE; i++) {
        spectrum_array[i] = (1/(sigma*qSqrt(2*M_PI)))*qExp(-(qPow((i*dQ-qmean),2))/(2*qPow(sigma,2)));
    }
    get_int_spectrum(spectrum_array);
}

void Spectrum::get_lin_spectrum(double qmin, double qmax, int n_points) {
    clear_array();

    int point[n_points] = {0};
    double q = 0;

    for(int i = 0; i < n_points; i++) {
        q = qmin + i*((qmax-qmin)/(n_points-1));
        point[i] = (int)qFloor((q/dQ));
    }

    for(int i = 0; i < n_points; i++) {
        spectrum_array[point[i]] = 1/(n_points*dQ);
    }
    get_int_spectrum(spectrum_array);
}

void Spectrum::get_exp_spectrum(double qmean){
    clear_array();

    for(int i = 0; i < SPEC_SIZE; i++) {
        spectrum_array[i] = (1/qmean)*qExp(-i*dQ/qmean);
    }
    get_int_spectrum(spectrum_array);
}

void Spectrum::get_int_spectrum(QVector<double> array) {
    QVector<double> quantile_array(SPEC_SIZE);
    QVector<double> temp_array(SPEC_SIZE);

    for(size_t i = 1; i < SPEC_SIZE; i++) {
        quantile_array[i] = quantile_array[i-1] + array[i]*dQ;
    }

    double quantile_max = get_max(quantile_array);
    for(size_t i = 0;  i < SPEC_SIZE; i++) {
        temp_array[i] = i*(quantile_max/(SPEC_SIZE-1));
    }

    int c = 0;
    for(size_t i = 0; i < SPEC_SIZE; i++) {
        while(quantile_array[c] <= temp_array[i]) {
            c +=1;
            if(c == SPEC_SIZE) {
                reverse_array[i] = reverse_array[i-1];
                break;
            }
            else {
                reverse_array[i] = (uint16_t)(q[c]/dQ);
            }
        }
        c = 0;
    }
}

double Spectrum::get_max(QVector<double> array) {
    double iMax = array[0];

    for (int i=1; i < array.size(); i++)
    {
        if (array[i] > iMax) iMax = array[i];
    }

    return iMax;
}

double Spectrum::get_square() {
    double square = 0;

    for (int i=0; i < SPEC_SIZE; i++)
    {
        square += spectrum_array[i];
    }

    return square*dQ;
}

double Spectrum::get_mean_charge() {
    double mean_charge = 0;

    for (int i=0; i < SPEC_SIZE; i++)
    {
        mean_charge += spectrum_array[i]*dQ*q[i];
    }

    return mean_charge;
}

double Spectrum::get_square_mean_charge() {
    double mean_charge = 0;

    for (int i=0; i < SPEC_SIZE; i++)
    {
        mean_charge += spectrum_array[i]*dQ*q[i]*q[i];
    }

    return mean_charge;
}

void Spectrum::clear_array(){
    for(int i = 0; i < SPEC_SIZE; i++) {
        spectrum_array[i] = 0;
    }
}
