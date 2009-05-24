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

#include "simplexmethod.h"
#include <QDebug>

namespace SimplexMethod
{
    Solver::Solver(const InitalParams& initalparams)
            : Params(initalparams)
    {

    }

    bool Solver::makeInitialBasis()
    {
        m_columnInitialB = m_columnB;
        m_rowInitialC = m_rowC;

        // change optimization type to minimum
        if (m_funcType == OptimizeToMax)
        {
            for(size_t j=0; j < m_width; j++)
                m_rowC[j] = -m_rowC[j];
        }

        // make B positive
        for(size_t i=0; i < m_height; i++)
        {
            if(m_columnB[i] < 0)
            {
                m_columnB[i] = -m_columnB[i];
                if(m_columnCompareOp[i] == LessEqual)
                    m_columnCompareOp[i] = GreatEqual;
                else if(m_columnCompareOp[i] == GreatEqual)
                    m_columnCompareOp[i] = LessEqual;

                for(size_t j=0; j < m_width; j++)
                    m_matrixA[i][j] = -m_matrixA[i][j];
            }
        }

        std::vector<bool> m_columnBasisFlags(m_height, false);
        // check for basises
        for(size_t j=0; j < m_width; j++)
        {
            size_t nullCount = 0;
            size_t basisRow = m_height;
            for(size_t i=0; i < m_height; i++)
            {
                if(checkEquals(m_matrixA[i][j], 0.0))
                    nullCount++;
                else
                    basisRow = i;
            }

            if((nullCount+1) == m_height)
            {
                if(!m_columnBasisFlags[basisRow] && m_matrixA[basisRow][j] > 0)
                {
                    double value = m_matrixA[basisRow][j];
                    for(size_t jj=0; jj < m_width; jj++)
                        m_matrixA[basisRow][jj] /= value;

                    m_matrixA[basisRow][j] = 1.0;
                    m_columnB[basisRow] /= value;
                    m_columnBasis[basisRow] = j;
                    m_columnBasisFlags[basisRow] = true;
                }
            }
        }

        // add variables
        for(size_t i=0; i < m_height; i++)
        {
            // check basis for row
            if(m_columnBasisFlags[i])
                continue;

            size_t count=(m_columnCompareOp[i] == GreatEqual) ? 2 : 1;

            for(size_t k=0; k < count; k++)
            {
                m_rowC.push_back(0.0);
                m_rowD.push_back(0.0);
                m_rowWD.push_back(0.0);
                m_variableInt.push_back(false);

                for(size_t ii=0; ii < m_height; ii++)
                    m_matrixA[ii].push_back(0);
            }

            // add additional balance variable for >=
            if(m_columnCompareOp[i] == GreatEqual)
            {
                m_variableType.push_back(VariableBalance);
                m_matrixA[i][m_width] = -1.0;
                m_width++;
            }

            if(m_columnCompareOp[i] == LessEqual)
                m_variableType.push_back(VariableBalance);
            else
                m_variableType.push_back(VariableArtificial);

            m_matrixA[i][m_width] = 1.0;
            m_columnBasis[i] = m_width;
            m_columnBasisFlags[i] = true;
            m_columnCompareOp[i] = Equal;
            m_width++;
        }

        m_status = BasisCreated;

        // backup initial basis column numbers for future use
        m_columnInitialBasis = m_columnBasis;
        return true;
    }

    bool Solver::changeBasis()
    {
        if(m_status != SolutionFound && m_status != OptimalSolutionFound)
            return false;

        double elem = m_matrixA[m_mini][m_maxj];
        if(elem == 0.0)
            return false;

        for(size_t j=0; j < m_width; j++)
            m_matrixA[m_mini][j] /= elem;

        m_columnB[m_mini] /= elem;

        for(size_t i=0; i < m_height; i++)
        {
            if(i == m_mini) continue;
            double coeff = m_matrixA[i][m_maxj];
            for(size_t j=0; j < m_width; j++)
                m_matrixA[i][j]-= m_matrixA[m_mini][j] * coeff;
            m_columnB[i]-= m_columnB[m_mini] * coeff;
        }

        m_columnBasis[m_mini] = m_maxj;

        return true;
    }

