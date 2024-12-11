#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include "employee.h"
#include <QPropertyAnimation>
#include <QScreen>
#include "addemployeeform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->dashboardButton, &QPushButton::clicked, this, [=]() {
    ui->contentStack->setCurrentIndex(0);
  });
  connect(ui->employeeButton, &QPushButton::clicked, this, [=]() {
    ui->contentStack->setCurrentIndex(1);
  });
  connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::showAddEmployeeForm);

  Employee::displayEmployeeData(ui->employeeTableView);
  Employee::search(ui->searchBar, ui->employeeTableView, 0, 1);
}

void MainWindow::showAddEmployeeForm() {
    AddEmployeeForm addEmployeeForm(this);

    connect(&addEmployeeForm, &AddEmployeeForm::employeeAdded, this, &MainWindow::updateEmployeeTable);

    addEmployeeForm.exec();
}

void MainWindow::updateEmployeeTable() {
    Employee::displayEmployeeData(ui->employeeTableView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
