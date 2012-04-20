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

#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>

#include <QGraphicsProxyWidget>

using namespace Conspire;

//////////
////////// Implementation of Page
//////////

QString Page::toString() const
{
    return QString("Page( %1, parent = %2, type = %3, title = %4 )")
                        .arg( QString::number((size_t)(this),16) )
                        .arg( QString::number((size_t)(this->parent_page.data()),16) )
                        .arg(this->metaObject()->className())
                        .arg(this->title());
}

void Page::build()
{
    conspireDebug() << "CONSTRUCTING PAGE" << this->toString();

    ref_count = 0;
    anims_enabled = true;
    anims_enabled_by_parent = true;
    anims_paused = false;
    anims_paused_by_parent = false;
    is_blocked = false;
    is_blocked_by_parent = false;
    is_blocked_by_anim = false;
    is_broken = false;
    display_title = true;
    display_description = true;

    this->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    
    //now build the holder for the page widget
    page_box = new QGroupBox();
    page_box->setFlat(true);
    page_box->setCheckable(false);
    page_box->setAlignment( ::Qt::AlignLeft );
    page_box_proxy = new QGraphicsProxyWidget(this);
    page_box_proxy->setWidget(page_box);
    page_box->setLayout( new QVBoxLayout(page_box) );
    page_box_proxy->setGeometry(this->geometry());

    description_label = new QLabel(page_box);
    description_label->setWordWrap(true);
    description_label->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    page_box->layout()->addWidget(description_label);
    description_label->hide();
    
    box_label = new QLabel(page_box);
    box_label->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    page_box->layout()->addWidget(box_label);
    box_label->show();
}

/** Called when the widget is moved */
void Page::moveEvent(QGraphicsSceneMoveEvent *e)
{
    QGraphicsWidget::moveEvent(e);
    page_box_proxy->setGeometry(this->geometry());
    
    if (page_widget)
        page_widget->setGeometry(box_label->geometry());
}

/** Called when the widget is resized */
void Page::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    QGraphicsWidget::resizeEvent(e);
    page_box_proxy->setGeometry(this->geometry());
    
    if (page_widget)
        page_widget->setGeometry(box_label->geometry());
}

/** Constructor, passing in the parent page of this page */
Page::Page(Page *parent) : QGraphicsWidget()
{
    build();
    
    if (parent)
        setParentPage(parent);
}

/** Destructor */
Page::~Page()
{
    conspireDebug() << "DESTROYING PAGE" << this->toString();

    if (parent_page)
        parent_page->childDeleted(this);
        
    for (QList<PageWeakPointer>::const_iterator it = child_pages.begin();
         it != child_pages.end();
         ++it)
    {
        if (it->data())
            (*it)->parentDeleted();
    }
}

/** Internal function called when one of the child pages of this page is deleted */
void Page::childDeleted(Page *child)
{
    QMutableListIterator<PageWeakPointer> it(child_pages);
    
    while (it.hasNext())
    {
        if (it.next().data() == child)
        {
            it.remove();
            break;
        }
    }
}

/** Internal function called when the parent page of this page is deleted */
void Page::parentDeleted()
{
    parent_page = 0;
    
    setBlockedByParent(false);
    setAnimationsEnabledByParent(true);
    setAnimationsPausedByParent(false);
}

/** Add the passed page as a child of this page */
void Page::addChild(Page *page)
{
    if (page)
    {
        page->setParentPage(this);
        
        //is this already a child page?
        for (QList<PageWeakPointer>::const_iterator it = child_pages.constBegin();
             it != child_pages.constEnd();
             ++it)
        {
            if (it->data() == page)
            {
                //yes it is - the child has been added already, so nothing
                //needs to be done
                return;
            }
        }
        
        //add the child page
        child_pages.append( PageWeakPointer(page) );
    }
}

/** Remove the passed page as a child of this page, returning a PagePointer
    to the removed page. If the passed page is not a child, then a null
    PagePointer is returned */
