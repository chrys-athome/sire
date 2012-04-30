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
#include "Conspire/GUI/button.h"
#include "Conspire/GUI/widgetrack.h"

#include <QTabBar>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QBrush>
#include <QPen>

#include <QGraphicsProxyWidget>

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
PageView::PageView(Page *parent) : Page(parent)
{
    build();
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
        popped(true);
    }
    else
    {
        if (tab->current_page != tab->top_page)
        {
            pushed(tab->top_page);
        }
    }
}

/** Push the passed page into the current tab. This displays the new
    page and pushes the previously viewed page into the view history. Note
    that if "new_tab" is true, then this creates a new tab using the passed
    page, which becomes its own top-level page in that tab */
void PageView::pushed(PagePointer page, bool new_tab)
{
    if (not page or page->isBroken())
        return;

    //make this page a child of this page
    // (need twice - one as a QGraphicsItem child, one as a Page child)
    page->setParentItem(this);
    page->setParentPage(this);

    //the new page is clipped to the parent's shape, and is also drawn
    //behind the parent - this allows us to draw decorations over the child page
    //page->setFlag(QGraphicsItem::ItemIsMovable, false);
    //page->setFlag(QGraphicsItem::ItemClipsToShape, true);
    //page->setFlag(QGraphicsItem::ItemStacksBehindParent, true);

    Tab *tab = 0;

    if ((current_tab == -1) or new_tab)
    {
        tab = new Tab();
        tab->top_page = page;
        tab->current_page = page;
        
        tabpages.append(tab);
        int idx = tabbar->addTab(page->description());
        tabbar->setTabEnabled(idx,true);
        
        if (idx != tabpages.count() - 1)
            conspireDebug() << "WEIRD TABBAR INDEX!!!";
            
        if (tabpages.count() < 2)
            tabbar->hide();
        else
            tabbar->show();
          
        this->changeTab(idx);
    }
    else
    {
        tab = tabpages[current_tab];
        this->pushView(page, true);
    }
    
    connect(page.data(), SIGNAL(push(PagePointer,bool)), 
            this, SLOT(pushed(PagePointer,bool)));
    connect(page.data(), SIGNAL(pop(bool)), this, SLOT(popped(bool)));

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
        //make sure that this page is not held by one of the tabs
        foreach (const Tab *tab, tabpages)
        {
            if (tab->top_page.data() == page.data() or
                tab->current_page.data() == page.data())
                return;
            
            else
            {
                for (QStack<PagePointer>::const_iterator 
                                it = tab->page_history.constBegin();
                     it != tab->page_history.constEnd();
                     ++it)
                {
                    if (it->data() == page.data())
                        return;
                }

                for (QStack<PagePointer>::const_iterator 
                                it = tab->page_future.constBegin();
                     it != tab->page_future.constEnd();
                     ++it)
                {
                    if (it->data() == page.data())
                        return;
                }
            }
        }

        //the page is not connected to this view
        if (page->parentItem() == this)
            page->setParentItem(0);
        
        if (page->weakParentPage().data() == this)
            page->setParentPage(0);
            
        page->disconnect(this);
        this->disconnect(page.data());
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
void PageView::popped(bool forget_page)
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
    
    if (current_tab == -1)
    {
        Tab *new_tab = tabpages[index];
        current_tab = index;
        animateNew(new_tab->current_page);
    }
    else
    {
        Tab *old_tab = tabpages[current_tab];
        Tab *new_tab = tabpages[index];

        bool move_forwards = (index > current_tab);

        current_tab = index;
        animateSwitch(old_tab->current_page, new_tab->current_page, move_forwards);
    }

    if (tabbar->currentIndex() != current_tab)
    {
        tabbar->setCurrentIndex(current_tab);
    }

    if (current_tab != -1)
    {
        Tab *tab = tabpages[current_tab];
        emit( canBackChanged( not tab->page_history.isEmpty() ) );
        emit( canForwardChanged( not tab->page_future.isEmpty() ) );
    }
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

/** Paint this page */
void PageView::paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget)
{
    conspireDebug() << "PageView::paint(...)";
    //the children will all have been painted before this view. We can
    //now draw decorations and the border

    int h = this->geometry().height();
    int w = this->geometry().width();

    //draw the border
    //  first the four sides
    painter->setPen( QPen( ::Qt::black ) );
    painter->setBrush( QBrush( ::Qt::black ) );
    
    painter->drawRect(0, 0, border_size, h);
    painter->drawRect(w-border_size, 0, border_size, h);
    
    painter->drawRect(0, 0, w, border_size);
    painter->drawRect(0, h-border_size, w, border_size);
    
    //  now the four curved corners
    QPainterPath path;
    path.moveTo( 0, 0 );
    path.arcTo( 0, 0, 2*border_size, 2*border_size, 90, 90 );
    
    painter->translate(border_size, border_size);
    painter->drawPath(path);
    painter->resetTransform();
    
    painter->translate(border_size, h - border_size);
    painter->rotate(-90);
    painter->drawPath(path);
    painter->resetTransform();
    
    painter->translate(w-border_size, border_size);
    painter->rotate(90);
    painter->drawPath(path);
    painter->resetTransform();
    
    painter->translate(w-border_size, h-border_size);
    painter->rotate(180);
    painter->drawPath(path);
    painter->resetTransform();
}

/** Build this widget */
void PageView::build()
{
    anims_enabled = true;
    current_tab = -1;

    tabbar = new QTabBar();
    
    border_size = 15;
    title_height = 25;

    view_geometry = QRectF( border_size, border_size + title_height, 
                            this->geometry().width() - 2*border_size, 
                            this->geometry().height() - 2*border_size - title_height );
}

