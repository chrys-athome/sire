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

#include "Conspire/GUI/editview.h"
#include "Conspire/option.h"
#include "Conspire/values.h"
#include "Conspire/exceptions.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsGridLayout>
#include <QGraphicsProxyWidget>

using namespace Conspire;

EditView::EditView(EditView *parent) : QGraphicsWidget(parent), prnt(parent)
{
    this->setOwnedByLayout(false);
}

EditView::~EditView()
{}

EditView* EditView::parentView()
{
    return prnt;
}

const EditView* EditView::parentView() const
{
    return prnt;
}

EditView* EditView::rootNode()
{
    EditView *parent = this->parentView();
    
    if (parent)
        return parent->rootNode();
    else
        return this;
}

const EditView* EditView::rootNode() const
{
    const EditView *parent = this->parentView();
    
    if (parent)
        return parent->rootNode();
    else
        return this;
}

QString EditView::rootKey() const
{
    const EditView *parent = this->parentView();
    
    if (parent)
        return parent->fullKey();
    else
        return QString::null;
}

QString EditView::key() const
{
    const EditView *parent = this->parentView();
    
    if (parent)
        return parent->key();
    else
        return QString::null;
}

QString EditView::fullKey() const
{
    QString root = rootKey();
    QString k = key();
    
    if (root.isEmpty())
        return k;
    
    else if (k.isEmpty())
        return root;
    
    else
        return QString("%1.%2").arg(root, k);
}

void EditView::added()
{
    conspireDebug() << "EditView::added()" << fullKey();
    emit( add(fullKey()) );
}

void EditView::removed()
{
    conspireDebug() << "EditView::removed()" << fullKey();
    emit( remove(fullKey()) );
}

void EditView::updated(Obj new_obj)
{
    conspireDebug() << "EditView::updated()" << fullKey() << new_obj.toString();
    emit( update(fullKey(),new_obj) );
}
