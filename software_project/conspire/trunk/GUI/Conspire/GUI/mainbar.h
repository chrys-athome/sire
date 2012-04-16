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

#include <QGraphicsWidget>

CONSPIRE_BEGIN_HEADER

class QAbstractButton;
class QLabel;

namespace Conspire
{
    /** This class provides the floating button
        bar for the OptionsWidget */
    class CONSPIRE_EXPORT MainBar : public QGraphicsWidget
    {
        Q_OBJECT
        
    public:
        MainBar(QGraphicsItem *parent=0);
        ~MainBar();
        
    public slots:
        void canBackChanged(bool can_back);
        void canForwardChanged(bool can_forward);
    
        void canRedoChanged(bool can_redo);
        void canUndoChanged(bool can_undo);
        
        void redoTextChanged(const QString &redotext);
        void undoTextChanged(const QString &undotext);
        
    signals:
        void undo();
        void redo();
        
        void back();
        void forward();

        void newPage();

    private:
        void build();
        
        QAbstractButton *new_button;
        
        QAbstractButton *undo_button;
        QAbstractButton *redo_button;

        QAbstractButton *back_button;
        QAbstractButton *forward_button;
    };
    
}

CONSPIRE_END_HEADER

#endif
