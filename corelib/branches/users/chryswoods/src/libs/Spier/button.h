/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SPIER_BUTTON_H
#define SPIER_BUTTON_H

#include "textlabel.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This is a button (obvious...)
 
@author Christopher Woods
*/
class SPIER_EXPORT ButtonBase : public TextLabelBase
{

friend class Wrapper<ButtonBase>;

Q_OBJECT

public:
    ~ButtonBase();

signals:
    void clicked(bool checked=false);

protected:
    ButtonBase(GLWidget parent=GLWidget());

    void paintWidget(QPainter &painter);

    void mouseClickEvent(QMouseEvent *e);

};

typedef Wrapper<ButtonBase> Button;

}

SIRE_END_HEADER

#endif
