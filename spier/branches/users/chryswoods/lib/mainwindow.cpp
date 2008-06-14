/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <stdio.h>

#include <boost/shared_array.hpp>

#include <QApplication>
#include <QFileInfo>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

#include "mainwindow.h"
#include "viewer.h"
#include "glcanvas.h"

#include "SireMol/editmol.h"
#include "SireIO/pdb.h"

using namespace Spier;
using namespace SireIO;
using namespace SireBase;
using namespace SireMol;

/** Construct the main window and the rest of the spier application */
MainWindow::MainWindow() : QMainWindow(), lastdir(QDir::currentPath())
{
    //initialise the application (possibly using data
    //supplied as a command line argument)
    this->initialise();
}

/** Destructor - this will delete everything to do with spier */
MainWindow::~MainWindow()
{}

/** Initialise all parts of the main window */
void MainWindow::initialise()
{
    this->resize(1024,768);
    
    viewer = new Viewer(this);
    this->setCentralWidget(viewer);

    QAction *loadaction = new QAction(tr("&Load"), this);
    loadaction->setShortcut(tr("Ctrl+L", "File|Load"));
    connect(loadaction,SIGNAL(triggered()),this,SLOT(load()));
    
    QAction *saveaction = new QAction(tr("&Save"), this);
    saveaction->setShortcut(tr("Ctrl+S", "File|Save"));
    connect(saveaction,SIGNAL(triggered()),this,SLOT(save()));
    
    QAction *clearaction = new QAction(tr("&Clear"), this);
    connect(clearaction,SIGNAL(triggered()),viewer,SLOT(clear()));
    
    QAction *quitaction = new QAction(tr("&Quit"), this);
    quitaction->setShortcut(tr("Ctrl+Q", "File|Quit"));
    connect(quitaction,SIGNAL(triggered()),this,SLOT(quit()));
    
    QAction *aboutaction = new QAction(tr("&About"), this);
    connect(aboutaction,SIGNAL(triggered()),this,SLOT(about()));
    
    QAction *aboutqtaction = new QAction(tr("About&Qt"), this);
    connect(aboutqtaction,SIGNAL(triggered()),this,SLOT(aboutQt()));

    QAction *fullscreenaction = new QAction(tr("Toggle &Full Screen"), this);
    fullscreenaction->setShortcut(tr("Ctrl+Shift+F", "Window|Full Screen"));
    connect(fullscreenaction,SIGNAL(triggered()),this,SLOT(toggleFullScreen()));

    QMenu *filemenu = menuBar()->addMenu(tr("&File"));
    filemenu->addAction(loadaction);
    filemenu->addAction(saveaction);
    filemenu->addSeparator();
    filemenu->addAction(clearaction);
    filemenu->addSeparator();
    filemenu->addAction(quitaction);
    
    QMenu *windowmenu = menuBar()->addMenu(tr("&Window"));
    windowmenu->addAction(fullscreenaction);
    windowmenu->addSeparator();
    
    buffergroup = new QActionGroup(this);
    
    for (int i=1; i<=viewer->nbuffers(); ++i)
    {
        QAction *baction = new QAction(tr("View Buffer &%1").arg(i), buffergroup);
        baction->setCheckable(true);
        if (i == 1)
            baction->setChecked(true);
        
        buffergroup->addAction(baction);
    }
    
    connect(buffergroup,SIGNAL(triggered(QAction*)),this,SLOT(viewBuffer()));
    connect(viewer,SIGNAL(viewingBuffer(int)),this,SLOT(viewingBuffer(int)));
    
    windowmenu->addActions(buffergroup->actions());
    windowmenu->addSeparator();
    
    QAction *nextbuffer = new QAction(tr("View &Next Buffer"), this);
    nextbuffer->setShortcut(Qt::ALT + Qt::Key_Right);
    connect(nextbuffer,SIGNAL(triggered()),viewer,SLOT(nextBuffer()));
    QAction *prevbuffer = new QAction(tr("View &Previous Buffer"), this);
    prevbuffer->setShortcut(Qt::ALT + Qt::Key_Left);
    connect(prevbuffer,SIGNAL(triggered()),viewer,SLOT(prevBuffer()));
    
    windowmenu->addAction(nextbuffer);
    windowmenu->addAction(prevbuffer);
    
    QMenu *viewmenu = menuBar()->addMenu(tr("&Viewer"));
    
    QActionGroup *modegroup = new QActionGroup(this);
    
    QAction *rotaction = new QAction(tr("&Rotate"), modegroup);
    rotaction->setShortcut(tr("R", "Viewer|Rotate"));
    rotaction->setCheckable(true);
    rotaction->setChecked(true);
    connect(rotaction,SIGNAL(triggered()),viewer,SLOT(setRotationMode()));
    
    QAction *tranaction = new QAction(tr("&Translate"), modegroup);
    tranaction->setShortcut(tr("T", "Viewer|Translate"));
    tranaction->setCheckable(true);
    connect(tranaction,SIGNAL(triggered()),viewer,SLOT(setTranslationMode()));
    
    QAction *centeraction = new QAction(tr("&Centre on selection"), this);
    centeraction->setShortcut(tr("C", "Viewer|Centre on selection"));
    connect(centeraction,SIGNAL(triggered()),viewer,SLOT(centerSelection()));
    
    viewmenu->addActions(modegroup->actions());
    viewmenu->addSeparator();
    viewmenu->addAction(centeraction);
    
    menuBar()->addSeparator();
    
    QMenu *helpmenu = menuBar()->addMenu(tr("&Help"));
    helpmenu->addAction(aboutaction);
    helpmenu->addAction(aboutqtaction);
}

