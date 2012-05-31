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
    
    connect(mainbar, SIGNAL(undo()), doc, SLOT(undo()));
    connect(mainbar, SIGNAL(redo()), doc, SLOT(redo()));
    connect(mainbar, SIGNAL(home()), doc, SLOT(home()));
    
    connect(mainbar, SIGNAL(submit()), doc, SLOT(toggleMenuVisible()));
    
    view = PagePointer(doc);
    this->scene()->addItem(view);
}

/** Destructor */
MainWindow::~MainWindow()
{}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    QGraphicsView::resizeEvent(e);
    this->scene()->setSceneRect(0, 0, this->viewport()->size().width(), 
                                      this->viewport()->size().height());
    
    mainbar->resize( this->viewport()->size().width(),
                     mainbar->size().height() );
    mainbar->setPos(0, this->viewport()->size().height() - mainbar->size().height());

    //bg->setRect(1, 1,
    //            this->viewport()->size().width()-3, 
    //            this->viewport()->size().height() - mainbar->size().height()-2);

    if (view)
    {
        view->setPos(0, 0);
        view->resize( this->viewport()->size().width(), 
                      this->viewport()->size().height() - mainbar->size().height() );
    }
}

/** Internal function used to build the GUI. In general, the GUI should
    consist of a MainBar, which holds the basic, always visible, commands,
    and a PageView, which can be used to view pages */
void MainWindow::build()
{
    this->setMinimumSize(400,400);

    this->setScene( new QGraphicsScene(this) );

    mainbar = new MainBar();
    mainbar->setZValue(2);
    mainbar->resize(this->viewport()->size().width(), mainbar->size().height());
    mainbar->setPos(0, this->viewport()->size().height() - mainbar->size().height());

    this->scene()->addItem(mainbar);

    /*bg = new QGraphicsRectItem();
    bg->setPen( QPen( ::Qt::black ) );
    
    QLinearGradient grad;
    grad.setColorAt(0.0, ::Qt::white );
    grad.setColorAt(1.0, ::Qt::blue );
    grad.setCoordinateMode( QGradient::StretchToDeviceMode );
    
    bg->setBrush( QBrush(grad) );
    bg->setZValue(-1);
    bg->setRect(1, 1,
                this->viewport()->size().width()-3, 
                this->viewport()->size().height() - mainbar->size().height()-2);
    
    this->scene()->addItem(bg);*/
}
