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

#include "outputqtdocument.h"
#include <cmath>

namespace Output {
    OutputQtDocument::OutputQtDocument(const SimplexMethod::Params* p)
            : OutputBase(p),
            m_document(new QTextDocument(this)),
            m_cursor(m_document)
    {
        m_variableName = "x";
        m_systemLocale = QLocale::system();
    }

    OutputQtDocument::~OutputQtDocument()
    {

    }

    void OutputQtDocument::variableName(const QString& var)
    {
        m_variableName = var;
    }

    const QString& OutputQtDocument::variableName() const
    {
        return m_variableName;
    }

    QTextDocument *OutputQtDocument::document() const
    {
        return m_document;
    }


    bool OutputQtDocument::clear()
    {
        m_document->clear();
        return true;
    }

    bool OutputQtDocument::writeHeader()
    {
        // some title here
        // m_cursor.insertBlock();
        return true;
    }

    bool OutputQtDocument::writeCanonical()
    {
        m_cursor.insertBlock();
        m_cursor.insertText(tr("Canonical form of problem is :"));
        m_cursor.insertBlock();

        m_cursor.insertText(QString("F(%1) = ").arg(m_variableName.toUpper()));

        if(SimplexMethod::OptimizeToMax == m_params->funcType())
            m_cursor.insertText("-(");

        bool firstFlag = true;
        for(size_t j=0; j < m_params->width(); j++)
        {
            if(m_params->variableType(j) == SimplexMethod::VariableArtificial)
            {
                putVariable(j, "W", true, firstFlag);
            }
            else
                putVariable(j, m_params->rowC(j), firstFlag);
        }

        if(SimplexMethod::OptimizeToMax == m_params->funcType())
            m_cursor.insertText(") -> max");
        else
            m_cursor.insertText(" -> min");

        // limits
        for(size_t i=0; i < m_params->height(); i++)
        {
            m_cursor.insertBlock();
            firstFlag = true;

            for(size_t j=0; j < m_params->width(); j++)
            {
                putVariable(j, m_params->matrixA(i, j), firstFlag);
            }

            switch(m_params->columnCompareOp(i))
            {
                    case SimplexMethod::Equal: m_cursor.insertText(" = "); break;
                    case SimplexMethod::GreatEqual: m_cursor.insertText(" >= "); break;
                    default: m_cursor.insertText(" <= "); break;
            }

            m_cursor.insertText(formatDouble(m_params->columnB(i)));
        }

        m_cursor.insertBlock();
        m_cursor.insertText(m_variableName);
        m_cursor.insertText(" >= 0");

        m_cursor.insertBlock();

        return true;
    }

