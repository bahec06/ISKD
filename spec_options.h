#ifndef SPEC_OPTIONS_H
#define SPEC_OPTIONS_H

#include <QDialog>
#include "spectrum.h"
#include <QTableWidgetItem>

namespace Ui {
class spec_options;
}

class spec_options : public QDialog
{
    Q_OBJECT

public:
    Spectrum spec;
    explicit spec_options(QWidget *parent = 0);
    ~spec_options();

private:
    QTableWidgetItem *spec_item0;
    QTableWidgetItem *spec_item1;
    QTableWidgetItem *spec_item2;
    QTableWidgetItem *spec_vh_item0;
    QTableWidgetItem *spec_vh_item1;
    QTableWidgetItem *spec_vh_item2;
    double q_const;
    double q_min;
    double q_max;
    double q_mean;
    double q_sigma;
    double l;
    double get_peak_amp(QVector<double> array);
    Ui::spec_options *ui;

private slots:
    void change_spec(int index);
    void replot_spec();
};

#endif // SPEC_OPTIONS_H
