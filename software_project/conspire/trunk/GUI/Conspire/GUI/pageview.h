#ifndef CONSPIRE_PAGEVIEW_H
#define CONSPIRE_PAGEVIEW_H
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

#include "Conspire/GUI/page.h"
#include "Conspire/GUI/animation.h"

CONSPIRE_BEGIN_HEADER

#include <QStack>
#include <QList>
#include <QRectF>
#include <QPointF>

class QTabBar;
class QLabel;
class QStaticText;
class QParallelAnimationGroup;

namespace Conspire
{
    class Button;

    /** This widget holds and manages a set of PageWidget pages.
        The widget keeps a page history, allowing the user to move
        back and forward through the pages, and it also provides the
        option of viewing the pages in multiple tabs.
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT PageView : public Page
    {
        Q_OBJECT
    
    public:
        PageView(Page *parent=0);
        
        ~PageView();
        
        int count() const;
      
        QPainterPath opaqueArea() const;
            
    public slots:
        virtual void back();
        virtual void forward();
        
        virtual void home(bool clear_history=false);
        
        virtual void pushed(PagePointer page, bool new_tab=false);
        virtual void popped(bool forget_page=false);
        
        virtual void moveTab(int from, int to);
        virtual void changeTab(int index);
        
        virtual void closeTab(int index);

        virtual void closeAll();
        
    signals:
        void canBackChanged(bool);
        void canForwardChanged(bool);
        
    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);

        void keyPressEvent(QKeyEvent *e);
        void mousePressEvent(QMouseEvent *e);

        void paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem *option, 
                   QWidget *widget);

    private:
        void positionTitleBar();
    
        class Tab
        {
        public:
            Tab();
            ~Tab();
            
            /** The top (home) page of this tab */
            PagePointer top_page;
            
            /** The page currently viewed */
            PagePointer current_page;
            
            /** The history of previous pages */
            QStack<PagePointer> page_history;
            
            /** The set of forward pages (if we have gone "back") */
            QStack<PagePointer> page_future;
        };

        void build();
        void checkDisconnectPage(PagePointer &page);
        void checkDisconnectPages(QStack<PagePointer> &pages);
        void checkDisconnectTab(Tab *tab);
        
        void pushView(PagePointer view, bool clear_future=true);
        PagePointer popView(bool forget_page=false);

        void animateTitleText(QString new_text, QParallelAnimationGroup *g,
                              bool move_forwards=true);

        void animateSwitch(PagePointer old_view, PagePointer new_view, 
                           bool move_forwards=true);
                           
        void animateNew(PagePointer new_view);
        void animateDestroy(PagePointer old_view);
                
        /** The pointer to the bar used to change the tabs */
        QTabBar *tabbar;
        
        /** The set of all visible tabs */
        QList<Tab*> tabpages;

        /** The correct size and location of the page being viewed */
        QRectF view_geometry;
        
        /** The old text in the page title header (used during animation) */
        QStaticText *old_title_text;
        
        /** The text in the page title header */
        QStaticText *title_text;
        
        /** The rectangle for the title text */
        QRectF title_geometry;
        
        /** The location for the title text */
        QPointF title_pos;
        
        /** The location for the old title text */
        QPointF old_title_pos;
        
        /** The "back" button in the page title header */
        Button *back_button;
        
        /** The "forward" button in the page title header */
        Button *forward_button;
        
        /** The height of the title bar */
        int title_height;
        
        /** The index of the current tab page */
        int current_tab;

    };

}

CONSPIRE_END_HEADER

#endif
