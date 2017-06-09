#ifndef FORM_OPTIONS_H
#define FORM_OPTIONS_H

#include <QDialog>
#include "pulseform.h"

namespace Ui {
class Form_Options;
}

class Form_Options : public QDialog
{
    Q_OBJECT

public:
    PulseForm form;
    explicit Form_Options(QWidget *parent = 0);
    ~Form_Options();

private:
    Ui::Form_Options *ui;
    double get_peak_amp(QVector<double> array);

private slots:
    void replot_form();
};

#endif // FORM_OPTIONS_H
