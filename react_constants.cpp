#include "react_constants.h"

react_constants::react_constants()
{
    dt = 1E-7;
}


void react_constants::init_react_consts_rbmk(double initial_F) {
    react_d_rbmk(energy_yield, enrichment);
    bet_rbmk = d[0] * bi[0] + d[1] * bi[1] + d[2] * bi[2] + d[3] * bi[3];
    j = 0;
    k = 0;
    for (i = 0; i < 6; i++) {
        bb_rbmk[k] = (d[j] * bi[j] * bb1[i]) / bet_rbmk;
        k++;
    }
    j++;
    for (i = 0; i < 6; i++) {
        bb_rbmk[k] = (d[j] * bi[j] * bb2[i]) / bet_rbmk;
        k++;
    }
    j++;
    for (i = 0; i < 6; i++) {
        bb_rbmk[k] = (d[j] * bi[j] * bb3[i]) / bet_rbmk;
        k++;
    }
    j++;
    for (i = 0; i < 6; i++) {
        bb_rbmk[k] = (d[j] * bi[j] * bb4[i]) / bet_rbmk;
        k++;
    }

    for(int h = 0; h < N; h++) {
        bb[h] = bb_rbmk[h];
        lam[h] = lam_rbmk[h];
    }

    bet = bet_rbmk;
    l = l_rbmk;
    preinit_react_consts(initial_F);
}

void react_constants::init_react_consts_bwr(double initial_F) {
    switch(pu_u) {
        case pu_u_0_000:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_0_000[h];
            }
            bet = bet_0_000;
            break;
        case pu_u_0_100:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_0_100[h];
            }
            bet = bet_0_100;
            break;
        case pu_u_0_193:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_0_193[h];
            }
            bet = bet_0_193;
            break;
        case pu_u_0_402:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_0_402[h];
            }
            bet = bet_0_402;
            break;
        case pu_u_0_672:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_0_672[h];
            }
            bet = bet_0_672;
            break;
        case pu_u_1_051:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_1_051[h];
            }
            bet = bet_1_051;
            break;
        case pu_u_1_604:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_1_604[h];
            }
            bet = bet_1_604;
            break;
        case pu_u_24_950:
            for(int h = 0; h < N; h++) {
                bb[h] = bb_24_950[h];
            }
            bet = bet_24_950;
            break;
        default:
            break;
    }

    for(int h = 0; h < N; h++) {
        lam[h] = lam_bwr[h];
    }

    l = l_bwr;

    preinit_react_consts(initial_F);
}

void react_constants::init_react_consts_bn(double initial_F) {
    for(int h = 0; h < N; h++) {
        lam[h] = lam_bn[h];
        bb[h] = bb_bn[h];
    }
    bet = bet_bn;
    l = l_bn;
    preinit_react_consts(initial_F);
}

void react_constants::init_react_consts(r_models rmod, double initial_F) {
    if(rmod == RBMK) {
        N = 24;
        init_react_consts_rbmk(initial_F);
    }
    else if(rmod == VVR) {
        N = 6;
        init_react_consts_bwr(initial_F);
    }
    else if(rmod == BN) {
        N = 8;
        init_react_consts_bn(initial_F);
    }
}

void react_constants::preinit_react_consts(double initial_F) {
    for (int i = 0; i < N; i++)
    {
        e[i] = qExp(-dt * lam[i]);
        a[i] = (1 - ((1 - e[i]) / (dt * lam[i]))) * bb[i];
        b[i] = (e[i] - ((1 - e[i]) / (dt * lam[i]))) * bb[i];
        x[i] = bb[i] * initial_F;
    }

    sum_a = 0;
    sum_b = 0;
    for (int j = 0; j < N; j++) {
        sum_b = sum_b + b[j];
        sum_a = sum_a + a[j];
    }
}

