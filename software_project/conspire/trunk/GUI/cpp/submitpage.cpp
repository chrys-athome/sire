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

#include "Conspire/GUI/submitpage.h"

#include "Conspire/option.h"

#include <QPainter>
#include <QRectF>

using namespace Conspire;

void SubmitPage::build()
{}

/** Constructor */
SubmitPage::SubmitPage(Page *parent) : Page(parent)
{
    build();
}

/** Construct, passing in the options used to submit the job */
SubmitPage::SubmitPage(Options options, Page *parent) : Page(parent)
{
    build();
}

/** Destructor */
SubmitPage::~SubmitPage()
{}

void SubmitPage::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
}

void SubmitPage::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);
}

void SubmitPage::paint(QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget)
{
    Page::paint(painter, option, widget);
    
    painter->drawRect( QRectF(10, 10, geometry().width()-20, geometry().height()-20) );
}
