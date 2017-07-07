#include "wdg_params.h"

wdg_params::wdg_params(QObject *parent) : QObject(parent)
{
    qle_widget = new QLineEdit();

    create_rmod_qcb();
    create_en_y_qcb();
    create_enrch_qcb();
    create_pu_u_qcb();

    t_widget = new QTableWidget();

    qcb_rmod_widget->setCurrentIndex(0);
    r_mod = RBMK;
    st_dist = CONST;
    En_Y = yld_0;
    Enrch = en2_0;
    Pu_U = pu_u_0_000;
}

void wdg_params::create_en_y_qcb() {
    qcb_en_y_widget = new QComboBox();
    qcb_en_y_widget->addItem("0");
    qcb_en_y_widget->addItem("5");
    qcb_en_y_widget->addItem("10");
    qcb_en_y_widget->addItem("15");
    qcb_en_y_widget->addItem("20");

    qcb_en_y_widget->setCurrentIndex((int)En_Y);

    connect(qcb_en_y_widget, SIGNAL(currentIndexChanged(int)), this, SLOT(en_y_update()));
}

void wdg_params::create_enrch_qcb() {
    qcb_enrch_widget = new QComboBox();
    qcb_enrch_widget->addItem("2.0");
    qcb_enrch_widget->addItem("2.4");
    qcb_enrch_widget->addItem("2.6");
    qcb_enrch_widget->addItem("2.8");

    qcb_enrch_widget->setCurrentIndex((int)Enrch);

    connect(qcb_enrch_widget, SIGNAL(currentIndexChanged(int)), this, SLOT(enrch_update()));
}

void wdg_params::create_pu_u_qcb() {
    qcb_pu_u_widget = new QComboBox();
    qcb_pu_u_widget->addItem("0");
    qcb_pu_u_widget->addItem("0.1");
    qcb_pu_u_widget->addItem("0.193");
    qcb_pu_u_widget->addItem("0.402");
    qcb_pu_u_widget->addItem("0.672");
    qcb_pu_u_widget->addItem("1.051");
    qcb_pu_u_widget->addItem("1.604");
    qcb_pu_u_widget->addItem("24.950");

    qcb_pu_u_widget->setCurrentIndex((int)Pu_U);

    connect(qcb_pu_u_widget, SIGNAL(currentIndexChanged(int)), this, SLOT(pu_u_update()));
}

void wdg_params::create_rmod_qcb() {
    qcb_rmod_widget = new QComboBox();
    qcb_rmod_widget->addItem("РБМК");
    qcb_rmod_widget->addItem("ВВЭР");
    qcb_rmod_widget->addItem("БН");

    switch (r_mod) {
        case RBMK:
            qcb_rmod_widget->setCurrentIndex(0);
            break;
        case VVR:
            qcb_rmod_widget->setCurrentIndex(1);
            break;
        case BN:
            qcb_rmod_widget->setCurrentIndex(2);
            break;
        default:
            break;
    }

    connect(qcb_rmod_widget, SIGNAL(currentIndexChanged(int)), this, SLOT(rmod_update()));
}

void wdg_params::init_table() {
    t_widget->clear();
    t_widget->setColumnCount(1);

    m_TableHeader.clear();
    m_TableHeader << "Значение";
    t_widget->setHorizontalHeaderLabels(m_TableHeader);
}

void wdg_params::set_reg_signal()
{
    init_table();
    t_widget->setRowCount(2);

    m_TableHeader.clear();
    QString sig_param;
    if(s_type == REG) {
        sig_param = reg_F;
    }
    else {
        sig_param = rand_C;
    }
    m_TableHeader << sig_param << time;

    t_widget->setVerticalHeaderLabels(m_TableHeader);

    t_widget->setItem(0, 0, new QTableWidgetItem(QString::number(S)));
    t_widget->setItem(1, 0, new QTableWidgetItem(QString::number(TT)));
}

