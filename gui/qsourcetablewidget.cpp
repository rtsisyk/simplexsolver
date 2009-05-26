/*******************************************************************************

    This file is part of the SimplexSolver.

    Copyright (C) 2009 Roman O Tsisyck <inbox@art1x.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

#include "qsourcetablewidget.h"

QSourceTableWidget::QSourceTableWidget(QWidget * parent)
        : QTableView(parent), m_variableCount(0), m_limitCount(0),

        m_defaultBValue(1.0),
        m_defaultCValue(1.0),
        m_defaultVariableValue(1.0),

        m_defaultCompareOp(SimplexMethod::LessEqual),
        m_reservedRowsCount(1),
        m_selectedVariable(-1),
        m_selectedLimit(-1)
{
    m_varFormat = QString("%1");

    m_model = new QStandardItemModel(m_reservedRowsCount+1, 2);

    m_model->setData(m_model->index(0, 0),
        qVariantFromValue<SimplexMethod::OptimizeType>(SimplexMethod::OptimizeToMax));

    m_model->setHeaderData(m_variableCount, Qt::Horizontal, QVariant());
    m_model->setHeaderData(m_variableCount+1, Qt::Horizontal, "B");
    m_model->setHeaderData(0, Qt::Vertical, QVariant());
    m_model->setHeaderData(1, Qt::Vertical, tr("Number"));

    setModel(m_model);

    setItemDelegate(new QSourceTableDelegate(this));
}

bool QSourceTableWidget::insertVariable( int variable )
{
    if(variable >= 0 && variable <= m_variableCount)
    {
        m_variableCount++;

        m_model->insertColumn(variable);

        //  Add X vector components
        for(int i=0; i < m_limitCount; i++)
        {
            m_model->setData(
                    m_model->index(i+m_reservedRowsCount, variable),
                    m_defaultVariableValue
                    );
        }

        // Add C component
        m_model->setData(m_model->index(0, variable), m_defaultCValue);

        //  Add Integer flags
        m_model->setData(
                m_model->index(m_limitCount+m_reservedRowsCount, variable),
                false
                );

        //  Update headers
        updateHeaders(Qt::Horizontal);

        return true;
    }

    return false;
}

bool QSourceTableWidget::appendVariable()
{
    if(m_selectedVariable!=-1)
        return insertVariable(m_selectedVariable);
    else
        return insertVariable(m_variableCount);
}

bool QSourceTableWidget::insertLimit( int limit )
{
    if(limit >=0 && limit <= m_limitCount)
    {
        m_limitCount++;
        selectionModel()->clear();
        m_model->insertRow(limit+m_reservedRowsCount);

        //  Update X vector components
        for(int j=0; j < m_variableCount; j++)
        {
            m_model->setData(
                    m_model->index(m_reservedRowsCount+limit, j),
                    m_defaultVariableValue
                    );
        }

        //  Add compare operation
        m_model->setData(
                m_model->index(m_reservedRowsCount+limit,
                               m_variableCount),
                qVariantFromValue<SimplexMethod::CompareOp>(m_defaultCompareOp));

        //  Add B
        m_model->setData(m_model->index(m_reservedRowsCount+limit, m_variableCount+1), m_defaultBValue);

        updateHeaders(Qt::Vertical);

        return true;
    }

    return false;
}

bool QSourceTableWidget::appendLimit()
{
    if(m_selectedLimit!=-1)
        return insertLimit(m_selectedLimit);
    else
        return insertLimit(m_limitCount);
}

bool QSourceTableWidget::removeLimit( int limit )
{
    if(limit >=0 && limit < m_limitCount)
    {
        m_limitCount--;
        selectionModel()->clear();
        m_model->removeRow(limit+m_reservedRowsCount);

        //  Update headers
        updateHeaders(Qt::Vertical);

        return true;
    }

    return false;
}

bool QSourceTableWidget::removeSelectedLimit()
{
    return removeLimit(m_selectedLimit);
}

bool QSourceTableWidget::removeVariable( int variable )
{
    if(variable >= 0 && variable < m_variableCount)
    {
        m_variableCount--;
        m_model->removeColumn(variable);

        //  Update headers
        updateHeaders(Qt::Horizontal);

        return true;
    }

    return false;
}

bool QSourceTableWidget::removeSelectedVariable()
{
    return removeVariable(m_selectedVariable);
}

void QSourceTableWidget::updateHeaders(Qt::Orientation orientation)
{
    if(orientation == Qt::Horizontal)
    {
        for(int j=0; j < m_variableCount; j++)
        {
            m_model->setHeaderData(j, orientation, m_varFormat.arg(j+1));
        }
    }
    else
    {
        m_model->setHeaderData(0, orientation, tr("Function"));
        m_model->setHeaderData(1, orientation, "");
        for(int i=0; i < m_limitCount; i++)
        {
            m_model->setHeaderData(i+m_reservedRowsCount, orientation,
                                   QString(tr("Limit %1")).arg(i+1));
        }
    }
}

void QSourceTableWidget::setVarFormat(const QString& fmt)
{
    m_varFormat = fmt;
    updateHeaders(Qt::Horizontal);
}
QString QSourceTableWidget::varFormat() const
{
    return m_varFormat;
}

int QSourceTableWidget::variableCount() const
{
    return m_variableCount;
}

int QSourceTableWidget::limitCount() const
{
    return m_limitCount;
}

bool QSourceTableWidget::edit(const QModelIndex & index, EditTrigger trigger,
                             QEvent * event)
{
    if(index.row() == 0 && index.column() > m_variableCount)
        return false;

    if(index.row() >= (m_reservedRowsCount + m_limitCount) &&
       index.column() >= m_variableCount)
        return false;

    return QTableView::edit(index, trigger, event);
}

void QSourceTableWidget::selectionChanged(const QItemSelection &selected,
                                          const QItemSelection &/*deselected*/)
{
    QModelIndexList  indexes = selected.indexes();
    int newVariable = -1;
    int newLimit = -1;

    if(indexes.count() > 0)
    {
        QModelIndex index = indexes.at(0);

        if(index.column() >= 0 && index.column() < m_variableCount)
            newVariable = index.column();

        if(index.row() >= m_reservedRowsCount && index.row() < (m_limitCount+m_reservedRowsCount))
            newLimit = index.row() - m_reservedRowsCount;
    }

    if(newVariable != m_selectedVariable)
    {
        m_selectedVariable = newVariable;
        emit variableSelectionChanged(m_selectedVariable);
    }

    if(newLimit != m_selectedLimit)
    {
        m_selectedLimit = newLimit;
        emit limitSelectionChanged(m_selectedLimit);
    }
}

