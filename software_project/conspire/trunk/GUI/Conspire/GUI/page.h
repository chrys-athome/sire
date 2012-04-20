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

#include "Conspire/GUI/animation.h"

#include <QGraphicsWidget>
#include <QPointer>
#include <QDebug>

CONSPIRE_BEGIN_HEADER

namespace Conspire{ class Page; }

namespace Conspire
{
    class Page;
    class PagePointer;
    class PageView;

    typedef QPointer<Page> PageWeakPointer;

    /** This class holds a safe pointer to a Page. This
        pointer will automatically manage the reference count
        to the Page, and will ensure that the Page
        is deleted once the reference count drops to zero. It will
        also automatically lose the pointer to the Page
        if the Page is deleted by someone else.
    */
    class CONSPIRE_EXPORT PagePointer
    {
    public:
        PagePointer();
        PagePointer(Page *page);
        
        PagePointer(const PagePointer &other);
        
        ~PagePointer();
        
        Page* data();
        const Page* data() const;
        
        QString toString() const;
        
        bool isNull() const;
        
        operator Page*();
    
        Page& operator*() const;
        Page* operator->() const;
        
        PagePointer& operator=(const PagePointer &other);
        PagePointer& operator=(Page *page);
        
        bool operator==(const PagePointer &other) const;
        bool operator!=(const PagePointer &other) const;

    private:
        QPointer<Page> p;
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
        Page(Page *parent=0);
        
        virtual ~Page();

        QString toString() const;

        QString title() const;
        QString description() const;

        void setTitle(QString title);
        void setDescription(QString description);

        bool animationsEnabled() const;
        bool animationsPaused() const;

        bool isBlocked() const;

        bool isBroken() const;

        PagePointer parentPage() const;
        void setParentPage(Page *new_parent);
        
        QList<PagePointer> childPages() const;

    public slots:
        void block();
        void unblock();
        
        void setBlocked(bool blocked);

        void setAnimationsEnabled(bool enabled);

        void animate(AnimationPointer anim, bool bg_anim=false);

        void pauseAnimations();
        void resumeAnimations();
        
        void setAnimationsPaused(bool paused);
        
        void stopAnimations();

    protected slots:
        void animationFinished(Animation *animation);

    signals:
        /** Signal emitted when this page has created a new page that 
            it wants to be displayed to the user */
        void push(PagePointer new_page, bool new_tab=false);
        
        /** Signal emitted when this page no longer wants to be viewed. 
            If "forget_page" is true, then this page is removed from the
            view history. This is useful if this page is a dialog page */
        void pop(bool forget_page=false);

        /** Signal emitted whenever animations are enabled or disabled */
        void animationsEnabledChanged(bool);

        /** Signal emitted whenever the animations are paused or resumed */
        void animationsPausedChanged(bool);

        /** Signal emitted whenever the blocked status of the page changes */
        void blockedChanged(bool blocked);
        
        /** Signal emitted whenever the description changes */
        void descriptionChanged(QString description);
        
        /** Signal emitted whenver the title of the page changes */
        void titleChanged(QString title);

        /** Signal emitted when the page becomes broken */
        void broken();

    protected:
        friend class PagePointer;
        void incref();
        bool decref();
        
        void setBroken();
        
        void addChild(Page *page);
        PageWeakPointer takeChild(Page *page);
        
        friend class PageView;
        PageWeakPointer weakParentPage() const;
        QList<PageWeakPointer> weakChildPages() const;

    private:
        void build();
    
        void childDeleted(Page *child);
        void parentDeleted();
    
        void setBlockedByParent(bool blocked);
        void setBlockedByAnim(bool blocked);
        
        void setAnimationsEnabledByParent(bool enabled);
        void setAnimationsPausedByParent(bool paused);
    
        void pvt_enableAnims();
        void pvt_disableAnims();
        
        void pvt_block();
        void pvt_unblock();
        
        void pvt_pauseAnims();
        void pvt_resumeAnims();
    
        /** Pointer to the parent page of this page */
        PageWeakPointer parent_page;
        
        /** The full set of child pages - these are held using
            PagePointers so that they won't be deleted behind our back */
        QList<PageWeakPointer> child_pages;
    
        /** The title of this page */
        QString page_title;
        
        /** The description of this page */
        QString page_description;
    
        /** Any foreground animation running on this page.
            This will be animating either this page, or children
            of this page. Only one foreground animation can be
            running at a time, and while it is running, this
            page and all of its children are blocked (to prevent
            the user interacting with a page in motion) */
        AnimationPointer fg_anim;
        
        /** The set of any background animations running on this page.
            These will not block the page, and are designed to run in 
            parallel, perhaps providing little sparkles or details to
            the page */
        QList<AnimationPointer> bg_anims;
    
        /** The number of PagePointer pointers currently 
            pointing to this widget */
        int ref_count;
        
        /** Whether or not animations are enabled on this page */
        bool anims_enabled;
        
        /** Whether or not animations are enabled by the parent */
        bool anims_enabled_by_parent;
        
        /** Whether or not animations are paused */
        bool anims_paused;
        
        /** Whether or not animations are paused by the parent */
        bool anims_paused_by_parent;
        
        /** Whether or not this page is bloked. A blocked page
            cannot receive user input events */
        bool is_blocked;
        
        /** Whether or not this page is blocked by the parent */
        bool is_blocked_by_parent;
        
        /** Whether or not this page is blocked by the foreground animation */
        bool is_blocked_by_anim;
        
        /** Whether or not this page is broken. Once set, a page
            is irrevecorably broken */
        bool is_broken;
    };

}

CONSPIRE_END_HEADER

#endif
