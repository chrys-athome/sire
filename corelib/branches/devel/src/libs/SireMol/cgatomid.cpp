/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "cgatomid.h"

#include <QDataStream>
#include <QString>
#include <QDebug>

using namespace SireMol;

/** primitive type streaming function */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const CGAtomID &cgatomid)
{
    ds << cgatomid.cutGroupID() << cgatomid.atomID();
    
    return ds;
}

/** primitive type streaming function */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, CGAtomID &cgatomid)
{
    ds >> cgatomid._cgid >> cgatomid._atmid;
    return ds;
}

QString CGAtomID::toString() const
{
    return QString("%1:%2").arg(_cgid.toString(),_atmid.toString());
}
