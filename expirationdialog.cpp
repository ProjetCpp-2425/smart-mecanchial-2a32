#include "expirationdialog.h"
#include "ui_expirationdialog.h"
#include <QSqlQuery>
#include <QDebug>
#include <QTextCharFormat>
#include <QDir>
#include "smtp.h"

expirationdialog::expirationdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::expirationdialog)
{
    ui->setupUi(this);
    connect(ui->Veh_combo_ID, &QComboBox::currentTextChanged, this, &expirationdialog::onVehicleSelectionChanged);
    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, this, &expirationdialog::on_calendarWidget_selectionChanged);
    clearCalendar();
}

expirationdialog::~expirationdialog()
{
    delete ui;
}

void expirationdialog::populateVehicleComboBox(const QStringList &vehicleIds)
{
    ui->Veh_combo_ID->clear();
    ui->Veh_combo_ID->addItems(vehicleIds);
}

void expirationdialog::onVehicleSelectionChanged(const QString &vehicleId)
{
    clearCalendar();
    QSqlQuery query;
    query.prepare("SELECT DATE_EXPIRATION_ASSURANCE, DATE_EXPIRATION_GARANTIE FROM VEHICULE WHERE ID_VEHICULE = :vehicleId");
    query.bindValue(":vehicleId", vehicleId);

    if (query.exec() && query.next()) {
        QDate insuranceExpiry = query.value(0).toDate();
        QDate warrantyExpiry = query.value(1).toDate();

        ui->Veh_dateEdit_Dateassurance->setDate(insuranceExpiry);
        ui->Veh_dateEdit_Dategarantie->setDate(warrantyExpiry);

        ui->calendarWidget->setSelectedDate(QDate::currentDate()); // Clear previous selection

        QTextCharFormat insuranceFormat;
        QTextCharFormat warrantyFormat;
        QTextCharFormat sameDateFormat;

        insuranceFormat.setForeground(Qt::black);
        insuranceFormat.setBackground(Qt::red);
        warrantyFormat.setForeground(Qt::black);
        warrantyFormat.setBackground(Qt::blue);
        sameDateFormat.setForeground(Qt::black);
        sameDateFormat.setBackground(Qt::cyan);

        if (insuranceExpiry == warrantyExpiry) {
            ui->calendarWidget->setDateTextFormat(insuranceExpiry, sameDateFormat);
        } else {
            ui->calendarWidget->setDateTextFormat(insuranceExpiry, insuranceFormat);
            ui->calendarWidget->setDateTextFormat(warrantyExpiry, warrantyFormat);
        }
    } else {
        qDebug() << "Failed to fetch expiration dates for vehicle ID:" << vehicleId;
    }
}

void expirationdialog::on_Veh_Cancel_Button_expiration_clicked()
{
    ui->Veh_dateEdit_Dategarantie->setDate(QDate::currentDate());
    ui->Veh_dateEdit_Dateassurance->setDate(QDate::currentDate());
    clearCalendar();
}

void expirationdialog::on_calendarWidget_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    QSqlQuery query;
    query.prepare("SELECT MODELE, MARQUE FROM VEHICULE WHERE DATE_EXPIRATION_ASSURANCE = :selectedDate OR DATE_EXPIRATION_GARANTIE = :selectedDate");
    query.bindValue(":selectedDate", selectedDate);

    if (query.exec() && query.next()) {
        QString vehicleModel = query.value("MODELE").toString();
        QString vehicleMarque = query.value("MARQUE").toString();
        ui->Veh_Label_GestionInfo2->setText("Véhicule: " + vehicleModel + ", Marque: " + vehicleMarque);
    } else {
        ui->Veh_Label_GestionInfo2->setText("Aucun véhicule pour la date sélectionnée.");
    }

}
void expirationdialog::clearCalendar()
{
    QTextCharFormat defaultFormat;
    ui->calendarWidget->setDateTextFormat(QDate(), defaultFormat);
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
}




void expirationdialog::on_Veh_push_Envoyer_clicked()
{
    QString selectedVehicleId = ui->Veh_combo_ID->currentText();
    if (selectedVehicleId.isEmpty()) {
        QMessageBox::warning(this, tr("No Vehicle Selected"), tr("Please select a vehicle before sending the email."));
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT MARQUE, MODELE, DATE_EXPIRATION_ASSURANCE, DATE_EXPIRATION_GARANTIE FROM VEHICULE WHERE ID_VEHICULE = :id");
    query.bindValue(":id", selectedVehicleId);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to retrieve vehicle information from the database."));
        return;
    }

    QString marque = query.value("MARQUE").toString();
    QString modele = query.value("MODELE").toString();
    QDate assuranceExpiry = query.value("DATE_EXPIRATION_ASSURANCE").toDate();
    QDate garantieExpiry = query.value("DATE_EXPIRATION_GARANTIE").toDate();

    QString subject = QString("Rappel des Dates d'Expiration pour le Véhicule ID: %1").arg(selectedVehicleId);
    QString message = QString(
        "Bonjour,\n\n"
        "Nous souhaitons vous rappeler les dates importantes liées au véhicule suivant :\n\n"
        "ID Véhicule : %1\n"
        "Marque : %2\n"
        "Modèle : %3\n"
        "Date d'Expiration de l'Assurance : %4\n"
        "Date d'Expiration de la Garantie : %5\n\n"
        "Veuillez prendre les mesures nécessaires pour renouveler les documents avant leur expiration.\n\n"
        "Cordialement,\n"
        "Votre système de gestion des véhicules."
    ).arg(selectedVehicleId)
     .arg(marque)
     .arg(modele)
     .arg(assuranceExpiry.toString("dd-MMMM-yyyy"))
     .arg(garantieExpiry.toString("dd-MMMM-yyyy"));

    QString recipient = "aifatmaandslim@gmail.com";//houni l mail li theb tebaathou rappel
    Smtp* smtp = new Smtp("zeineb.elabed012345@gmail.com", "bsvwzaifsekguowf", "smtp.gmail.com", 465);

    connect(smtp->getSocket(), &QAbstractSocket::disconnected, smtp, &Smtp::deleteLater);

    // Send the email
    smtp->sendMail("zeineb.elabed012345@gmail.com", recipient, subject, message);

    // Notify the user immediately after sendMail
    QMessageBox::information(this, tr("Success"), tr("Message sent successfully!"));

}

