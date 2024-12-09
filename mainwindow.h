#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vehicule.h>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QQueue>
#include "expirationdialog.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QTimer>
#include "arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QTcpSocket;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(QString, QString, QMainWindow *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Veh_push_Ajouter_clicked();
    void on_Veh_push_Modifier_clicked();
    void on_Veh_push_Supprimer_clicked();
    void on_Veh_push_TriID_clicked();
    void on_Veh_push_TriMarque_clicked();
    void on_Veh_line_Recherche_textChanged(const QString &arg1);
    void on_Veh_push_UpdateStats_clicked();
    void on_Veh_push_ExportPDF_clicked();
    void on_Veh_push_Fermer_clicked();
    void Veh_choix_pie();
    void on_Veh_Cancel_Button_clicked();
    void verifierEfficaciteVehicule();
    void saveAlertsToPDF(const QStringList &kmAlerts, const QStringList &maintenanceAlerts);
    void on_Veh_push_dashboard_clicked();
    void on_Veh_combo_ID_Main_currentIndexChanged(int index); // Traiter la matricule reçue
    void handleArduinoData(const QString &data);


private:
    Ui::MainWindow *ui;
    Vehicule V;
    Arduino *arduino;
    expirationdialog *expirationDialogInstance;
     QSet<int> alertedVehicles;
     QQueue<QString> alertQueue; // Queue to hold alerts
    void processNextAlert();
    void clearFields();
    void clearFields1();

};

#endif // MAINWINDOW_H
