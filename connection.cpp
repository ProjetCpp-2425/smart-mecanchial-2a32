#include "connection.h"

Connection::Connection() {}

QSqlDatabase Connection::getDatabase() const {
    return db;
}

bool Connection::createConnection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    bool test = false;
    db.setDatabaseName("CPP_Project");
    db.setUserName("projetcpp2a32");
    db.setPassword("mp2a32");

    if (db.open()) test=true;

    return test;
}
void Connection::closeConnection()
{
    db.close();
}
