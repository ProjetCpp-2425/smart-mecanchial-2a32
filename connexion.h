#ifndef CONNEXION_H
#define CONNEXION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class connexion
{
    QSqlDatabase db;
public:
    connexion();
    bool CreateConnexion();
    void FermerConnexion();
    QSqlDatabase getDatabase() const;
};

#endif // CONNEXION_H
