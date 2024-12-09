#include "notificationdialog.h"
#include "ui_notificationdialog.h"
#include <QDebug>

notificationDialog::notificationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::notificationDialog)
{
    ui->setupUi(this);
    ui->WarningText->setReadOnly(true);
}

notificationDialog::~notificationDialog()
{
    delete ui;
}

void notificationDialog::setWarningText(const QString &title, const QString &details)
{
    ui->Warninglabel->setText(title);
    ui->WarningText->setText(details);
}
