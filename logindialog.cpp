#include "logindialog.h"
#include "ui_logindialog.h"
#include <QSqlError>

bool LoginDialog::connectToDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("CPP_Project");
    db.setUserName("projetcpp2a32");
    db.setPassword("mp2a32");

    if (!db.open()) {
        qDebug() << "Error: Unable to connect to the database!";
        qDebug() << "Database Error:" << db.lastError().text();
        return false;
    }
    return true;
}

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Database Error", "Unable to connect to the database.");
        close();  // Exit if the database connection fails
    }
}

bool LoginDialog::verifyUser(const QString &email, const QString &password, QString &userId) {
    QSqlQuery query;
    query.prepare("SELECT user_id FROM users WHERE email = ? AND password = ?");
    query.addBindValue(email);
    query.addBindValue(password);

    if (query.exec() && query.next()) {
        userId = query.value("user_id").toString();
        return true;
    } else {
        qDebug() << "Login failed:" << query.lastError().text();
        return false;
    }
}

void LoginDialog::on_loginButton_clicked() {
    QString email = ui->emailLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString userId;

    if (verifyUser(email, password, userId)) {
        close();  // Close the login dialog
        openMainWindow(userId);  // Proceed to the next step
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid email or password.");
    }
}

void LoginDialog::openMainWindow(const QString &userId) {
    if (userId.startsWith("EMP")) {
        // Launch the "project" main window
        QProcess::startDetached("C:/Users/Omar/Documents/build-project-Desktop_Qt_6_3_0_MinGW_64_bit-Release/release/project.exe");
    } else if (userId.startsWith("VEC")) {
        // Launch the "Autocare" main window
        QProcess::startDetached("C:/Users/Omar/Documents/build-gestionVehicule-Desktop_Qt_6_3_0_MinGW_64_bit-Debug/debug/gestionVehicule.exe");
    } else {
        QMessageBox::warning(this, "Error", "Invalid user type.");
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

