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

#include "simplexmethod.h"
#include "simplexmethodparams.h"

#include <iostream>

int main(int argc, char *argv[])
{
    // Step 0. Input data
    SimplexMethod::InitalParamsFactory paramsFactory(5, 4);

    paramsFactory.setFuncType(SimplexMethod::OptimizeToMin);
    paramsFactory.setRowC(0, 4.0);
    paramsFactory.setRowC(1, 1.2);
    paramsFactory.setRowC(2, 5.8);
    paramsFactory.setRowC(3, 6.0);
    paramsFactory.setRowC(4, 7.5);

    paramsFactory.setColumnB(0, 1.12);
    paramsFactory.setColumnB(1, 0.3);
    paramsFactory.setColumnB(2, 0.1);
    paramsFactory.setColumnB(3, 0.4);

    paramsFactory.setColumnCompareOp(0, SimplexMethod::Equal);
    paramsFactory.setColumnCompareOp(1, SimplexMethod::LessEqual);
    paramsFactory.setColumnCompareOp(2, SimplexMethod::GreatEqual);
    paramsFactory.setColumnCompareOp(3, SimplexMethod::LessEqual);

    paramsFactory.setMatrixA(0, 0, 1);
    paramsFactory.setMatrixA(0, 1, 1);
    paramsFactory.setMatrixA(0, 2, 1);
    paramsFactory.setMatrixA(0, 3, 1);
    paramsFactory.setMatrixA(0, 4, 1);

    paramsFactory.setMatrixA(1, 0, 0.8);
    paramsFactory.setMatrixA(1, 1, 0.6);
    paramsFactory.setMatrixA(1, 2, 0.1);
    paramsFactory.setMatrixA(1, 3, 0.1);
    paramsFactory.setMatrixA(1, 4, 0.1);

    paramsFactory.setMatrixA(2, 0, 0.1);
    paramsFactory.setMatrixA(2, 1, 0.3);
    paramsFactory.setMatrixA(2, 2, 0.5);
    paramsFactory.setMatrixA(2, 3, 0.3);
    paramsFactory.setMatrixA(2, 4, 0.2);

    paramsFactory.setMatrixA(3, 0, 0.1);
    paramsFactory.setMatrixA(3, 1, 0.1);
    paramsFactory.setMatrixA(3, 2, 0.4);
    paramsFactory.setMatrixA(3, 3, 0.6);
    paramsFactory.setMatrixA(3, 4, 0.7);

    SimplexMethod::Solver *solver = new SimplexMethod::Solver(paramsFactory.initalParams());

    // Step 1. Make canonical form of problem
    solver->makeInitialBasis();

    // Step 2. Try to find optimal basis
    solver->iterate();
    SimplexMethod::Status status;

    while(SimplexMethod::SolutionFound == (status = solver->status()))
    {
        solver->changeBasis();
        solver->iterate();
    }

    // Make integer basis
    while(SimplexMethod::OptimalSolutionFound == (status = solver->status()))
    {
        solver->iterateInt();
    }

    // Step 3. Write result
    switch(status)
    {
    // you can use SimplexMethod::Params functions to print result
    case SimplexMethod::OptimalIntSolutionFound:
    case SimplexMethod::OptimalSolutionFound: std::cout << "Optimum is founded: " << solver->F(); break;
    case SimplexMethod::SolutionNotExists: std::cout << "Optimum is not found."; break;
    case SimplexMethod::Error: std::cout << "Some error was occured while calculating solution: "; break;
    default: std::cout << "Invalid state";
    }

    std::cout << std::endl;
    delete solver;

    return 0;
}