PageWeakPointer Page::takeChild(Page *child)
{
    if (child)
    {
        PageWeakPointer child_page;
        
        QMutableListIterator<PageWeakPointer> it(child_pages);
        
        while (it.hasNext())
        {
            PageWeakPointer &c = it.next();
            
            if (c.data() == child)
            {
                child_page = c;
                break;
            }
        }
        
        if (child_page)
        {
            child_page->setParentPage(0);
        }
        
        return child_page;
    }
    else
        return PageWeakPointer();
}

/** Set the parent page of this Page */
void Page::setParentPage(Page *new_parent)
{
    if (new_parent == parent_page)
        return;
        
    PageWeakPointer this_page;
        
    //remove the page from any existing parent page
    if (parent_page)
    {
        PageWeakPointer old_parent = parent_page;
        parent_page = 0;
    
        this_page = old_parent->takeChild(this);
        
        if (this_page.data() != this)
        {
            conspireDebug() << "MAJOR BUG! HOW DID THE PARENT LOSE THIS CHILD?";
        }
        
        //if we are changing parent, then make sure that any parent-caused
        //blocks are properly reset here - this ensures that they are not
        //disabled, then enabled
        if (new_parent)
        {
            if (old_parent->isBlocked() != new_parent->isBlocked())
            {
                this->setBlockedByParent(new_parent->isBlocked());
            }
            
            if (old_parent->animationsEnabled() != new_parent->animationsEnabled())
            {
                this->setAnimationsEnabledByParent(new_parent->animationsEnabled());
            }
            
            if (old_parent->animationsPaused() != new_parent->animationsPaused())
            {
                if (new_parent->anims_paused or new_parent->anims_paused_by_parent)
                {
                    this->setAnimationsPausedByParent(true);
                }
                else
                {
                    this->setAnimationsPausedByParent(false);
                }
            }
        }
    }
    
    //now add this page to the new parent
    if (new_parent)
    {
        this_page = PageWeakPointer(this);
    
        parent_page = new_parent;
        parent_page->addChild(this_page);
        
        setBlockedByParent( parent_page->isBlocked() );
        setAnimationsEnabledByParent( parent_page->animationsEnabled() );
        setAnimationsPausedByParent( parent_page->anims_paused or
                                     parent_page->anims_paused_by_parent );
    }
    else
    {
        setBlockedByParent(false);
        setAnimationsEnabledByParent(true);
        setAnimationsPausedByParent(false);
    }
}

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

/** Return the title of the page */
QString Page::title() const
{
    return page_title;
}

/** Return the description of the page */
QString Page::description() const
{
    return page_description;
}

/** Set the title of the page */
void Page::setTitle(QString title)
{
    if (title != page_title)
    {
        page_title = title;

        if (display_title)
            page_box->setTitle(page_title);
        
        emit( titleChanged(page_title) );
    }
}

/** Set the description of the page */
void Page::setDescription(QString description)
{
    if (description != page_description)
    {
        page_description = description;
        
        if (display_description)
        {
            description_label->setText(page_description);
            description_label->setVisible( not page_description.isEmpty() );
        }
        
        emit( descriptionChanged(page_description) );
    }
}

/** Set whether or not the page title should be displayed */
void Page::setTitleDisplayed(bool displayed)
{
    if (display_title != displayed)
    {
        if (displayed)
        {
            display_title = true;
            page_box->setTitle(page_title);
        }
        else
        {
            display_title = false;
            page_box->setTitle( QString::null );
        }
    }
}

/** Set whether or not the page description should be displayed */
void Page::setDescriptionDisplayed(bool displayed)
{
    if (display_description != displayed)
    {
        if (displayed)
        {
            display_description = true;
            description_label->setText(page_description);
            description_label->show();
        }
        else
        {
            display_description = false;
            description_label->setText(QString::null);
            description_label->hide();
        }
    }
}

/** Set the widget that is displayed on this page. This Page takes
    over ownership of the widget and is free to delete it whenever it wants... */
void Page::setPageWidget(QGraphicsWidget *widget)
{
    if (page_widget)
    {
        page_widget->deleteLater();
        page_widget = 0;
    }
    
    page_widget = widget;
    
    if (page_widget)
    {
        page_widget->setParentItem(this);
        page_widget->setGeometry(box_label->geometry());
        page_widget->show();
    }
}

