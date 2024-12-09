#ifndef NOTIFICATIONDIALOG_H
#define NOTIFICATIONDIALOG_H

#include <QDialog>

namespace Ui {
class notificationDialog;
}

class notificationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit notificationDialog(QWidget *parent = nullptr);
    ~notificationDialog();

    // Method to set warning title and details
    void setWarningText(const QString &title, const QString &details);

private:
    Ui::notificationDialog *ui;
};

#endif // NOTIFICATIONDIALOG_H
