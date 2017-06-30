#include "wdg_params.h"

wdg_params::wdg_params(QObject *parent) : QObject(parent)
{
    qle_widget = new QLineEdit();

    qcb_widget = new QComboBox();
    qcb_widget->addItem("РБМК");
    qcb_widget->addItem("БН");
    qcb_widget->addItem("ВВЭР");

    t_widget = new QTableWidget();
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

    t_widget->setCellWidget(0, 0, qcb_widget);
    t_widget->setItem(1, 0, new QTableWidgetItem(QString::number(S)));
    t_widget->setItem(2, 0, new QTableWidgetItem(QString::number(R0)));
    t_widget->setItem(3, 0, new QTableWidgetItem(QString::number(R)));
    t_widget->setItem(4, 0, new QTableWidgetItem(QString::number(Rt)));

    switch(r_mod) {
        case 0:
            t_widget->setRowCount(8);
            m_TableHeader << r_model << sig_param << s_r0 << s_r << s_rt << energy_yield << enrichment << time;
            t_widget->setItem(5, 0, new QTableWidgetItem(QString::number(En_Y)));
            t_widget->setItem(6, 0, new QTableWidgetItem(QString::number(Enrch)));
            t_widget->setItem(7, 0, new QTableWidgetItem(QString::number(TT)));
            break;
        case 1:
            t_widget->setRowCount(7);
            m_TableHeader << r_model << sig_param << s_r0 << s_r << s_rt << s_pu_u << time;
            t_widget->setItem(5, 0, new QTableWidgetItem(QString::number(Pu_U)));
            t_widget->setItem(6, 0, new QTableWidgetItem(QString::number(TT)));
            break;
        case 2:
            t_widget->setRowCount(6);
            m_TableHeader << r_model << sig_param << s_r0 << s_r << s_rt << time;
            t_widget->setItem(5, 0, new QTableWidgetItem(QString::number(TT)));
            break;
        default:
            break;
    }

    t_widget->setVerticalHeaderLabels(m_TableHeader);
}

void wdg_params::read_params_from_table(int model_index) {
    switch(model_index){
        case 0:
            S = t_widget->item(0, 0)->text().toDouble();
            TT = t_widget->item(1, 0)->text().toDouble();
            break;
        case 1:
            S = t_widget->item(0, 0)->text().toDouble();
            A = t_widget->item(1, 0)->text().toDouble();
            TT = t_widget->item(2, 0)->text().toDouble();
            break;
        case 2:

            break;
        case 3:
            S = t_widget->item(0, 0)->text().toDouble();
            T = t_widget->item(1, 0)->text().toDouble();
            TT = t_widget->item(2, 0)->text().toDouble();
            break;
        case 4:
            S = t_widget->item(0, 0)->text().toDouble();
            T = t_widget->item(1, 0)->text().toDouble();
            Kt = t_widget->item(2, 0)->text().toDouble();
            TT = t_widget->item(3, 0)->text().toDouble();
            break;
        case 5:
            switch(qcb_widget->currentIndex()) {
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
            S = t_widget->item(1, 0)->text().toDouble();
            R0 = t_widget->item(2, 0)->text().toDouble();
            R = t_widget->item(3, 0)->text().toDouble();
            Rt = t_widget->item(4, 0)->text().toDouble();
            switch(r_mod) {
                case 0:
                    En_Y = t_widget->item(5, 0)->text().toDouble();
                    Enrch = t_widget->item(6, 0)->text().toDouble();
                    TT = t_widget->item(7, 0)->text().toDouble();
                    break;
                case 1:
                    Pu_U = t_widget->item(5, 0)->text().toDouble();
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
}

