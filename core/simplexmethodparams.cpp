/*******************************************************************************

    This file is part of the SimplexSolver.

    Copyright (C) 2009 Roman Tsisyk <roman@tsisyk.com>

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

#include "simplexmethodparams.h"

namespace SimplexMethod
{
    /*
        Interval methods
    */
    Interval::Interval()
        :
        m_start(-std::numeric_limits<double>::infinity()),
        m_end(std::numeric_limits<double>::infinity())
    {

    }

    Interval::Interval(const Interval& vals)
    {
        operator=(vals);
    }

    Interval::Interval(double val1, double val2)
        :
        m_start(val1),
        m_end(val2)
    {

    }

    Interval& Interval::operator=(const Interval& vals)
                                 {
        if(this != &vals)
        {
            m_start = vals.m_start;
            m_end = vals.m_end;
        }

        return *this;
    }

    double Interval::start() const
    {
        return m_start;
    }

    double Interval::end() const
    {
        return m_end;
    }

    void Interval::setStart(double val)
    {
        m_start = val;
    }

    void Interval::setEnd(double val)
    {
        m_end = val;
    }

    /*
        InitalParams methods
    */

    InitalParams::InitalParams()
        : m_width(0),
        m_height(0),
        m_epsilon(0.0001)
    {

    }

    InitalParams::InitalParams(const InitalParams& params)
    {
        operator=(params);
    }

    InitalParams& InitalParams::operator=(const InitalParams& params)
    {
        if(this != &params)
        {
            m_width = params.m_width;
            m_height = params.m_height;
            m_funcType = params.m_funcType;

            m_rowC.resize(m_width, 0);
            m_variableInt.resize(m_width, false);
            for(size_t j=0; j < m_width; j++)
            {
                m_rowC[j] = params.m_rowC[j];
                m_variableInt[j] = params.m_variableInt[j];
            }

            m_columnB.resize(m_height, 0.0);
            m_columnCompareOp.resize(m_height, LessEqual);

            for(size_t i=0; i < m_height; i++)
            {
                m_columnB[i] = params.m_columnB[i];
                m_columnCompareOp[i] = params.m_columnCompareOp[i];
            }

            m_matrixA = params.m_matrixA;
            m_epsilon = params.m_epsilon;

            /*for(size_t i=0; i< m_matrixA.size(); i++)
            {
                m_matrixA[i].resize(m_width, 0.0);
            }

            std::vector<double> tmp(m_width, 0);
            m_matrixA.resize(m_height, tmp);

            for(size_t i=0; i < m_height; i++)
            {
                for(size_t i=0; i < m_height; i++)
                {

                }
            }
            */
        }

        return *this;
    }

    OptimizeType InitalParams::funcType() const
    {
        return m_funcType;
    }

    double InitalParams::rowC(size_t j) const
    {
        if(j < m_width)
            return m_rowC[j];
        else
            return 0;
    }

    double InitalParams::matrixA(size_t i, size_t j) const
    {
        if(i < m_height && j < m_width)
            return m_matrixA[i][j];
        else
            return 0;
    }

    double InitalParams::columnB(size_t i) const
    {
        if(i < m_height)
            return m_columnB[i];
        else
            return 0;
    }

    CompareOp InitalParams::columnCompareOp(size_t i) const
    {
        if(i < m_height)
            return m_columnCompareOp[i];
        else
            return LessEqual;
    }

    bool InitalParams::variableInt(size_t j) const
    {
        if(j < m_width)
            return m_variableInt[j];
        else
            return false;
    }

    size_t InitalParams::width() const
    {
        return m_width;
    }

    size_t InitalParams::height() const
    {
        return m_height;
    }

    double InitalParams::epsilon() const
    {
        return m_epsilon;
    }

    /*
        InitalParamsFactory methods
    */
    InitalParamsFactory::InitalParamsFactory(size_t w, size_t h)
    {
        m_params.m_width = w;
        m_params.m_height = h;

        m_params.m_rowC.resize(m_params.m_width, 0.0);
        m_params.m_variableInt.resize(m_params.m_width, false);

        m_params.m_columnB.resize(m_params.m_height, 0.0);
        m_params.m_columnCompareOp.resize(m_params.m_height, LessEqual);

        m_params.m_matrixA.resize(m_params.m_height, m_params.m_rowC);
    }

    const InitalParams& InitalParamsFactory::initalParams() const
    {
        return m_params;
    }

    void InitalParamsFactory::setFuncType(OptimizeType ft)
    {
        m_params.m_funcType = ft;
    }

    void InitalParamsFactory::setMatrixA(size_t i, size_t j, double value)
    {
        if(i < m_params.m_height && j < m_params.m_width)
            m_params.m_matrixA[i][j] = value;
    }

    void InitalParamsFactory::setRowC(size_t j, double value)
    {
        if(j < m_params.m_width)
            m_params.m_rowC[j] = value;
    }

    void InitalParamsFactory::setColumnB(size_t i, double value)
    {
        if(i < m_params.m_height)
            m_params.m_columnB[i] = value;
    }

    void InitalParamsFactory::setColumnCompareOp(size_t i, CompareOp op)
    {
        if(i < m_params.m_height)
            m_params.m_columnCompareOp[i] = op;
    }

    void InitalParamsFactory::setVariableInt(size_t j, bool value)
    {
        if(j < m_params.m_width)
            m_params.m_variableInt[j] = value;
    }

    void InitalParamsFactory::setEpsilon(double value)
    {
        m_params.m_epsilon = value;
    }

    /*
        Params methods
    */
    Params::Params(const InitalParams& initalparams)
        : InitalParams(initalparams)
    {
        m_F = 0;
        m_WF = 0;

        m_mini = 0;
        m_maxj = 0;

        m_rowD.resize(m_width, 0.0);
        m_rowWD.resize(m_width, 0.0);

        for(size_t i=0; i < m_height; i++)
            m_columnBasis.push_back(m_width);

        m_variableType.resize(m_width, VariableMain);
        m_limitType.resize(m_height, LimitMain);

        m_columnTheta.resize(m_height, 0.0);

        m_artificialFlag = true;
    }

    VariableType Params::variableType(size_t j) const
    {
        if(j < m_width)
            return m_variableType[j];
        else
            return VariableMain;
    }

    LimitType Params::limitType(size_t i) const
    {
        if(i < m_height)
            return m_limitType[i];
        else
            return LimitMain;
    }

    size_t Params::columnBasis(size_t i) const
    {
        if(i < m_height)
            return m_columnBasis[i];
        else
            return 0;
    }

    double Params::F() const
    {
        return m_F;
    }

    double Params::WF() const
    {
        return m_WF;
    }

    double Params::rowD(size_t j) const
    {
        if(j < m_width)
            return m_rowD[j];
        else
            return 0;
    }

    double Params::rowWD(size_t j) const
    {
        if(j < m_width)
            return m_rowWD[j];
        else
            return 0;
    }

    double Params::columnTheta(size_t i) const
    {
        if(i < m_height)
            return m_columnTheta[i];
        else
            return 0;
    }

    double Params::mini() const
    {
        return m_mini;
    }

    double Params::maxj() const
    {
        return m_maxj;
    }

    Interval Params::stabilityRowC(size_t j) const
    {
        if(j < m_stabilityRowC.size())
            return m_stabilityRowC[j];
        else
        {
            return Interval();
        }
    }

    Interval Params::stabilityColumnB(size_t i) const
    {
        if(i < m_stabilityColumnB.size())
            return m_stabilityColumnB[i];
        else
        {
            return Interval();
        }
    }

    bool Params::artificialFlag() const
    {
        return m_artificialFlag;
    }

    bool Params::checkEquals(double val1, double val2) const
    {
        return (std::fabs(val1 - val2) < m_epsilon);
    }

    Status Params::status() const
    {
        return m_status;
    }
}
