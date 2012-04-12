#ifndef CONSPIRE_ADDWIDGET_H
#define CONSPIRE_ADDWIDGET_H
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
#include "Conspire/GUI/pagewidget.h"

CONSPIRE_BEGIN_HEADER

class QSignalMapper;

namespace Conspire
{
    class Options;

    /** This widget is used to allow the user to add new
        option values to an Options object */
    class CONSPIRE_EXPORT AddWidget : public PageWidget
    {
        Q_OBJECT
        
    public:
        AddWidget(QGraphicsItem *parent=0);
        AddWidget(Options options, QGraphicsItem *parent=0);
        
        ~AddWidget();
        
        void setOptions(Options options);

    signals:
        void add(QString option);

    private slots:
        void addOption(QString option);

    private:
        void build();
        
        QSignalMapper *mapper;
    };
}

CONSPIRE_END_HEADER

#endif
