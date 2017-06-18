#include "widget_params.h"

widget_params::widget_params()
{
    qle_widget = new QLineEdit();

    qcb_widget = new QComboBox();
    qcb_widget->addItem("РБМК");
    qcb_widget->addItem("БН");
    qcb_widget->addItem("ВВЭР");
    qcb_widget->addItem("Произвольная");

    t_widget = new QTableWidget();
    //reg_const();
}

void widget_params::reg_const()
{
    t_widget->clear();

    h_list << reg_F;
    t_widget->setVerticalHeaderLabels(h_list);
}
