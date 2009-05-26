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

#ifndef OUTPUTBASE_H
#define OUTPUTBASE_H

#include "simplexmethodparams.h"

namespace Output
{
    class OutputBase
    {
    public:
        OutputBase(const SimplexMethod::Params* p);
        virtual ~OutputBase();

        virtual bool clear() { return true; }
        virtual bool writeHeader() = 0;
        virtual bool writeCanonical() = 0;
        virtual bool writeTable() = 0;
        virtual bool writeResult() = 0;
        virtual bool writeFooter() = 0;
        virtual bool writeStability() = 0;

        void setParams(const SimplexMethod::Params *params);
        const SimplexMethod::Params *params() const;
protected:
        const SimplexMethod::Params *m_params;

    };

}
#endif // OUTPUTBASE_H
