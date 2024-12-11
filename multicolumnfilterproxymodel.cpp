#include "multicolumnfilterproxymodel.h"

MultiColumnFilterProxyModel::MultiColumnFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void MultiColumnFilterProxyModel::setSearchColumns(const QList<int>& columns)
{
    m_columns = columns;
}

void MultiColumnFilterProxyModel::setFilterText(const QString &text)
{
    m_filterText = text;
    invalidateFilter();  // Trigger re-filtering whenever the search text changes
}

bool MultiColumnFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_filterText.isEmpty())
        return true;  // If there's no filter text, accept all rows

    // Loop through the specified columns
    for (int col : m_columns) {
        QModelIndex index = sourceModel()->index(sourceRow, col, sourceParent);
        QString data = index.data(Qt::DisplayRole).toString();

        // Perform the case-insensitive matching
        if (data.contains(m_filterText, Qt::CaseInsensitive)) {
            return true;  // Accept this row if any column matches the filter text
        }
    }

    return false;  // Reject this row if none of the columns match
}
