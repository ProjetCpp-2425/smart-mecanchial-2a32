#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connexion.h"
#include <QCalendarWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QHttpMultiPart>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QEventLoop>
#include <QPdfWriter>
#include <QPainter>
#include <QTcpSocket>
#include <QTextStream>
#include <QCoreApplication>
#include <QWidget>
#include <QChart>
#include <QtCharts/QChartView>
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
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QDir>
#include "smtp.h"
#include <QDir>
#include "vehicule.h"
#include "QSqlQuery"
#include "notificationdialog.cpp"
#include "notificationdialog.h"
#include "arduino.h"
#include "expirationdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      expirationDialogInstance(new expirationdialog(this)),
      arduino(new Arduino(this))  // Use 'arduino' instead of 'arduinoInstance'
{
    ui->setupUi(this);
    connect(ui->Vehicule, &QTabWidget::currentChanged, this, &MainWindow::clearFields);
    ui->Veh_combo_ID_Main->addItem("Select ID");  // Placeholder
    ui->Veh_combo_ID_Main->setModel(V.afficherIdV());
    ui->Veh_TableView->setModel(V.afficherV());
    connect(ui->Veh_push_Ajouter, &QPushButton::clicked, this, &MainWindow::clearFields1);
    connect(ui->Veh_push_Supprimer, &QPushButton::clicked, this, &MainWindow::clearFields1);
    connect(ui->Veh_push_Modifier, &QPushButton::clicked, this, &MainWindow::clearFields1);
    Veh_choix_pie();
    show();
    verifierEfficaciteVehicule();//pour les alerts
    ui->Veh_line_ID->setValidator(new QIntValidator(0, 99999999, this));
    ui->Veh_dateEdit_DateVehicule->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dategarantie->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dateassurance->setDate(QDate::currentDate());

    connect(arduino, &Arduino::dataReceived, this, &MainWindow::handleArduinoData);

        // Attempt to connect to Arduino
        if (arduino->connectArduino()) {  // Use 'arduino' instead of 'arduinoInstance'
            qDebug() << "Arduino connected successfully.";
        } else {
            qDebug() << "Failed to connect to Arduino.";
        }



}


MainWindow::~MainWindow() {
    delete ui;
    if (arduino) {  // Use 'arduino' instead of 'arduinoInstance'
           arduino->disconnectArduino();
       }

}

void MainWindow::handleArduinoData(const QString &data)
{
    // Handle the data received from Arduino
    qDebug() << "Data received from Arduino: " << data;

    // Trim any leading/trailing whitespace from the received data
    QString trimmedData = data.trimmed();

    // Query the database to check if the id_vehicule exists
    QSqlQuery query;
    query.prepare("SELECT * FROM vehicule WHERE id_vehicule = :id");
    query.bindValue(":id", trimmedData);  // Bind the trimmed data to the query

    if (query.exec()) {
        if (query.next()) {  // If a record is found matching the id_vehicule
            QString modele = query.value("modele").toString();  // Get the model of the vehicle
            ui->labelModel->setText(modele);  // Display the model in the label

            // Show a message box to inform that the vehicle ID was found
            QMessageBox::information(this, "ID Found", "Vehicule " + modele);

            // Send the message to Arduino (this sends the message to the LCD)
            QString lcdMessage = "Vehicule " + modele;
            arduino->writeData(lcdMessage.toUtf8());  // Send message to Arduino

        } else {
            // If no record is found, show a message box
            QMessageBox::warning(this, "ID Not Found", "No vehicule found with this ID.");

            // Send the "ID Not Found" message to Arduino
            arduino->writeData("ID Not Found\n");
        }
    } else {
        // Handle SQL query error
        qDebug() << "Error executing query: " << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to query the database.");

        // Send the error message to Arduino
        arduino->writeData("Error: SQL Query Failed\n");
    }
}


