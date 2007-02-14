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

#ifndef SIREDB_RELATEIDMAP_H
#define SIREDB_RELATEIDMAP_H

#include <QMap>
#include <QMapIterator>

namespace SireDB
{

/** This is the ID number that is used to uniquely identify 
    each parameter */
typedef quint32 ParamID;

/** Typedef an unsigned integer as the type used internally to identify 
    relationships. Relationships are used to match atoms/atomgroups to 
    forcefield parameters. */
typedef quint32 RelateID;

/** This type is used to store the ID numbers of matching relationships,
    indexed by the score of the relationship */
typedef QMap<qint32, RelateID> RelateIDMap;

/** This is the iterator type over a RelateIDMap */
typedef QMapIterator<qint32, RelateID> RelateIDMapIterator;

}

#endif
