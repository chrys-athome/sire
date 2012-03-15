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

#include "Conspire/GUI/optioneditview.h"
#include "Conspire/GUI/indexeditview.h"

#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <QPushButton>

using namespace Conspire;

/** Constructor */
OptionEditView::OptionEditView(EditView *parent) : EditView(parent)
{
    build();
}

/** Construct to edit the passed option */
OptionEditView::OptionEditView(Option option, EditView *parent)
               : EditView(parent)
{
    build();
    setOption(option);
}

/** Destructor */
OptionEditView::~OptionEditView()
{
    delete index_views;
}

/** Internal function used to build the widget */
void OptionEditView::build()
{
    index_views = new QHash<int,IndexEditView*>();

    grid_layout = new QGraphicsGridLayout(this);
    grid_layout->setSpacing(0);
    this->setLayout(grid_layout);
    
    help_button = new QPushButton(QObject::tr("Help"));
    connect(help_button, SIGNAL(clicked()), this, SLOT(showHelp()));
    
    help_button_proxy = new QGraphicsProxyWidget(this);
    help_button_proxy->setWidget(help_button);
    
    grid_layout->addItem(help_button_proxy, 0, 0, ::Qt::AlignVCenter);
}

/** Return the key for the edited option */
QString OptionEditView::key() const
{
    return opt.key();
}

/** Return the option being edited */
Option OptionEditView::option() const
{
    return opt;
}

/** Display the help for this option */
void OptionEditView::showHelp() const
{
}

/** Set the option to be edited */
void OptionEditView::setOption(Option option)
{
    opt = option;
        
    QList<int> idxs = option.indiciesWithValue();
    
    foreach (int idx, idxs)
    {
        if (index_views->contains(idx))
        {
            index_views->value(idx)->setOption(option[idx]);
        }
        else
        {
            index_views->insert(idx, new IndexEditView(option[idx],this));
        }
    }
    
    foreach (int idx, index_views->keys())
    {
        if (not idxs.contains(idx))
        {
            delete index_views->value(idx);
            index_views->remove(idx);
        }
    }
    
    if (idxs.isEmpty())
    {
        help_button->setVisible(false);
        return;
    }
    else
    {
        help_button->setVisible(true);
        grid_layout->addItem(help_button_proxy, 0, 0, idxs.count(), 1,
                             ::Qt::AlignVCenter);
        
        qSort(idxs);
        
        for (int i=0; i<idxs.count(); ++i)
        {
            grid_layout->addItem(index_views->value(idxs[i]), i, 1);
        }
    }
}
