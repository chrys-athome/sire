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

#include "Conspire/GUI/view.h"
#include "Conspire/option.h"

using namespace Conspire;

/** Constructor */
View::View(QWidget *parent) : QLabel(parent)
{}

/** Construct from the passed set of Options */
View::View(QWidget *parent, const Options &options) : QLabel(parent)
{
    this->setTextFormat( ::Qt::PlainText );
    this->setText( options.toString() );
}


/** Construct from the passed set of Options */
View::View(const Options &options) : QLabel()
{
    this->setTextFormat( ::Qt::PlainText );
    this->setText( options.toString() );
}

/** Destructor */
View::~View()
{}