    bool Solver::iterate()
    {
        if(m_status != BasisCreated && m_status != SolutionFound)
            return false;

        m_F = 0.0;
        m_WF = 0.0;

        // Calculating m_F and m_WF
        for(size_t i = 0; i < m_height; i++)
        {
            size_t j = m_columnBasis[i];
            if(m_variableType[j] != VariableArtificial)
                m_F += m_rowC[j] * m_columnB[i];
            else
                m_WF += m_columnB[i];
        }

        // Calculate deltas
        m_artificialFlag = false;
        for(size_t j=0; j < m_width; j++)
        {
            m_rowD[j] = 0;
            m_rowWD[j] = 0;

            for(size_t i=0; i < m_height; i++)
            {
                if(m_variableType[m_columnBasis[i]] != VariableArtificial)
                    m_rowD[j] += m_rowC[m_columnBasis[i]] * m_matrixA[i][j];
                else
                    m_rowWD[j] += m_matrixA[i][j];
            }

            if(m_variableType[j] != VariableArtificial)
            {
                // check artificial deltas
                if(!checkEquals(m_rowWD[j], 0.0))
                    m_artificialFlag = true;
                m_rowD[j] -= m_rowC[j];
            }
            else
            {
                m_rowWD[j] -= 1.0;
            }
        }

        m_maxj = m_width;
        if(m_artificialFlag)
        {
            // some artificial variable exists
            for(size_t j=0; j < m_width; j++)
            {
                if(m_variableType[j] != VariableArtificial && m_rowWD[j] > 0 &&
                   (m_maxj == m_width || m_rowWD[j] > m_rowWD[m_maxj]))
                    m_maxj = j;
            }

            if(m_maxj == m_width)
            {
                if(!checkEquals(m_WF, 0.0))
                {
                    m_status = SolutionNotExists;
                    return true;
                }
                else
                {
                    // try to find max element above null W deltas
                    for(size_t j=0; j < m_width; j++)
                    {
                        if((checkEquals(m_rowWD[j], 0.0) && m_rowD[j] > 0) &&
                           (m_maxj == m_width || m_rowD[j] > m_rowD[m_maxj]))
                            m_maxj = j;
                    }
                }
            }
        }
        else
        {
            // all artificial variables was removed from basis
            for(size_t j=0; j < m_width; j++)
            {
                if((m_rowD[j] > 0) &&
                   (m_maxj == m_width || m_rowD[j] > m_rowD[m_maxj]))
                    m_maxj = j;
            }
        }

        if(m_maxj == m_width)
        {
            // optimal solution is founded
            m_status = OptimalSolutionFound;
            return true;
        }

        m_mini = m_height;

        for(size_t i=0; i < m_height; i++)
        {
            if(m_matrixA[i][m_maxj] > 0)
            {
                m_columnTheta[i] = m_columnB[i] / m_matrixA[i][m_maxj];
                if(m_mini == m_height)
                    m_mini = i;
                else if(m_columnTheta[m_mini] > m_columnTheta[i])
                    m_mini = i;
                // TODO: equal check
            }
            else
            {
                m_columnTheta[i] = 0;
            }
        }

        if(m_mini == m_height)
        {
            m_status = SolutionNotExists;
        }
        else
        {
            m_status = SolutionFound;
        }

        return true;
    }

    bool Solver::iterateInt()
    {
        if(m_status != OptimalSolutionFound)
            return false;

        size_t cuti = m_height;
        for(size_t i=0; i < m_height; i++)
        {
            if(LimitCutoff == m_limitType[i] && m_columnB[i] < 0)
            {
                cuti = i;
                break;
            }
        }

        // basis changing
        if(cuti != m_height)
        {
            size_t minj = m_width;
            /* double minTheta = std::numeric_limits<double>::infinity(); */

            for(size_t j=0; j < m_width; j++)
            {
                if(m_matrixA[cuti][j] < 0)
                {
                    minj = j;
                    break;
                    /*
                    FIXME: very slow
                    double theta = m_columnB[cuti] / m_matrixA[cuti][j];
                    if(theta < minTheta)
                    {
                        minj = j;
                        minTheta = theta;
                    }
                    */
                    // TODO: equals check
                }
            }

            qDebug() << "minj" << minj << "; cuti " << cuti;
            if(minj == m_width)
            {
                m_status = SolutionNotExists;

                return true;
            }
            m_maxj = minj;
            m_mini = cuti;

            changeBasis();
            m_F = 0.0;
            for(size_t i=0; i < m_height; i++)
            {
                m_F += m_columnB[i] * m_rowC[m_columnBasis[i]];
            }

            return true;
        }

        for(size_t i=0; i < m_height; i++)
        {
            if(!m_variableInt[m_columnBasis[i]])
                continue;

            if(!isInteger(m_columnB[i]))
            {
                // make goomory cutoff
                m_rowC.push_back(0.0);
                m_rowD.push_back(0.0);
                m_rowWD.push_back(0.0);
                m_variableInt.push_back(false);
                m_variableType.push_back(VariableCutoff);

                for(size_t ii=0; ii < m_height; ii++)
                    m_matrixA[ii].push_back(0.0);

                // add limit
                std::vector<double> tmp(m_width, 0);
                for(size_t j=0; j < m_width; j++)
                    tmp[j] = -fractionalPart(m_matrixA[i][j]);

                tmp.push_back(1.0);
                m_matrixA.push_back(tmp);
                m_columnCompareOp.push_back(Equal);
                m_columnTheta.push_back(0.0);
                m_columnB.push_back(-fractionalPart(m_columnB[i]));
                m_columnBasis.push_back(m_width);
                m_limitType.push_back(LimitCutoff);

                m_width++;
                m_height++;

                return true;
            }
        }

        m_status = OptimalIntSolutionFound;
        return true;
    }

