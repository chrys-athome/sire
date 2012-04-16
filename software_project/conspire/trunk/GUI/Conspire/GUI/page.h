#ifndef CONSPIRE_PAGE_H
#define CONSPIRE_PAGE_H
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

#include "Conspire/conspire.h"

#include <QGraphicsWidget>

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    class Page;

    /** This class holds a safe pointer to a Page. This
        pointer will automatically manage the reference count
        to the Page, and will ensure that the Page
        is deleted once the reference count drops to zero. It will
        also automatically lose the pointer to the Page
        if the Page is deleted by someone else.
    */
    class CONSPIRE_EXPORT PagePointer : public QObject
    {
        Q_OBJECT
        
    public:
        PagePointer();
        PagePointer(Page *page);
        
        PagePointer(const PagePointer &other);
        
        ~PagePointer();
        
        Page* data();
        
        bool isNull() const;
        
        operator Page*();
    
        Page& operator*() const;
        Page* operator->() const;
        
        PagePointer& operator=(const PagePointer &other);
        PagePointer& operator=(Page *page);
        
        bool operator==(const PagePointer &other) const;
        bool operator!=(const PagePointer &other) const;

    private slots:
        void destroyed(QObject *obj);
        
    private:
        Page *p;
    };

    /** This is the base class of all of the main full-page widgets
        in the Conspire GUI. Each page represents a dialog with the
        user, with the GUI experience being modelled on the user
        navigating through and interacting with a series of pages.
        
        Each page can be pushed and popped from the view, with 
        a page view history that is similar to a web browser. To achieve
        this, each page has a "push" and a "pop" signal. "push" is used
        when this page creates a new page, and wants to change the view
        to the new page. "pop" is used when this page no longer needs to
        be viewed, and the view should return to the previous page.
        The "pop" signal is accompanied by an optional flag that says
        whether or not the page should be removed from the view forward
        history after it has been popped, e.g. if the page is a dialog, 
        and it would make no sense to return to that page once it has
        been viewed.
        
        Pages have a reference count, and can be held by PagePointers, 
        so that they automatically destroy themselves when they can no 
        longer be accessed by the user
        
        Navigation between pages is animated :-)
    */
    class CONSPIRE_EXPORT Page : public QGraphicsWidget
    {
        Q_OBJECT
        
    public:
        Page(QGraphicsItem *parent=0);
        
        virtual ~Page();

        virtual QString description() const;

        virtual bool isBroken() const;

    signals:
        /** Signal emitted when this page has created a new page that 
            it wants to be displayed to the user */
        void push(PagePointer new_page, bool new_tab=false);
        
        /** Signal emitted when this page no longer wants to be viewed. 
            If "forget_page" is true, then this page is removed from the
            view history. This is useful if this page is a dialog page */
        void pop(bool forget_page=false);

    protected:
        friend class PagePointer;
        void incref();
        bool decref();
        
    private:
        /** The number of PagePointer pointers currently 
            pointing to this widget */
        int ref_count;
    };

}

CONSPIRE_END_HEADER

#endif
