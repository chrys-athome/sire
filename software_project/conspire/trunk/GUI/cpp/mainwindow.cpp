/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "Conspire/GUI/mainwindow.h"
#include "Conspire/GUI/configview.h"
#include "Conspire/GUI/mainbar.h"

#include "Conspire/GUI/configdocument.h"

#include "Conspire/option.h"

using namespace Conspire;

/** Constructor */
MainWindow::MainWindow(QWidget *parent) : QGraphicsView(parent)
{
    build();
}

/** Constructor, automatically loading an initial set of options */
MainWindow::MainWindow(Options options, QWidget *parent)
           : QGraphicsView(parent)
{
    build();
    
    ConfigDocument *doc = new ConfigDocument(options);
    
    connect(doc, SIGNAL(canUndoChanged(bool)), mainbar, SLOT(canUndoChanged(bool)));
    connect(doc, SIGNAL(canRedoChanged(bool)), mainbar, SLOT(canRedoChanged(bool)));
    connect(doc, SIGNAL(canForwardChanged(bool)),
            mainbar, SLOT(canForwardChanged(bool)));
    connect(doc, SIGNAL(canBackChanged(bool)), mainbar, SLOT(canBackChanged(bool)));
    
    connect(mainbar, SIGNAL(undo()), doc, SLOT(undo()));
    connect(mainbar, SIGNAL(redo()), doc, SLOT(redo()));
    connect(mainbar, SIGNAL(back()), doc, SLOT(back()));
    connect(mainbar, SIGNAL(forward()), doc, SLOT(forward()));
    
    view->push(PagePointer(doc), true);
}

/** Destructor */
MainWindow::~MainWindow()
{}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::resizeEvent(e);
    
    view->resize( this->viewport()->size() );
    mainbar->resize( this->viewport()->size() );
    
    this->scene()->setSceneRect(0, 0, view->size().width(), view->size().height());
}

/** Internal function used to build the GUI. In general, the GUI should
    consist of a MainBar, which holds the basic, always visible, commands,
    and a PageView, which can be used to view pages */
void MainWindow::build()
{
    this->setMinimumSize(600,400);

    this->setScene( new QGraphicsScene(this) );

    mainbar = new MainBar();
    mainbar->setPos(0, 0);
    mainbar->setZValue(1);

    this->scene()->addItem(mainbar);

    view = new ConfigView();
    view->setPos(0, 0);
    view->setZValue(0);
    view->resize(this->viewport()->size());
    
    this->scene()->addItem(view);

    connect(mainbar, SIGNAL(undo()), view, SLOT(undo()));
    connect(mainbar, SIGNAL(redo()), view, SLOT(redo()));
    connect(mainbar, SIGNAL(back()), view, SLOT(back()));
    connect(mainbar, SIGNAL(forward()), view, SLOT(forward()));
    
    connect(view, SIGNAL(canBackChanged(bool)), mainbar, SLOT(canBackChanged(bool)));
    connect(view, SIGNAL(canForwardChanged(bool)),
            mainbar, SLOT(canForwardChanged(bool)));
    
    connect(view, SIGNAL(canUndoChanged(bool)),
            mainbar, SLOT(canUndoChanged(bool)));
    connect(view, SIGNAL(canRedoChanged(bool)),
            mainbar, SLOT(canRedoChanged(bool)));
}
