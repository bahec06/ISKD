#include "wr_file_loops.h"

wr_file_loops::wr_file_loops(QObject *parent) : QObject(parent)
{
    i = 0;
    j = 0;
    k = 0;
    dt = 1e-7;
    F = 0;
    a_r = 0.05;
    filename = "D:/Qt_repo/time_file.bin";
    mFile.setFileName(filename);
    stream.setDevice(&mFile);
    test = false;
}

void wr_file_loops::gen_rand_const()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    std::poisson_distribution<int> distribution(F0*dt);
    for (i = 0; i < rolls; ++i) {
        number[i % BUF_SIZE] = distribution(generator);
        if(test) {
            stream << F0;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }

        if((i%(rolls/100)) == 0) {
            emit loop_index(i*100/rolls);
        }
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_rand_lin()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    F = F0;
    for (i = 0; i < rolls; ++i) {
        std::poisson_distribution<int> distribution(F*dt);
        number[i % BUF_SIZE] = distribution(generator);
        if(test) {
            stream << F;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }
        F = F0+i*dt*A;

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_rand_exp()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    F = F0;
    for (i = 0; i < rolls; ++i) {
        std::poisson_distribution<int> distribution(F*dt);
        number[i % BUF_SIZE] = distribution(generator);
        if(test) {
            stream << F;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }
        F = F0*qExp(i*dt/T0);

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_rand_exp_var()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    T = T0;
    F = F0;
    for (i = 0; i < rolls; ++i) {
        std::poisson_distribution<int> distribution(F*dt);
        number[i % BUF_SIZE] = distribution(generator);
        if(test) {
            stream << F;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }
        if(((fmod(i*dt, Ktt)) == 0) && (i != 0)) {
            T = T+Kt;
        }
        F = F*qExp(dt/T);

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_rand_react() {
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    F_p = F0;
    F_n = 0;
    double r1 = 0;
    r_const.init_react_consts(rmod, F0);
    lb = r_const.l / (r_const.bet*dt);
    S = -r0*F0;

    for (i = 0; i < rolls; i++) {
        std::poisson_distribution<int> distribution(F_p*dt);
        number[i % BUF_SIZE] = distribution(generator);
        if(test) {
            stream << F_p;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }

        r_const.sum_xe = 0;
        r_const.sum_b = 0;
        r_const.sum_a = 0;

        for (int j = 0; j < r_const.N; j++) {
            r_const.sum_xe = r_const.sum_xe + (r_const.e[j] * r_const.x[j]);
            r_const.sum_b = r_const.sum_b + r_const.b[j];
            r_const.sum_a = r_const.sum_a + r_const.a[j];
        }

        F_n = (r_const.sum_xe+F_p*(lb-r_const.sum_b) + S)/(1-r1-r0-r_const.sum_a+lb);

        for (j = 0; j < r_const.N; j++) {
            r_const.x[j] = r_const.x[j]*r_const.e[j] + F_n*r_const.a[j] - F_p*r_const.b[j];
        }
        F_p = F_n;

        if (abs(r1) < abs(r))
        {
            r1 += a_r * dt;
        }
        else {
            r1 = r;
        }

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_reg_lin()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    int_F = 0;
    omeg_p = 0;
    F = F0;
    for (i = 0; i<rolls; ++i) {
        int_F = int_F + F*dt;
        omeg_n = 2*M_PI*int_F;

        if (((fmod(omeg_p, 2 * M_PI) - M_PI) < 0) && ((fmod(omeg_n, 2 * M_PI) - M_PI)) > 0) {
            number[i % BUF_SIZE] = 1;
        }
        else {
            number[i % BUF_SIZE] = 0;

        }
        omeg_p = omeg_n;

        if(test) {
            stream << F;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }
        F = F0 + A*dt*i;

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_reg_exp()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    int_F = 0;
    omeg_p = 0;
    F = F0;
    for (i = 0; i<rolls; ++i) {
        int_F = int_F + F*dt;
        omeg_n = 2*M_PI*int_F;
        if (((fmod(omeg_p, 2 * M_PI) - M_PI) < 0) && ((fmod(omeg_n, 2 * M_PI) - M_PI)) > 0) {
            number[i % BUF_SIZE] = 1;
        }
        else {
            number[i % BUF_SIZE] = 0;

        }
        omeg_p = omeg_n;

        if(test) {
            stream << F;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }
        F = F0*qExp(i*dt/T0);

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_reg_exp_var()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
    }
    else {
        dt = 1e-7;
    }
    rolls = uint64_t(time/dt + 1);
    int_F = 0;
    omeg_p = 0;
    T = T0;
    F = F0;
    for (i = 0; i<rolls; ++i) {
        int_F = int_F + F*dt;
        omeg_n = 2*M_PI*int_F;
        if (((fmod(omeg_p, 2 * M_PI) - M_PI) < 0) && ((fmod(omeg_n, 2 * M_PI) - M_PI)) > 0) {
            number[i % BUF_SIZE] = 1;
        }
        else {
            number[i % BUF_SIZE] = 0;

        }
        omeg_p = omeg_n;

        if(test) {
            stream << F;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }
        if(((fmod(i*dt, Ktt)) == 0) && (i != 0)) {
            T = T+Kt;
        }
        F = F*qExp(dt/T);

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}

void wr_file_loops::gen_reg_react()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    if(test) {
        dt = 1e-4;
        r_const.dt = dt;
    }
    else {
        dt = 1e-7;
        r_const.dt = dt;
    }
    rolls = uint64_t(time/dt + 1);
    F_p = F0;
    F_n = 0;
    double r1 = 0;
    r_const.init_react_consts(rmod, F0);
    lb = r_const.l / (r_const.bet*dt);
    S = -r0*F0;

    for (i = 0; i < rolls; i++) {
        r_const.sum_xe = 0;
        r_const.sum_b = 0;
        r_const.sum_a = 0;

        int_F = int_F + F_p * dt;
        omeg_n = 2 * M_PI * int_F;

        if (((fmod(omeg_p, 2 * M_PI) - M_PI) < 0) && ((fmod(omeg_n, 2 * M_PI) - M_PI)) > 0)
        {
            number[i % BUF_SIZE] = 1;
        }
        else
        {
            number[i % BUF_SIZE] = 0;
        }
        omeg_p = omeg_n;

        if(test) {
            stream << F_p;
        }
        else {
            if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
        }

        for (int j = 0; j < r_const.N; j++) {
            r_const.sum_xe = r_const.sum_xe + (r_const.e[j] * r_const.x[j]);
        }

        F_n = (r_const.sum_xe+F_p*(lb-r_const.sum_b) + S)/(1-r1-r0-r_const.sum_a+lb);

        for (j = 0; j < r_const.N; j++) {
            r_const.x[j] = r_const.x[j]*r_const.e[j] + F_n*r_const.a[j] - F_p*r_const.b[j];
        }
        F_p = F_n;

        if (abs(r1) < abs(r))
        {
            r1 += a_r * dt;
        }
        else {
            r1 = r;
        }

        if((i%(rolls/100)) == 0) emit loop_index(i);
    }

    if(!test) {
        stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));
    }

    mFile.close();
}