void MainWindow::on_Veh_push_Ajouter_clicked() {
    int ID = ui->Veh_line_ID->text().toInt();
    QString MARQUE = ui->Veh_line_Marque->text();
    QString MODELE = ui->Veh_line_Modele->text();
    QString COULEUR = ui->Veh_line_Couleur->text();
    QString ETAT = ui->Veh_combo_Etat->currentText();
    QDate DATEVEHICULE = ui->Veh_dateEdit_DateVehicule->date();
    QDate DATEGARANTIE = ui->Veh_dateEdit_Dategarantie->date();
    QDate DATEASSURANCE = ui->Veh_dateEdit_Dateassurance->date();
    int PARCOURS = ui->Veh_line_Parcours->text().toInt();

    if (ui->Veh_line_ID->text().isEmpty() || MARQUE.isEmpty() || MODELE.isEmpty() || COULEUR.isEmpty() || ETAT.isEmpty()) {
        ui->Veh_Label_GestionInfo->setText("Erreur de contrôle de saisie");
        return;
    }

    Vehicule V(ID, MARQUE, MODELE, COULEUR, DATEVEHICULE, ETAT, PARCOURS, DATEASSURANCE, DATEGARANTIE);
    bool test = V.ajouterV();
    if (test) {
        ui->Veh_Label_GestionInfo->setText("Ajout effectué");
        ui->Veh_combo_ID_Main->setModel(V.afficherIdV());
        ui->Veh_TableView->setModel(V.afficherV());
    } else {
        ui->Veh_Label_GestionInfo->setText("Ajout non effectué");
    }
    clearFields1();
}


void MainWindow::on_Veh_push_Modifier_clicked() {
    int ID = ui->Veh_line_ID->text().toInt();
    QString MARQUE = ui->Veh_line_Marque->text();
    QString MODELE = ui->Veh_line_Modele->text();
    QString COULEUR = ui->Veh_line_Couleur->text();
    QString ETAT = ui->Veh_combo_Etat->currentText();
    QDate DATEVEHICULE = ui->Veh_dateEdit_DateVehicule->date();
    QDate DATEGARANTIE = ui->Veh_dateEdit_Dategarantie->date();
    QDate DATEASSURANCE = ui->Veh_dateEdit_Dateassurance->date();
    int PARCOURS = ui->Veh_line_Parcours->text().toInt();

    if (ui->Veh_line_ID->text().isEmpty() || MARQUE.isEmpty() || MODELE.isEmpty() || COULEUR.isEmpty() || ETAT.isEmpty()) {
        ui->Veh_Label_GestionInfo->setText("Erreur de contrôle de saisie");
        return;
    }

    Vehicule V(ID, MARQUE, MODELE, COULEUR, DATEVEHICULE, ETAT, PARCOURS, DATEASSURANCE, DATEGARANTIE);
    bool test = V.modifierV();
    if (test) {
        ui->Veh_Label_GestionInfo->setText("Modification effectuée");
        ui->Veh_combo_ID_Main->setModel(V.afficherIdV());
        ui->Veh_TableView->setModel(V.afficherV());
    } else {
        ui->Veh_Label_GestionInfo->setText("Modification non effectuée");
    }
    clearFields1();
}


void MainWindow::on_Veh_push_Supprimer_clicked() {
    Vehicule V;
    V.setId(ui->Veh_combo_ID_Main->currentText().toInt());
    bool test = V.supprimerV(V.getId());
    if (test) {
        ui->Veh_Label_GestionInfo->setText("Suppression effectuée");
        ui->Veh_TableView->setModel(V.afficherV());
        ui->Veh_combo_ID_Main->setModel(V.afficherIdV());
    } else {
        ui->Veh_Label_GestionInfo->setText("Suppression non effectuée");
    }
}

void MainWindow::on_Veh_push_TriID_clicked() {
    V.viderTableV(ui->Veh_TableView);
    ui->Veh_Label_ListeInfo->setText("Tri par ID effectué");
    ui->Veh_TableView->setModel(V.trierParIdV());
}

void MainWindow::on_Veh_push_TriMarque_clicked() {
    V.viderTableV(ui->Veh_TableView);
    ui->Veh_Label_ListeInfo->setText("Tri par Marque effectué");
    ui->Veh_TableView->setModel(V.trierParMarqueV());
}


