/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SPIER_SPIERVIEW_H
#define SPIER_SPIERVIEW_H

#include <QWidget>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/** This is the container widget that is used to hold the Spier
    view and control widgets. It is this widget that is exposed
    to the browser as a plugin, or to the main application as
    the main window
    
    @author Christopher Woods
*/
class SPIER_EXPORT SpierView : public QWidget
{
    Q_OBJECT
    
public:
    SpierView(QWidget *parent=0);

    ~SpierView();
};

}

SIRE_END_HEADER

#endif
