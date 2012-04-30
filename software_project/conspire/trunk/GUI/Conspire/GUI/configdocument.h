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

CONSPIRE_BEGIN_HEADER

class QUndoStack;

namespace Conspire
{
    class ConfigView;

    /** This page holds a complete Options document. This is the top-level
        page used to edit an Options object, and allows multiple, tabbed
        sub-views of the document, and manages all of the editing of
        the document */
    class CONSPIRE_EXPORT ConfigDocument : public Page
    {
        Q_OBJECT
        
    public:
        ConfigDocument(Page *parent=0);
        ConfigDocument(Options options, Page *parent=0);
        
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
        
    signals:
        void reread(Options options);
        
        void canUndoChanged(bool);
        void canRedoChanged(bool);
        
        void canBackChanged(bool);
        void canForwardChanged(bool);
        
    protected:
        void resizeEvent(QGraphicsSceneResizeEvent *e);
        void moveEvent(QGraphicsSceneMoveEvent *e);
        
    private:
        void build();

        /** The Options object being viewed and edited */
        Options opts;

        /** The tabbed views of the Options object */
        ConfigView *view;
        
        /** The undo stack used to provide an undo history to editing commands */
        QUndoStack *undo_stack;
    };

}

CONSPIRE_END_HEADER

#endif
