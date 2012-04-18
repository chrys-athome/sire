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

#include "Conspire/GUI/pageview.h"

#include <QTabBar>
#include <QLabel>

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

using namespace Conspire;

/** Constructor for a tab in the page view */
PageView::Tab::Tab()
{}

/** Destructor for a tab in the page view */
PageView::Tab::~Tab()
{}

/** Constructor */
PageView::PageView(QGraphicsItem *parent) : Page(parent)
{
    build();
}

/** Constructor, passing in the top page for the first tab */
PageView::PageView(PagePointer top_page, QGraphicsItem *parent) : Page(parent)
{
    build();
    push(top_page, true);
}

/** Destructor */
PageView::~PageView()
{
    //delete all of the tabs
    foreach (Tab *tab, tabpages)
    {
        delete tab;
    }
}

/** Return the number of tabs */
int PageView::count() const
{
    return tabpages.count();
}

/** Go back to the last page in the history in the current tab */
void PageView::back()
{
    if (current_tab == -1)
        return;

    this->popView(false);

    Tab *tab = tabpages[current_tab];
    emit( canBackChanged(not tab->page_history.isEmpty()) );
    emit( canForwardChanged(not tab->page_future.isEmpty()) );
}

/** Go forward to the next page in the forward history in the current tab */
void PageView::forward()
{
    if (current_tab == -1)
        return;

    Tab *tab = tabpages[current_tab];
    
    if (tab->page_future.isEmpty())
        return;
        
    PagePointer page = tab->page_future.pop();
    
    if (not page or page->isBroken())
    {
        //this widget has died - try to get another one
        this->forward();
        return;
    }
    
    this->pushView(page, false);
    
    emit( canBackChanged(not tab->page_history.isEmpty()) );
    emit( canForwardChanged(not tab->page_future.isEmpty()) );
}

/** Return home in the current tab */
void PageView::home(bool clear_history)
{
    if (current_tab == -1)
        return;
        
    Tab *tab = tabpages[current_tab];

    if (clear_history)
    {
        tab->page_history.clear();
        tab->page_future.clear();
        pop(true);
    }
    else
    {
        if (tab->current_page != tab->top_page)
        {
            push(tab->top_page);
        }
    }
}

/** Push the passed page into the current tab. This displays the new
    page and pushes the previously viewed page into the view history. Note
    that if "new_tab" is true, then this creates a new tab using the passed
    page, which becomes its own top-level page in that tab */
void PageView::push(PagePointer page, bool new_tab)
{
    conspireDebug() << "PUSHING A PAGE" << new_tab;

    if (not page or page->isBroken())
    {
        conspireDebug() << "TRYING TO PUSH A BROKEN PAGE?";
        return;
    }

    page->hide();
    page->setParentItem(this);

    Tab *tab = 0;

    if ((current_tab == -1) or new_tab)
    {
        conspireDebug() << "BUILDING A NEW TAB";
    
        tab = new Tab();
        tab->top_page = page;
        tab->current_page = page;
        
        tabpages.append(tab);
        int idx = tabbar->addTab(page->description());
        tabbar->setTabEnabled(idx,true);
        
        if (idx != tabpages.count() - 1)
            conspireDebug() << "WEIRD TABBAR INDEX!!!";
            
        if (tabpages.count() <= 2)
            tabbar->hide();
        else
            tabbar->show();
            
        conspireDebug() << "CHANGE TO THE NEW TAB";
        this->changeTab(idx);
    }
    else
    {
        tab = tabpages[current_tab];
        this->pushView(page, true);
    }
    
    connect(page.data(), SIGNAL(push(PagePointer,bool)), 
            this, SLOT(push(PagePointer,bool)));
    connect(page.data(), SIGNAL(pop(bool)), this, SLOT(pop(bool)));

    //make sure that this view can override the events of the page,
    //if so desired
    page->installEventFilter(this);

    //if this page is itself a PageView, then make sure that any
    //animation-enabled state changes are propogated to this child
    PageView *view = dynamic_cast<PageView*>(page.data());
    
    if (view)
    {
        view->setAnimationsEnabled(this->animationsEnabled());
        connect(this, SIGNAL(animationsEnabledChanged(bool)),
                view, SLOT(setAnimationsEnabled(bool)));
    }

    conspireDebug() << "ADDED PAGE" << page.data() << "TO TAB" << tab;

    emit( canBackChanged(not tab->page_history.isEmpty()) );
    emit( canForwardChanged(not tab->page_future.isEmpty()) );
}