void MainWindow::on_Veh_line_Recherche_textChanged(const QString &arg1) {
    ui->Veh_Label_ListeInfo->clear();
    V.viderTableV(ui->Veh_TableView);
    V.rechercherV(ui->Veh_TableView, arg1);
}
void MainWindow::Veh_choix_pie() {
    QChartView *chartView;
    QSqlQuery query;
    qreal total = 0, enServiceCount = 0, enMaintenanceCount = 0, horsServiceCount = 0;

    query.prepare("SELECT COUNT(*) FROM VEHICULE");
    if (query.exec() && query.next()) {
        total = query.value(0).toDouble();
    }

    query.prepare("SELECT ETAT, COUNT(*) FROM VEHICULE GROUP BY ETAT");
    if (query.exec()) {
        while (query.next()) {
            QString etat = query.value(0).toString();
            int count = query.value(1).toInt();

            if (etat == "En Service") {
                enServiceCount = count;
            } else if (etat == "En Maintenance") {
                enMaintenanceCount = count;
            } else if (etat == "Hors Service") {
                horsServiceCount = count;
            }
        }
    }

    qreal enServiceRatio = (total > 0) ? (enServiceCount / total) : 0;
    qreal enMaintenanceRatio = (total > 0) ? (enMaintenanceCount / total) : 0;
    qreal horsServiceRatio = (total > 0) ? (horsServiceCount / total) : 0;

    // Create the pie chart series
    QPieSeries *series = new QPieSeries();
    series->append("En Service", enServiceRatio);
    series->append("En Maintenance", enMaintenanceRatio);
    series->append("Hors Service", horsServiceRatio);

    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible();
        slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
    }


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Véhicules par État");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignRight);

    chartView = new QChartView(chart, ui->Veh_label_Stats);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(1040, 570);
    chartView->show();
}


void MainWindow::on_Veh_push_UpdateStats_clicked() {
    Veh_choix_pie();
    show();
}

void MainWindow::clearFields() {
    ui->Veh_line_ID->clear();
    ui->Veh_line_Marque->clear();
    ui->Veh_line_Modele->clear();
    ui->Veh_line_Couleur->clear();
    ui->Veh_combo_Etat->setCurrentIndex(0);
    ui->Veh_dateEdit_DateVehicule->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dategarantie->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dateassurance->setDate(QDate::currentDate());
    ui->Veh_Label_ListeInfo->clear();
    ui->Veh_combo_ID_Main->clear();
    ui->Veh_Label_GestionInfo->clear();
}

void MainWindow::clearFields1() {
    ui->Veh_line_ID->clear();
    ui->Veh_line_Marque->clear();
    ui->Veh_line_Modele->clear();
    ui->Veh_line_Couleur->clear();
    ui->Veh_combo_Etat->setCurrentIndex(0);
    ui->Veh_dateEdit_DateVehicule->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dategarantie->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dateassurance->setDate(QDate::currentDate());
    ui->Veh_Label_ListeInfo->clear();
}


void MainWindow::on_Veh_push_ExportPDF_clicked() {
    QString currentPath = QDir::currentPath();
    QString pdfPath = currentPath + "/Liste-Vehicules.pdf";
    QPdfWriter pdf(pdfPath);
    QPainter painter(&pdf);
    int i = 4000;
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 30));
    painter.drawText(3000, 1500, "LISTE DES VEHICULES");
    painter.setPen(Qt::cyan);
    painter.setFont(QFont("Arial", 50));
    painter.drawRect(2700, 200, 6300, 2600);//(X, Y, H, W)
    painter.drawRect(0, 3000, 9600, 500);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9));
    painter.drawText(300, 3300, "ID");
    painter.drawText(1100, 3300, "Marque");
    painter.drawText(2300, 3300, "Modele");
    painter.drawText(3500, 3300, "Couleur");
    painter.drawText(4700, 3300, "Date Vehicule");
    painter.drawText(6300, 3300, "Etat");
    painter.drawText(8000, 3300, "Parcours (KM)");

    QSqlQuery query;
    query.prepare("SELECT ID_VEHICULE, MARQUE, MODELE, COULEUR, TO_CHAR(DATE_VEHICULE, 'DD/MM/YYYY') AS DATE_VEHICULE, ETAT, PARCOURS_KM FROM VEHICULE");
    query.exec();

    while (query.next()) {
        qDebug() << "ETAT value: " << query.value(5).toString(); // Debugging output
        painter.drawText(300, i, query.value(0).toString());      // ID
        painter.drawText(1100, i, query.value(1).toString());    // Marque
        painter.drawText(2300, i, query.value(2).toString());    // Modele
        painter.drawText(3500, i, query.value(3).toString());    // Couleur
        painter.drawText(4700, i, query.value(4).toString());    // Date Vehicule
        painter.drawText(6300, i, query.value(5).toString());   // ETAT
        painter.drawText(8000, i, query.value(6).toString());   // Parcours (KM)
        i += 500;
    }

    int reponse = QMessageBox::question(this, "PDF généré", "Afficher le PDF ?", QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath));
    }
    painter.end();
}