void wdg_params::set_lin_signal()
{
    init_table();
    t_widget->setRowCount(3);

    m_TableHeader.clear();
    QString sig_param1;
    QString sig_param2;
    if(s_type == REG) {
        sig_param1 = reg_F0;
        sig_param2 = reg_AF;
    }
    else {
        sig_param1 = rand_C0;
        sig_param2 = rand_AC;
    }
    m_TableHeader << sig_param1 << sig_param2 << time;

    t_widget->setVerticalHeaderLabels(m_TableHeader);

    t_widget->setItem(0, 0, new QTableWidgetItem(QString::number(S)));
    t_widget->setItem(1, 0, new QTableWidgetItem(QString::number(A)));
    t_widget->setItem(2, 0, new QTableWidgetItem(QString::number(TT)));
}

void wdg_params::set_step_signal()
{

}

void wdg_params::set_exp_signal()
{
    init_table();
    t_widget->setRowCount(3);

    m_TableHeader.clear();
    QString sig_param;
    if(s_type == REG) {
        sig_param = reg_F0;
    }
    else {
        sig_param = rand_C0;
    }
    m_TableHeader << sig_param << period << time;

    t_widget->setVerticalHeaderLabels(m_TableHeader);

    t_widget->setItem(0, 0, new QTableWidgetItem(QString::number(S)));
    t_widget->setItem(1, 0, new QTableWidgetItem(QString::number(T)));
    t_widget->setItem(2, 0, new QTableWidgetItem(QString::number(TT)));
}

void wdg_params::set_explin_signal()
{
    init_table();
    t_widget->setRowCount(4);

    m_TableHeader.clear();
    QString sig_param;
    if(s_type == REG) {
        sig_param = reg_F0;
    }
    else {
        sig_param = rand_C0;
    }
    m_TableHeader << sig_param << initial_period << t_coe << time;

    t_widget->setVerticalHeaderLabels(m_TableHeader);

    t_widget->setItem(0, 0, new QTableWidgetItem(QString::number(S)));
    t_widget->setItem(1, 0, new QTableWidgetItem(QString::number(T)));
    t_widget->setItem(2, 0, new QTableWidgetItem(QString::number(Kt)));
    t_widget->setItem(3, 0, new QTableWidgetItem(QString::number(TT)));
}

void wdg_params::set_react_signal()
{
    init_table();

    m_TableHeader.clear();
    QString sig_param;
    if(s_type == REG) {
        sig_param = reg_F0;
    }
    else {
        sig_param = rand_C0;
    }

    create_rmod_qcb();
    create_en_y_qcb();
    create_enrch_qcb();
    create_pu_u_qcb();

    switch(r_mod) {
        case 0:
            t_widget->setRowCount(8);
            m_TableHeader << r_model << sig_param << s_r0 << s_r << s_rt << energy_yield << enrichment << time;
            break;
        case 1:
            t_widget->setRowCount(7);
            m_TableHeader << r_model << sig_param << s_r0 << s_r << s_rt << s_pu_u << time;
            break;
        case 2:
            t_widget->setRowCount(6);
            m_TableHeader << r_model << sig_param << s_r0 << s_r << s_rt << time;
            break;
        default:
            break;
    }
    t_widget->setVerticalHeaderLabels(m_TableHeader);
    t_widget->setCellWidget(0, 0, qcb_rmod_widget);
    t_widget->setItem(1, 0, new QTableWidgetItem(QString::number(S)));
    t_widget->setItem(2, 0, new QTableWidgetItem(QString::number(R0)));
    t_widget->setItem(3, 0, new QTableWidgetItem(QString::number(R)));
    t_widget->setItem(4, 0, new QTableWidgetItem(QString::number(Rt)));

    switch(r_mod) {
        case 0:
            t_widget->setCellWidget(5, 0, qcb_en_y_widget);
            t_widget->setCellWidget(6, 0, qcb_enrch_widget);
            t_widget->setItem(7, 0, new QTableWidgetItem(QString::number(TT)));
            break;
        case 1:
            t_widget->setCellWidget(5, 0, qcb_pu_u_widget);
            t_widget->setItem(6, 0, new QTableWidgetItem(QString::number(TT)));
            break;
        case 2:
            t_widget->setItem(5, 0, new QTableWidgetItem(QString::number(TT)));
            break;
        default:
            break;
   }
}