/** Set the widget that is displayed on this page. This Page takes
    over ownership of the widget and is free to delete it whenever it wants... */
void Page::setPageWidget(QWidget *widget)
{
    if (widget)
    {
        QGraphicsProxyWidget *widget_proxy = new QGraphicsProxyWidget(this);
        widget_proxy->setWidget(widget);
        this->setPageWidget(widget_proxy);
    }
    else if (page_widget)
    {
        //clear the old widget
        page_widget->deleteLater();
        page_widget = 0;
    }
}

/** Internal function that actually blocks the page */
void Page::pvt_block()
{
    this->setEnabled(false);

    //need to pause all animations in the child pages
    if (not animationsPaused())
    {
        for (QList<PageWeakPointer>::iterator it = child_pages.begin();
             it != child_pages.end();
             ++it)
        {
            if (it->data())
                (*it)->setAnimationsPausedByParent(true);
        }
    }
    
    emit( blockedChanged(true) );
}

/** Internal function that actually unblocks a page */
void Page::pvt_unblock()
{
    this->setEnabled(true);
    
    //may have to resume all animations in the child pages
    if (not animationsPaused())
    {
        for (QList<PageWeakPointer>::iterator it = child_pages.begin();
             it != child_pages.end();
             ++it)
        {
            if (it->data())
                (*it)->setAnimationsPausedByParent(false);
        }
    }
    
    emit( blockedChanged(false) );
}

/** Internal function called by the parent to block this child page */
void Page::setBlockedByParent(bool blocked)
{
    if (is_blocked_by_parent == blocked)
        return;
        
    if (blocked)
    {
        is_blocked_by_parent = true;
        
        if (not (is_blocked or is_blocked_by_anim))
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setBlockedByParent(true);
            }
            
            pvt_block();
        }
    }
    else
    {
        is_blocked_by_parent = false;
        
        if (not (is_blocked or is_blocked_by_anim))
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setBlockedByParent(false);
            }
            
            pvt_unblock();
        }
    }
}

/** Internal function that is called when this page is blocked by the 
    running of the foreground animation */
void Page::setBlockedByAnim(bool blocked)
{
    if (is_blocked_by_anim == blocked)
        return;
        
    if (blocked)
    {
        is_blocked_by_anim = true;
        
        if (not (is_blocked or is_blocked_by_parent))
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setBlockedByParent(true);
            }
            
            pvt_block();
        }
    }
    else
    {
        is_blocked_by_anim = false;
        
        if (not (is_blocked or is_blocked_by_parent))
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setBlockedByParent(false);
            }
            
            pvt_unblock();
        }
    }
}

/** Set whether or not this page is blocked. Note that a page will remain
    blocked if the parent page is blocked */
void Page::setBlocked(bool blocked)
{
    if (is_blocked == blocked)
        return;
        
    if (blocked)
    {
        is_blocked = true;
        
        if (not (is_blocked_by_parent or is_blocked_by_anim))
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setBlockedByParent(true);
            }
            
            pvt_block();
        }
    }
    else
    {
        is_blocked = false;
        
        if (not (is_blocked_by_parent or is_blocked_by_anim))
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setBlockedByParent(false);
            }
            
            pvt_unblock();
        }
    }
}

/** Block this page */
void Page::block()
{
    setBlocked(true);
}

/** Unblock this page. Note that this page will remain blocked if the
    parent page is blocked (a child is always blocked if the parent
    is blocked) */
void Page::unblock()
{
    setBlocked(false);
}

/** Internal function used to actually disable all animations */
void Page::pvt_disableAnims()
{
    if (fg_anim)
    {
        fg_anim->stop();
    }
    
    QList<AnimationPointer> anims = bg_anims;
    
    for (QList<AnimationPointer>::iterator it = anims.begin();
         it != anims.end();
         ++it)
    {
        (*it)->stop();
    }
    
    emit( animationsEnabledChanged(false) );
}

/** Internal function used to enable all animations */
void Page::pvt_enableAnims()
{
    emit( animationsEnabledChanged(true) );
}

