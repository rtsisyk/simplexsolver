/*******************************************************************************

    This file is part of the SimplexSolver.

    Copyright (C) 2009 Roman O Tsisyck <r.tsisyck@gmail.com>

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

#ifndef QSOURCETABLEWIDGET_H
#define QSOURCETABLEWIDGET_H

#include <QtCore/QModelIndex>
#include <QtGui/QTableView>
#include <QtGui/QStandardItemModel>
#include <QtGui/QHeaderView>

#include "simplexmethod.h"
#include "qsourcetabledelegate.h"

class QSourceTableWidget : public QTableView
{
    Q_OBJECT
    Q_PROPERTY(int variableCount READ variableCount)
    Q_PROPERTY(int limitCount READ limitCount)
    Q_PROPERTY(QString varFormat READ varFormat WRITE setVarFormat USER true)
public:
    QSourceTableWidget(QWidget * parent = 0);

    bool insertVariable( int variable );
    bool appendVariable();

    bool insertLimit( int limit );
    bool appendLimit();

    bool removeLimit( int limit );
    bool removeSelectedLimit();
    bool removeVariable( int variable );
    bool removeSelectedVariable();

    int variableCount() const;
    int limitCount() const;

    void setVarFormat(const QString& fmt);
    QString varFormat() const;
    void resizeRowsColumns();
    SimplexMethod::InitalParams initialParams();

protected:
    void updateHeaders(Qt::Orientation orientation);

    // QTableView Methods
    bool edit( const QModelIndex & index, EditTrigger trigger, QEvent * event);
    void selectionChanged(const QItemSelection &selected,
                                          const QItemSelection &deselected);
private:
     QStandardItemModel *m_model;
     int m_variableCount;
     int m_limitCount;

     QString m_varFormat;

     const double m_defaultBValue;
     const double m_defaultCValue;
     const double m_defaultVariableValue;

     const SimplexMethod::CompareOp m_defaultCompareOp;
     const int m_reservedRowsCount;

     int m_selectedVariable;
     int m_selectedLimit;

     SimplexMethod::InitalParamsFactory *paramsFactory;
signals:
     void variableSelectionChanged(int variable);
     void limitSelectionChanged(int limit);

};

#endif // QSOURCETABLEWIDGET_H
