#include "wdg_params.h"

wdg_params::wdg_params(QObject *parent) : QObject(parent)
{
    qle_widget = new QLineEdit();

    create_rmod_qcb();
    create_en_y_qcb();
    create_enrch_qcb();
    create_pu_u_qcb();
    create_step_qpb();

    t_widget = new QTableWidget();
    param_file = new xml_rw();

    read_params_from_file();

    connect(this, SIGNAL(update_table()), this, SLOT(slot_update()));
}

void wdg_params::read_params_from_file() {
       QMap<QString, QString> map;

       Si.clear();
       TTi.clear();
       map = param_file->read_opt_file(fname, "step");
       for(int i = 0; i < map.size(); i ++) {
       if(i < map.size()/2) {
       Si << map.values().at(i).toDouble();
       }
       else {
       TTi << map.values().at(i).toDouble();
       }
       }
       cnt = 1 + map.size()/2;

       map = param_file->read_opt_file(fname, "global");

       int i_rmod = map["rmod"].toInt();
       qcb_rmod_widget->setCurrentIndex(i_rmod);

       int i_st_dist = map["st_dist"].toInt();
       st_dist = (sig_time_dist)i_st_dist;

       int i_sig_type = map["sig_type"].toInt();
       s_type = (sig_type)i_sig_type;

       TT = map["time"].toDouble();

       S = map["S"].toDouble();
       A = map["A"].toDouble();
       T = map["T"].toDouble();
       Kt = map["Kt"].toDouble();
       R0 = map["R0"].toDouble();
       R = map["R"].toDouble();
       Rt = map["Rt"].toDouble();
       En_Y = (yield_enum)(map["En_Y"].toInt());
       Enrch = (enrichment_enum)(map["Enrch"].toInt());
       Pu_U = (pu_u_enum)(map["Pu_U"].toInt());
   }

void wdg_params::create_step_qpb() {
    qpb_add_step = new QPushButton();

    qpb_add_step->setText("+");
    connect(qpb_add_step, SIGNAL(pressed()), this, SLOT(add_new_step()));
}

void wdg_params::add_new_step() {
    t_widget->insertRow(t_widget->rowCount()-1);
    cnt++;
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
    create_step_qpb();
    t_widget->clear();
    t_widget->setColumnCount(2);

    m_TableHeader.clear();
    QString sig_param;
    if(s_type == REG) {
        sig_param = reg_F;
    }
    else {
        sig_param = rand_C;
    }
    m_TableHeader << sig_param << "Время теста, с";
    t_widget->setHorizontalHeaderLabels(m_TableHeader);

    t_widget->setRowCount(cnt);
    t_widget->setSpan(cnt-1,0,1,2);
    m_TableHeader.clear();
    m_TableHeader << QString::number(cnt);

    for(int i = 0; i < cnt-1; i ++) {
        t_widget->setItem(i, 0, new QTableWidgetItem(QString::number(Si.at(i))));
        t_widget->setItem(i, 1, new QTableWidgetItem(QString::number(TTi.at(i))));
    }

    t_widget->setCellWidget(cnt-1, 0, qpb_add_step);
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
    QFile* file = new QFile(fname);
    file->open(QIODevice::WriteOnly);
    QXmlStreamWriter xml_strm(file);

    xml_strm.setAutoFormatting(true);
    xml_strm.writeStartDocument();
    xml_strm.writeStartElement("params");
    bool empty_cell = false;
    QTableWidgetItem *itm;
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
            for(int i = 0; i < cnt-1; i++) {
                itm = t_widget->item(i, 0);
                if(itm == NULL) {
                    empty_cell = true;
                    break;
                }
                itm = t_widget->item(i, 1);
                if(itm == NULL) {
                    empty_cell = true;
                    break;
                }
            }

            if(!empty_cell) {
                Si.clear();
                TTi.clear();

                for(int i = 0; i < cnt-1; i++) {
                    Si << t_widget->item(i, 0)->text().toDouble();
                    TTi << t_widget->item(i, 1)->text().toDouble();
                }
            }
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
                case RBMK:
                    TT = t_widget->item(7, 0)->text().toDouble();
                    break;
                case VVR:
                    TT = t_widget->item(6, 0)->text().toDouble();
                    break;
                case BN:
                    TT = t_widget->item(5, 0)->text().toDouble();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    QVector<QString> tag_value;
    tag_value.clear();
    tag_value << QString::number(r_mod);
    tag_value << QString::number(st_dist);
    tag_value << QString::number(s_type);
    tag_value << QString::number(TT);
    tag_value << QString::number(S);
    tag_value << QString::number(A);
    tag_value << QString::number(T);
    tag_value << QString::number(Kt);
    tag_value << QString::number(R0);
    tag_value << QString::number(R);
    tag_value << QString::number(Rt);
    tag_value << QString::number(En_Y);
    tag_value << QString::number(Enrch);
    tag_value << QString::number(Pu_U);

    xml_strm.writeStartElement("global");
    for(int i = 0; i < global_tag.size(); i ++) {
        xml_strm.writeTextElement(global_tag[i], tag_value[i]);
    }

    QVector<QString> step_tag;
    step_tag.clear();
    tag_value.clear();
    for(int i = 0; i < Si.size(); i ++) {
        step_tag << "S"+QString::number(i);
        step_tag << "T"+QString::number(i);
        tag_value << QString::number(Si[i]);
        tag_value << QString::number(TTi[i]);
    }

    xml_strm.writeStartElement("step");
    for(int i = 0; i < step_tag.size(); i ++) {
        xml_strm.writeTextElement(step_tag[i], tag_value[i]);
    }

    xml_strm.writeEndElement();
    xml_strm.writeEndDocument();
    file->close();
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
    //read_params_from_file();
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
