#ifndef FORM_OPTIONS_H
#define FORM_OPTIONS_H

#include <QDialog>
#include "pulseform.h"
#include "xml_rw.h"

namespace Ui {
class Form_Options;
}

class Form_Options : public QDialog
{
    Q_OBJECT

public:
    PulseForm form;
    QString s_dur;
    QString s_form;
    explicit Form_Options(QWidget *parent = 0);
    ~Form_Options();

private:
    Ui::Form_Options *ui;
    xml_rw *xrw;
    const QString fname = "D:/Qt_repo/form.xml";
    const QString opt_name = "f_opt";
    QVector<QString> tag_name = {"form_id", "form_duration"};
    double get_peak_amp(QVector<double> array);

private slots:
    void replot_form();

signals:
    void update_form();
};

#endif // FORM_OPTIONS_H
