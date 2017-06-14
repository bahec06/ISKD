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
}
