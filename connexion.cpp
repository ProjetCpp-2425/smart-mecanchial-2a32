#include "connexion.h"
#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QSqlDatabase>

connexion::connexion() {}
QSqlDatabase connexion::getDatabase() const {
    return db;
}

bool connexion::CreateConnexion()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    bool test = false;
    db.setDatabaseName("source2a32");
    db.setUserName("projetcpp2a32");
    db.setPassword("mp2a32");

    if (db.open()) test = true;
    return test;
}

// Close the database connection
void connexion::FermerConnexion() {
        db.close();
}
