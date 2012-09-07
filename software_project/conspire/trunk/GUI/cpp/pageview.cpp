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

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QStaticText>
#include <QFont>

#include <QGraphicsProxyWidget>

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

using namespace Conspire;

/** Constructor */
PageView::PageView(Page *parent) : Page(parent), side_to_side(true)
{
    build();
}

/** Constructor, specifying whether or not we are in side_to_side mode */
PageView::PageView(bool side, Page *parent) : Page(parent), side_to_side(side)
{
    build();
}

/** Destructor */
PageView::~PageView()
{
    delete title_text;
}

/** Go back to the last page in the history in the current tab */
void PageView::back()
{
    this->popView(false);

    emit( canBackChanged(not page_history.isEmpty()) );
    emit( canForwardChanged(not page_future.isEmpty()) );
}

/** Go forward to the next page in the forward history in the current tab */
void PageView::forward()
{
    if (page_future.isEmpty())
        return;
        
    PagePointer page = page_future.pop();
    
    if (not page or page->isBroken())
    {
        //this widget has died - try to get another one
        this->forward();
        return;
    }
    
    this->pushView(page, false);
    
    emit( canBackChanged(not page_history.isEmpty()) );
    emit( canForwardChanged(not page_future.isEmpty()) );
}

/** Return home in the current tab */
void PageView::home(bool clear_history)
{
    if (clear_history)
    {
        page_history.clear();
        page_future.clear();
        popped(true);
    }
    else
    {
        if (current_page != top_page)
        {
            this->pushed(top_page);
        }
    }
}

/** Push the passed page into the view. This displays the new
    page and pushes the previously viewed page into the view history. */
void PageView::pushed(PagePointer page)
{
    if (not page or page->isBroken())
        return;

    //make this page a child of this page
    // (need twice - one as a QGraphicsItem child, one as a Page child)
    page->setParentItem(this);
    page->setParentPage(this);

    //the new page is clipped to the parent's shape, and is also drawn
    //behind the parent - this allows us to draw decorations over the child page
    page->setFlag(QGraphicsItem::ItemIsMovable, false);
    page->setFlag(QGraphicsItem::ItemClipsToShape, true);
    page->setFlag(QGraphicsItem::ItemStacksBehindParent, true);

    if (top_page.isNull())
    {
        top_page = page;
        current_page = page;
        top_page->setGeometry(0, 0, this->geometry().width(), this->geometry().height());
        top_page->show();
        setTitleText(current_page->title());
    }
    else
    {
        this->pushView(page);
    }
    
    connect(page.data(), SIGNAL(push(PagePointer)), 
            this, SLOT(pushed(PagePointer)));
    connect(page.data(), SIGNAL(pop(bool)), this, SLOT(popped(bool)));

    emit( canBackChanged(not page_history.isEmpty()) );
    emit( canForwardChanged(not page_future.isEmpty()) );
}

/** This function checks to see if the Page is no longer part of this view.
    If it is not part of the view, then it disconnects all signals between
    the page and the view, and makes sure that this view is no longer
    intercepting events */