void MainWindow::on_Veh_Cancel_Button_clicked()
{

    ui->Veh_line_ID->clear();
    ui->Veh_line_Marque->clear();
    ui->Veh_line_Modele->clear();
    ui->Veh_line_Couleur->clear();
    ui->Veh_combo_Etat->setCurrentIndex(0);
    ui->Veh_dateEdit_DateVehicule->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dategarantie->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dateassurance->setDate(QDate::currentDate());
    ui->Veh_line_Parcours->clear();
    ui->Veh_Label_ListeInfo->clear();
    ui->Veh_Label_GestionInfo->clear();
}
void MainWindow::on_Veh_push_Fermer_clicked()
{
    QApplication::quit();
}

void MainWindow::verifierEfficaciteVehicule() {
    qDebug() << "Running verifierEfficaciteVehicule...";

    QSqlQuery query;
    query.prepare("SELECT ID_VEHICULE, MARQUE, MODELE, PARCOURS_KM, ETAT FROM VEHICULE");
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }
    while (query.next()) {
        int id = query.value(0).toInt();
        QString marque = query.value(1).toString();
        QString modele = query.value(2).toString();
        int km = query.value(3).toInt();
        QString etat = query.value(4).toString();

        if (km > 15000) {
            QString alert = QString("Le véhicule %1 (%2 %3) a parcouru plus de 15 000 km (%4 km).")
                .arg(id).arg(marque).arg(modele).arg(km);
            alertQueue.enqueue(alert);
        }

        if (etat.toLower() == "hors service") {
            QString alert = QString("Le véhicule %1 (%2 %3) est marqué comme 'hors service'.")
                .arg(id).arg(marque).arg(modele);
            alertQueue.enqueue(alert);
        }

        if (etat.toLower() == "en maintenance") {
            QString alert = QString("Le véhicule %1 (%2 %3) est en maintenance.")
                .arg(id).arg(marque).arg(modele);
            alertQueue.enqueue(alert);
        }
    }
    if (alertQueue.isEmpty()) {
        qDebug() << "No alerts generated.";
    } else {
        processNextAlert();
    }
}
void MainWindow::processNextAlert() {
    static QStringList processedKmAlerts;
    static QStringList processedMaintenanceAlerts;

    if (alertQueue.isEmpty()) {
        saveAlertsToPDF(processedKmAlerts, processedMaintenanceAlerts);
        processedKmAlerts.clear();
        processedMaintenanceAlerts.clear();
        return;
    }

    QString nextAlert = alertQueue.dequeue();

    QString title;
    QString details = nextAlert;

    if (nextAlert.contains("parcouru", Qt::CaseInsensitive)) {
        processedKmAlerts.append(nextAlert);
        title = "Alerte de Parcours (KM)";
    } else if (nextAlert.contains("maintenance", Qt::CaseInsensitive)) {
        processedMaintenanceAlerts.append(nextAlert);
        title = "Alerte de Maintenance";
    } else {
        title = "Alerte Générale";
    }


    notificationDialog *dialog = new notificationDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete dialog after closing

    connect(dialog, &notificationDialog::finished, this, &MainWindow::processNextAlert);
    dialog->setWarningText(title, details);
    dialog->exec();
}