void wdg_params::read_params_from_table() {
    switch(st_dist){
        case REG:
            S = t_widget->item(0, 0)->text().toDouble();
            TT = t_widget->item(1, 0)->text().toDouble();
            break;
        case LIN:
            S = t_widget->item(0, 0)->text().toDouble();
            A = t_widget->item(1, 0)->text().toDouble();
            TT = t_widget->item(2, 0)->text().toDouble();
            break;
        case STEP:

            break;
        case EXP:
            S = t_widget->item(0, 0)->text().toDouble();
            T = t_widget->item(1, 0)->text().toDouble();
            TT = t_widget->item(2, 0)->text().toDouble();
            break;
        case EXPLIN:
            S = t_widget->item(0, 0)->text().toDouble();
            T = t_widget->item(1, 0)->text().toDouble();
            Kt = t_widget->item(2, 0)->text().toDouble();
            TT = t_widget->item(3, 0)->text().toDouble();
            break;
        case REACT:
            S = t_widget->item(1, 0)->text().toDouble();
            R0 = t_widget->item(2, 0)->text().toDouble();
            R = t_widget->item(3, 0)->text().toDouble();
            Rt = t_widget->item(4, 0)->text().toDouble();
            switch(r_mod) {
                case 0:
                    TT = t_widget->item(7, 0)->text().toDouble();
                    break;
                case 1:
                    TT = t_widget->item(6, 0)->text().toDouble();
                    break;
                case 2:
                    TT = t_widget->item(5, 0)->text().toDouble();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void wdg_params::pulse_dist_change(int p_dist) {
    switch(p_dist) {
    case 0:
        s_type = REG;
        break;
    case 1:
        s_type = RAND;
        break;
    default:
        break;
    }

    emit update_table();
}

void wdg_params::st_dist_change(int ext_st_dist) {
    switch(ext_st_dist) {
    case 0:
        st_dist = CONST;
        break;
    case 1:
        st_dist = LIN;
        break;
    case 2:
        st_dist = STEP;
        break;
    case 3:
        st_dist = EXP;
        break;
    case 4:
        st_dist = EXPLIN;
        break;
    case 5:
        st_dist = REACT;
        break;
    default:
        break;
    }

    emit update_table();
}

void wdg_params::slot_update() {
    t_widget->blockSignals(true);
    switch(st_dist) {
        case CONST:
            set_reg_signal();
            break;
        case LIN:
            set_lin_signal();
            break;
        case STEP:
            set_step_signal();
            break;
        case EXP:
            set_exp_signal();
            break;
        case EXPLIN:
            set_explin_signal();
            break;
        case REACT:
            set_react_signal();
            break;
        default:
            break;
    }
    t_widget->blockSignals(false);
}

void wdg_params::rmod_update() {
    switch (qcb_rmod_widget->currentIndex()) {
        case 0:
            r_mod = RBMK;
            break;
        case 1:
            r_mod = VVR;
            break;
        case 2:
            r_mod = BN;
            break;
        default:
            break;
    }

    emit update_table();
}

void wdg_params::enrch_update() {
    switch (qcb_enrch_widget->currentIndex()) {
        case 0:
            Enrch = en2_0;
            break;
        case 1:
            Enrch = en2_4;
            break;
        case 2:
            Enrch = en2_6;
            break;
        case 3:
            Enrch = en2_8;
            break;
        default:
            break;
    }

    emit update_table();
}

void wdg_params::en_y_update() {
    switch (qcb_en_y_widget->currentIndex()) {
        case 0:
            En_Y = yld_0;
            break;
        case 1:
            En_Y = yld_5;
            break;
        case 2:
            En_Y = yld_10;
            break;
        case 3:
            En_Y = yld_15;
            break;
        case 4:
            En_Y = yld_20;
            break;
        default:
            break;
    }

    emit update_table();
}

void wdg_params::pu_u_update() {
    switch (qcb_pu_u_widget->currentIndex()) {
        case 0:
            Pu_U = pu_u_0_000;
            break;
        case 1:
            Pu_U = pu_u_0_100;
            break;
        case 2:
            Pu_U = pu_u_0_193;
            break;
        case 3:
            Pu_U = pu_u_0_402;
            break;
        case 4:
            Pu_U = pu_u_0_672;
            break;
        case 5:
            Pu_U = pu_u_1_051;
            break;
        case 6:
            Pu_U = pu_u_1_604;
            break;
        case 7:
            Pu_U = pu_u_24_950;
            break;
        default:
            break;
    }

    emit update_table();
}
