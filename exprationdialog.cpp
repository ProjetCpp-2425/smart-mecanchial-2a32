#include "exprationdialog.h"
#include "ui_exprationdialog.h"

exprationDialog::exprationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exprationDialog)
{
    ui->setupUi(this);
}

exprationDialog::~exprationDialog()
{
    delete ui;
}