/** Called when this widget is resized */
void PageView::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);
    
    view_geometry = QRectF( border_size, border_size + title_height, 
                            this->geometry().width() - 2*border_size, 
                            this->geometry().height() - 2*border_size - title_height );

    if (current_tab != -1)
    {
        if (tabpages[current_tab]->current_page)
        {
            tabpages[current_tab]->current_page->setGeometry(view_geometry);
            
            conspireDebug() << "RESIZE" << view_geometry
                            << tabpages[current_tab]->current_page->geometry();
        }
    }
}

/** Called when this widget is moved */
void PageView::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);

    view_geometry = QRectF( border_size, border_size + title_height, 
                            this->geometry().width() - 2*border_size, 
                            this->geometry().height() - 2*border_size - title_height );

    if (current_tab != -1)
    {
        if (tabpages[current_tab]->current_page)
        {
            tabpages[current_tab]->current_page->setGeometry(view_geometry);

            conspireDebug() << "RESIZE" << view_geometry
                            << tabpages[current_tab]->current_page->geometry();
        }
    }
}

/** Internal function used to make the passed view visible */
void PageView::pushView(PagePointer new_page, bool clear_future)
{
    if ((not new_page) or new_page->isBroken())
        return;

    if (current_tab == -1)
    {
        //need to create a new tab for this page
        this->pushed(new_page, true);
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
        
    old_page->setOpacity(1.0);

    QParallelAnimationGroup *g = new QParallelAnimationGroup();
    
    QPropertyAnimation *anim = new QPropertyAnimation(old_page.data(), "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    g->addAnimation(anim);
    
    anim = new QPropertyAnimation(old_page.data(), "x");
    anim->setDuration(500);
    anim->setStartValue(old_page->x());
    anim->setEndValue(old_page->x() - 2 * old_page->geometry().width());
    anim->setEasingCurve(QEasingCurve::OutBack);
    g->addAnimation(anim);

    this->animate( AnimationPointer(new Animation(g)) );
}

/** Internal function used to animate the appearance of a new page */
void PageView::animateNew(PagePointer new_page)
{
    if (not new_page or (new_page->isBroken()))
    {
        return;
    }
    
    new_page->setOpacity(1.0);
    new_page->setGeometry(view_geometry);
    new_page->show();
    conspireDebug() << "Showing page" << new_page->title() << new_page->description()
                    << view_geometry << new_page->geometry();
    
    /*QParallelAnimationGroup *g = new QParallelAnimationGroup();
    
    QPropertyAnimation *anim = new QPropertyAnimation(new_page.data(), "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.5);
    anim->setEndValue(1.0);
    g->addAnimation(anim);
    
    anim = new QPropertyAnimation(new_page.data(), "x");
    anim->setDuration(500);
    anim->setStartValue(new_page->x() + 2 * new_page->geometry().width());
    anim->setEndValue(new_page->x());
    anim->setEasingCurve(QEasingCurve::OutBack);
    g->addAnimation(anim);

    this->animate( AnimationPointer(new Animation(g)) );*/
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
    
    //make sure that the geometry of the page is correct
    new_page->setGeometry(view_geometry);
    
    //make sure that the new page has the size of the background
    new_page->setTransformOriginPoint( 0.5 * new_page->geometry().width(),
                                       0.5 * new_page->geometry().height() );
    
    //also ensure that the old page has the size of the background
    old_page->setTransformOriginPoint( 0.5 * old_page->geometry().width(),
                                       0.5 * old_page->geometry().height() );

    QParallelAnimationGroup *g = new QParallelAnimationGroup();

    QPropertyAnimation *anim = new QPropertyAnimation(old_page.data(), "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    g->addAnimation(anim);

    anim = new QPropertyAnimation(new_page.data(), "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    g->addAnimation(anim);

    if (move_forwards)
    {
        anim = new QPropertyAnimation(old_page.data(), "x");
        anim->setDuration(500);
        anim->setStartValue(old_page->x());
        anim->setEndValue(old_page->x() - 2 * old_page->geometry().width());
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_page.data(), "x");
        anim->setDuration(500);
        anim->setStartValue(new_page->x() + 2 * new_page->geometry().width());
        anim->setEndValue(new_page->x());
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_page.data(), "rotation");
        anim->setDuration(500);
        anim->setStartValue(45);
        anim->setEndValue(0);
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(old_page.data(), "rotation");
        anim->setDuration(500);
        anim->setStartValue(0);
        anim->setEndValue(-45);
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
    }
    else
    {
        anim = new QPropertyAnimation(old_page.data(), "x");
        anim->setDuration(500);
        anim->setStartValue(old_page->x());
        anim->setEndValue(old_page->x() + 2 * old_page->geometry().width());
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_page.data(), "x");
        anim->setDuration(500);
        anim->setStartValue(new_page->x() - 2 * new_page->geometry().width());
        anim->setEndValue(new_page->x());
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(new_page.data(), "rotation");
        anim->setDuration(500);
        anim->setStartValue(-45);
        anim->setEndValue(0);
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
        
        anim = new QPropertyAnimation(old_page.data(), "rotation");
        anim->setDuration(500);
        anim->setStartValue(0);
        anim->setEndValue(45);
        anim->setEasingCurve(QEasingCurve::OutBack);
        g->addAnimation(anim);
    }

    this->animate( AnimationPointer(new Animation(g)) );
}
