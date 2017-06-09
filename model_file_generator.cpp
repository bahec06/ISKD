#include "model_file_generator.h"

model_file_generator::model_file_generator()
{
    i = 0;
    j = 0;
    k = 0;
    dt = 1e-4;
    F = 0;
    a_r = 0.05;
    filename = "D:/Qt_repo/time_file.bin";
    mFile.setFileName(filename);
    stream.setDevice(&mFile);
}

void model_file_generator::gen_rand_const()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    std::poisson_distribution<int> distribution(F0*dt);
    for (i = 0; i < rolls; ++i) {
        number[i % BUF_SIZE] = distribution(generator);
        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_rand_lin()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    for (i = 0; i < rolls; ++i) {
        F = F0+i*dt*A;
        std::poisson_distribution<int> distribution(F*dt);
        number[i % BUF_SIZE] = distribution(generator);
        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_rand_exp()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    for (i = 0; i < rolls; ++i) {
        F = F0*qExp(i*dt/T0);
        std::poisson_distribution<int> distribution(F*dt);
        number[i % BUF_SIZE] = distribution(generator);
        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_rand_exp_var()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    for (i = 0; i < rolls; ++i) {
        T = T0+K*i*dt;
        F = F0*qExp(i*dt/T);
        std::poisson_distribution<int> distribution(F*dt);
        number[i % BUF_SIZE] = distribution(generator);
        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_rand_react() {
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    F_p = F0;
    F_n = 0;
    r1 = 0;
    r_const.init_react_consts(rmod, F0);
    lb = r_const.l / (r_const.bet*dt);
    S = -r0*F0;

    for (i = 0; i < rolls; i++) {
        std::poisson_distribution<int> distribution(F_p*dt);
        number[i % BUF_SIZE] = distribution(generator);

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

        if (r1   < r0)
        {
            r1 += a_r * dt;
        }

        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_reg_lin()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    int_F = 0;
    omeg_p = 0;

    for (i = 0; i<rolls; ++i) {
        F = F0 + A*dt*i;
        int_F = int_F + F*dt;
        omeg_n = 2*M_PI*int_F;
        if (((fmod(omeg_p, 2 * M_PI) - M_PI) < 0) && ((fmod(omeg_n, 2 * M_PI) - M_PI)) > 0) {
            number[i % BUF_SIZE] = 1;
        }
        else {
            number[i % BUF_SIZE] = 0;

        }
        omeg_p = omeg_n;
        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_reg_exp()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    int_F = 0;
    omeg_p = 0;

    for (i = 0; i<rolls; ++i) {
        F = F0*qExp(i*dt/T0);
        int_F = int_F + F*dt;
        omeg_n = 2*M_PI*int_F;
        if (((fmod(omeg_p, 2 * M_PI) - M_PI) < 0) && ((fmod(omeg_n, 2 * M_PI) - M_PI)) > 0) {
            number[i % BUF_SIZE] = 1;
        }
        else {
            number[i % BUF_SIZE] = 0;

        }
        omeg_p = omeg_n;
        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_reg_exp_var()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    int_F = 0;
    omeg_p = 0;

    for (i = 0; i<rolls; ++i) {
        T = T0+K*i*dt;
        F = F0*qExp(i*dt/T);
        int_F = int_F + F*dt;
        omeg_n = 2*M_PI*int_F;
        if (((fmod(omeg_p, 2 * M_PI) - M_PI) < 0) && ((fmod(omeg_n, 2 * M_PI) - M_PI)) > 0) {
            number[i % BUF_SIZE] = 1;
        }
        else {
            number[i % BUF_SIZE] = 0;

        }
        omeg_p = omeg_n;
        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

void model_file_generator::gen_reg_react()
{
    mFile.open(QFile::WriteOnly | QFile::Truncate);
    rolls = uint64_t(time/dt);
    F_p = F0;
    F_n = 0;
    r1 = 0;
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

        if (r < r0)
        {
            r += a_r * dt;
        }

        if ((i % BUF_SIZE) == (BUF_SIZE-1)) stream.writeBytes((char *)number,BUF_SIZE*2);
    }

    stream.writeBytes((char *)number, (i % (BUF_SIZE * 2)));

    mFile.close();
}

