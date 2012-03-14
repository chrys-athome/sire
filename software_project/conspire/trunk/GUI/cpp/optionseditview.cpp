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

#include "Conspire/GUI/optionseditview.h"
#include "Conspire/GUI/optioneditview.h"
#include "Conspire/option.h"

#include <QPushButton>
#include <QGraphicsGridLayout>

using namespace Conspire;

/** Constructor */
OptionsEditView::OptionsEditView(EditView *parent) 
                : EditView(parent), option_views(0)
{
    build();
}

/** Construct to edit the passed options */
OptionsEditView::OptionsEditView(Options options, EditView *parent)
                : EditView(parent), option_views(0)
{
    build();
    setOptions(options);
}

/** Destructor */
OptionsEditView::~OptionsEditView()
{
    delete option_views;
}

/** Return the current set of options being edited */
Options OptionsEditView::options() const
{
    return opts;
}

/** Internal function used to build the widget */
void OptionsEditView::build()
{
    //first, construct the set of OptionView objects and 
    //add them to the hash
    option_views = new QHash<QString,OptionEditView*>();
    grid_layout = new QGraphicsGridLayout();
    grid_layout->setSpacing(0);
    this->setLayout(grid_layout);
}

/** Call this function to set the options to be edited */
void OptionsEditView::setOptions(Options options)
{
    QStringList keys = options.keysWithValue();
    
    //update or add new options
    foreach (QString key, keys)
    {
        if (option_views->contains(key))
        {
            option_views->value(key)->setOption(options[key]);
        }
        else
        {
            option_views->insert(key, new OptionEditView(options[key],this));
        }
    }
    
    //add new options
    foreach (QString key, option_views->keys())
    {
        if (not keys.contains(key))
        {
            delete option_views->value(key);
            option_views->remove(key);
        }
    }
    
    //now move all of the options to their correct location
    for (int i=0; i<keys.count(); ++i)
    {
        grid_layout->addItem(option_views->value(keys[i]), i, 0, 1, 2, ::Qt::AlignLeft);
    }
    
    opts = options;
}
