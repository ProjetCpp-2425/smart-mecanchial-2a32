#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection connection;
    bool isConnected = connection.createConnection();

    MainWindow w;
    if (isConnected)
    {
        QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                                     QObject::tr("Connection successful!\n"
                                                 "You are now connected to the database."), QMessageBox::Ok);
        w.show();       
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection Error"),
                              QObject::tr("Connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              connection.getDatabase().lastError().text(), QMessageBox::Cancel);
        return -1;
    }

    return a.exec();
}
