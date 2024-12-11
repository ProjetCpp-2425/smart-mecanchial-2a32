#ifndef ADDEMPLOYEEFORM_H
#define ADDEMPLOYEEFORM_H

#include <QDialog>

namespace Ui {
class AddEmployeeForm;
}

class AddEmployeeForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddEmployeeForm(QWidget *parent = nullptr);
    ~AddEmployeeForm();

signals:
    void employeeAdded();

private slots:
    void onSaveButtonClicked();
    void resetForm();
    void onCancelButtonClicked();

private:
    Ui::AddEmployeeForm *ui;
};

#endif // ADDEMPLOYEEFORM_H
