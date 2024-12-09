#ifndef EXPRATIONDIALOG_H
#define EXPRATIONDIALOG_H

#include <QDialog>

namespace Ui {
class exprationDialog;
}

class exprationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit exprationDialog(QWidget *parent = nullptr);
    ~exprationDialog();

private:
    Ui::exprationDialog *ui;
};

#endif // EXPRATIONDIALOG_H