void QSourceTableWidget::resizeRowsColumns()
{
    int width = (viewport()->size().width() - verticalHeader()->size().width())
                / (m_model->columnCount());

    for(int j=0; j < m_model->columnCount(); j++)
        setColumnWidth(j, width);
}

SimplexMethod::InitalParams QSourceTableWidget::initialParams()
{
    SimplexMethod::InitalParamsFactory paramsFactory(
            m_variableCount,
            m_limitCount);

    paramsFactory.setFuncType(m_model->data(
            m_model->index(0, m_variableCount)
                           ).value<SimplexMethod::OptimizeType>());

    for(int j=0; j < m_variableCount; j++)
    {
        paramsFactory.setRowC(j, m_model->data(
                m_model->index(0, j)).toDouble());

        paramsFactory.setVariableInt(j, m_model->data(
                m_model->index(m_reservedRowsCount + m_limitCount, j)).toBool());
    }

    for(int i=0; i < m_limitCount; i++)
    {
        paramsFactory.setColumnB(i, m_model->data(
                m_model->index(m_reservedRowsCount + i, m_variableCount+1)
                ).toDouble());

        paramsFactory.setColumnCompareOp(i, m_model->data(
                m_model->index(m_reservedRowsCount + i, m_variableCount)
                ).value<SimplexMethod::CompareOp>());
    }

    for(int i=0; i < m_limitCount; i++)
    {
        for(int j=0; j < m_variableCount; j++)
        {
            paramsFactory.setMatrixA(i, j, m_model->data(
                    m_model->index(m_reservedRowsCount + i, j)
                    ).toDouble());
        }
    }

    return paramsFactory.initalParams();
}
