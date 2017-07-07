#ifndef WDG_PARAMS_H
#define WDG_PARAMS_H

#include <QObject>
#include "QComboBox"
#include "QLineEdit"
#include "QTableWidget"
#include "service_types.h"

enum sig_type {
    REG = 0,
    RAND = 1
};

enum sig_time_dist {
    CONST = 0,
    LIN = 1,
    STEP = 2,
    EXP = 3,
    EXPLIN = 4,
    REACT = 5
};

class wdg_params : public QObject
{
    Q_OBJECT
public:
    explicit wdg_params(QObject *parent = nullptr);
    void set_reg_signal();
    void set_lin_signal();
    void set_step_signal();
    void set_exp_signal();
    void set_explin_signal();
    void set_react_signal();
    void create_rmod_qcb();
    void create_en_y_qcb();
    void create_enrch_qcb();
    void create_pu_u_qcb();

    QTableWidget *t_widget;
    sig_type s_type;
    sig_time_dist st_dist;
    r_models r_mod;

    double S;
    double A;
    double TT;
    double T;
    double Kt;
    double R0;
    double R;
    double Rt;
    yield_enum En_Y;
    enrichment_enum Enrch;
    pu_u_enum Pu_U;

private:
    const QString reg_F = "Частота, Гц";
    const QString reg_F0 = "Начальная частота, Гц";
    const QString reg_AF = "Скорость изменения частоты, Гц/с";
    const QString rand_C = "Скорость счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B9");
    const QString rand_C0 = "Начальная cкорость счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B9");
    const QString rand_AC = "Скорость изменения скорости счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B2");
    const QString period = "Период, с";
    const QString initial_period = "Начальный период, с";
    const QString t_coe = "Коэффициент периода";
    const QString r_model = "Модель";
    const QString s_r0 = "Начальная подкритичность, " + QString::fromUtf8("\u03B2");
    const QString s_r = "Величина ступеньки реактивности, " + QString::fromUtf8("\u03B2");
    const QString s_rt = "Время введения реактивности, с";
    const QString energy_yield = "Энерговыработка, МВт*сут/кг";
    const QString enrichment = "Обогащение, %";
    const QString s_pu_u = "Соотношение ядер Pu и U в топливе";
    const QString time = "Время теста, с";

    QStringList m_TableHeader;

    QComboBox *qcb_rmod_widget;
    QComboBox *qcb_en_y_widget;
    QComboBox *qcb_enrch_widget;
    QComboBox *qcb_pu_u_widget;
    QLineEdit *qle_widget;

    void init_table();
signals:
    void update_table();

public slots:
    void pulse_dist_change(int p_dist);
    void st_dist_change(int ext_st_dist);
    void slot_update();
    void rmod_update();
    void en_y_update();
    void enrch_update();
    void pu_u_update();
    void read_params_from_table();
};

#endif // WDG_PARAMS_H
