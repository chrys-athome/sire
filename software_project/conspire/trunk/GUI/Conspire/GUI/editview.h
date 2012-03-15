#ifndef CONSPIRE_EDITVIEW_H
#define CONSPIRE_EDITVIEW_H
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

#include <QGraphicsWidget>
#include <QPointer>

CONSPIRE_BEGIN_HEADER

namespace Conspire
{
    /** This is the base class of the widget used to edit and view
        Option and Options objects */
    class CONSPIRE_EXPORT EditView : public QGraphicsWidget
    {
        Q_OBJECT
        
    public:
        EditView(EditView *parent=0);
        
        virtual ~EditView();
        
        EditView* parentView();
        const EditView* parentView() const;
        
        EditView* rootNode();
        const EditView* rootNode() const;
        
        virtual QString rootKey() const;
        virtual QString key() const;
        virtual QString fullKey() const;
        
    signals:
        void add(QString full_key);
        void remove(QString full_key);
        void update(QString full_key, Obj new_value);
        
    protected slots:
        void added();
        void removed();
        void updated(Obj new_value);
        
    private:
        QPointer<EditView> prnt;
    };
}

CONSPIRE_END_HEADER

#endif
