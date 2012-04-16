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
PageView::PageView(QGraphicsItem *parent) : PageWidget(parent)
{
    build();
}

/** Constructor, passing in the top page for the first tab */
PageView::PageView(PagePointer top_page, QGraphicsItem *parent) : PageWidget(parent)
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

/** Go back to the last page in the history in the current tab */
void PageView::back()
{
    if (current_tab == -1)
        return;

    this->popView(false);
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

    conspireDebug() << "ADDED PAGE" << page.data() << "TO TAB" << tab;
}

/** Pop the current view and return to the previous view in the current tab.
    This will forget the popped view if "forget_page" is true, which is useful
    if this page is a dialog type widget */
void PageView::pop(bool forget_page)
{
    this->popView(forget_page);
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

/** Called when this widget is resized */
void PageView::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    PageWidget::resizeEvent(e);
    
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
            PageWidget::keyPressEvent(e);
        }
    }
}

/** Build this widget */
void PageView::build()
{
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
void PageView::popView(bool forget_page)
{
    if (current_tab == -1)
        return;
        
    Tab *tab = tabpages[current_tab];

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
        return;
    }
    else if (tab->current_page == new_page)
    {
        //there is no change
        return;
    }
    else if (tab->current_page)
    {
        //animate the transition from the current view to the new view
        animateSwitch(tab->current_page, new_page, false);
    }
    else
    {
        //there is no current view, so animate a zoom into the new view
        animateNew(new_page);
    }

    if (not forget_page)
        tab->page_future.push(tab->current_page);
        
    tab->current_page = new_page;

    emit( canBackChanged( not tab->page_history.isEmpty() ) );
    emit( canForwardChanged( not tab->page_future.isEmpty() ) );
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

    g->start(QAbstractAnimation::DeleteWhenStopped);
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

    g->start(QAbstractAnimation::DeleteWhenStopped);
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

    g->start(QAbstractAnimation::DeleteWhenStopped);
}