/** Internal function used by a parent to disable a child's animations */
void Page::setAnimationsEnabledByParent(bool enable)
{
    if (anims_enabled_by_parent == enable)
        return;
        
    if (enable)
    {
        anims_enabled_by_parent = true;
        
        if (anims_enabled)
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setAnimationsEnabledByParent(true);
            }
            
            pvt_enableAnims();
        }
    }
    else
    {
        anims_enabled_by_parent = false;

        for (QList<PageWeakPointer>::iterator it = child_pages.begin();
             it != child_pages.end();
             ++it)
        {
            if (it->data())
                (*it)->setAnimationsEnabledByParent(false);
        }
        
        if (anims_enabled)
        {
            pvt_disableAnims();
        }
    }
}

/** Set whether or not animations are enabled */
void Page::setAnimationsEnabled(bool enabled)
{
    if (anims_enabled == enabled)
        return;
        
    if (enabled)
    {
        anims_enabled = true;
        
        if (anims_enabled_by_parent)
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setAnimationsEnabledByParent(true);
            }
            
            pvt_enableAnims();
        }
    }
    else
    {
        anims_enabled = false;
        
        for (QList<PageWeakPointer>::iterator it = child_pages.begin(); 
             it != child_pages.end();
             ++it)
        {
            if (it->data())
                (*it)->setAnimationsEnabledByParent(false);
        }

        if (anims_enabled_by_parent)
        {
            pvt_disableAnims();
        }
    }
}

/** Return whether or not animations are enabled */
bool Page::animationsEnabled() const
{
    return anims_enabled and anims_enabled_by_parent;
}

/** Return whether or not this page is blocked */
bool Page::isBlocked() const
{
    return is_blocked or is_blocked_by_parent;
}

/** Return the parent page of this page. This returns a null pointer
    if there is no parent of this page */
PagePointer Page::parentPage() const
{
    return PagePointer(parent_page);
}

/** Internal function used to get a weak pointer to the parent page */
PageWeakPointer Page::weakParentPage() const
{
    return parent_page;
}

/** Return all of the direct child pages of this page. This returns
    an empty list if there are no child pages */
QList<PagePointer> Page::childPages() const
{
    QList<PagePointer> children;
    
    for (QList<PageWeakPointer>::const_iterator it = child_pages.constBegin();
         it != child_pages.constEnd();
         ++it)
    {
        children.append( PagePointer(*it) );
    }
    
    return children;
}

/** Internal function used to get weak pointer to the child pages */
QList<PageWeakPointer> Page::weakChildPages() const
{
    return child_pages;
}

/** Add an animation to this page, adding it by default as a foreground
    (blocking) animation, or as a background animation if "bg_anim" is true */
void Page::animate(AnimationPointer anim, bool bg_anim)
{
    if (not anim)
        return;

    if (bg_anim)
    {
        bg_anims.append(anim);
        connect(anim, SIGNAL(finished(Animation*)), 
                this, SLOT(animationFinished(Animation*)));
                
        anim->start();
        
        //immediately stop the animation if animations are not enabled
        if (not animationsEnabled())
        {
            anim->stop();
        }
        //immediately pause the animation if animations are paused
        else if (animationsPaused())
        {
            anim->pause();
        }
    }
    else
    {
        this->setBlockedByAnim(true);
    
        if (fg_anim)
        {
            fg_anim->disconnect(this);
            this->disconnect(fg_anim);
            fg_anim->stop();
            fg_anim = 0;
        }
        
        fg_anim = anim;
        connect(anim, SIGNAL(finished(Animation*)),
                this, SLOT(animationFinished(Animation*)));

        anim->start();
                
        //immediately stop the animation if animations are not enabled
        if (not animationsEnabled())
        {
            anim->stop();
        }
        //immediately pause the animation if animations are paused
        else if (animationsPaused())
        {
            anim->pause();
        }
    }
}

/** Internal function used to actually pause animations */
void Page::pvt_pauseAnims()
{
    if (fg_anim)
        fg_anim->pause();
        
    for (QList<AnimationPointer>::iterator it = bg_anims.begin(); 
         it != bg_anims.end();
         ++it)
    {
        if (it->data())
            (*it)->pause();
    }
    
    emit( animationsPausedChanged(true) );
}

