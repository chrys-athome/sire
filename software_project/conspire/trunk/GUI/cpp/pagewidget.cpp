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

#include "Conspire/option.h"

#include "Conspire/GUI/pagewidget.h"

#include "Conspire/exceptions.h"

using namespace Conspire;

//////////
////////// Implementation of PageWidget
//////////

/** Constructor */
PageWidget::PageWidget(QGraphicsItem *parent) 
           : QGraphicsWidget(parent), ref_count(0)
{}

/** Destructor */
PageWidget::~PageWidget()
{}

/** Internal function used to increase the reference count */
void PageWidget::incref()
{
    ref_count += 1;
}

/** Internal function used to decrease the reference count. This 
    returns "true" if the reference count has dropped to zero */
bool PageWidget::decref()
{
    ref_count -= 1;
    
    if (ref_count <= 0)
    {
        ref_count = 0;
        return true;
    }
    else
        return false;
}

/** Return whether or not this page is broken. This can occur when
    the page is updated with an incompatible new Options object */
bool PageWidget::isBroken() const
{
    return false;
}

/** This slot is called when the options object for the GUI has
    been changed. The parent Options object is passed to this page
    so that it can be updated. The default implementation does nothing. */
void PageWidget::update(Options options)
{}

//////////
////////// Implementation of PagePointer
//////////

/** Constructor */
PagePointer::PagePointer() : QObject(), p(0)
{}

/** Construct to hold a pointer to 'page'. */
PagePointer::PagePointer(PageWidget *page) : QObject(), p(page)
{
    if (p)
    {
        p->incref();
        connect(p, SIGNAL(destroyed(QObject*)), this, SLOT(destroyed(QObject*)));
    }
}

/** Copy constructor */
PagePointer::PagePointer(const PagePointer &other) : QObject(), p(other.p)
{
    if (p)
    {
        p->incref();
        connect(p, SIGNAL(destroyed(QObject*)), this, SLOT(destroyed(QObject*)));
    }
}

/** Destructor */
PagePointer::~PagePointer()
{
    if (p)
    {
        if (p->decref())
            p->deleteLater();
    }
}

/** Return the raw pointer to the page */
PageWidget* PagePointer::data()
{
    return p;
}

/** Return whether or not this pointer is null */
bool PagePointer::isNull() const
{
    return p == 0;
}

/** Return the raw pointer to the page */
PagePointer::operator PageWidget*()
{
    return p;
}

/** Dereference the pointer */
PageWidget& PagePointer::operator*() const
{

    if (not p)
    {
        throw Conspire::program_bug( Conspire::tr( 
                "An attempt has been made to dereference a null PagePointer!"),
                    CODELOC );
    }
    
    return *p;
}

/** Return the raw pointer to the page */
PageWidget* PagePointer::operator->() const
{
    return p;
}

/** Copy assignment operator */
PagePointer& PagePointer::operator=(PageWidget *page)
{
    if (p != page)
    {
        if (p)
        {
            //make sure that we are no longer told when this object is destroyed
            p->disconnect(this);
            
            //decrease the reference count
            if (p->decref())
                p->deleteLater();
        }

        page->incref();
        connect(page, SIGNAL(destroyed(QObject*)), this, SLOT(destroyed(QObject*)));
        p = page;
    }
    
    return *this;
}

/** Copy assignment operator */
PagePointer& PagePointer::operator=(const PagePointer &other)
{
    return this->operator=(other.p);
}

/** Comparison operator */
bool PagePointer::operator==(const PagePointer &other) const
{
    return p == other.p;
}

/** Comparison operator */
bool PagePointer::operator!=(const PagePointer &other) const
{
    return p != other.p;
}

/** Function called when the page pointed to by this pointer
    is destroyed */
void PagePointer::destroyed(QObject *obj)
{
    if (obj != p)
        conspireDebug() << "WEIRD - POINTER MISMATCH" << p << obj;
        
    p = 0;
}
