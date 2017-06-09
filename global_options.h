#ifndef GLOBAL_OPTIONS_H
#define GLOBAL_OPTIONS_H

#include <QDialog>

namespace Ui {
class global_options;
}

class global_options : public QDialog
{
    Q_OBJECT

public:
    explicit global_options(QWidget *parent = 0);
    ~global_options();

private:
    Ui::global_options *ui;
};

#endif // GLOBAL_OPTIONS_H
