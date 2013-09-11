#ifndef CONSPIRE_MAINBAR_H
#define CONSPIRE_MAINBAR_H
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

#include "Conspire/GUI/widgetrack.h"

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    class Button;

    /** This class provides the floating button
        bar for the OptionsWidget */
    class CONSPIRE_EXPORT MainBar : public WidgetRack
    {
        Q_OBJECT
        
    public:
        MainBar(QGraphicsItem *parent=0);
        ~MainBar();
        
    public slots:
        void canRedoChanged(bool can_redo);
        void canUndoChanged(bool can_undo);
        
        void redoTextChanged(const QString &redotext);
        void undoTextChanged(const QString &undotext);
        
    signals:
        void undo();
        void redo();
        
        void home();

        void submit();

    private:
        void build();
        
        Button *submit_button;
        
        Button *undo_button;
        Button *redo_button;

        Button *home_button;
    };
    
}

CONSPIRE_END_HEADER

#endif