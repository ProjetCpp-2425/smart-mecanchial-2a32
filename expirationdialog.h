#ifndef EXPIRATIONDIALOG_H
#define EXPIRATIONDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QCalendarWidget>
#include <QDate>

namespace Ui {
class expirationdialog;
}

class expirationdialog : public QDialog
{
    Q_OBJECT

public:
    explicit expirationdialog(QWidget *parent = nullptr);
    ~expirationdialog();

    void populateVehicleComboBox(const QStringList &vehicleIds);
    void clearCalendar();

private:
    Ui::expirationdialog *ui;

public slots:
    void onVehicleSelectionChanged(const QString &vehicleId);

private slots:
    void on_Veh_Cancel_Button_expiration_clicked();
    void on_calendarWidget_selectionChanged();
    void on_Veh_push_Envoyer_clicked();
};

#endif // EXPIRATIONDIALOG_H
