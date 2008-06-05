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

#ifndef SIREMOL_MOLECULECUTTING_H
#define SIREMOL_MOLECULEUTTING_H

#include "cuttingfunction.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class MoleculeCutting;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeCutting&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeCutting&);

namespace SireMol
{

/**
This is the ConvertFunction that is used to create a molecule that uses
molecule-based cutting - i.e. every atom in the molecule is in the same
CutGroup (CutGroup number 1).

This is a simple function object. Its only purpose is to return
the number of the CutGroup into which a particular atom should be
placed within a molecule whose data is contained in an EditMolData object.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeCutting : public CuttingFunctionBase
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeCutting&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeCutting&);

public:
    MoleculeCutting();
    MoleculeCutting(const MoleculeCutting &other);

    ~MoleculeCutting();

    CutGroupNum operator()(const AtomIndex &atom, const EditMolData &moldata) const;

    static const char* typeName()
    {
        return "SireMol::MoleculeCutting";
    }

    const char* what() const
    {
        return MoleculeCutting::typeName();
    }

    MoleculeCutting* clone() const
    {
        return new MoleculeCutting(*this);
    }
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeCutting)

SIRE_END_HEADER

#endif
