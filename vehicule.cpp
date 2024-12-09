#include "vehicule.h"
#include "QSqlQuery"
Vehicule::Vehicule()
{
    ID = 0;
    MARQUE = "";
    MODELE = "";
    COULEUR = "";
    ETAT = "";
    DATEVEHICULE = QDate();
    PARCOURS = 0;
    DATE_EXPIRATION_ASSURANCE = QDate();
    DATE_EXPIRATION_GARANTIE = QDate();
}
Vehicule::Vehicule(int ID, QString MARQUE, QString MODELE, QString COULEUR, QDate DATEVEHICULE, QString ETAT, int PARCOURS, QDate DATE_EXPIRATION_ASSURANCE, QDate DATE_EXPIRATION_GARANTIE)
{
    this->ID = ID;
    this->MARQUE = MARQUE;
    this->MODELE = MODELE;
    this->COULEUR = COULEUR;
    this->DATEVEHICULE = DATEVEHICULE;
    this->ETAT = ETAT;
    this->PARCOURS = PARCOURS;
    this->DATE_EXPIRATION_ASSURANCE = DATE_EXPIRATION_ASSURANCE;
    this->DATE_EXPIRATION_GARANTIE = DATE_EXPIRATION_GARANTIE;
}
bool Vehicule::ajouterV()
{
    QSqlQuery query;
    query.prepare("INSERT INTO VEHICULE (ID_VEHICULE, MARQUE, MODELE, COULEUR, DATE_VEHICULE, ETAT, PARCOURS_KM, DATE_EXPIRATION_ASSURANCE, DATE_EXPIRATION_GARANTIE) "
                  "VALUES (:ID, :MARQUE, :MODELE, :COULEUR, :DATE_VEHICULE, :ETAT, :PARCOURS_KM, :DATE_EXPIRATION_ASSURANCE, :DATE_EXPIRATION_GARANTIE)");
    query.bindValue(":ID", ID);
    query.bindValue(":MARQUE", MARQUE);
    query.bindValue(":MODELE", MODELE);
    query.bindValue(":COULEUR", COULEUR);
    query.bindValue(":DATE_VEHICULE", DATEVEHICULE);
    query.bindValue(":ETAT", ETAT);
    query.bindValue(":PARCOURS_KM", PARCOURS);
    query.bindValue(":DATE_EXPIRATION_ASSURANCE", DATE_EXPIRATION_ASSURANCE);
    query.bindValue(":DATE_EXPIRATION_GARANTIE", DATE_EXPIRATION_GARANTIE);
    return query.exec();
}
bool Vehicule::modifierV()
{
    QSqlQuery query;
    query.prepare("UPDATE VEHICULE SET MARQUE = :MARQUE, MODELE = :MODELE, COULEUR = :COULEUR, "
                  "DATE_VEHICULE = :DATE_VEHICULE, ETAT = :ETAT, PARCOURS_KM = :PARCOURS_KM, "
                  "DATE_EXPIRATION_ASSURANCE = :DATE_EXPIRATION_ASSURANCE, DATE_EXPIRATION_GARANTIE = :DATE_EXPIRATION_GARANTIE "
                  "WHERE ID_VEHICULE = :ID");
    query.bindValue(":MARQUE", MARQUE);
    query.bindValue(":MODELE", MODELE);
    query.bindValue(":COULEUR", COULEUR);
    query.bindValue(":DATE_VEHICULE", DATEVEHICULE);
    query.bindValue(":ETAT", ETAT);
    query.bindValue(":PARCOURS_KM", PARCOURS);
    query.bindValue(":DATE_EXPIRATION_ASSURANCE", DATE_EXPIRATION_ASSURANCE);
    query.bindValue(":DATE_EXPIRATION_GARANTIE", DATE_EXPIRATION_GARANTIE);
    query.bindValue(":ID", ID);

    return query.exec();
}
QSqlQueryModel* Vehicule::afficherV()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_VEHICULE, MARQUE, MODELE, COULEUR, TO_CHAR(DATE_VEHICULE, 'DD/MM/YYYY') AS DATE_VEHICULE, "
                    "ETAT, PARCOURS_KM, TO_CHAR(DATE_EXPIRATION_ASSURANCE, 'DD/MM/YYYY') AS DATE_EXPIRATION_ASSURANCE, "
                    "TO_CHAR(DATE_EXPIRATION_GARANTIE, 'DD/MM/YYYY') AS DATE_EXPIRATION_GARANTIE FROM VEHICULE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_VEHICULE"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MODELE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("COULEUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DATE_VEHICULE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ETAT"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("PARCOURS_KM"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DATE_EXPIRATION_ASSURANCE"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("DATE_EXPIRATION_GARANTIE"));
    return model;
}

bool Vehicule::supprimerV(int ID)
{
    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULE WHERE ID_VEHICULE = :ID");
    query.bindValue(":ID", ID);
    return query.exec();
}

QSqlQueryModel* Vehicule::afficherIdV()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_VEHICULE FROM VEHICULE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_VEHICULE"));
    return model;
}

