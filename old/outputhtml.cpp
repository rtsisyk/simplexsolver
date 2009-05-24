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

#include "outputhtml.h"

#include <QDebug>

using namespace std;

namespace Output {
    OutputHTML::OutputHTML(const SimplexMethod::Params* p,
                   std::wostream *stream)
            : OutputBase(p), m_stream(stream)
    {
        m_variableName = L"x";
    }

    OutputHTML::~OutputHTML()
    {

    }

    void OutputHTML::variableName(const std::wstring& var)
    {
        m_variableName = var;
    }

    std::wstring OutputHTML::variableName()
    {
        return m_variableName;
    }

    void OutputHTML::setStream(std::wostream *stream)
    {
        m_stream = stream;
    }

    std::wostream *OutputHTML::stream()
    {
        return m_stream;
    }

    bool OutputHTML::writeHeader()
    {

        if(NULL == m_stream)
            return false;
        try {
            *m_stream << L"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">" << std::endl;
            *m_stream << L"<html>" << std::endl;
            *m_stream << L"<body>" << std::endl;
        }
        catch(...)
        {
            return false;
        }

        return true;
    }

    bool OutputHTML::writeCanonical()
    {
        if(NULL == m_stream)
            return false;

        try {
            *m_stream << L"<div id=\"canonical\">" << std::endl;

            // function
            *m_stream << L"<div class=\"function\">" << std::endl;
            if(m_params->funcType() == SimplexMethod::OptimizeToMax)
                *m_stream << "-";

            *m_stream << L"F(" << m_variableName << L") = ";

            bool firstFlag = true;
            for(size_t j=0; j < m_params->width(); j++)
            {
                if(m_params->variableType(j) == SimplexMethod::VariableArtificial)
                {
                    *m_stream << formatVariable(j, L"W", true, firstFlag);
                }
                else
                    *m_stream << formatVariable(j, m_params->rowC(j), firstFlag);
            }
            *m_stream << L" -&gt; min" << std::endl;
            *m_stream << L"</div>" << std::endl;

            // limits
            for(size_t i=0; i < m_params->height(); i++)
            {
                firstFlag = true;
                *m_stream << L"<p class=\"condition\">";
                for(size_t j=0; j < m_params->width(); j++)
                {
                    qDebug() <<  "Matrix" << i << j << m_params->matrixA(i, j);
                    *m_stream << formatVariable(j, m_params->matrixA(i, j), firstFlag);
                }

                switch(m_params->columnCompareOp(i))
                {
                    case SimplexMethod::Equal:  *m_stream << L" = "; break;
                    case SimplexMethod::GreatEqual:  *m_stream << L" &gt;= "; break;
                    default: *m_stream << L" &lt;= "; break;
                }

                *m_stream << m_params->columnB(i);
                *m_stream << L"</p>" << std::endl;
            }

            *m_stream << L"<p>" << m_variableName << L"_i &gt; 0</p>" << std::endl;

            for(size_t j=0; j < m_params->width(); j++)
            {
                if(m_params->variableInt(j))
                    *m_stream << L"<p>" << m_variableName << L"_" << j << L" int </p>" << std::endl;
            }

            *m_stream << L"</div>" << std::endl;
        }
        catch(...)
        {
            // skip STL errors
            return false;
        }

        return true;
    }

