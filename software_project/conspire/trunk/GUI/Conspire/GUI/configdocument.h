#ifndef CONSPIRE_CONFIGDOCUMENT_H
#define CONSPIRE_CONFIGDOCUMENT_H
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

#include <QImage>

CONSPIRE_BEGIN_HEADER

class QUndoStack;
class QGraphicsPixmapItem;

namespace Conspire
{
    class ConfigView;
    class PageView;
    class WidgetRack;
    class Button;

    /** This page holds a complete Options document. This is the top-level
        page used to edit an Options object, and allows multiple, tabbed
        sub-views of the document, and manages all of the editing of
        the document */
    class CONSPIRE_EXPORT ConfigDocument : public Page
    {
        Q_OBJECT
        
    public:
        ConfigDocument(QString ibrowsedir, QString itype, Page *parent=0);
        ConfigDocument(QString ibrowsedir, QString itype, QString iquuid, Page *parent=0);
        ConfigDocument(QString ibrowsedir, QString itype, Options options, Page *parent=0);
        ConfigDocument(QString ibrowsedir, QString itype, Options options, QString iquuid, Page *parent=0);
        
        ~ConfigDocument();
        
        Options options() const;
        
    public slots:
        void add(QString full_key);
        void remove(QString full_key);
        void update(QString full_key, Obj new_value);
        
        void undo();
        void redo();
        
        void forward();
        void back();
        void home(bool clear_history=false);

        void setOptions(Options options);
        
        void toggleMenuVisible();
        void setMenuVisible(bool display);
        
        void load();
        void save();
        void rsave();
        void submit();
        void cancel();
        
        
    protected:
        void paint(QPainter *painter, 
                   const QStyleOptionGraphicsItem *option, 
                   QWidget *widget);
        
    signals:
        void cancellation();
        void reread(Options options);
        
        void canUndoChanged(bool);
        void canRedoChanged(bool);
        
        void canBackChanged(bool);
        void canForwardChanged(bool);
        void saveDocument(Options opts);
        
    protected slots:
        void pop2();
        void viewPoppedPastBeginning();

    protected:
        void buttonsmodegeom();
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);
        
    private:
        void build();

        int buttonsmode;
        /** The Options object being viewed and edited */
        Options opts;
        QString jobtype;
        QString quuid;
        QString browsedir;

        /** The context-sensitive button */
        Button *submit_button;
        Button *load_button;
        Button *save_button;
        Button *rsave_button;
        
        /** The tabbed views of the Options object */
        ConfigView *view;
        
        /** The top-level PageView */
        PageView *top_view;
        
        /** The undo stack used to provide an undo history to editing commands */
        QUndoStack *undo_stack;
        
        /** The image of the widget used to improve rendering
            speed when the document is not being edited */
        QGraphicsPixmapItem *disabled_view;
        
        /** Whether or not the menu is visible */
        bool menu_visible;
    };

}

CONSPIRE_END_HEADER

#endif