/** Load the molecules contained in 'file' into the viewer */
void MainWindow::load(const QString &file)
{
    qDebug() << QObject::tr("Attempting to display file \"%1\"...").arg(file);
            
    //for now, just load PDBs - we can load other things once I 
    //write the plugins...
    PDB pdb;
    EditMolList mols = pdb.read(file);
       
    for (int i=0; i<mols.count(); ++i)
    {
        EditMol mol = mols[i];
                  
        if (mol.name().isNull())
            mol.setName( QFileInfo(file).baseName() );
                  
        viewer->display(mols[i]);
    }
    
    viewer->canvas().center(mols);
}

/** Load a molecule into the viewer */
void MainWindow::load()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Choose a molecule to load"),
                        lastdir.absolutePath(), tr("PDB files (*.pdb)"));

    QDir filedir = QFileInfo(file).absoluteDir();
    
    if (filedir.exists())
    {
        lastdir = filedir;

        if (not file.isNull())
            this->load(file);
    }
}

/** Save the selected molecule to a new file */
void MainWindow::save()
{}

/** Quit the application */
void MainWindow::quit()
{
    qApp->quit();
}

/** Show an 'about' box */
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Spier"),
                      tr("Spier is a molecular view and editor etc. etc."));
}

/** Show an 'about' box for Qt */
void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, tr("Spier is built using Qt"));
}

/** Toggle full screen mode */
void MainWindow::toggleFullScreen()
{
    //toggle fullscreen mode
    this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
}

/** Recieve the signal that says we are viewing a new buffer */
void MainWindow::viewingBuffer(int i)
{
    //ensure that the ith buffer action is checked
    QList<QAction*> actions = buffergroup->actions();
    
    if (i > 0 and i <= actions.count())
    {
        QAction *action = actions[i-1];
        if (not action->isChecked())
            action->setChecked(true);
    }
}

/** View the buffer associated with the checked action */
void MainWindow::viewBuffer()
{
    //get the list of actions...
    QList<QAction*> actions = buffergroup->actions();
    
    //get the location of the current action in the list
    int idx = actions.indexOf( buffergroup->checkedAction() );
    
    //set the buffer (note that the buffers are '1' indexed, not '0' indexed)
    viewer->setBuffer(idx + 1);
}