    bool OutputHTML::writeTable()
    {
        try {
            /*
                 start of table
            */
            *m_stream << L"<table cellpadding=\"0px\" cellspacing=\"0px\" border=\"1px\" class=\"simplextable\">" << std::endl;

            /*
                 headers
            */
            *m_stream << L"<tr>" << std::endl;

            *m_stream << L"<th>" << L"i" << L"</th>" << std::endl;
            *m_stream << L"<th>" << L"basis" << L"</th>" << std::endl;
            *m_stream << L"<th>" << L"C<sub>i</sub>" << L"</th>" << std::endl;
            *m_stream << L"<th>" << L"B" << L"</th>" << std::endl;

            for(size_t j=0; j < m_params->width(); j++)
            {
                *m_stream << L"<th>" << L"C<sub>" << j+1 << L"</sub> = ";
                if(m_params->variableType(j) == SimplexMethod::VariableArtificial)
                     *m_stream << L"W";
                else
                    *m_stream << m_params->rowC(j);

                *m_stream << L"</th>" << std::endl;
            }

            *m_stream << L"<th>" << L"Θ" << L"</th>" << std::endl;
            *m_stream << "</tr>" << std::endl;


            /*
                matrix, columnCompareOp, columnB, columnTheta
            */
            for(size_t i=0; i < m_params->height(); i++)
            {
                // start row
                *m_stream << L"<tr>" << std::endl;
                *m_stream << L"<td>" << i+1 << L"</td>" << std::endl;

                // basis
                size_t basisColumn = m_params->columnBasis(i);
                *m_stream << L"<td>" << basisColumn+1 << L"</td>" << std::endl;

                // basis C
                *m_stream << L"<td>";
                if(m_params->variableType(basisColumn) == SimplexMethod::VariableArtificial)
                     *m_stream << L"W";
                else
                    *m_stream << m_params->rowC(basisColumn);
                *m_stream << L"</td>" << std::endl;

                // B
                *m_stream << L"<td>" << m_params->columnB(i) << L"</td>" << std::endl;

                // matrix
                for(size_t j=0; j < m_params->width(); j++)
                {
                    *m_stream << L"<td>" << m_params->matrixA(i, j) << L"</td>" << std::endl;
                }

                // theta
                *m_stream << L"<td>";
                if(m_params->columnTheta(i) > 0)
                    *m_stream << m_params->columnTheta(i);
                else
                    *m_stream << L"-";
                *m_stream << L"</td>" << std::endl;

                //end row
                *m_stream << L"</tr>" << std::endl;
            }

            /*
                m+1 row
            */
            *m_stream << L"<tr>" << std::endl;

            *m_stream << L"<td>" << L"m+1" << L"</td>" << std::endl;
            *m_stream << L"<td>" << L"&nbsp;" << L"</td>" << std::endl;
            *m_stream << L"<td>" << L"&nbsp;" << L"</td>" << std::endl;
            *m_stream << L"<td>" << m_params->F() << L"</td>" << std::endl;

            for(size_t j=0; j < m_params->width(); j++)
            {
                *m_stream << L"<td>" << m_params->rowD(j) << L"</td>" << std::endl;
            }

            *m_stream << L"<td>" << L"&nbsp;" << L"</td>" << std::endl;

            *m_stream << L"</tr>" << std::endl;

            /*
                m+2 row (for artificial variables)
            */
            if(m_params->artificialFlag())
            {
                *m_stream << L"<tr>" << std::endl;

                *m_stream << L"<td>" << L"m+2" << L"</td>" << std::endl;
                *m_stream << L"<td>" << L"&nbsp;" << L"</td>" << std::endl;
                *m_stream << L"<td>" << L"&nbsp;" << L"</td>" << std::endl;
                *m_stream << L"<td>" << m_params->WF() << L"</td>" << std::endl;

                for(size_t j=0; j < m_params->width(); j++)
                {
                    *m_stream << L"<td>" << m_params->rowWD(j) << L"</td>" << std::endl;
                }

                *m_stream << L"<td>" << L"&nbsp;" << L"</td>" << std::endl;

                *m_stream << L"</tr>" << std::endl;
            }

            /*
                end of table
            */
            *m_stream << L"</table>" << std::endl;
        }
        catch(...)
        {
            return false;
        }

        return true;
    }

    bool OutputHTML::writeResult()
    {
        return false;
    }

    bool OutputHTML::writeFooter()
    {
        if(NULL == m_stream)
            return false;
        try {
            *m_stream << L"</body>" << std::endl;
            *m_stream << L"</html>" << std::endl;
        }
        catch(...)
        {
            return false;
        }

        return true;
    }

    std::wstring OutputHTML::formatVariable(int index,
                                double multiplier, bool &firstFlag)
    {
        std::wostringstream buffer;

        try
        {
            buffer.precision(m_stream->precision());
            buffer.width(m_stream->width());

            if(multiplier != 0.0)
            {
                if(firstFlag)
                {
                    if(std::abs(multiplier) != 1.0)
                        buffer << multiplier;
                    firstFlag = false;
                }
                else
                {
                    if(multiplier > 0)
                        buffer << L"+";

                    if(std::abs(multiplier) != 1.0)
                        buffer << multiplier;
                    else if(multiplier < 0)
                        buffer << L"-";
                }

                buffer << m_variableName;
                buffer << L"<sub>" << index+1 << L"</sub>";
            }
        }
        catch(...)
        {
        }

        return buffer.str();
    }

    std::wstring OutputHTML::formatVariable(int index, const std::wstring& multiplier,
                                            bool positive, bool &firstFlag)
    {
        std::wostringstream buffer;
        try {
            if(positive)
            {
                if(!firstFlag)
                    buffer << L"+";
                else
                    firstFlag = false;
            }
            else
                buffer << L"-";

            buffer << multiplier;

            buffer << m_variableName;
            buffer << L"<sub>" << index+1 << L"</sub>";
        }
        catch(...)
        {
        }
        return buffer.str();
    }

}
