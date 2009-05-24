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

#ifndef SIMPLEXMETHOD_H
#define SIMPLEXMETHOD_H

#include <cmath>
#include "simplexmethodparams.h"

namespace SimplexMethod
{
    class Solver: public Params
    {
    public:
        Solver(const InitalParams& initalparams);
        virtual ~Solver() {}
        virtual bool makeInitialBasis();
        virtual bool changeBasis();
        virtual bool iterate();
        virtual bool iterateInt();

        bool calcStabilityForRowC();
        bool calcStabilityForColumnB();

        const Params *params() const
        {
            return this;
        }

    protected:
        inline double fractionalPart(double val) const;
        double isInteger(double val) const;

        std::vector<double> m_rowInitialC;
        std::vector<double> m_columnInitialB;
    };
}

#endif // SIMPLEXMETHOD_H
