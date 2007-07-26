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

#ifndef SIREMM_INTRACLJFF_H
#define SIREMM_INTRACLJFF_H

#include "cljff.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class IntraCLJFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::IntraCLJFF&);
QDataStream& operator>>(QDataStream&, SireMM::IntraCLJFF&);

namespace SireMM
{

using SireMol::MoleculeID;
using SireMol::Molecules;

using SireFF::ParameterMap;

/** This forcefield is used to calculate the intramolecular coulomb
    and Lennard Jones energies of all contained molecules.

    @author Christopher Woods
*/
class SIREMM_EXPORT IntraCLJFF : public CLJFF
{

friend QDataStream& ::operator<<(QDataStream&, const IntraCLJFF&);
friend QDataStream& ::operator>>(QDataStream&, IntraCLJFF&);

public:
    IntraCLJFF();

    IntraCLJFF(const Space &space, const SwitchingFunction &switchfunc);

    IntraCLJFF(const IntraCLJFF &other);

    ~IntraCLJFF();

    class IntraCGPairs;
    class IntraAtomPairs;

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

        /** Return the default source of the non-bonded scale factors */
        const ParameterName& nbScale() const
        {
            return nbscl_params;
        }

        static Parameters default_sources;

    private:
        /** The name and default source of the non-bonded scale factors */
        ParameterName nbscl_params;
    };

    const IntraCLJFF::Parameters& parameters() const
    {
        return IntraCLJFF::Parameters::default_sources;
    }

    class SIREMM_EXPORT Groups : public CLJFF::Groups
    {
    public:
        Groups();
        Groups(const Groups &other);

        ~Groups();
    };

    static const char* typeName()
    {
        return "SireMM::IntraCLJFF";
    }

    const char* what() const
    {
        return IntraCLJFF::typeName();
    }

    IntraCLJFF* clone() const
    {
        return new IntraCLJFF(*this);
    }

    void mustNowRecalculateFromScratch();

    bool change(const PartialMolecule &molecule);
    bool change(const Molecules &molecules);

    bool add(const PartialMolecule &molecule,
             const ParameterMap &map = ParameterMap());

    bool add(const Molecules &molecules,
             const ParameterMap &map = ParameterMap());

    bool remove(const PartialMolecule &molecule);
    bool remove(const Molecules &molecules);

    bool contains(const PartialMolecule &molecule) const;

    bool refersTo(MoleculeID molid) const;

    QSet<FFBase::Group> groupsReferringTo(MoleculeID molid) const;

    QSet<MoleculeID> moleculeIDs() const;

    PartialMolecule molecule(MoleculeID molid) const;

    Molecules contents() const;

protected:
    void _pvt_copy(const FFBase &other);

    void recalculateViaDelta();
    void recalculateTotalEnergy();

    void recalculateEnergy();

private:
    /** All of the molecules that have at least one atom in this forcefield */
    QVector<CLJFF::CLJMolecule> mols;

    /** Hash mapping the MoleculeID to the index of the Molecule in 'mols' */
    QHash<MoleculeID,uint> molid_to_index;

    /** Information about all of the molecules that have changed since the
        last evaluation */
    QVector<CLJFF::ChangedCLJMolecule> changed_mols;

    /** Hash mapping the MoleculeID of a changed molecule to its
        index in changed_mols */
    QHash<MoleculeID, uint> molid_to_changedindex;

    /** The non-bonded scale factors for each pair of atoms in
        each molecule */
    QHash<MoleculeID,CLJNBPairs> mol_nbpairs;

    /** Whether or not a total energy recalculation is required */
    bool need_total_recalc;
};

}

Q_DECLARE_METATYPE(SireMM::IntraCLJFF);

SIRE_END_HEADER

#endif