/** This function checks to see if the Page is no longer part of this view.
    If it is not part of the view, then it disconnects all signals between
    the page and the view, and makes sure that this view is no longer
    intercepting events */
void PageView::checkDisconnectPage(PagePointer &page)
{
    if (page)
    {
        if (not isChildPage(page.data()))
        {
            if (page->parentItem() == this)
            {
                page->setParentItem(0);
            }
            
            page->disconnect(this);
            this->disconnect(page.data());
            page->removeEventFilter(this);
        }
    }
}

/** This function checks to see if any of the pages in the passed stack  
    are part of this view. If they are not, then they are disconnected
    from this view */
void PageView::checkDisconnectPages(QStack<PagePointer> &pages)
{
    for (QStack<PagePointer>::iterator it = pages.begin();
         it != pages.end();
         ++it)
    {
        checkDisconnectPage(*it);
    }
}

/** This function checks to see if any of the pages in the passed tab
    are part of this view. If they are not, then they are disconnected
    from this view */
void PageView::checkDisconnectTab(Tab *tab)
{
    if (tab)
    {
        checkDisconnectPage(tab->current_page);
        checkDisconnectPage(tab->top_page);
        checkDisconnectPages(tab->page_history);
        checkDisconnectPages(tab->page_future);
    }
}

/** Pop the current view and return to the previous view in the current tab.
    This will forget the popped view if "forget_page" is true, which is useful
    if this page is a dialog type widget */
void PageView::pop(bool forget_page)
{
    PagePointer page = this->popView(forget_page);

    checkDisconnectPage(page);

    if (current_tab != -1)
    {
        Tab *tab = tabpages[current_tab];
        emit( canBackChanged(not tab->page_history.isEmpty()) );
        emit( canForwardChanged(not tab->page_future.isEmpty()) );
    }
}

/** Change the order of the tabs, so that tab "from" now has index "to" */
void PageView::moveTab(int from, int to)
{
    if (from >= 0 and from < tabpages.count() and
        to >= 0 and to < tabpages.count() and
        from != to)
    {
        Tab *tab = tabpages.takeAt(from);
        tabpages.insert(to,tab);
        
        for (int i=0; i<tabpages.count(); ++i)
        {
            tab = tabpages.at(i);
            
            if (tab->current_page)
            {
                tabbar->setTabText(i, tab->current_page->description());
            }
            else
            {
                tabbar->setTabText(i, Conspire::tr("EMPTY"));
            }
        }
    }
}

/** Change the currently viewed tab */
void PageView::changeTab(int index)
{
    if (index < 0 or index >= tabpages.count() or index == current_tab)
    {
        return;
    }
    
    conspireDebug() << index << tabpages.count();
    
    if (current_tab == -1)
    {
        conspireDebug() << "animateNew new tab and new page";
        Tab *new_tab = tabpages[index];
        current_tab = index;
        animateNew(new_tab->current_page);
    }
    else
    {
        conspireDebug() << "animateSwitch from one tab to the other";
        Tab *old_tab = tabpages[current_tab];
        Tab *new_tab = tabpages[index];

        bool move_forwards = (index > current_tab);

        current_tab = index;
        animateSwitch(old_tab->current_page, new_tab->current_page, move_forwards);
    }

    conspireDebug() << "HERE";
    
    if (tabbar->currentIndex() != current_tab)
    {
        tabbar->setCurrentIndex(current_tab);
    }

    conspireDebug() << "HERE2";

    if (current_tab != -1)
    {
        Tab *tab = tabpages[current_tab];
        emit( canBackChanged( not tab->page_history.isEmpty() ) );
        emit( canForwardChanged( not tab->page_future.isEmpty() ) );
    }

    conspireDebug() << "HERE3";
}

/** Close the current tab. This will delete the tab and all of its contents */
void PageView::closeTab(int index)
{
    if (index < 0 or index >= tabpages.count())
        return;
        
    if (index != current_tab)
    {
        //remove the tab in the background
        Tab *tab = tabpages.takeAt(index);
        checkDisconnectTab(tab);
        
        delete tab;
    }
    else
    {
        //we need to move to the last tab or the next tab
        int new_tab = -1;
    
        if (index < tabpages.count() - 1)
        {
            new_tab = index + 1;
        }
        else if (index > 0)
        {
            new_tab = index - 1;
        }
        
        changeTab(new_tab);

        Tab *tab = tabpages.takeAt(index);
        checkDisconnectTab(tab);

        delete tab;
    }
    
    if (tabpages.count() != tabbar->count())
    {
        tabbar->removeTab(index);
    }
    
    if (tabpages.count() <= 1)
    {
        tabbar->hide();
    }
}

