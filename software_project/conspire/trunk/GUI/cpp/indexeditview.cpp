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

#include "Conspire/GUI/indexeditview.h"
#include "Conspire/GUI/editer.h"

#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QPushButton>

using namespace Conspire;

/** Constructor */
IndexEditView::IndexEditView(EditView *parent) : EditView(parent)
{
    build();
}

/** Construct to edit the passed option */
IndexEditView::IndexEditView(Option option, EditView *parent)
              : EditView(parent)
{
    build();
    setOption(option);
}

/** Destructor */
IndexEditView::~IndexEditView()
{}

/** Return the key of this option */
QString IndexEditView::key() const
{
    if (opt.allowMultiple())
    {
        if (opt.index() == 1 and opt.indiciesWithValue().count() == 1)
            return opt.key();
        
        else
            return QString("%1[%2]").arg(opt.key()).arg(opt.index());
    }
    else
        return opt.key();
}

QString IndexEditView::rootKey() const
{
    const EditView *parent = this->parentView();
    
    if (parent)
        return parent->rootKey();
    else
        return QString::null;
}

/** Return the index of this option */
int IndexEditView::index() const
{
    return opt.index();
}

/** Return the actual option */
Option IndexEditView::option() const
{
    return opt;
}

/** Set the option to be viewed and edited */
void IndexEditView::setOption(Option option)
{
    opt = option;

    if (editer)
    {
        try
        {
            editer->setObject(option.value());
        }
        catch(...)
        {
            editer->deleteLater();
            editer = Editer::create(option.value(), this);
            grid_layout->addItem(editer, 0, 1);
            connect(editer, SIGNAL(edited(Obj)), this, SLOT(updated(Obj)));
        }
    }
    else
    {
        editer = Editer::create(option.value(), this);
        grid_layout->addItem(editer, 0, 1);
        connect(editer, SIGNAL(edited(Obj)), this, SLOT(updated(Obj)));
    }
    
    label->setText(this->key());
    del_button->setVisible( option.canDelete() );
    
}

/** Internal function called to build the widget */
void IndexEditView::build()
{
    grid_layout = new QGraphicsGridLayout(this);
    grid_layout->setSpacing(0);
    this->setLayout(grid_layout);

    label = new QLabel();
    QGraphicsProxyWidget *label_proxy = new QGraphicsProxyWidget(this);
    label_proxy->setWidget(label);    
    grid_layout->addItem(label_proxy, 0, 0);

    label->setAlignment( ::Qt::AlignCenter );
    label->setMinimumWidth(100);
    label->setVisible(true);
    
    del_button = new QPushButton("-");
    QGraphicsProxyWidget *del_button_proxy = new QGraphicsProxyWidget(this);
    del_button_proxy->setWidget(del_button);
    grid_layout->addItem(del_button_proxy, 0, 2);
    connect(del_button, SIGNAL(clicked()), this, SLOT(removed()));
   
    EditView *root = this->rootNode();
    
    if (root and root != this)
    {
        connect(this, SIGNAL(update(QString, Obj)), root, SIGNAL(update(QString, Obj)));
        connect(this, SIGNAL(add(QString)), root, SIGNAL(add(QString)));
        connect(this, SIGNAL(remove(QString)), root, SIGNAL(remove(QString)));
    }
    
    editer = 0;
}
