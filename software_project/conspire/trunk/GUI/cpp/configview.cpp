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

#include "Conspire/GUI/configview.h"
#include "Conspire/GUI/configpage.h"

using namespace Conspire;

/** Constructor */
ConfigView::ConfigView(QGraphicsItem *parent) : PageView(parent)
{}

/** Construct, passing in the initial page */
ConfigView::ConfigView(PagePointer top_page, QGraphicsItem *parent)
           : PageView(parent)
{
    push(top_page, true);
}
        
ConfigView::~ConfigView()
{}

/** Reimplementation of PageView::push, with additional functionality to
    ensure that all ConfigPage pages are updated correctly when the Options
    object changes */
void ConfigView::push(PagePointer page, bool new_tab)
{
    if (page and not page->isBroken())
    {
        PageView::push(page, new_tab);
    
        ConfigPage *config = dynamic_cast<ConfigPage*>(page.data());
        
        if (config)
        {
            connect(this, SIGNAL(rereadOptions(Options)), config, SLOT(reread(Options)));

            connect(config, SIGNAL(add(QString)), this, SIGNAL(add(QString)));
            connect(config, SIGNAL(remove(QString)), this, SIGNAL(remove(QString)));
            connect(config, SIGNAL(update(QString,Obj)), 
                    this, SIGNAL(update(QString,Obj)));
        }
    }
}

/** This slot is used to update the Options object viewed by these pages */
void ConfigView::reread(Options options)
{
    emit( rereadOptions(options) );
}
