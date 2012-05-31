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

#include "Conspire/GUI/mainbar.h"
#include "Conspire/GUI/button.h"

#include <QCoreApplication>

using namespace Conspire;

MainBar::MainBar(QGraphicsItem *parent) : WidgetRack(::Qt::Horizontal, parent)
{
    build();
}

MainBar::~MainBar()
{}

void MainBar::build()
{
    submit_button = new Button( QObject::tr("Submit") );
    submit_button->setEnabled(true);
    connect(submit_button, SIGNAL(clicked()), this, SIGNAL(submit()));

    undo_button = new Button( QObject::tr("Undo") );
    undo_button->setEnabled(false);
    connect(undo_button, SIGNAL(clicked()), this, SIGNAL(undo()));
    
    redo_button = new Button( QObject::tr("Redo") );
    redo_button->setEnabled(false);
    connect(redo_button, SIGNAL(clicked()), this, SIGNAL(redo()));

    home_button = new Button( QObject::tr("Home") );
    home_button->setEnabled(true);
    connect(home_button, SIGNAL(clicked()), this, SIGNAL(home()));

    Button *quit_button = new Button( QObject::tr("Quit") );
    quit_button->setEnabled(true);
    connect(quit_button, SIGNAL(clicked()), QCoreApplication::instance(), SLOT(quit()));

    this->addWidget(submit_button);
    this->addWidget(home_button);
    this->addWidget(undo_button);
    this->addWidget(redo_button);
    this->addWidget(quit_button);
    
    this->setOpacity(0.5);
}

void MainBar::canRedoChanged(bool can_redo)
{
    redo_button->setEnabled(can_redo);
}

void MainBar::canUndoChanged(bool can_undo)
{
    undo_button->setEnabled(can_undo);
}

void MainBar::redoTextChanged(const QString &redotext)
{}

void MainBar::undoTextChanged(const QString &undotext)
{}
