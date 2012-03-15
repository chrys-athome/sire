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

#include "Conspire/GUI/stringediter.h"
#include "Conspire/values.h"

#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>

using namespace Conspire;

/** Constructor */
StringEditer::StringEditer(QGraphicsItem *parent) : Editer(parent)
{
    build();
}

/** Construct to edit the passed object */
StringEditer::StringEditer(Obj object, QGraphicsItem *parent)
             : Editer(parent)
{
    build();
    setObject(object);
}

/** Destructor */
StringEditer::~StringEditer()
{}

/** Internal function used to construct the widget */
void StringEditer::build()
{
    edit = new QLineEdit();
    connect(edit, SIGNAL(returnPressed()), this, SLOT(textChanged()));

    QGraphicsProxyWidget *w = new QGraphicsProxyWidget(this);
    w->setWidget(edit);
    
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(this);
    l->setSpacing(0);
    l->addItem(w);
    this->setLayout(l);
}

/** Set the object to be edited */
void StringEditer::setObject(Obj object)
{
    edit->setText(object.toString());
    Editer::setObject(object);
}

/** Slot called when the text in the editer is changed */
void StringEditer::textChanged()
{
    try
    {
        StringValue val(edit->text());
        this->update(val);
    }
    catch(...)
    {
        edit->setText( this->object().toString() );
        throw;
    }
}
