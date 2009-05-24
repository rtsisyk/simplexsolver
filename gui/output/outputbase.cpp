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

#include "outputbase.h"

namespace Output
{
    OutputBase::OutputBase(const SimplexMethod::Params* p)
        : m_params(p)
    {

    }

    OutputBase::~OutputBase()
    {

    }

    void OutputBase::setParams(const SimplexMethod::Params *p)
    {
        m_params = p;
    }

    const SimplexMethod::Params *OutputBase::params() const
    {
        return m_params;
    }
}
