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

#include <QGraphicsView>
#include <QStack>
#include <QPointer>

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
        
        void add();
        void help(Option option);

        void caughtException(const Conspire::Exception &e);

    signals:
        void canBackChanged(bool);
        void canForwardChanged(bool);
        void canAddChanged(bool);
    
    protected:
        void resizeEvent(QResizeEvent *event);    
        void keyPressEvent(QKeyEvent *event);

    protected slots:
        void add(QString key);
        void remove(QString key);
        void update(QString key, Obj object);
        void viewDeleted(QObject *obj);
        
    private:
        void build();
        void updateStates();
    
        void pushView(QGraphicsWidget *view, bool clear_future=true);
        QGraphicsWidget* popView();
        
        void popAllViews();

        /** The options being edited */
        Options opts;

        /** The current viewed widget */
        QPointer<QGraphicsWidget> current_view;
        
        /** The stack of widget views */
        QStack< QPointer<QGraphicsWidget> > view_history;
        
        /** The stack of future widget views */
        QStack< QPointer<QGraphicsWidget> > view_future;
        
        /** The top-level options edit/view */
        QPointer<OptionsEditView> top_view;
        
        /** The menu bar that sits at the bottom of the screen */
        QPointer<MainBar> mainbar;
        
        /** The undo stack of commands applied to the options */
        QUndoStack *undo_stack;
    };

}

CONSPIRE_END_HEADER

#endif