/** Close all of the tabs - this will delete every page and all of their contents */
void PageView::closeAll()
{
    if (current_tab != -1)
    {
        Tab *tab = tabpages[current_tab];
        
        if (tab->current_page)
            animateDestroy(tab->current_page);
    }

    current_tab = -1;
    
    QList<Tab*> old_tabpages = tabpages;
    tabpages = QList<Tab*>();
    
    foreach (Tab *tab, old_tabpages)
    {
        checkDisconnectTab(tab);
        delete tab;
    }
    
    while (tabbar->count() > 0)
    {
        tabbar->removeTab(0);
    }
}

/** Called when this widget is resized */
void PageView::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
    
    if (current_tab != -1)
    {
        Tab *tab = tabpages[current_tab];
        
        if (tab->current_page)
        {
            tab->current_page->resize(label->size());
        }
    }
}

/** Called when a key press is detected in this widget */
void PageView::keyPressEvent(QKeyEvent *e)
{
    if (e)
    {
        if (e->key() == ::Qt::Key_Escape)
        {
            this->popView();
        }
        else
        {
            Page::keyPressEvent(e);
        }
    }
}

/** Return whether or not the passed Page is a child view of this PageView */
bool PageView::isChildPage(const Page *page) const
{
    if (page)
    {
        foreach (const Tab *tab, tabpages)
        {
            if (tab->top_page.data() == page or
                tab->current_page.data() == page)
                return true;
            
            else
            {
                for (QStack<PagePointer>::const_iterator 
                                it = tab->page_history.constBegin();
                     it != tab->page_history.constEnd();
                     ++it)
                {
                    if (it->data() == page)
                        return true;
                }

                for (QStack<PagePointer>::const_iterator 
                                it = tab->page_future.constBegin();
                     it != tab->page_future.constEnd();
                     ++it)
                {
                    if (it->data() == page)
                        return true;
                }
            }
        }
    }

    return false;
}

/** Return whether or not the passed QObject is a child view of this PageView */
bool PageView::isChildObject(const QObject *object) const
{
    foreach (const QObject *child, this->children())
    {
        if (object == child)
            return true;
    }

    return isChildPage(dynamic_cast<const Page*>(object));
}

/** Allow filtering of our own events... */
bool PageView::event(QEvent *event)
{
    if (fg_anim)
    {
        switch( event->type() )
        {
            case QEvent::InputMethod:
            case QEvent::KeyPress:
            case QEvent::MouseButtonDblClick:
            case QEvent::MouseButtonPress:
            case QEvent::Shortcut:
            case QEvent::TabletPress:
            case QEvent::Wheel:
            case QEvent::TouchBegin:
            case QEvent::TouchUpdate:
            case QEvent::TouchEnd:
            case QEvent::Gesture:
            case QEvent::GestureOverride:
            {
                conspireDebug() << "INPUT EVENT CANCELLED ANIMATION"
                                << event->type();
                          
                fg_anim->disconnect(this);
                fg_anim->stop();
                fg_anim = 0;
                return true;
            }
            
            default:
                break;
        }
    }
    
    return Page::event(event);
}

/** Filter events for the passed object. If this is a child page, and a foreground
    animation is in progress, then this intercepts and blocks all user-input
    events to the child page. This prevents the user from interacting with pages
    while they are in transition or being animated. Instead, the animation is
    automatically stopped and advanced to the end frame */
bool PageView::eventFilter(QObject *object, QEvent *event)
{
    if (not event)
        return false;

    else if (fg_anim)
    {
        if (isChildObject(object))
        {
            switch( event->type() )
            {
                case QEvent::InputMethod:
                case QEvent::KeyPress:
                case QEvent::MouseButtonDblClick:
                case QEvent::MouseButtonPress:
                case QEvent::Shortcut:
                case QEvent::TabletPress:
                case QEvent::Wheel:
                case QEvent::TouchBegin:
                case QEvent::TouchUpdate:
                case QEvent::TouchEnd:
                case QEvent::Gesture:
                case QEvent::GestureOverride:
                {
                    conspireDebug() << "INPUT EVENT CANCELLED ANIMATION"
                                    << event->type();
                              
                    fg_anim->disconnect(this);
                    fg_anim->stop();
                    fg_anim = 0;
                    return true;
                }
                
                default:
                    break;
            }
        }
    }
    
    return false;
}

