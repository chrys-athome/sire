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

#include "Conspire/GUI/widgetstack.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsProxyWidget>

using namespace Conspire;

/** Constructor */
WidgetStack::WidgetStack(QGraphicsItem *parent) : QGraphicsWidget(parent)
{}

/** Destructor */
WidgetStack::~WidgetStack()
{}

/** Add the passed widget to this page */
void WidgetStack::addWidget(QGraphicsWidget *page)
{
    if (page)
    {
        page->setParentItem(this);
        page->setGeometry(0, 0, this->geometry().width(),
                                this->geometry().height());
        page->hide();
        pgs.append(page);
        
        if (not current_page)
            this->switchTo(0);
    }
}

void WidgetStack::addWidget(QWidget *page)
{
    if (page)
    {
        QGraphicsProxyWidget *page_proxy = new QGraphicsProxyWidget(this);
        page_proxy->setWidget(page);
        this->addWidget(page_proxy);
    }
}

/** Switch to the page at index 'idx' */
void WidgetStack::switchTo(int idx)
{
    if (idx < 0 or idx >= pgs.count())
        return;
        
    QPointer<QGraphicsWidget> new_page = pgs[idx];
    
    if (not new_page)
        return;
        
    if (new_page == current_page)
        return;

    QPointer<QGraphicsWidget> old_page = current_page;
    current_page = new_page;
        
    new_page->setGeometry(0, 0, this->geometry().width(),
                                this->geometry().height());

    new_page->setOpacity(0.0);
    new_page->show();
    
    QParallelAnimationGroup *g = new QParallelAnimationGroup();

    QPropertyAnimation *anim = new QPropertyAnimation(old_page.data(), "opacity");
    anim->setDuration(150);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    g->addAnimation(anim);

    anim = new QPropertyAnimation(new_page.data(), "opacity");
    anim->setDuration(150);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    g->addAnimation(anim);
    
    g->start(QAbstractAnimation::DeleteWhenStopped);
}

/** Resize event */
void WidgetStack::resizeEvent(QGraphicsSceneResizeEvent *e)
{
    QGraphicsWidget::resizeEvent(e);
    
    if (current_page)
        current_page->setGeometry(0, 0, this->geometry().width(),
                                        this->geometry().height());
}

/** Resize event */
void WidgetStack::moveEvent(QGraphicsSceneMoveEvent *e)
{
    QGraphicsWidget::moveEvent(e);
    
    if (current_page)
        current_page->setGeometry(0, 0, this->geometry().width(),
                                        this->geometry().height());
}

