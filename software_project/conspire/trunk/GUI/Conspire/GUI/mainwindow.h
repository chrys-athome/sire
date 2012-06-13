#ifndef CONSPIRE_MAINWINDOW_H
#define CONSPIRE_MAINWINDOW_H
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

#include <QGraphicsView>

#include "Conspire/conspire.h"

#include "Conspire/GUI/page.h"

CONSPIRE_BEGIN_HEADER

class QGraphicsRectItem;

namespace Conspire
{
    class Options;

    class MainBar;

    /** This is a simple MainWindow class that houses all of the 
        graphical components of the application. Use this class
        as a template of how to combine together the main views
        needed to build a Conspire GUI application
        
        @author Christopher Woods
    */
    class CONSPIRE_EXPORT MainWindow : public QGraphicsView
    {
        Q_OBJECT
    
    public:
        MainWindow(QWidget *parent=0);
        MainWindow(Options options, QWidget *parent=0);
        
        ~MainWindow();

    protected:
        void resizeEvent(QResizeEvent *e);
        
    private:
        void build();
    
        //MainBar *mainbar;
        PagePointer view;
        
        QGraphicsRectItem *bg;
    };

}

CONSPIRE_END_HEADER

#endif
