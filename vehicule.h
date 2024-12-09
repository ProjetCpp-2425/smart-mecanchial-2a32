#ifndef VEHICULE_H
#define VEHICULE_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QTableView>
class Vehicule
{
private:
    int ID, PARCOURS;
    QString MARQUE, MODELE, COULEUR, ETAT;
    QDate DATEVEHICULE, DATE_EXPIRATION_ASSURANCE,DATE_EXPIRATION_GARANTIE;

public:
   Vehicule();
   Vehicule(int, QString, QString, QString, QDate, QString, int, QDate, QDate);


   int getId() { return ID; };
   QString getMarque() { return MARQUE; };
   QString getModele() { return MODELE; };
   QString getCouleur() { return COULEUR; };
   QDate getDateVehicule() { return DATEVEHICULE; };
   QString getEtat() { return ETAT; };
   int getParcours() { return PARCOURS; };
   QDate getDateExpirationAssurance() { return DATE_EXPIRATION_ASSURANCE; };
   QDate getDateExpirationGarantie() { return DATE_EXPIRATION_GARANTIE; };


   void setId(int id) { this->ID = id; };
   void setMarque(QString marque) { this->MARQUE = marque; };
   void setModele(QString modele) { this->MODELE = modele; };
   void setCouleur(QString couleur) { this->COULEUR = couleur; };
   void setDateVehicule(QDate dateVehicule) { this->DATEVEHICULE = dateVehicule; };
   void setEtat(QString etat) { this->ETAT = etat; };
   void setParcours(int parcours) { this->PARCOURS = parcours; };
   void setDateExpirationAssurance(QDate date) { this->DATE_EXPIRATION_ASSURANCE = date; };
   void setDateExpirationGarantie(QDate date) { this->DATE_EXPIRATION_GARANTIE = date; };


    bool ajouterV();
    bool modifierV();
    bool supprimerV(int);
    QSqlQueryModel* afficherV();
    QSqlQueryModel* afficherIdV();
    QSqlQueryModel* trierParIdV();
    QSqlQueryModel* trierParMarqueV();
    void viderTableV(QTableView* table);
    void rechercherV(QTableView* table, QString x);//(Result,input)


};

#endif // VEHICULE_H
