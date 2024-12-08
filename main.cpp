#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "connexion.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connexion c;
    bool test = c.CreateConnexion();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                                 QObject::tr("Connection successful.\nClick Cancel to exit."),
                                 QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
    }

    MainWindow w;
    w.show();

    int result = a.exec();
    c.FermerConnexion(); // Close the connection when the application exits

    return result;
}