    bool Solver::calcStabilityForRowC()
    {
        m_stabilityRowC.resize(m_width, Interval());
        for(size_t j=0; j < m_width; j++)
        {
            Interval vals;

            if(m_variableType[j] == VariableMain)
            {
                size_t basisi;
                for(basisi=0; basisi < m_height; basisi++)
                {
                    if(m_columnBasis[basisi] == j)
                        break;
                }

                // basis variable
                if(basisi != m_height)
                {
                    // we can't change basis variables without changing rowD
                    vals.setStart(0);
                    vals.setEnd(0);

                    /*
                    for(size_t jj=0; jj < m_width; jj++)
                    {
                        if(checkEquals(m_rowD[jj], 0.0) ||
                           checkEquals(m_matrixA[basisi][jj], 0.0))
                            continue;

                        double deltac = m_rowD[jj] / m_matrixA[basisi][jj];

                        if(m_funcType == OptimizeToMin)
                            deltac = -deltac;

                        if(deltac < 0)
                        {
                            if(deltac > vals.start())
                                 vals.setStart(deltac);
                        }
                        else if(deltac > 0)
                        {
                            if(deltac < vals.end())
                                 vals.setEnd(deltac);
                        }
                    }*/
                }
                else // free variable
                {
                    double deltac = m_rowD[j];

                    if(m_funcType == OptimizeToMax)
                        deltac = -deltac;

                    if(deltac > 0)
                        vals.setEnd(deltac);
                    else if(deltac < 0)
                        vals.setStart(deltac);
                    else
                    {
                        vals.setStart(deltac);
                        vals.setEnd(deltac);
                    }
                }

                vals.setStart(m_rowC[j] + vals.start());
                vals.setEnd(m_rowC[j] + vals.end());
            }

            m_stabilityRowC[j] = vals;
        }

        return true;
    }

    bool Solver::calcStabilityForColumnB()
    {
        m_stabilityColumnB.resize(m_height, Interval());
        for(size_t i=0; i < m_height; i++)
        {
            Interval vals(
                    -std::numeric_limits<double>::infinity(),
                    std::numeric_limits<double>::infinity()
                    );

            size_t basisj = m_columnInitialBasis[i];
            if(m_limitType[i] == LimitMain)
            {
                for(size_t ii=0; ii < m_height; ii++)
                {
                    if(checkEquals(m_matrixA[ii][basisj], 0.0))
                        continue;

                    double deltab = m_columnB[ii] / m_matrixA[ii][basisj];

                    if(m_funcType == OptimizeToMin)
                        deltab = -deltab;

                    if(deltab < 0)
                    {
                        if(deltab > vals.start())
                             vals.setStart(deltab);
                    }
                    else if(deltab > 0)
                    {
                        if(deltab < vals.end())
                             vals.setEnd(deltab);
                    }
                }

                vals.setStart(m_columnInitialB[i] + vals.start());
                vals.setEnd(m_columnInitialB[i] + vals.end());
            }

            m_stabilityColumnB[i] = vals;
        }

        return true;
    }

    double Solver::isInteger(double val) const
    {
        double valr = val < 0.0 ? std::ceil(val - 0.5) : std::floor(val + 0.5);

        return checkEquals(valr, val);
    }

    double Solver::fractionalPart(double val) const
    {
        return (val - std::floor(val));
    }

}
