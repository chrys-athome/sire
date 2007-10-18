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

#include "molcutgroupid.h"

#include <QDataStream>

using namespace SireMol;

QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const MolCutGroupID &molcgid)
{
    ds << molcgid.moleculeID() << molcgid.cutGroupID();
    return ds;
}

QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, MolCutGroupID &molcgid)
{
    ds >> molcgid._molid >> molcgid._cgid;
    return ds;
}

QString MolCutGroupID::toString() const
{
    return QString("%1:%2").arg(_molid.toString(),_cgid.toString());
}
