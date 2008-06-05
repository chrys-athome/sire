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

#ifndef SIREDB_MATCHMOL_H
#define SIREDB_MATCHMOL_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

class MatchRes;

/**
This class is used by MatchData to contain the information that is used to match a molecule.
This information can include the group that the molecule is in (e.g. "solute", "protein",
"solute" etc.) and also the state of the molecule (e.g. "reference", "perturbed"). The
group and state are really just extra strings that can be attached in addition to the 
name of the molecule.

@author Christopher Woods
*/
class SIREDB_EXPORT MatchMol
{
public:
    MatchMol();
    
    MatchMol(const MatchMol &other);
    
    ~MatchMol();
    
    bool operator==(const MatchMol &other) const;
    bool operator!=(const MatchMol &other) const;
    
    QString toString() const;
    
    bool isEmpty() const;
    
    bool hasNameMatch() const;
    bool hasGroupMatch() const;
    bool hasStateMatch() const;
    
    void setName(const QString &name);
    QString name() const;

    void setGroup(const QString &group);
    QString group() const;
    
    void setState(const QString &state);
    QString state() const;

    MatchMol operator&&(const MatchMol &other) const;

private:

    /** The name of the molecule */
    QString molname;
    
    /** The group that the molecule is in */
    QString molgroup;
    
    /** The state of the molecule */
    QString molstate;
};

}

SIRE_END_HEADER

#endif
