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

#ifndef SIREMM_INTERGROUPLJFF_H
#define SIREMM_INTERGROUPLJFF_H

#include "ljff.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterGroupLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterGroupLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterGroupLJFF&);

namespace SireMM
{

using SireMol::Molecule;
using SireMol::Residue;
using SireMol::NewAtom;
using SireMol::MoleculeID;

using SireFF::ParameterMap;

/** This forcefield is used to calculate the LJ interaction energy
    between two groups of molecules.

    @author Christopher Woods
*/
class SIREMM_EXPORT InterGroupLJFF : public LJFF
{

friend QDataStream& ::operator<<(QDataStream&, const InterGroupLJFF&);
friend QDataStream& ::operator>>(QDataStream&, InterGroupLJFF&);

public:
    InterGroupLJFF();

    InterGroupLJFF(const Space &space, const SwitchingFunction &switchfunc);

    InterGroupLJFF(const InterGroupLJFF &other);

    ~InterGroupLJFF();

    InterGroupLJFF& operator=(const InterGroupLJFF &other);

    class SIREMM_EXPORT Components : public LJFF::Components
    {
    public:
        Components();

        Components(const FFBase &ffbase, const Symbols &symbols);

        Components(const Components &other);

        ~Components();

        Components& operator=(const Components &other);
    };

    class SIREMM_EXPORT Parameters : public LJFF::Parameters
    {
    public:
        Parameters();
        Parameters(const Parameters &other);

        ~Parameters();
    };

    class SIREMM_EXPORT Groups : public LJFF::Groups
    {
    public:
        Groups();
        Groups(const Groups &other);

        ~Groups();

        int A() const
        {
            return 0;
        }

        int B() const
        {
            return 1;
        }
    };

    static const char* typeName()
    {
        return "SireMM::InterGroupLJFF";
    }

    const char* what() const
    {
        return InterGroupLJFF::typeName();
    }

    InterGroupLJFF* clone() const
    {
        return new InterGroupLJFF(*this);
    }

    bool change(const Molecule &molecule);
    bool change(const Residue &residue);
    bool change(const NewAtom &atom);

    bool add(const Molecule &mol, const ParameterMap &map = ParameterMap());
    bool add(const Residue &res, const ParameterMap &map = ParameterMap());
    bool add(const NewAtom &atom, const ParameterMap &map = ParameterMap());

    bool add(const Molecule &mol, const AtomSelection &selected_atoms,
             const ParameterMap &map = ParameterMap());

    bool addTo(int group, const Molecule &molecule,
               const ParameterMap &map = ParameterMap());
    bool addTo(int group, const Residue &residue,
               const ParameterMap &map = ParameterMap());
    bool addTo(int group, const NewAtom &atom,
               const ParameterMap &map = ParameterMap());

    bool addTo(int group, const Molecule &molecule,
               const AtomSelection &selected_atoms,
               const ParameterMap &map = ParameterMap());

    bool remove(const Molecule &molecule);
    bool remove(const Residue &residue);
    bool remove(const NewAtom &atom);

    bool remove(const Molecule &mol, const AtomSelection &selected_atoms);

protected:
    void recalculateViaDelta();
    void recalculateTotalEnergy();

    void recalculateEnergy();

    ChangedLJMolecule changeRecord(MoleculeID molid) const;

    bool applyChange(MoleculeID molid,
                     const ChangedLJMolecule &new_molecule);

    int otherGroup(int group_id) const;

    void assertValidGroup(int group_id, MoleculeID molid) const;

private:
    double recalculateWithTwoChangedGroups();
    double recalculateWithOneChangedGroup(int changed_idx);
    double recalculateChangedWithUnchanged(int unchanged_idx, int changed_idx);
    double recalculateChangedWithChanged();

    void recordChange(int group_idx, MoleculeID molid,
                      const ChangedLJMolecule &new_molecule);

    void addToCurrentState(int group_idx, const LJMolecule &new_molecule);
    void removeFromCurrentState(int group_idx, MoleculeID molid);
    void updateCurrentState(int group_idx, const LJMolecule &molecule);

    template<class T>
    bool _pvt_add(const T &mol, const ParameterMap &map);
    template<class T>
    bool _pvt_addTo(int group_idx, const T &mol, const ParameterMap &map);
    template<class T>
    bool _pvt_change(const T &mol);
    template<class T>
    bool _pvt_remove(const T &mol);


    /** All of the molecules that have at least one molecule in
        for forcefield, divided into their two groups */
    QVector<LJMolecule> mols[2];

    /** Hash mapping the MoleculeID to the index of the molecules
        in the forcefield - one index for each group */
    QHash<MoleculeID,uint> molid_to_index[2];

    /** Information about all of the changed molecules since the
        last energy calculation, for both of the groups */
    QVector<LJFF::ChangedLJMolecule> changed_mols[2];

    /** Hash mapping the MoleculeID of a changed molecule to its
        index in changed_mols, for both of the groups */
    QHash<MoleculeID, uint> molid_to_changedindex[2];
};

}

Q_DECLARE_METATYPE(SireMM::InterGroupLJFF);

SIRE_END_HEADER

#endif
