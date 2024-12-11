#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QDate>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QTableView>
#include <QColor>
#include <QFont>
#include <QString>
#include <QSqlQueryModel>

class Employee
{
    QString employee_id;
    QString name;
    QString email;
    QString role;
    QString schedule;
    QDate join_date;
    QString status;
public:
    Employee();
    Employee(const QString &id, const QString &name, const QString &email, const QString &role,
                 const QString &schedule, const QDate &joinDate, const QString &status);
    static void search(QLineEdit* searchBar, QTableView* tableView, int column1, int column2);
    static void formatTable(QAbstractItemModel *model, QTableView *tableView);
    QString getId() const;
    QString getName() const;
    QString getEmail() const;
    QString getRole() const;
    QString getSchedule() const;
    QDate getJoinDate() const;
    QString getStatus() const;

    bool saveToDatabase();
    static void displayEmployeeData(QTableView *tableView);
    void saveModifiedData();
};

#endif // EMPLOYEE_H
