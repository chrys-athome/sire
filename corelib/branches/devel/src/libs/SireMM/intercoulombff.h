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

#ifndef SIREMM_INTERCOULOMBFF_H
#define SIREMM_INTERCOULOMBFF_H

#include "coulombff.h"

#include "SireMol/moleculeid.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterCoulombFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterCoulombFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterCoulombFF&);

namespace SireMM
{

using SireMol::MoleculeID;
using SireFF::ParameterMap;

/** An InterCoulombFF is used to calculate the intermolecular Coulomb
    energy of a group of molecules.

    @author Christopher Woods
*/
class SIREMM_EXPORT InterCoulombFF : public CoulombFF
{

friend QDataStream& ::operator<<(QDataStream&, const InterCoulombFF&);
friend QDataStream& ::operator>>(QDataStream&, InterCoulombFF&);

public:
    InterCoulombFF();

    InterCoulombFF(const Space &space, const SwitchingFunction &switchingfunction);

    InterCoulombFF(const InterCoulombFF &other);

    ~InterCoulombFF();

    class SIREMM_EXPORT Components : public CoulombFF::Components
    {
    public:
        Components();

        Components(const FFBase &ffbase, const Symbols &symbols);

        Components(const Components &other);

        ~Components();

        Components& operator=(const Components &other);
    };

    class SIREMM_EXPORT Parameters : public CoulombFF::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();
    };

    class SIREMM_EXPORT Groups : public CoulombFF::Groups
    {
    public:
        Groups();
        Groups(const Groups &other);

        ~Groups();
    };

    static const char* typeName()
    {
        return "SireMM::InterCoulombFF";
    }

    const char* what() const
    {
        return InterCoulombFF::typeName();
    }

    InterCoulombFF* clone() const
    {
        return new InterCoulombFF(*this);
    }

    void mustNowRecalculateFromScratch();

    bool change(const PartialMolecule &molecule);
    bool add(const PartialMolecule &mol, const ParameterMap &map = ParameterMap());
    bool remove(const PartialMolecule &molecule);

    bool contains(const PartialMolecule &molecule) const;
    
    bool refersTo(MoleculeID molid) const;

    QSet<FFBase::Group> groupsReferringTo(MoleculeID molid) const;
    
    QSet<MoleculeID> moleculeIDs() const;
    
    PartialMolecule molecule(MoleculeID molid) const;
    
    QHash<MoleculeID,PartialMolecule> contents() const;

protected:
    void recalculateViaDelta();
    void recalculateTotalEnergy();

    void recalculateEnergy();

    ChangedCoulombMolecule changeRecord(MoleculeID molid) const;

    bool applyChange(MoleculeID molid,
                     const ChangedCoulombMolecule &new_molecule);

    void _pvt_copy(const FFBase &other);

private:
    void updateCurrentState(const CoulombMolecule &new_molecule);
    void removeFromCurrentState(MoleculeID molid);

    /** All of the molecules that have at least one atom
        in this forcefield */
    QVector<CoulombFF::CoulombMolecule> mols;

    /** Hash mapping the MoleculeID to the index of the molecule in 'mols' */
    QHash<MoleculeID, uint> molid_to_index;

    /** Information about all of the changed molecules since the
        last energy calculation */
    QVector<CoulombFF::ChangedCoulombMolecule> changed_mols;

    /** Hash mapping the MoleculeID of a changed molecule to its
        index in changed_mols */
    QHash<MoleculeID, uint> molid_to_changedindex;

    /** MoleculeIDs of all molecules that have been removed since
        the last energy evaluation */
    QSet<MoleculeID> removed_mols;
    
    /** Whether or not a total energy recalculation is required */
    bool need_total_recalc;
};

}

Q_DECLARE_METATYPE(SireMM::InterCoulombFF);

SIRE_END_HEADER

#endif