void MainWindow::saveAlertsToPDF(const QStringList &kmAlerts, const QStringList &maintenanceAlerts) {

    /*QString folderPath = "C:/Users/Slim/Desktop/Autocare-Manager-Gestion- Vehicule";
    QDir dir(folderPath);
    if (!dir.exists()) {
        if (!dir.mkpath(folderPath)) {
            qDebug() << "Failed to create directory:" << folderPath;
            return;
        }
    }*/
    QString currentPath = QDir::currentPath();
    QString pdfPath = currentPath + "/Rapport-Vehicules.pdf";
    QPdfWriter pdf(pdfPath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdf);

    int xStart = 500;
    int yStart = 1500;
    int titleSpacing = 1000;
    int subsectionSpacing = 600;
    int lineSpacing = 500;


    painter.setPen(QColor(0, 51, 102));
    painter.setFont(QFont("Arial", 28, QFont::Bold));
    painter.drawText(xStart, yStart, "Rapport d'Alerte des Véhicules");
    yStart += titleSpacing;


    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.setPen(QColor(0, 102, 204));
    painter.drawText(xStart, yStart, "Alertes de Parcours (KM):");
    yStart += subsectionSpacing;

    painter.setPen(QColor(0, 102, 204));
    painter.drawLine(xStart, yStart - 200, pdf.width() - xStart, yStart - 200);
    yStart += lineSpacing;


    painter.setFont(QFont("Arial", 14));
    painter.setPen(Qt::black);
    if (kmAlerts.isEmpty()) {
        painter.drawText(xStart, yStart, "Aucune alerte de parcours à afficher.");
        yStart += lineSpacing;
    } else {
        for (const QString &alert : kmAlerts) {
            painter.drawText(xStart, yStart, alert);
            yStart += lineSpacing;
        }
    }
    yStart += subsectionSpacing;
    painter.setFont(QFont("Arial", 20, QFont::Bold));
    painter.setPen(QColor(0, 102, 204)); // Slightly dark blue
    painter.drawText(xStart, yStart, "Alertes de Maintenance:");
    yStart += subsectionSpacing;

    painter.setPen(QColor(0, 102, 204)); // Slightly dark blue
    painter.drawLine(xStart, yStart - 200, pdf.width() - xStart, yStart - 200); // Line
    yStart += lineSpacing;
    painter.setFont(QFont("Arial", 14));
    painter.setPen(Qt::black);
    if (maintenanceAlerts.isEmpty()) {
        painter.drawText(xStart, yStart, "Aucune alerte de maintenance à afficher.");
    } else {
        for (const QString &alert : maintenanceAlerts) {
            painter.drawText(xStart, yStart, alert);
            yStart += lineSpacing;
        }
    }

    painter.end();
}


void MainWindow::on_Veh_push_dashboard_clicked()
{
    QSqlQuery query("SELECT ID_VEHICULE FROM VEHICULE");
            QStringList vehicleIds;
            while (query.next()) {
                vehicleIds << query.value(0).toString();
            }
    expirationDialogInstance = new expirationdialog();
    expirationDialogInstance->setWindowTitle("Dashboard");
    expirationDialogInstance->populateVehicleComboBox(vehicleIds);
    expirationDialogInstance->show();
}

void MainWindow::on_Veh_combo_ID_Main_currentIndexChanged(int index)
{
    int id = ui->Veh_combo_ID_Main->currentText().toInt();
    Q_UNUSED(index);
    QString id_1 = QString::number(id);
    QSqlQuery query;
    query.prepare("SELECT * FROM VEHICULE WHERE ID_VEHICULE = :id");
    query.bindValue(":id", id_1);

    if (query.exec()) {
        while (query.next()) {
            ui->Veh_line_ID->setText(query.value(0).toString());
            ui->Veh_line_Marque->setText(query.value(1).toString());
            ui->Veh_line_Modele->setText(query.value(2).toString());
            ui->Veh_line_Couleur->setText(query.value(3).toString());
            ui->Veh_dateEdit_DateVehicule->setDate(query.value(4).toDate());
            ui->Veh_combo_Etat->setCurrentText(query.value(5).toString());
            ui->Veh_line_Parcours->setText(query.value(6).toString());
            ui->Veh_dateEdit_Dateassurance->setDate(query.value(7).toDate());
            ui->Veh_dateEdit_Dategarantie->setDate(query.value(8).toDate());
        }
    } else {
        ui->Veh_Label_GestionInfo->setText("Echec de chargement");
    }
}

