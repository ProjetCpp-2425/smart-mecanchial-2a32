#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>


class Connection
{
    QSqlDatabase db;

public:
    Connection();
    bool createConnection();
    void closeConnection();
    QSqlDatabase getDatabase() const;
};

#endif // CONNECTION_H