QSqlQueryModel* Vehicule::trierParIdV()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_VEHICULE, MARQUE, MODELE, COULEUR, TO_CHAR(DATE_VEHICULE, 'DD/MM/YYYY') AS DATE_VEHICULE, "
                    "ETAT, PARCOURS_KM, TO_CHAR(DATE_EXPIRATION_ASSURANCE, 'DD/MM/YYYY') AS DATE_EXPIRATION_ASSURANCE, "
                    "TO_CHAR(DATE_EXPIRATION_GARANTIE, 'DD/MM/YYYY') AS DATE_EXPIRATION_GARANTIE FROM VEHICULE ORDER BY ID_VEHICULE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_VEHICULE"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MODELE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("COULEUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DATE_VEHICULE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ETAT"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("PARCOURS_KM"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DATE_EXPIRATION_ASSURANCE"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("DATE_EXPIRATION_GARANTIE"));
    return model;
}

QSqlQueryModel* Vehicule::trierParMarqueV()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_VEHICULE, MARQUE, MODELE, COULEUR, TO_CHAR(DATE_VEHICULE, 'DD/MM/YYYY') AS DATE_VEHICULE, "
                    "ETAT, PARCOURS_KM, TO_CHAR(DATE_EXPIRATION_ASSURANCE, 'DD/MM/YYYY') AS DATE_EXPIRATION_ASSURANCE, "
                    "TO_CHAR(DATE_EXPIRATION_GARANTIE, 'DD/MM/YYYY') AS DATE_EXPIRATION_GARANTIE FROM VEHICULE ORDER BY MARQUE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_VEHICULE"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("MARQUE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("MODELE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("COULEUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DATE_VEHICULE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ETAT"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("PARCOURS_KM"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("DATE_EXPIRATION_ASSURANCE"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("DATE_EXPIRATION_GARANTIE"));
    return model;
}

void Vehicule::viderTableV(QTableView* table)
{
    QSqlQueryModel* modelFeragh = new QSqlQueryModel();
    modelFeragh->clear();
    table->setModel(modelFeragh);
}

void Vehicule::rechercherV(QTableView* table, QString x)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery* query = new QSqlQuery;
    QString lowerX = x.toLower();
    query->prepare("SELECT ID_VEHICULE, MARQUE, MODELE, COULEUR, TO_CHAR(DATE_VEHICULE, 'DD/MM/YYYY') AS DATE_VEHICULE, "
                   "ETAT, PARCOURS_KM, TO_CHAR(DATE_EXPIRATION_ASSURANCE, 'DD/MM/YYYY') AS DATE_EXPIRATION_ASSURANCE, "
                   "TO_CHAR(DATE_EXPIRATION_GARANTIE, 'DD/MM/YYYY') AS DATE_EXPIRATION_GARANTIE FROM VEHICULE WHERE "
                   "regexp_like(LOWER(ID_VEHICULE), :ID) OR regexp_like(LOWER(MARQUE), :MARQUE) OR "
                   "regexp_like(LOWER(MODELE), :MODELE) OR regexp_like(LOWER(ETAT), :ETAT)");
    query->bindValue(":ID", lowerX);
    query->bindValue(":MARQUE", lowerX);
    query->bindValue(":MODELE", lowerX);
    query->bindValue(":ETAT", lowerX);

    if (x.isEmpty())
    {
        query->prepare("SELECT ID_VEHICULE, MARQUE, MODELE, COULEUR, TO_CHAR(DATE_VEHICULE, 'DD/MM/YYYY') AS DATE_VEHICULE, "
                       "ETAT, PARCOURS_KM, TO_CHAR(DATE_EXPIRATION_ASSURANCE, 'DD/MM/YYYY') AS DATE_EXPIRATION_ASSURANCE, "
                       "TO_CHAR(DATE_EXPIRATION_GARANTIE, 'DD/MM/YYYY') AS DATE_EXPIRATION_GARANTIE FROM VEHICULE");
    }

    query->exec();
    model->setQuery(*query);
    table->setModel(model);
    table->show();
}

