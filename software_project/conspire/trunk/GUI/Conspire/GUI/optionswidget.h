#ifndef CONSPIRE_OPTIONSWIDGET_H
#define CONSPIRE_OPTIONSWIDGET_H
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
#include "Conspire/option.h"
#include "Conspire/exceptions.h"

#include "Conspire/GUI/pagewidget.h"

#include <QGraphicsView>
#include <QStack>
#include <QPointer>
#include <QSignalMapper>

CONSPIRE_BEGIN_HEADER

class QGraphicsView;
class QGraphicsWidget;
class QGraphicsScene;
class QGraphicsGridLayout;
class QUndoStack;
class QLabel;
class QAbstractButton;

namespace Conspire
{
    class OptionsEditView;
    class MainBar;

    /** This is a page that is used to view and edit a single page
        of options */
    class CONSPIRE_EXPORT NewOptionsWidget : public PageWidget
    {
        Q_OBJECT
    
    public:
        NewOptionsWidget(QGraphicsItem *parent=0);
        NewOptionsWidget(Options options, QGraphicsItem *parent=0);
        NewOptionsWidget(Options options, QString root_key,
                         QGraphicsItem *parent=0);
        
        ~NewOptionsWidget();
        
        Options options() const;
        
        QString rootKey() const;
    
    public slots:
        void update(Options options);

    private slots:
        void clicked(const QString &key);

    private:
        void build();
        void setOptions(Options options, QString root_key = QString::null);
        
        /** The options that can be edited by this page */
        Options opts;
        
        /** The root key of this options object */
        QString root_key;
        
        /** The list of active buttons */
        QList<QAbstractButton*> buttons;
        
        /** The signal mapper */
        QSignalMapper *mapper;
    };

    /** This is a widget that can view and edit the options
        held in a Conspire::Options object */
    class CONSPIRE_EXPORT OptionsWidget : public QGraphicsView
    {
        Q_OBJECT
    
    public:
        OptionsWidget(QWidget *parent=0);
        OptionsWidget(Options options, QWidget *parent=0);
        
        ~OptionsWidget();
        
        Options options() const;
        
        void setOptions(Options options);
    
    public slots:
        void undo();
        void redo();
        
        void back();
        void forward();

        void home(bool clear_history=false);
        
        void push(PagePointer page);
        void pop(bool forget_page=false);

        void add(QString full_key);
        void remove(QString full_key);
        void update(QString full_key, Obj new_value);

    signals:
        void canBackChanged(bool);
        void canForwardChanged(bool);
        void canUndoChanged(bool);
        void canRedoChanged(bool);
    
    protected:
        void resizeEvent(QResizeEvent *event);    
        void keyPressEvent(QKeyEvent *event);

    private:
        void build();
    
        void pushView(PagePointer view, bool clear_future=true);
        void popView(bool forget_page=false);

        void animateSwitch(PagePointer old_view, PagePointer new_view, 
                           bool move_forwards);
                           
        void animateNew(PagePointer new_view);
        void animateDestroy(PagePointer old_view);

        /** The options being edited */
        Options opts;
        
        /** The top-level options edit/view */
        PagePointer top_view;

        /** The current viewed widget */
        PagePointer current_view;
        
        /** The stack of widget views */
        QStack<PagePointer> view_history;
        
        /** The stack of future widget views */
        QStack<PagePointer> view_future;
        
        /** The main bar that acts as a global control */
        QPointer<MainBar> mainbar;
        
        /** The undo stack of commands applied to the options */
        QUndoStack *undo_stack;
    };

}

CONSPIRE_END_HEADER

#endif