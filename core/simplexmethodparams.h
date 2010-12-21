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

#ifndef SIMPLEXMETHODPARAMS_H
#define SIMPLEXMETHODPARAMS_H

#include <limits>
#include <vector>
#include <cmath>

namespace SimplexMethod
{
    enum CompareOp { LessEqual, Equal, GreatEqual };
    enum OptimizeType { OptimizeToMin, OptimizeToMax };
    enum VariableType { VariableMain, VariableBalance,
                        VariableArtificial, VariableCutoff };
    enum LimitType { LimitMain, LimitCutoff };

    enum Status { Error, BasisCreated, SolutionFound, OptimalSolutionFound,
                  OptimalIntSolutionFound, SolutionNotExists };

    class Interval {
    public:
        Interval();
        Interval(const Interval& vals);
        Interval(double val1, double val2);
        Interval& operator=(const Interval& vals);
        double start() const;
        double end() const;
        void setStart(double val);
        void setEnd(double val);

    protected:
        double m_start;
        double m_end;
    };

    class InitalParamsFactory;
    class InitalParams
    {
    public:
        InitalParams(const InitalParams& params);
        InitalParams& operator=(const InitalParams& params);

        OptimizeType funcType() const;
        double rowC(size_t j) const;

        double matrixA(size_t i, size_t j) const;
        double columnB(size_t i) const;

        CompareOp columnCompareOp(size_t i) const;
        bool variableInt(size_t j) const;
        size_t width() const;
        size_t height() const;
        double epsilon() const;

    protected:
        InitalParams();
        OptimizeType m_funcType; // type of function (min, max)
        std::vector<double> m_rowC; // function coefficients

        std::vector< std::vector<double> > m_matrixA; // limits coefficients
        std::vector<double> m_columnB; // limits free part (B)
        std::vector<CompareOp> m_columnCompareOp; // limits compare operation

        std::vector<bool> m_variableInt; // int flags

        size_t m_width;
        size_t m_height;

        double m_epsilon;
        friend class InitalParamsFactory;
    };

    class InitalParamsFactory
    {
    public:
        InitalParamsFactory(size_t w, size_t h);
        const InitalParams& initalParams() const;

        void setFuncType(OptimizeType ft);
        void setMatrixA(size_t i, size_t j, double value);
        void setRowC(size_t j, double value);
        void setColumnB(size_t i, double value);
        void setColumnCompareOp(size_t i, CompareOp op);
        void setVariableInt(size_t j, bool value);
        void setEpsilon(double value);

    protected:
        InitalParams m_params;
    };

    class Params: public InitalParams
    {
    public:
        VariableType variableType(size_t j) const;
        LimitType limitType(size_t i) const;
        size_t columnBasis(size_t i) const;
        double F() const;
        double WF() const;
        double rowD(size_t j) const;
        double rowWD(size_t j) const;
        double columnTheta(size_t i) const;
        double mini() const;
        double maxj() const;

        Interval stabilityRowC(size_t j) const;
        Interval stabilityColumnB(size_t i) const;

        bool artificialFlag() const;
        bool checkEquals(double val1, double val2) const;
        Status status() const;

    protected:
        Params(const InitalParams& );
        std::vector<VariableType> m_variableType; // types of variable
        std::vector<LimitType> m_limitType; // types of limits

        std::vector<size_t> m_columnBasis; // basis columns
        std::vector<size_t> m_columnInitialBasis; // initial basis

        double m_F; // function value (real part)
        double m_WF; // function value (artificial part)

        std::vector<double> m_rowD; // deltas (real)
        std::vector<double> m_rowWD; // deltas (artificials)
        std::vector<double> m_columnTheta; // theta

        std::vector<Interval> m_stabilityRowC;
        std::vector<Interval> m_stabilityColumnB;

        size_t m_mini; // for changeBasis
        size_t m_maxj; // for changeBasis
        Status m_status;
        bool m_artificialFlag;
    };

}
#endif // SIMPLEXMETHODPARAMS_H