/** Internal function used to actually resume animations */
void Page::pvt_resumeAnims()
{
    if (fg_anim)
        fg_anim->resume();
        
    for (QList<AnimationPointer>::iterator it = bg_anims.begin();
         it != bg_anims.end();
         ++it)
    {
        if (it->data())
            (*it)->resume();
    }
    
    emit( animationsPausedChanged(false) );
}

/** Return whether or not the animations are paused */
bool Page::animationsPaused() const
{
    return anims_paused or anims_paused_by_parent;
}

/** Internal function used by the parent page to pause or resume animations */
void Page::setAnimationsPausedByParent(bool paused)
{
    if (paused == anims_paused_by_parent)
        return;
        
    if (paused)
    {
        anims_paused_by_parent = true;
        
        for (QList<PageWeakPointer>::iterator it = child_pages.begin();
             it != child_pages.end();
             ++it)
        {
            if (it->data())
                (*it)->setAnimationsPausedByParent(true);
        }
        
        if (not anims_paused)
        {
            pvt_pauseAnims();
        }
    }
    else
    {
        anims_paused_by_parent = false;
        
        if (not anims_paused)
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setAnimationsPausedByParent(false);
            }
            
            pvt_resumeAnims();
        }
    }
}

/** Change the state of whether or not the animations are paused */
void Page::setAnimationsPaused(bool paused)
{
    if (paused == anims_paused)
        return;
        
    if (paused)
    {
        anims_paused = true;
        
        for (QList<PageWeakPointer>::iterator it = child_pages.begin();
             it != child_pages.end();
             ++it)
        {
            if (it->data())
                (*it)->setAnimationsPausedByParent(true);
        }
        
        if (not anims_paused_by_parent)
        {
            pvt_pauseAnims();
        }
    }
    else
    {
        anims_paused = false;
        
        if (not anims_paused_by_parent)
        {
            for (QList<PageWeakPointer>::iterator it = child_pages.begin();
                 it != child_pages.end();
                 ++it)
            {
                if (it->data())
                    (*it)->setAnimationsPausedByParent(false);
            }
            
            pvt_resumeAnims();
        }
    }
}

/** Pause all animations - this also pauses all child page animations */
void Page::pauseAnimations()
{
    setAnimationsPaused(true);
}

/** Resume all animations - note that this will only resume the animation
    if the parent has not paused the animation. Also note that parents
    will automatically pause all animations of children when the parent
    is blocked */
void Page::resumeAnimations()
{
    setAnimationsPaused(false);
}

/** Stop all animations on this page, and on all child pages */
void Page::stopAnimations()
{
    for (QList<PageWeakPointer>::iterator it = child_pages.begin(); 
         it != child_pages.end();
         ++it)
    {
        if (it->data())
            (*it)->stopAnimations();
    }
    
    QList<AnimationPointer> anims = bg_anims;
    
    for (QList<AnimationPointer>::iterator it = anims.begin();
         it != anims.end();
         ++it)
    {
        (*it)->stop();
    }
    
    if (fg_anim)
        fg_anim->stop();
}

/** This internal slot is called when one of the animations running
    on this page finishes */
void Page::animationFinished(Animation *animation)
{
    if (animation)
    {
        animation->disconnect(this);
        this->disconnect(animation);

        if (animation == fg_anim.data())
        {
            //the foreground animation has finished - we can unblock
            //the page
            fg_anim = 0;
            this->setBlockedByAnim(false);
        }
        
        //now remove the animation from the list of background animations
        QMutableListIterator<AnimationPointer> it(bg_anims);
        
        while (it.hasNext())
        {
            if (it.next().data() == animation)
            {
                it.remove();
            }
        }
    }
}

/** Set this page as broken. Once a page is broken, it is disabled
    and irrevecorably placed into a "broken" state. Use this if something
    causes a page to become invalid */
void Page::setBroken()
{
    if (not is_broken)
    {
        is_broken = true;
        emit( broken() );
    }
}

/** Return whether or not this page is broken. This can occur when
    the page is updated with an incompatible new Options object */
bool Page::isBroken() const
{
    return is_broken;
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

QString PagePointer::toString() const
{
    if (p)
        return p->toString();
    else
        return "Page::null";
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
