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

#ifndef OUTPUTQTDOCUMENT_H
#define OUTPUTQTDOCUMENT_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QLocale>
#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>
#include <QtGui/QTextTable>
#include <QtGui/QTextFrame>

#include "outputbase.h"
#include "simplexmethodparams.h"

namespace Output {
    class OutputQtDocument : public QObject, public OutputBase
    {
        Q_OBJECT

    public:
        OutputQtDocument(const SimplexMethod::Params* p);
        virtual ~OutputQtDocument();

        virtual bool clear();
        virtual bool writeHeader();
        virtual bool writeCanonical();
        virtual bool writeTable();
        virtual bool writeResult();
        virtual bool writeFooter();
        virtual bool writeStability();

        virtual void variableName(const QString& var);
        virtual const QString& variableName() const;

        QTextDocument *document() const;
    protected:
        QString m_variableName;
        QTextDocument *m_document;
        QTextCursor m_cursor;

        QString formatDouble(double value) const;

        void putVariable(int index, double multiplier, bool &firstFlag);
        void putVariable(int index, const QString& multiplier,
                         bool positive, bool &firstFlag);

        QLocale m_systemLocale;
    };
}

#endif // OUTPUTQTDOCUMENT_H
