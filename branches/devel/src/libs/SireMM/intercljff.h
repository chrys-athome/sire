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

#ifndef SIREMM_INTERCLJFF_H
#define SIREMM_INTERCLJFF_H

#include "cljff.h"

#include "SireFF/parametermap.h"

namespace SireMM
{
class InterCLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterCLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterCLJFF&);

namespace SireMM
{

class ChargeTable;
class LJTable;

using SireFF::ParameterMap;

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::MoleculeID;

/** An InterCLJFF is a forcefield that calculates the intermolecular coulomb and
    Lennard Jones energies of all contained molecules. An InterCLJFF is perhaps
    the most expensive type of MM forcefield, as it must calculate the full
    pair-pair interactions between all pairs of molecules in the forcefield
    that are within the cutoff distance.

    \author Christopher Woods
*/
class SIREMM_EXPORT InterCLJFF : public CLJFF
{

friend QDataStream& ::operator<<(QDataStream&, const InterCLJFF&);
friend QDataStream& ::operator>>(QDataStream&, InterCLJFF&);

public:
    InterCLJFF();

    InterCLJFF(const Space &space, const SwitchingFunction &switchfunc);

    InterCLJFF(const InterCLJFF &other);

    ~InterCLJFF();

    class SIREMM_EXPORT Components : public CLJFF::Components
    {
    public:
        Components();

        Components(const FFBase &ffbase, const Symbols &symbols);

        Components(const Components &other);

        ~Components();

        Components& operator=(const Components &other);
    };

    class SIREMM_EXPORT Parameters : public CLJFF::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();
    };

    class SIREMM_EXPORT Groups : public CLJFF::Groups
    {
    public:
        Groups();
        Groups(const Groups &other);
        
        ~Groups();
    };

    static const char* typeName()
    {
        return "SireMM::InterCLJFF";
    }

    const char* what() const
    {
        return InterCLJFF::typeName();
    }

    InterCLJFF* clone() const
    {
        return new InterCLJFF(*this);
    }

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);

    bool add(const Molecule &mol, const ParameterMap &map = ParameterMap());

    bool remove(const Molecule &mol);

protected:
    void recalculateViaDelta();
    void recalculateTotalEnergy();

    void recalculateEnergy();

    void setCurrentState(const detail::MolCLJInfo &mol);
    void removeCurrentState(const Molecule &mol);

    /** Information about every molecule contained in this forcefield */
    QVector<detail::MolCLJInfo> mols;

    /** Hash mapping MoleculeID to index in 'mols' */
    QHash<MoleculeID, int> molid_to_molindex;

    /** Information about all of the changed molecules since the last
        energy update */
    QVector<detail::ChangedMolCLJInfo> changedmols;

    /** Hash mapping MoleculeID to index in 'changedmols' */
    QHash<MoleculeID, int> molid_to_changedindex;

    /** The IDs of all of the molecules that were removed since the last
        energy update */
    QSet<MoleculeID> removedmols;
};

}


#endif
