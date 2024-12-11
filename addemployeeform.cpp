#include "addemployeeform.h"
#include "ui_addemployeeform.h"
#include "employee.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

AddEmployeeForm::AddEmployeeForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEmployeeForm)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &AddEmployeeForm::onSaveButtonClicked);

    connect(ui->cancelButton, &QPushButton::clicked, this, &AddEmployeeForm::onCancelButtonClicked);
}

AddEmployeeForm::~AddEmployeeForm()
{
    delete ui;
}

void AddEmployeeForm::onSaveButtonClicked() {
    QString employeeID = ui->employeeIdLineEdit->text();
    QString name = ui->nameLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString role = ui->roleComboBox->currentText();
    QString schedule = ui->scheduleComboBox->currentText();
    QDate joinDate = ui->joinDateEdit->date();
    QString joinDateString = joinDate.toString("yyyy-MM-dd");
    QString status = ui->statusComboBox->currentText();

    QRegularExpression nameRegex("^[a-zA-Z.]+$");
    if (!nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, "Invalid Name", "Name can only contain letters and periods.");
        return;
    }

    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Invalid Email", "Please enter a valid email address.");
        return;
    }

    bool isNumeric = true;
    for (QChar c : employeeID) {
        if (!c.isDigit()) {
            isNumeric = false;
            break;
        }
    }
    if (!isNumeric) {
        QMessageBox::warning(this, "Invalid Employee ID", "Employee ID must contain only digits.");
        return;
    }

    if (employeeID.length() != 6) {
        QMessageBox::warning(this, "Invalid Employee ID", "Employee ID must be exactly 6 digits long.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM employees WHERE employee_id = :employee_id");
    checkQuery.bindValue(":employee_id", employeeID);
    checkQuery.exec();
    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Duplicate Employee ID", "This Employee ID is already taken. Please choose a different one.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO employees (employee_id, name, email, role, schedule, join_date, status) "
                  "VALUES (:employee_id, :name, :email, :role, :schedule, :join_date, :status)");
    query.bindValue(":employee_id", employeeID);
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":role", role);
    query.bindValue(":schedule", schedule);
    query.bindValue(":join_date", joinDateString);
    query.bindValue(":status", status);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Employee added successfully!");
        emit employeeAdded();
        resetForm();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add employee to the database: " + query.lastError().text());
    }
}

void AddEmployeeForm::onCancelButtonClicked() {
    this->close(); // Close the form dialog
}

void AddEmployeeForm::resetForm() {
    ui->employeeIdLineEdit->clear();
    ui->nameLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->roleComboBox->setCurrentIndex(0);
    ui->scheduleComboBox->setCurrentIndex(0);
    ui->joinDateEdit->setDate(QDate::currentDate());
    ui->statusComboBox->setCurrentIndex(0);
}
