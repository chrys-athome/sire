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

#include "Conspire/GUI/page.h"

#include "Conspire/exceptions.h"

using namespace Conspire;

//////////
////////// Implementation of Page
//////////

/** Constructor */
Page::Page(QGraphicsItem *parent) : QGraphicsWidget(parent), ref_count(0)
{}

/** Destructor */
Page::~Page()
{}

/** Internal function used to increase the reference count */
void Page::incref()
{
    ref_count += 1;
}

/** Internal function used to decrease the reference count. This 
    returns "true" if the reference count has dropped to zero */
bool Page::decref()
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
bool Page::isBroken() const
{
    return false;
}

/** This function returns a string description of this page, which is useful
    e.g. for tab page headers */
QString Page::description() const
{
    return "???";
}

//////////
////////// Implementation of PagePointer
//////////

/** Constructor */
PagePointer::PagePointer()
{}

/** Construct to hold a pointer to 'page'. */
PagePointer::PagePointer(Page *page) : p(page)
{
    if (p)
        p->incref();
}

/** Copy constructor */
PagePointer::PagePointer(const PagePointer &other) : p(other.p)
{
    if (p)
        p->incref();
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
Page* PagePointer::data()
{
    return p;
}

/** Return the raw pointer to the page */
const Page* PagePointer::data() const
{
    return p;
}

/** Return whether or not this pointer is null */
bool PagePointer::isNull() const
{
    return (not p);
}

/** Return the raw pointer to the page */
PagePointer::operator Page*()
{
    return p;
}

/** Dereference the pointer */
Page& PagePointer::operator*() const
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
Page* PagePointer::operator->() const
{
    return p;
}

/** Copy assignment operator */
PagePointer& PagePointer::operator=(Page *page)
{
    if (p != page)
    {
        if (p)
        {
            //decrease the reference count
            if (p->decref())
                p->deleteLater();
        }

        if (page)
            page->incref();
            
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