void react_constants::react_d_rbmk(yield_enum z, enrichment_enum x) {

    if ((z == yld_0) && (x == en2_0)) {
        d[0] = 0.9548;
        d[1] = 0.0452;
        d[2] = 0;
        d[3] = 0;
    }
    else if ((z == yld_5) && (x == en2_0)) {
        d[0] = 0.7071;
        d[1] = 0.0465;
        d[2] = 0.2385;
        d[3] = 0.0079;
    }
    else if ((z == yld_10) && (x == en2_0)) {
        d[0] = 0.5594;
        d[1] = 0.0499;
        d[2] = 0.3566;
        d[3] = 0.0343;
    }
    else if ((z == yld_15) && (x == en2_0)) {
        d[0] = 0.4379;
        d[1] = 0.0549;
        d[2] = 0.4360;
        d[3] = 0.0718;
    }
    else if ((z == yld_20) && (x == en2_0)) {
        d[0] = 0.3398;
        d[1] = 0.0608;
        d[2] = 0.4972;
        d[3] = 0.1132;
    }
    else if ((z == yld_0) && (x == en2_4)) {
        d[0] = 0.9592;
        d[1] = 0.0408;
        d[2] = 0;
        d[3] = 0;
    }
    else if ((z == yld_5) && (x == en2_4)) {
        d[0] = 0.7549;
        d[1] = 0.0421;
        d[2] = 0.1977;
        d[3] = 0.0053;
    }
    else if ((z == yld_10) && (x == en2_4)) {
        d[0] = 0.6251;
        d[1] = 0.0450;
        d[2] = 0.3053;
        d[3] = 0.0246;
    }
    else if ((z == yld_15) && (x == en2_4)) {
        d[0] = 0.5173;
        d[1] = 0.0490;
        d[2] = 0.3801;
        d[3] = 0.0536;
    }
    else if ((z == yld_20) && (x == en2_4)) {
        d[0] = 0.4159;
        d[1] = 0.0544;
        d[2] = 0.4411;
        d[3] = 0.0887;
    }
    else if ((z == yld_0) && (x == en2_6)) {
        d[0] = 0.9576;
        d[1] = 0.0424;
        d[2] = 0;
        d[3] = 0;
    }
    else if ((z == yld_5) && (x == en2_6)) {
        d[0] = 0.7544;
        d[1] = 0.0428;
        d[2] = 0.1975;
        d[3] = 0.0053;
    }
    else if ((z == yld_10) && (x == en2_6)) {
        d[0] = 0.6251;
        d[1] = 0.0450;
        d[2] = 0.3053;
        d[3] = 0.0246;
    }
    else if ((z == yld_15) && (x == en2_6)) {
        d[0] = 0.5175;
        d[1] = 0.0486;
        d[2] = 0.3803;
        d[3] = 0.0536;
    }
    else if ((z == yld_20) && (x == en2_6)) {
        d[0] = 0.4126;
        d[1] = 0.0535;
        d[2] = 0.4416;
        d[3] = 0.0887;
    }
    else if ((z == yld_0) && (x == en2_8)) {
        d[0] = 0.9568;
        d[1] = 0.0432;
        d[2] = 0;
        d[3] = 0;
    }
    else if ((z == yld_5) && (x == en2_8)) {
        d[0] = 0.7540;
        d[1] = 0.0435;
        d[2] = 0.1972;
        d[3] = 0.0053;
    }
    else if ((z == yld_10) && (x == en2_8)) {
        d[0] = 0.6248;
        d[1] = 0.0455;
        d[2] = 0.3051;
        d[3] = 0.0246;
    }
    else if ((z == yld_15) && (x == en2_8)) {
        d[0] = 0.5174;
        d[1] = 0.0487;
        d[2] = 0.3803;
        d[3] = 0.0536;
    }
    else {
        d[0] = 0.4162;
        d[1] = 0.0533;
        d[2] = 0.4418;
        d[3] = 0.0887;
    }
}
