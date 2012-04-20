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

#include <QCoreApplication>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <QPushButton>
#include <QLabel>

using namespace Conspire;

MainBar::MainBar(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
    build();
}

MainBar::~MainBar()
{}

void MainBar::build()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(this);
    this->setLayout(layout);

    new_button = new QPushButton( QObject::tr("New") );
    new_button->setEnabled(true);
    connect(new_button, SIGNAL(clicked()), this, SIGNAL(newPage()));

    QGraphicsProxyWidget *new_button_proxy = new QGraphicsProxyWidget(this);
    new_button_proxy->setWidget(new_button);

    undo_button = new QPushButton( QObject::tr("Undo") );
    undo_button->setEnabled(false);
    connect(undo_button, SIGNAL(clicked()), this, SIGNAL(undo()));
    
    QGraphicsProxyWidget *undo_button_proxy = new QGraphicsProxyWidget(this);
    undo_button_proxy->setWidget(undo_button);

    redo_button = new QPushButton( QObject::tr("Redo") );
    redo_button->setEnabled(false);
    connect(redo_button, SIGNAL(clicked()), this, SIGNAL(redo()));
    
    QGraphicsProxyWidget *redo_button_proxy = new QGraphicsProxyWidget(this);
    redo_button_proxy->setWidget(redo_button);

    back_button = new QPushButton( QObject::tr("Back") );
    back_button->setEnabled(false);
    connect(back_button, SIGNAL(clicked()), this, SIGNAL(back()));
    
    QGraphicsProxyWidget *back_button_proxy = new QGraphicsProxyWidget(this);
    back_button_proxy->setWidget(back_button);

    forward_button = new QPushButton( QObject::tr("Forward") );
    forward_button->setEnabled(false);
    connect(forward_button, SIGNAL(clicked()), this, SIGNAL(forward()));
    
    QGraphicsProxyWidget *forward_button_proxy = new QGraphicsProxyWidget(this);
    forward_button_proxy->setWidget(forward_button);

    home_button = new QPushButton( QObject::tr("Home") );
    home_button->setEnabled(true);
    connect(home_button, SIGNAL(clicked()), this, SIGNAL(home()));
    
    QGraphicsProxyWidget *home_button_proxy = new QGraphicsProxyWidget(this);
    home_button_proxy->setWidget(home_button);

    QPushButton *quit_button = new QPushButton( QObject::tr("Quit") );
    quit_button->setEnabled(true);
    connect(quit_button, SIGNAL(clicked()), QCoreApplication::instance(), SLOT(quit()));
    
    QGraphicsProxyWidget *quit_button_proxy = new QGraphicsProxyWidget(this);
    quit_button_proxy->setWidget(quit_button);

    layout->addItem(home_button_proxy);
    layout->addItem(back_button_proxy);
    layout->addItem(forward_button_proxy);
    layout->addItem(new_button_proxy);
    layout->addItem(undo_button_proxy);
    layout->addItem(redo_button_proxy);
    layout->addItem(quit_button_proxy);
    
    this->setOpacity(0.5);
}

void MainBar::canBackChanged(bool can_back)
{
    back_button->setEnabled(can_back);
}

void MainBar::canForwardChanged(bool can_forward)
{
    forward_button->setEnabled(can_forward);
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
{
    redo_button->setToolTip(redotext);
}

void MainBar::undoTextChanged(const QString &undotext)
{
    undo_button->setToolTip(undotext);
}
