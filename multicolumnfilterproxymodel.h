#ifndef MULTICOLUMNFILTERPROXYMODEL_H
#define MULTICOLUMNFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QRegularExpression>

class MultiColumnFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit MultiColumnFilterProxyModel(QObject *parent = nullptr);

    void setSearchColumns(const QList<int>& columns);  // Set columns to search in
    void setFilterText(const QString &text);  // Set the filter text

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QList<int> m_columns;  // Columns to be filtered
    QString m_filterText;  // The filter text to search for
};

#endif // MULTICOLUMNFILTERPROXYMODEL_H
