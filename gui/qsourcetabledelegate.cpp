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

#include "qsourcetabledelegate.h"

QSourceTableDelegate::QSourceTableDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    m_optimizeTypeList << tr("Min") << tr("Max");
    m_compareOpList << "<=" << "=" << ">=";
    m_integerList << tr("Fractional") << tr("Integer") ;
}

QWidget *QSourceTableDelegate::createEditor(QWidget *parent,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    if(qVariantCanConvert<SimplexMethod::CompareOp>(index.data()))
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->insertItems(0, m_compareOpList );
        return comboBox;
    }
    else if(qVariantCanConvert<SimplexMethod::OptimizeType>(index.data()))
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->insertItems(0, m_optimizeTypeList );
        return comboBox;
    }
    else if(index.data().type() == QVariant::Bool)
    {
        QComboBox *comboBox = new QComboBox(parent);
        comboBox->insertItems(0, m_integerList );
        return comboBox;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void QSourceTableDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    if(qVariantCanConvert<SimplexMethod::CompareOp>(index.data()))
    {
        int value = index.data().value<SimplexMethod::CompareOp>();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentIndex(value);
    }
    else if(qVariantCanConvert<SimplexMethod::OptimizeType>(index.data()))
    {
        int value = index.data().value<SimplexMethod::OptimizeType>();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        comboBox->setCurrentIndex(value);
    }
    else
    {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void QSourceTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    if(qVariantCanConvert<SimplexMethod::CompareOp>(index.data()))
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        SimplexMethod::CompareOp value = SimplexMethod::CompareOp(
                comboBox->currentIndex());

        model->setData(index, qVariantFromValue<SimplexMethod::CompareOp>(value));
    }
    else if(qVariantCanConvert<SimplexMethod::OptimizeType>(index.data()))
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        SimplexMethod::OptimizeType value = SimplexMethod::OptimizeType(
                comboBox->currentIndex());

        model->setData(index, qVariantFromValue<SimplexMethod::OptimizeType>(value));
    }
    else if(index.data().type() == QVariant::Bool)
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        model->setData(index, qVariantFromValue<bool>(comboBox->currentIndex()));
    }
    else
    {

        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

QString QSourceTableDelegate::displayText(const QVariant &value, const QLocale& locale) const
{
    if(qVariantCanConvert<SimplexMethod::CompareOp>(value))
    {
        return m_compareOpList.at(value.value<SimplexMethod::CompareOp>());
    }
    else if(qVariantCanConvert<SimplexMethod::OptimizeType>(value))
    {
        return m_optimizeTypeList.at(value.value<SimplexMethod::OptimizeType>());
    }
    else if(value.type() == QVariant::Bool)
    {
        return m_integerList[value.toBool()];
    }
    return QStyledItemDelegate::displayText(value, locale);
}