/** Run an animation in this view. The animation can either be a foreground
    animation (default), or a background animation if "bg_anim" is true. 
    
    If the animation is a foreground animation, then any current foreground
    animation is stopped, and is replaced by this animation. When a foreground
    animation is running, it blocks all user input events sent to child pages 
    or child widgets of this view. Instead, any user event will automatically
    cancel the animation and advance it to the end.
    
    If the animation is a background animation, then it runs in the background,
    free from any interaction with the user. Background animations should be
    simple things that will not interupt the workflow of the user, or expose
    functionality to the user outside of their current workflow.
    
    Note that all animations will be immediately stopped and advanced to the
    end frame if "animationsEnabled()" is false, or if it is ever set to false.
*/
void PageView::animate(AnimationPointer animation, bool bg_anim)
{
    if (not animation)
        return;
        
    if (animationsEnabled())
    {
        if (bg_anim)
        {
            bg_anims.append(animation);
            animation->start();
            connect(animation.data(), SIGNAL(finished()),
                    this, SLOT(animationFinished()));
        }
        else
        {
            if (fg_anim)
            {
                fg_anim->disconnect(this);
                fg_anim->stop();
            }
                
            fg_anim = animation;
            
            connect(fg_anim.data(), SIGNAL(finished()), 
                    this, SLOT(animationFinished()));
                
            fg_anim->start();
        }
    }
    else
    {
        animation->stop();
    }
}    

/** This slot is called whenever a child animation stops. This is used
    to clean up the pointers to active animations */
void PageView::animationFinished()
{
    if (fg_anim)
    {
        if (not fg_anim->isRunning())
            fg_anim = 0;
    }
    
    QMutableListIterator<AnimationPointer> it(bg_anims);
    
    while (it.hasNext())
    {
        AnimationPointer &anim = it.next();
        
        if (anim)
        {
            if (not anim->isRunning())
                it.remove();
        }
        else
            it.remove();
    }
}

/** Call this function to enable and disable animations in pages
    viewed by this viewer */
void PageView::setAnimationsEnabled(bool on)
{
    if (on != anims_enabled)
    {
        anims_enabled = on;
        
        if (not anims_enabled)
        {
            if (fg_anim)
            {
                fg_anim->disconnect(this);
                fg_anim->stop();
                fg_anim = 0;
            }
            
            QMutableListIterator<AnimationPointer> it(bg_anims);
            
            while (it.hasNext())
            {
                AnimationPointer &anim = it.next();
                
                if (anim)
                {
                    anim->disconnect(this);
                    anim->stop();
                    anim = 0;
                }
            }
            
            bg_anims.clear();
            
            emit( animationsEnabledChanged(anims_enabled) );
        }
    }
}

/** Return whether or not animations are enabled for this page view */
bool PageView::animationsEnabled() const
{
    return anims_enabled;
}

/** Build this widget */
void PageView::build()
{
    anims_enabled = true;
    current_tab = -1;

    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(::Qt::Vertical, this);
    this->setLayout(l);
    
    tabbar = new QTabBar();
    QGraphicsProxyWidget *tabbar_proxy = new QGraphicsProxyWidget(this);
    tabbar_proxy->setWidget(tabbar);
    
    l->addItem(tabbar_proxy);

    label = new QLabel();
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    QGraphicsProxyWidget *label_proxy = new QGraphicsProxyWidget(this);
    label_proxy->setWidget(label);
    
    l->addItem(label_proxy);
}

/** Internal function used to make the passed view visible */
void PageView::pushView(PagePointer new_page, bool clear_future)
{
    if ((not new_page) or new_page->isBroken())
        return;

    if (current_tab == -1)
    {
        //need to create a new tab for this page
        this->push(new_page, true);
        return;
    }

    Tab *tab = tabpages[current_tab];

    if (clear_future)
        tab->page_future.clear();

    if (tab->current_page == new_page)
    {
        //there is no change
        return;
    }
    else if (tab->current_page)
    {
        //animate the transition from the current view to the new view
        animateSwitch(tab->current_page, new_page, true);
    }
    else
    {
        //there is no current view, so animate a zoom into the new view
        animateNew(new_page);
    }

    tab->page_history.push(tab->current_page);
    tab->current_page = new_page;

    emit( canBackChanged( not tab->page_history.isEmpty() ) );
    emit( canForwardChanged( not tab->page_future.isEmpty() ) );
}

