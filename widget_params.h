#ifndef WIDGET_PARAMS_H
#define WIDGET_PARAMS_H

#include "QComboBox"
#include "QLineEdit"
#include "QTableWidget"

class widget_params
{
private:
    const QString reg_F = "Частота, Гц";
    const QString reg_F0 = "Начальная частота, Гц";
    const QString reg_AF = "Скорость изменения частоты, Гц/с";
    const QString rand_C = "Скорость счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B9");
    const QString rand_C0 = "Начальная cкорость счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B9");
    const QString rand_AC = "Скорость изменения скорости счёта, с" + QString::fromUtf8("\u207B") + QString::fromUtf8("\u00B2");
    const QString period = "Период, с";
    const QString initial_period = "Начальный период, с";
    const QString k_t = "Коэффициент периода";
    const QString r_model = "Модель";
    const QString r0 = "Начальная подкритичность, " + QString::fromUtf8("\u03B2");
    const QString r = "Величина ступеньки реактивности, " + QString::fromUtf8("\u03B2");
    const QString r_t = "Время введения реактивности, с";
    const QString energy_yield = "Энерговыработка, МВт*сут/кг";
    const QString enrichment = "Обогащение, %";
    const QString pu_u = "Соотношение ядер Pu и U в топливе";

    QComboBox *qcb_widget;
    QLineEdit *qle_widget;
public:
    widget_params();

    QTableWidget *t_widget;
};

#endif // WIDGET_PARAMS_H
