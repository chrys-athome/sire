#ifndef CONSPIRE_EXCEPTIONPAGE_H
#define CONSPIRE_EXCEPTIONPAGE_H
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

#include "Conspire/exceptions.h"

#include "Siren/obj.h"

CONSPIRE_BEGIN_HEADER

class QLabel;
class QTextEdit;

namespace Conspire
{
    /** This is a page that is used to display an error message */
    class CONSPIRE_EXPORT ExceptionPage : public Page
    {
        Q_OBJECT
    
    public:
        ExceptionPage(QGraphicsItem *parent=0);
        ExceptionPage(QString description, const Conspire::Exception &e,
                      QGraphicsItem *parent=0);
                        
        ~ExceptionPage();
        
    private:
        void build();
        void setException(QString description, const Conspire::Exception &e);
        
        QString desc;
        Siren::Obj e;
        
        QLabel *short_error;
        QTextEdit *long_error;
    };
}

CONSPIRE_END_HEADER

#endif