void PageView::checkDisconnectPage(PagePointer &page)
{
    if (page)
    {
        if (top_page.data() == page.data() or
            current_page.data() == page.data())
            return;
        
        else
        {
            for (QStack<PagePointer>::const_iterator 
                            it = page_history.constBegin();
                 it != page_history.constEnd();
                 ++it)
            {
                if (it->data() == page.data())
                    return;
            }

            for (QStack<PagePointer>::const_iterator 
                            it = page_future.constBegin();
                 it != page_future.constEnd();
                 ++it)
            {
                if (it->data() == page.data())
                    return;
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

/** Pop the current view and return to the previous view in the current tab.
    This will forget the popped view if "forget_page" is true, which is useful
    if this page is a dialog type widget */
void PageView::popped(bool forget_page)
{
    PagePointer page = this->popView(forget_page);

    checkDisconnectPage(page);
    
    emit( canBackChanged(not page_history.isEmpty()) );
    emit( canForwardChanged(not page_future.isEmpty()) );
}

/** Paint this page */
void PageView::paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget)
{
    if (show_title)
    {
        int h = this->geometry().height();
        int w = this->geometry().width();

        painter->setRenderHint( QPainter::Antialiasing, true );

        //now the title bar
        painter->setPen( QPen(::Qt::black) );
        painter->setBrush( QBrush(::Qt::black) );
        
        painter->setOpacity(0.5);
        painter->drawRect(0, 0, w, title_height);
        
        //now draw the title
        painter->setPen( QPen(::Qt::white) );

        if (title_text)
            painter->drawStaticText(title_pos, *title_text);
    }
    else
    {
        back_button->hide();
        forward_button->hide();
    }
}

/** This widget is totally transparent - it is the children that are opaque */
QPainterPath PageView::opaqueArea() const
{
    return QPainterPath();
}

QFont title_font("LucidaGrande", 10);

void fitIntoRectangle(QRectF rect, QStaticText &text)
{
    text.setTextWidth(rect.width());
    text.prepare(QTransform(), title_font);
    
    float w_factor = rect.width() / text.size().width();
    float h_factor = rect.height() / text.size().height();
    
    float factor = qMin(w_factor, h_factor);

    if (factor != 1)
    {
        QFont scaled_font = title_font;
        scaled_font.setPointSizeF(title_font.pointSizeF() * factor);
        text.prepare(QTransform(), scaled_font);
    }
}

void PageView::positionTitleBar()
{
    float w = this->geometry().width();

    float button_width = 50;

    back_button->setGeometry(20, 3,
                             button_width, title_height-6);
                             
    forward_button->setGeometry(w-20-button_width, 3,
                                button_width, title_height-6);

    float title_width = w - 40 - 2*button_width - 20;

    title_geometry = QRectF( 0.5*(w-title_width), 3,
                             title_width, title_height-6);

    if (title_text)
    {
        fitIntoRectangle(title_geometry, *title_text);
    
        title_pos = title_geometry.topLeft() 
            + 0.5 * QPointF( title_geometry.width() - title_text->size().width(),
                             title_geometry.height() - title_text->size().height() );

    }
}

void PageView::positionBottomBar()
{
    if (bottom_bar_buttons.isEmpty()) return;

    float w = this->geometry().width();
    float h = this->geometry().height();

    float button_width = w / bottom_bar_buttons.count();

    for (int i = 0; i < bottom_bar_buttons.count(); i++)
    {
       bottom_bar_buttons[i]->setGeometry(i*button_width, h - bottom_bar_height, button_width, bottom_bar_height);
    }
}


/** Return whether or not this view is empty */
bool PageView::isEmpty() const
{
    return top_page.isNull();
}

/** Build this widget */
void PageView::build()
{
    this->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);

    anims_enabled = true;
    show_title = true;
    
    setTitle("Unnamed PageView");
    setDescription("Description of an unnamed PageView");

    this->setAutoFillBackground(false);
    
    title_height = 25;
    bottom_bar_height = 25;

    view_geometry = QRectF( 0, title_height, 
                            this->geometry().width(), 
                            this->geometry().height() - title_height );

    back_button = new Button(this);
    back_button->setText("Back");
    
    forward_button = new Button(this);
    forward_button->setText("Next");
    
    connect(back_button, SIGNAL(clicked()), this, SLOT(back()));
    connect(forward_button, SIGNAL(clicked()), this, SLOT(forward()));
    
    connect(this, SIGNAL(canBackChanged(bool)), back_button, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(canForwardChanged(bool)),
            forward_button, SLOT(setEnabled(true)));

    title_text = 0;

    positionTitleBar();
}

void PageView::setTitleVisible(bool visible)
{
    show_title = visible;
}

/** Called when this widget is resized */
void PageView::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    Page::resizeEvent(e);

    int bottom_height = 0;
    
    if (not bottom_bar_buttons.isEmpty())
       bottom_height = bottom_bar_height;
    
    if (show_title)
    {
        view_geometry = QRectF( 0, title_height, 
                                this->geometry().width(),
                                this->geometry().height() - title_height - bottom_height );

        if (current_page)
        {
            current_page->setGeometry(view_geometry);
        }
        
        positionTitleBar();
        positionBottomBar();
    }
    else
    {
        view_geometry = QRectF( 0, 0, 
                                this->geometry().width(),
                                this->geometry().height() - bottom_height );

        current_page->setGeometry(view_geometry);
        positionBottomBar();
    }
}

/** Called when this widget is moved */
void PageView::moveEvent(QGraphicsSceneMoveEvent *e)
{
    Page::moveEvent(e);

    if (show_title)
    {
        view_geometry = QRectF( 0, title_height, 
                                this->geometry().width(),
                                this->geometry().height() - title_height );

        if (current_page)
        {
            current_page->setGeometry(view_geometry);
        }
        
        positionTitleBar();
    }
    else
    {
        view_geometry = QRectF( 0, 0, 
                                this->geometry().width(),
                                this->geometry().height() );

        current_page->setGeometry(0, 0, this->geometry().width(),
                                        this->geometry().height());
    }
}

/** Internal function used to make the passed view visible */
void PageView::pushView(PagePointer new_page, bool clear_future)
{
    if ((not new_page) or new_page->isBroken())
        return;

    if (current_page.isNull())
    {
        this->pushed(new_page);
        return;
    }

    if (clear_future)
        page_future.clear();

    if (current_page == new_page)
    {
        //there is no change
        return;
    }
    else
    {
        //animate the transition from the current view to the new view
        animateSwitch(current_page, new_page, true);
    }

    page_history.push(current_page);
    current_page = new_page;

    emit( canBackChanged( not page_history.isEmpty() ) );
    emit( canForwardChanged( not page_future.isEmpty() ) );
}

/** Internal function used to pop the current view off the current tab */
PagePointer PageView::popView(bool forget_page)
{
    if (current_page.isNull())
        return PagePointer();
        
    PagePointer old_page = current_page;
    PagePointer new_page;

    //get the last, non-broken view to display
    while (not page_history.isEmpty())
    {
        new_page = page_history.pop();
        
        if (new_page and not new_page->isBroken())
            break;
    }
    
    if (not new_page)
        new_page = top_page;

    if (not new_page)
    {
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
        page_future.push(old_page);
        
    current_page = new_page;

    emit( canBackChanged( not page_history.isEmpty() ) );
    emit( canForwardChanged( not page_future.isEmpty() ) );
    
    return old_page;
}

void PageView::setTitleText(QString new_text)
{
    delete title_text;
    title_text = 0;
    
    if (not new_text.isEmpty())
    {
        title_text = new QStaticText(new_text);
        QTextOption opt;
        opt.setWrapMode(QTextOption::NoWrap);
        opt.setAlignment(::Qt::AlignCenter);
        title_text->setTextOption(opt);
        
        fitIntoRectangle(title_geometry, *title_text);
        
        title_pos = title_geometry.topLeft() 
                + 0.5 * QPointF( title_geometry.width() - title_text->size().width(),
                                 title_geometry.height() - title_text->size().height() );
    }
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
    
    setTitleText(QString::null);
}

/** Internal function used to animate the appearance of a new page */
void PageView::animateNew(PagePointer new_page)
{
    if (not new_page or (new_page->isBroken()))
    {
        return;
    }
    
    new_page->setOpacity(0.0);
    new_page->setGeometry(view_geometry);
    new_page->show();
    conspireDebug() << "Showing page" << new_page->title() << new_page->description()
                    << view_geometry << new_page->geometry();
    
    QParallelAnimationGroup *g = new QParallelAnimationGroup();
    
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

    this->animate( AnimationPointer(new Animation(g)) );
    
    setTitleText(new_page->title());
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
    new_page->setBlocked(false);
    old_page->setBlocked(true);
    
    //make sure that the geometry of the page is correct
    new_page->setGeometry(view_geometry);
    
    //make sure that the new page has the size of the background
    new_page->setTransformOriginPoint( 0.5 * new_page->geometry().width(),
                                       0.5 * new_page->geometry().height() );
    
    //also ensure that the old page has the size of the background
    old_page->setTransformOriginPoint( 0.5 * old_page->geometry().width(),
                                       0.5 * old_page->geometry().height() );

    QParallelAnimationGroup *g = new QParallelAnimationGroup();

    if (side_to_side)
    {
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
            anim->setDuration(300);
            anim->setStartValue(old_page->x());
            anim->setEndValue(old_page->x() - old_page->geometry().width());
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
            
            anim = new QPropertyAnimation(new_page.data(), "x");
            anim->setDuration(300);
            anim->setStartValue(new_page->x() + new_page->geometry().width());
            anim->setEndValue(new_page->x());
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
        }
        else
        {
            anim = new QPropertyAnimation(old_page.data(), "x");
            anim->setDuration(300);
            anim->setStartValue(old_page->x());
            anim->setEndValue(old_page->x() + old_page->geometry().width());
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
            
            anim = new QPropertyAnimation(new_page.data(), "x");
            anim->setDuration(300);
            anim->setStartValue(new_page->x() - new_page->geometry().width());
            anim->setEndValue(new_page->x());
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
        }

        this->animate( AnimationPointer(new Animation(g)) );
    }
    else
    {
        QPropertyAnimation *anim;
        
        if (move_forwards)
        {
            anim = new QPropertyAnimation(old_page.data(), "opacity");
            anim->setDuration(500);
            anim->setStartValue(1.0);
            anim->setEndValue(0.7);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(old_page.data(), "blur");
            anim->setDuration(500);
            anim->setStartValue(0.0);
            anim->setEndValue(100.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(new_page.data(), "opacity");
            anim->setDuration(500);
            anim->setStartValue(0.0);
            anim->setEndValue(0.8);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(new_page.data(), "blur");
            anim->setDuration(500);
            anim->setStartValue(100.0);
            anim->setEndValue(0.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(old_page.data(), "scale");
            anim->setDuration(500);
            anim->setStartValue(1.0);
            anim->setEndValue(0.5);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
            
            anim = new QPropertyAnimation(new_page.data(), "scale");
            anim->setDuration(500);
            anim->setStartValue(3.0);
            anim->setEndValue(1.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
        }
        else
        {
            anim = new QPropertyAnimation(old_page.data(), "opacity");
            anim->setDuration(500);
            anim->setStartValue(1.0);
            anim->setEndValue(0);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(old_page.data(), "blur");
            anim->setDuration(500);
            anim->setStartValue(0.0);
            anim->setEndValue(100.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(new_page.data(), "opacity");
            anim->setDuration(500);
            anim->setStartValue(0.0);
            anim->setEndValue(1.0);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(new_page.data(), "blur");
            anim->setDuration(500);
            anim->setStartValue(100.0);
            anim->setEndValue(0.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);

            anim = new QPropertyAnimation(old_page.data(), "scale");
            anim->setDuration(500);
            anim->setStartValue(1.0);
            anim->setEndValue(2.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
            
            anim = new QPropertyAnimation(new_page.data(), "scale");
            anim->setDuration(500);
            anim->setStartValue(0.3);
            anim->setEndValue(1.0);
            anim->setEasingCurve(QEasingCurve::OutCubic);
            g->addAnimation(anim);
        }

        this->animate( AnimationPointer(new Animation(g)) );
    }
    
    setTitleText(new_page->title());
}

void PageView::addToBottomBar(Button *tbutton)
{
   if (tbutton)
      bottom_bar_buttons.append(tbutton);
}