    bool OutputQtDocument::writeTable()
    {
        /*
            start of table
        */
        m_cursor.beginEditBlock();

        m_cursor.insertBlock();
        m_cursor.insertBlock();
        QTextFrame *topFrame = m_cursor.currentFrame();

        QTextTableFormat tableFormat;
        tableFormat.setCellPadding(4);
        tableFormat.setHeaderRowCount(1);
        /* tableFormat.setBorderStyle(
                QTextFrameFormat::BorderStyle_Double); */
        tableFormat.setMargin(2);
        tableFormat.setWidth(QTextLength(
                QTextLength::PercentageLength, 100));

        QTextTable *table = m_cursor.insertTable(
                m_params->height()+2, m_params->width()+5, tableFormat);

        /*
            headers
        */
        m_cursor = table->cellAt(0, 0).firstCursorPosition();
        m_cursor.insertText("i");
        m_cursor = table->cellAt(0, 1).firstCursorPosition();
        m_cursor.insertText(tr("basis"));
        m_cursor = table->cellAt(0, 2).firstCursorPosition();
        m_cursor.insertHtml("C<sub>i</sub> ");
        m_cursor = table->cellAt(0, 3).firstCursorPosition();
        m_cursor.insertText("B");

        for(size_t j=0; j < m_params->width(); j++)
        {
            m_cursor = table->cellAt(0, j+4).firstCursorPosition();
            m_cursor.insertHtml(QString("P<sub>%1</sub> ").arg(j+1));
            /* m_cursor.insertHtml(QString("C<sub>%1</sub> =").arg(j+1));
            if(m_params->variableType(j) == SimplexMethod::VariableArtificial)
                m_cursor.insertText("W");
            else
                m_cursor.insertText(formatDouble(m_params->rowC(j))); */

        }

        m_cursor = table->cellAt(0, m_params->width()+4).firstCursorPosition();
        m_cursor.insertText(QChar(0x0398)); // theta

        /*
            matrix, columnCompareOp, columnB, columnTheta
        */
        for(size_t i=0; i < m_params->height(); i++)
        {
            m_cursor = table->cellAt(i+1, 0).firstCursorPosition();
            m_cursor.insertText(QString("%1").arg(i+1));

            // basis
            m_cursor = table->cellAt(i+1, 1).firstCursorPosition();
            size_t basisColumn = m_params->columnBasis(i);
            m_cursor.insertHtml(QString("P<sub>%1</sub> ").arg(basisColumn+1));

            // basis C
            m_cursor = table->cellAt(i+1, 2).firstCursorPosition();
            if(m_params->variableType(basisColumn) == SimplexMethod::VariableArtificial)
                m_cursor.insertText("W");
            else
                m_cursor.insertText(formatDouble(m_params->rowC(basisColumn)));

            // B
            m_cursor = table->cellAt(i+1, 3).firstCursorPosition();
            m_cursor.insertText(formatDouble(m_params->columnB(i)));

            // matrix
            for(size_t j=0; j < m_params->width(); j++)
            {
                m_cursor = table->cellAt(i+1, j+4).firstCursorPosition();
                m_cursor.insertText(formatDouble(m_params->matrixA(i, j)));
            }

            // theta
            m_cursor = table->cellAt(i+1, m_params->width()+4).firstCursorPosition();
            if(m_params->columnTheta(i) > 0)
                 m_cursor.insertText(formatDouble(m_params->columnTheta(i)));
            else
                 m_cursor.insertText("-");
        }

        /*
            m+1 row
        */
        m_cursor = table->cellAt(m_params->height()+1, 0).firstCursorPosition();
        m_cursor.insertText("m+1");
        m_cursor = table->cellAt(m_params->height()+1, 3).firstCursorPosition();
        m_cursor.insertText(formatDouble(m_params->F()));

        for(size_t j=0; j < m_params->width(); j++)
        {
            m_cursor = table->cellAt(m_params->height()+1, j+4).firstCursorPosition();
            m_cursor.insertText(formatDouble(m_params->rowD(j)));
        }

        /*
            m+2 row (for artificial variables)
        */
        if(m_params->artificialFlag())
        {
            table->appendRows(1);

            m_cursor = table->cellAt(m_params->height()+2, 0).firstCursorPosition();
            m_cursor.insertText("m+2");

            m_cursor = table->cellAt(m_params->height()+2, 3).firstCursorPosition();
            m_cursor.insertText(formatDouble(m_params->WF()));

            for(size_t j=0; j < m_params->width(); j++)
            {
                m_cursor = table->cellAt(m_params->height()+2, j+4).firstCursorPosition();
                m_cursor.insertText(formatDouble(m_params->rowWD(j)));
            }
        }

        /*
            end of table
        */
        m_cursor.endEditBlock();
        m_cursor.setPosition(topFrame->lastPosition());
        return true;
    }

    bool OutputQtDocument::writeResult()
    {
        m_cursor.insertBlock();
        m_cursor.insertBlock();
        if(SimplexMethod::OptimalSolutionFound == m_params->status() ||
           SimplexMethod::OptimalIntSolutionFound == m_params->status())
        {
            m_cursor.insertHtml(QString("<b>%1</b> ").
                                arg(tr("Optimum is founded:")));
            m_cursor.insertBlock();


            if(SimplexMethod::OptimizeToMax == m_params->funcType())
                m_cursor.insertText(QString("F = %1,").arg(formatDouble(-(m_params->F()))));
            else
                m_cursor.insertText(QString("F = %1,").arg(formatDouble(m_params->F())));

            m_cursor.insertBlock();
            m_cursor.insertText(QString("%1 = (").arg(m_variableName.toUpper()));
            QStringList plan;
            for(size_t j=0; j < m_params->width(); j++)
            {
                double val = 0.0;
                for(size_t i = 0; i < m_params->height(); i++)
                {
                    if(m_params->columnBasis(i) == j)
                    {
                        val = m_params->columnB(i);
                        break;
                    }
                }
                plan.push_back(formatDouble(val));
            }

            m_cursor.insertText(plan.join(";"));
            m_cursor.insertText(").");
            m_cursor.insertBlock();
        }
        else if(SimplexMethod::SolutionNotExists == m_params->status())
        {
            m_cursor.insertHtml(QString("<b>%1</b> ").
                                arg(tr("Optimum is not founded.")));
        }
        else
        {
            m_cursor.insertHtml(QString("<b>%1</b> ").
                                arg(tr("Some error was occured while calculating solution.")));
        }
        return true;
    }

