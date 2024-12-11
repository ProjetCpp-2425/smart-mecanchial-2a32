#include "employee.h"
#include "multicolumnfilterproxymodel.h"
#include <QStandardItem>
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QSqlTableModel>

Employee::Employee() {}

Employee::Employee(const QString &id, const QString &name, const QString &email, const QString &role,
                   const QString &schedule, const QDate &joinDate, const QString &status)
    : employee_id(id), name(name), email(email), role(role), schedule(schedule), join_date(joinDate), status(status) {}

QString Employee::getId() const { return employee_id; }
QString Employee::getName() const { return name; }
QString Employee::getEmail() const { return email; }
QString Employee::getRole() const { return role; }
QString Employee::getSchedule() const { return schedule; }
QDate Employee::getJoinDate() const { return join_date; }
QString Employee::getStatus() const { return status; }

bool Employee::saveToDatabase() {
    QSqlQuery query;
    query.prepare("INSERT INTO employees (employee_id, name, email, role, schedule, join_date, status) "
                  "VALUES (:employee_id, :name, :email, :role, :schedule, :join_date, :status)");
    query.bindValue(":employee_id", employee_id);
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":role", role);
    query.bindValue(":schedule", schedule);
    query.bindValue(":join_date", join_date.toString("yyyy-MM-dd"));
    query.bindValue(":status", status);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error adding employee to database: " << query.lastError().text();
        return false;
    }
}

void Employee::search(QLineEdit* searchBar, QTableView* tableView, int column1, int column2)
{
    MultiColumnFilterProxyModel* proxyModel = new MultiColumnFilterProxyModel(tableView);
    proxyModel->setSourceModel(tableView->model());
    tableView->setModel(proxyModel);

    proxyModel->setSearchColumns({column1, column2});

    QObject::connect(searchBar, &QLineEdit::textChanged, [proxyModel](const QString &text) {
        proxyModel->setFilterText(text);
    });
}

void Employee::displayEmployeeData(QTableView *tableView) {
    if (!tableView)
        return;

    QStandardItemModel *model = new QStandardItemModel();

    QSqlQuery query;
    query.prepare("SELECT * FROM employees");
    query.exec();

    model->setHorizontalHeaderLabels({"Employee ID", "Name", "Email", "Role", "Schedule", "Join Date", "Status"});

    int row = 0;
    while (query.next()) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QStandardItem *item = new QStandardItem(query.value(col).toString());
            model->setItem(row, col, item);
        }
        ++row;
    }

    tableView->setModel(model);

    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QStandardItem *item = model->item(row, col);

            if (col == 0) {
                item->setFont(QFont("Lexend Medium", 10, QFont::Normal));
                item->setForeground(QColor("#007BFF"));
                item->setTextAlignment(Qt::AlignCenter);
            } else {
                item->setForeground(QColor("#6e6e6e"));
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }

    tableView->horizontalHeader()->setStretchLastSection(true);
}
