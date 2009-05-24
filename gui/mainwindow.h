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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QFileDialog>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QTextDocumentWriter>
#include <QtGui/QPixmap>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>

#include "simplexmethod.h"
#include "simplexmethodparams.h"
#include "outputbase.h"
#include "outputqtdocument.h"

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;

    const int m_minLimits;
    const int m_minVariables;
    SimplexMethod::Solver *solver;
    Output::OutputBase* output;
    QIcon icon;

    QString copyrightText;

private slots:
    void on_actionAbout_triggered();
    void on_actionPrintSolution_triggered();
    void on_actionSaveSolution_triggered();
    void on_actionSolve_triggered();
    void on_sourceTableView_customContextMenuRequested(QPoint pos);
    void on_actionRemoveLimit_triggered();
    void on_actionRemoveVariable_triggered();
    void on_actionAddLimit_triggered();
    void on_actionAddVariable_triggered();

    void on_sourceTableView_variableSelectionChanged(int);
    void on_sourceTableView_limitSelectionChanged(int);
};

#endif // MAINWINDOW_H