    bool OutputQtDocument::writeStability()
    {
        m_cursor.insertBlock();
        m_cursor.insertHtml(QString("</b>%1</b> ").
                            arg(tr("Solution is a stabled if:")));
        for(size_t i=0; i < m_params->height(); i++)
        {
            if(m_params->limitType(i) != SimplexMethod::LimitMain)
                continue;

            SimplexMethod::Interval ival = m_params->stabilityColumnB(i);
            m_cursor.insertBlock();
            m_cursor.insertHtml(QString("B<sub>%1</sub> ").arg(i+1));
            m_cursor.insertText(QChar(0x220A));
            m_cursor.insertText(" ");

            if(ival.start() == -std::numeric_limits<double>::infinity())
                m_cursor.insertText(QString("(-")+QChar(0x221E));
            else
            {
                m_cursor.insertText("[");
                m_cursor.insertText(formatDouble(ival.start()));
            }

            m_cursor.insertText(" ; ");

            if(ival.end() == std::numeric_limits<double>::infinity())
                m_cursor.insertText(QChar(0x221E)+QString(")"));
            else
            {
                m_cursor.insertText(formatDouble(ival.end()));
                m_cursor.insertText("]");
            }
        }

        m_cursor.insertBlock();
        for(size_t j=0; j < m_params->width(); j++)
        {
            if(m_params->variableType(j) != SimplexMethod::VariableMain)
                continue;

            SimplexMethod::Interval ival = m_params->stabilityRowC(j);
            m_cursor.insertBlock();
            m_cursor.insertHtml(QString("C<sub>%1</sub> ").arg(j+1));
            m_cursor.insertText(QChar(0x220A));
            m_cursor.insertText(" ");

            if(ival.start() == -std::numeric_limits<double>::infinity())
                m_cursor.insertText(QString("(-")+QChar(0x221E));
            else
            {
                m_cursor.insertText("[");
                m_cursor.insertText(formatDouble(ival.start()));
            }

            m_cursor.insertText(" ; ");

            if(ival.end() == std::numeric_limits<double>::infinity())
                m_cursor.insertText(QChar(0x221E)+QString(")"));
            else
            {
                m_cursor.insertText(formatDouble(ival.end()));
                m_cursor.insertText("]");
            }

        }

        return true;
    }

    bool OutputQtDocument::writeFooter()
    {
        return true;
    }

    QString OutputQtDocument::formatDouble(double value) const
    {
        if(m_params->checkEquals(value, 0.0))
            return "0";
        else
            return m_systemLocale.toString(value, 'g', 5);
    }

    void OutputQtDocument::putVariable(int index, double multiplier,
                                    bool &firstFlag)
    {
        if(!m_params->checkEquals(multiplier, 0.0))
        {
            if(firstFlag)
            {
                if(multiplier < 0)
                    m_cursor.insertText("-");

                if(std::abs(multiplier) != 1.0)
                {
                    m_cursor.insertText(formatDouble(std::abs(multiplier)));
                    // m_cursor.insertText(QChar(0x2027));
                }
                firstFlag = false;
            }
            else
            {
                if(multiplier > 0)
                    m_cursor.insertText(" + ");
                else
                    m_cursor.insertText(" - ");

                if(std::abs(multiplier) != 1.0)
                {
                    m_cursor.insertText(formatDouble(std::abs(multiplier)));
                    // m_cursor.insertText(QChar(0x2027));
                }
            }

            m_cursor.insertHtml(QString("%1<sub>%2</sub> ").
                                arg(m_variableName).
                                arg(index+1));
        }
    }

    void OutputQtDocument::putVariable(int index, const QString& multiplier,
                                            bool positive, bool &firstFlag)
    {
        if(firstFlag)
        {
            if(!positive)
                m_cursor.insertText("- ");
            else
                firstFlag = false;
        }
        else
        {
            if(positive)
                m_cursor.insertText(" + ");
            else
                m_cursor.insertText(" - ");
        }

        m_cursor.insertText(multiplier);
        m_cursor.insertHtml(QString("%1<sub>%2</sub> ").
                            arg(m_variableName).
                            arg(index+1));
    }


}
