#include "global_options.h"
#include "ui_global_options.h"

global_options::global_options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::global_options)
{
    ui->setupUi(this);
}

global_options::~global_options()
{
    delete ui;
}
