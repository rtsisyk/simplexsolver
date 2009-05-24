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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass),
    m_minLimits(2),
    m_minVariables(2),
    solver(NULL),
    output(NULL)
{
    ui->setupUi(this);
    icon.addFile(":/icon/16x16.png");
    icon.addFile(":/icon/24x24.png");
    icon.addFile(":/icon/32x32.png");
    icon.addFile(":/icon/64x64.png");
    icon.addFile(":/icon/96x96.png");
    icon.addFile(":/icon/128x128.png");
    icon.addFile(":/icon/256x256.png");

    this->setWindowIcon(icon);

    ui->sourceTableView->setVarFormat("X_%1");

    for(int j=0; j < m_minLimits; j++)
        ui->sourceTableView->appendLimit();

    for(int i=0; i < m_minVariables; i++)
        ui->sourceTableView->appendVariable();

    copyrightText = QString("%1 2009 Roman O Tsisyck &lt;%2&gt;").
           arg(QChar(0x00A9)).
           arg("<a href=\"mailto:r.tsisyck@gmail.com\">r.tsisyck@gmail.com</a>");

    ui->statusBar->addPermanentWidget(new QLabel(copyrightText));
    ui->sourceTableView->resizeRowsColumns();
}

MainWindow::~MainWindow()
{
    delete ui;

    if(NULL != solver)
        delete solver;
    if(NULL != output)
        delete output;
}

void MainWindow::on_actionAddVariable_triggered()
{
    if(ui->sourceTableView->appendVariable())
            ui->sourceTableView->resizeRowsColumns();
}

void MainWindow::on_actionAddLimit_triggered()
{
    if(ui->sourceTableView->appendLimit())
            ui->sourceTableView->resizeRowsColumns();
}

void MainWindow::on_actionRemoveVariable_triggered()
{
    if(ui->sourceTableView->variableCount() <= m_minVariables)
    {
        QMessageBox::warning(this, QString(tr("Remove variable")),
                                           QString(tr("Can't remove variable")));
    }
    else
    {
        if(ui->sourceTableView->removeSelectedVariable())
            ui->sourceTableView->resizeRowsColumns();
    }
}

void MainWindow::on_actionRemoveLimit_triggered()
{
    if(ui->sourceTableView->limitCount() <= m_minLimits)
    {
        QMessageBox::warning(this, QString(tr("Remove limit")),
                                           QString(tr("Can't remove limit")));
    }
    else
    {
        if(ui->sourceTableView->removeSelectedLimit())
            ui->sourceTableView->resizeRowsColumns();
    }
}

void MainWindow::on_sourceTableView_variableSelectionChanged(int variable)
{
    ui->actionRemoveVariable->setEnabled(variable!=-1);
    ui->buttonRemoveVariable->setEnabled(variable!=-1);
}

void MainWindow::on_sourceTableView_limitSelectionChanged(int limit)
{
    ui->actionRemoveLimit->setEnabled(limit!=-1);
    ui->buttonRemoveLimit->setEnabled(limit!=-1);
}

void MainWindow::on_sourceTableView_customContextMenuRequested(QPoint pos)
{
    ui->menuConditions->exec(ui->sourceTableView->mapToGlobal(pos));
}

void MainWindow::on_actionSolve_triggered()
{
    if(NULL != solver)
        delete solver;

    solver = new SimplexMethod::Solver(ui->sourceTableView->initialParams());

    if(NULL == output)
        output = new Output::OutputQtDocument(solver->params());
    else
        output->setParams(solver->params());

    output->clear();
    output->writeHeader();

    // Step 1. Make canonical form of problem
    solver->makeInitialBasis();
    output->writeCanonical();


    // Step 2. Try to find optimal basis
    solver->iterate();
    SimplexMethod::Status status;

    while(SimplexMethod::SolutionFound == (status = solver->status()))
    {
        output->writeTable();
        solver->changeBasis();
        solver->iterate();
    }

    // Step 3. Write result
    switch(status)
    {
    case SimplexMethod::OptimalSolutionFound:
        {
            solver->calcStabilityForColumnB();
            solver->calcStabilityForRowC();
            while(SimplexMethod::OptimalSolutionFound == (status = solver->status()))
            {
                output->writeTable();
                solver->iterateInt();
            }

            output->writeResult();
            output->writeStability();
            break;
        }
    case SimplexMethod::SolutionNotExists:
        {
            output->writeTable();
            output->writeResult();
            break;
        }
    default:
        {
            output->writeResult();
        }
    }

    output->writeFooter();

    Output::OutputQtDocument *outputqt = static_cast<Output::OutputQtDocument*>(output);
    ui->solutionView->setDocument(outputqt->document());

    ui->actionSaveSolution->setEnabled(true);
    ui->buttonSaveSolution->setEnabled(true);
    ui->actionPrintSolution->setEnabled(true);
    ui->buttonPrintSolution->setEnabled(true);
    ui->tabWidget->setCurrentIndex(1);
}



void MainWindow::on_actionSaveSolution_triggered()
{
    QFileDialog::Options options;
    QStringList filters;
    filters << tr("Open Document Text (*.odt)");
    filters << tr("HTML (*.html)");

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                tr("Save solution"),
                                QString("Solution"),
                                filters.join(";;"),
                                &selectedFilter,
                                options);

    if (!fileName.isEmpty())
    {
        QTextDocumentWriter writer;
        writer.setFileName(fileName);

        if(selectedFilter == filters[0])
        {
            writer.setFormat("odf");
        }
        else
        {
            writer.setFormat("html");
        }

        writer.write(ui->solutionView->document());
    }

}

void MainWindow::on_actionPrintSolution_triggered()
{
     QPrinter printer;
     QPrintDialog *dialog = new QPrintDialog(&printer, this);
     dialog->setWindowTitle(tr("Print document"));

     if (dialog->exec() != QDialog::Accepted)
         return;
     
     ui->solutionView->document()->print(&printer);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msg;
    msg.setIconPixmap(icon.pixmap(QSize(96, 96)));
    QString text;

    text+=QString("<h3>%1</h3>").arg(tr("About SimplexSolver"));

    text+= tr("This program is linear programming solver.");
    text+="<br/>";
    text+="<br/>";
    text+= tr("It uses simplex method to find optimum of function with selected limits. Use any search engine to find more information about this algorithm.");

    text+="<br/>";
    text+="<br/>";
    text+=copyrightText;
    text+="<br/>";
    text+="<br/>";
    text+=tr("This program comes with ABSOLUTELY NO WARRANTY.");
    text+=" ";
    text+=tr("This is free software, and you are welcome to redistribute it under certain conditions.");
    text+=" ";
    text+=tr("See %1 file of program package for details.").
            arg("LICENSE");

    msg.setWindowTitle(tr("About program"));
    msg.setText(text);
    msg.exec();

}
