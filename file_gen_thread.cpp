#include "file_gen_thread.h"

file_gen_thread::file_gen_thread(QObject *parent) : QObject(parent)
{
    file_gen = new wr_file_loops();
    k0 = 1.5;
    k1 = 0.52249;
    k2 = -0.035581;
    k3 = 0.02625;
    t_array.reserve(100);
    cr_array.reserve(100);

    connect(file_gen, SIGNAL(loop_index(int)), this, SLOT(update_index(int)));
}

void file_gen_thread::process()
{
    file_gen->F0 = S;
    file_gen->A = A;
    file_gen->T0 = T;
    file_gen->r0 = R0;
    file_gen->r = R;
    file_gen->time = TT;
    file_gen->test = false;
    file_gen->Kt = Kt;
    file_gen->Ktt = 1;
    file_gen->rmod = r_mod;
    file_gen->r_const.energy_yield = En_Y;
    file_gen->r_const.enrichment = Enrch;
    file_gen->r_const.pu_u = Pu_U;
    file_gen->a_r = A_R;

    file_gen->k3 = k3;
    file_gen->k2 = k2;
    file_gen->k1 = k1;
    file_gen->k0 = k0;
    file_gen->mean_q = mean_q;

    if(mode_index == 0) {
        switch (model_index) {
        case 0:
            file_gen->gen_reg_const();
            break;
        case 1:
            file_gen->gen_reg_lin();
            break;
        case 2:
            file_gen->gen_reg_step(t_array, cr_array);
            break;
        case 3:
            file_gen->gen_reg_exp();
            break;
        case 4:
            file_gen->gen_reg_exp_var();
            break;
        case 5:
            file_gen->gen_reg_react();
            break;
        default:
            break;
        }
    }
    else {
        switch (model_index) {
        case 0:
            file_gen->gen_rand_const();
            break;
        case 1:
            file_gen->gen_rand_lin();
            break;
        case 2:
            file_gen->gen_rand_step(t_array, cr_array);
            break;
        case 3:
            file_gen->gen_rand_exp();
            break;
        case 4:
            file_gen->gen_rand_exp_var();
            break;
        case 5:
            file_gen->gen_rand_react();
            break;
        default:
            break;
        }
    }

    emit finished();
}

void file_gen_thread::update_index(int a) {
    emit send_bar(a);
}