/** Internal function used to pop the current view off the current tab */
PagePointer PageView::popView(bool forget_page)
{
    if (current_tab == -1)
        return PagePointer();
        
    Tab *tab = tabpages[current_tab];

    PagePointer old_page = tab->current_page;
    PagePointer new_page;

    //get the last, non-broken view to display
    while (not tab->page_history.isEmpty())
    {
        new_page = tab->page_history.pop();
        
        if (new_page and not new_page->isBroken())
            break;
    }
    
    if (not new_page)
        new_page = tab->top_page;

    if (not new_page)
    {
        //there is nothing to display, so delete the current tab
        closeTab(current_tab);
        return old_page;
    }
    else if (old_page == new_page)
    {
        //there is no change, so no page can be popped
        return PagePointer();
    }
    else if (old_page)
    {
        //animate the transition from the current view to the new view
        animateSwitch(old_page, new_page, false);
    }
    else
    {
        //there is no current view, so animate a zoom into the new view
        animateNew(new_page);
    }

    if (not forget_page)
        tab->page_future.push(old_page);
        
    tab->current_page = new_page;

    emit( canBackChanged( not tab->page_history.isEmpty() ) );
    emit( canForwardChanged( not tab->page_future.isEmpty() ) );
    
    return old_page;
}

/** Internal function used to animate the destruction of an old page */
void PageView::animateDestroy(PagePointer old_page)
{
    if (not old_page)
        return;
        
    old_page->show();
    old_page->setOpacity(1.0);

    QParallelAnimationGroup *g = new QParallelAnimationGroup();
    
    QPropertyAnimation *anim = new QPropertyAnimation(old_page.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    g->addAnimation(anim);
    
    anim = new QPropertyAnimation(old_page.data(), "x");
    anim->setDuration(1000);
    anim->setStartValue(-2 * label->width());
    anim->setStartValue(0.0);
    anim->setEasingCurve(QEasingCurve::InOutBack);
    g->addAnimation(anim);

    this->animate( AnimationPointer(new Animation(g,old_page)) );
}

/** Internal function used to animate the appearance of a new page */
void PageView::animateNew(PagePointer new_page)
{
    if (not new_page or (new_page->isBroken()))
    {
        return;
    }
    
    conspireDebug() << "animateNew";
    new_page->setOpacity(0);
    new_page->show();
    new_page->resize(label->size());
    
    QParallelAnimationGroup *g = new QParallelAnimationGroup();
    
    QPropertyAnimation *anim = new QPropertyAnimation(new_page.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    g->addAnimation(anim);
    
    anim = new QPropertyAnimation(new_page.data(), "x");
    anim->setDuration(1000);
    anim->setStartValue(2 * label->width());
    anim->setEndValue(0.0);
    anim->setEasingCurve(QEasingCurve::InOutBack);
    g->addAnimation(anim);

    this->animate( AnimationPointer(new Animation(g,new_page)) );
}

/** Animate the switch from "old_page" to "new_page" */
void PageView::animateSwitch(PagePointer old_page, PagePointer new_page, 
                             bool move_forwards)
{
    if (not new_page or new_page->isBroken())
    {
        animateDestroy(old_page);
        return;
    }
    else if (not old_page or old_page->isBroken())
    {
        animateNew(new_page);
        return;
    }

    //make sure that the two views have the correct size
    new_page->setOpacity(0);
    new_page->show();
    
    //make sure that the new page has the size of the background label
    new_page->resize(label->size());
    
    //also ensure that the old page has the size of the background label
    old_page->resize(label->size());
    old_page->show();

    QParallelAnimationGroup *g = new QParallelAnimationGroup();

    QPropertyAnimation *anim = new QPropertyAnimation(old_page.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    g->addAnimation(anim);

    anim = new QPropertyAnimation(new_page.data(), "opacity");
    anim->setDuration(1000);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    g->addAnimation(anim);

    if (move_forwards)
    {
        anim = new QPropertyAnimation(old_page.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(0.0);
        anim->setEndValue(-2 * label->width());
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_page.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(2 * label->width());
        anim->setEndValue(0.0);
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
    }
    else
    {
        anim = new QPropertyAnimation(old_page.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(0.0);
        anim->setEndValue(2 * label->width());
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_page.data(), "x");
        anim->setDuration(1000);
        anim->setStartValue(-2 * label->width());
        anim->setEndValue(0.0);
        anim->setEasingCurve(QEasingCurve::InOutBack);
        g->addAnimation(anim);
    }

    this->animate( AnimationPointer(new Animation(g,old_page,new_page)) );
}
