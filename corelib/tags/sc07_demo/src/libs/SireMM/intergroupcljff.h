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

#ifndef SIREMM_INTERGROUPCLJFF_H
#define SIREMM_INTERGROUPCLJFF_H

#include "cljff.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterGroupCLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterGroupCLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterGroupCLJFF&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::Molecules;

using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::MoleculeID;

using SireFF::ParameterMap;

/** This forcefield is used to calculate the LJ interaction energy
    between two groups of molecules.

    @author Christopher Woods
*/
class SIREMM_EXPORT InterGroupCLJFF : public CLJFF
{

friend QDataStream& ::operator<<(QDataStream&, const InterGroupCLJFF&);
friend QDataStream& ::operator>>(QDataStream&, InterGroupCLJFF&);

public:
    InterGroupCLJFF();

    InterGroupCLJFF(const Space &space, const SwitchingFunction &switchfunc);

    InterGroupCLJFF(const InterGroupCLJFF &other);

    ~InterGroupCLJFF();

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
    friend class InterGroupCLJFF;

    public:
        Groups();
        Groups(const Groups &other);

        ~Groups();

        FFBase::Group A() const
        {
            return a;
        }

        FFBase::Group B() const
        {
            return b;
        }

    protected:
        static Groups default_group;

    private:
        /** The ID of group 'A' */
        FFBase::Group a;
        /** The ID of group 'B' */
        FFBase::Group b;
    };

    const InterGroupCLJFF::Groups& groups() const
    {
        return InterGroupCLJFF::Groups::default_group;
    }

    static const char* typeName()
    {
        return "SireMM::InterGroupCLJFF";
    }

    const char* what() const
    {
        return InterGroupCLJFF::typeName();
    }

    InterGroupCLJFF* clone() const
    {
        return new InterGroupCLJFF(*this);
    }

    void mustNowRecalculateFromScratch();

    bool change(const PartialMolecule &molecule);
    bool change(const Molecules &molecules);

    bool add(const PartialMolecule &molecule,
             const ParameterMap &map = ParameterMap());

    bool add(const Molecules &molecules,
             const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::Group &group, const PartialMolecule &molecule,
               const ParameterMap &map = ParameterMap());

    bool addTo(const FFBase::Group &group, const Molecules &molecules,
               const ParameterMap &map = ParameterMap());

    bool addToA(const PartialMolecule &molecule,
                const ParameterMap &map = ParameterMap());

    bool addToA(const Molecules &molecules,
                const ParameterMap &map = ParameterMap());

    bool addToB(const PartialMolecule &molecule,
                const ParameterMap &map = ParameterMap());

    bool addToB(const Molecules &molecules,
                const ParameterMap &map = ParameterMap());

    bool remove(const PartialMolecule &molecule);
    bool remove(const Molecules &molecules);

    bool removeFrom(const FFBase::Group &group,
                    const PartialMolecule &molecule);

    bool removeFrom(const FFBase::Group &group,
                    const Molecules &molecules);

    bool removeFromA(const PartialMolecule &molecule);
    bool removeFromA(const Molecules &molecules);

    bool removeFromB(const PartialMolecule &molecule);
    bool removeFromB(const Molecules &molecules);

    bool contains(const PartialMolecule &molecule) const;

    bool contains(const PartialMolecule &molecule,
                  const FFBase::Group &group) const;

    bool refersTo(MoleculeID molid) const;

    bool refersTo(MoleculeID molid,
                  const FFBase::Group &group) const;

    QSet<FFBase::Group> groupsReferringTo(MoleculeID molid) const;

    QSet<MoleculeID> moleculeIDs() const;

    QSet<MoleculeID> moleculeIDs(const FFBase::Group &group) const;

    PartialMolecule molecule(MoleculeID molid) const;

    PartialMolecule molecule(MoleculeID molid,
                             const FFBase::Group &group) const;

    Molecules contents() const;
    Molecules contents(const FFBase::Group &group) const;

protected:
    void recalculateViaDelta();
    void recalculateTotalEnergy();

    void recalculateEnergy();

    ChangedCLJMolecule changeRecord(MoleculeID molid) const;

    bool applyChange(MoleculeID molid,
                     const ChangedCLJMolecule &new_molecule);

    int groupIndex(FFBase::Group group) const;

    int otherGroup(int group_id) const;

    void assertValidGroup(int group_id, MoleculeID molid) const;

    void _pvt_copy(const FFBase &other);

private:
    CLJEnergy recalculateWithTwoChangedGroups();
    CLJEnergy recalculateWithOneChangedGroup(int changed_idx);
    CLJEnergy recalculateChangedWithUnchanged(int unchanged_idx, int changed_idx);
    CLJEnergy recalculateChangedWithChanged();

    void recordChange(int group_idx, MoleculeID molid,
                      const ChangedCLJMolecule &new_molecule);

    void addToCurrentState(int group_idx, const CLJMolecule &new_molecule);
    void removeFromCurrentState(int group_idx, MoleculeID molid);
    void updateCurrentState(int group_idx, const CLJMolecule &molecule);


    /** All of the molecules that have at least one molecule in
        for forcefield, divided into their two groups */
    QVector<CLJMolecule> mols[2];

    /** Hash mapping the MoleculeID to the index of the molecules
        in the forcefield - one index for each group */
    QHash<MoleculeID,uint> molid_to_index[2];

    /** Information about all of the changed molecules since the
        last energy calculation, for both of the groups */
    QVector<CLJFF::ChangedCLJMolecule> changed_mols[2];

    /** Hash mapping the MoleculeID of a changed molecule to its
        index in changed_mols, for both of the groups */
    QHash<MoleculeID, uint> molid_to_changedindex[2];

    /** Whether or not a total energy recalculation is required */
    bool need_total_recalc;
};

}

Q_DECLARE_METATYPE(SireMM::InterGroupCLJFF);

SIRE_END_HEADER

#endif
