/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#include "clj14group.h"

#include "SireMol/atomcoords.h"
#include "SireMol/atomcharges.h"
#include "SireMM/atomljs.h"

#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireBase;
using namespace SireStream;

namespace SireMM
{
    namespace detail
    {
        class CLJ14AtomData
        {
        public:
            /** index of the two atoms, in sorted CGAtomIdx order */
            CGAtomIdx atom0, atom1;
        
            /** The reduced charges of both atoms */
            float chg0, chg1;
            
            /** The reduced sigma parameters of both atoms */
            float sig0, sig1;
            
            /** The reduced epsilon parameters for both atoms */
            float eps0, eps1;
            
            /** The 14 coulomb and LJ scale factors between these atoms */
            float coul14scl, lj14scl;
        
            CLJ14AtomData();
            
            CLJ14AtomData(const CLJ14AtomData &other)
                : atom0(other.atom0), atom1(other.atom1),
                  chg0(other.chg0), chg1(other.chg1),
                  sig0(other.sig0), sig1(other.sig1),
                  eps0(other.eps0), eps1(other.eps1),
                  coul14scl(other.coul14scl), lj14scl(other.lj14scl)
            {}
            
            ~CLJ14AtomData()
            {}
            
            CLJ14AtomData& operator=(const CLJ14AtomData &other)
            {
                if (this != &other)
                {
                    atom0 = other.atom0;
                    atom1 = other.atom1;
                    chg0 = other.chg0;
                    chg1 = other.chg1;
                    sig0 = other.sig0;
                    sig1 = other.sig1;
                    eps0 = other.eps0;
                    eps1 = other.eps1;
                    coul14scl = other.coul14scl;
                    lj14scl = other.lj14scl;
                }
                
                return *this;
            }
            
            bool operator==(const CLJ14AtomData &other) const
            {
                return atom0 == other.atom0 and
                       atom1 == other.atom1 and
                       coul14scl == other.coul14scl and
                       lj14scl == other.lj14scl;
            }
        };
    }
}

QDataStream& operator<<(QDataStream &ds, const SireMM::detail::CLJ14AtomData &atom)
{
    quint32 version = 1;
    
    ds << version
       << atom.atom0 << atom.atom1
       << atom.chg0 << atom.chg1
       << atom.sig0 << atom.sig1
       << atom.eps0 << atom.eps1
       << atom.coul14scl << atom.lj14scl;
    
    return ds;
}

QDataStream& operator>>(QDataStream &ds, SireMM::detail::CLJ14AtomData &atom)
{
    quint32 version;
    
    ds >> version;
    
    if (version == 1)
    {
        ds >> atom.atom0 >> atom.atom1
           >> atom.chg0 >> atom.chg1
           >> atom.sig0 >> atom.sig1
           >> atom.eps0 >> atom.eps1
           >> atom.coul14scl >> atom.lj14scl;
    }
    else
        throw version_error( QObject::tr(
                "You are trying to load an unsupported version of CLJ14AtomData. "
                "You are loading version %1, but this code only supports version 1.")
                    .arg(version), CODELOC );

    return ds;
}

static const RegisterMetaType<CLJ14Group> r_group;

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJ14Group &group)
{
    writeHeader(ds, r_group, 1);
    
    SharedDataStream sds(ds);
    
    sds << group.mol << group.newmol << group.propmap
        << group.data_for_pair << group.cgidx_to_idx
        << group.cgpair_to_idx
        << group.total_cnrg << group.total_ljnrg
        << group.needs_energy;
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJ14Group &group)
{
    VersionID v = readHeader(ds, r_group);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> group.mol >> group.newmol >> group.propmap
            >> group.data_for_pair >> group.cgidx_to_idx
            >> group.cgpair_to_idx
            >> group.total_cnrg >> group.total_ljnrg
            >> group.needs_energy;
    }
    else
        throw version_error(v, "1", r_group, CODELOC );
    
    return ds;
}

/** Constructor */
CLJ14Group::CLJ14Group()
           : total_cnrg(0), total_ljnrg(0),
             needs_energy(false)
{}

/** Construct to calculate the 14-energy of the passed molecule */
CLJ14Group::CLJ14Group(const MoleculeView &molecule, const PropertyMap &map)
           : propmap(map),
             total_cnrg(0), total_ljnrg(0),
             needs_energy(false)
{
    this->add(molecule);
}

/** Copy constructor */
CLJ14Group::CLJ14Group(const CLJ14Group &other)
           : mol(other.mol), newmol(other.newmol), propmap(other.propmap),
             data_for_pair(other.data_for_pair), cgidx_to_idx(other.cgidx_to_idx),
             cgpair_to_idx(other.cgpair_to_idx),
             total_cnrg(other.total_cnrg), total_ljnrg(other.total_ljnrg),
             needs_energy(other.needs_energy)
{}

/** Destructor */
CLJ14Group::~CLJ14Group()
{}

/** Copy assignment operator */
CLJ14Group& CLJ14Group::operator=(const CLJ14Group &other)
{
    if (this != &other)
    {
        mol = other.mol;
        newmol = other.newmol;
        propmap = other.propmap;
        data_for_pair = other.data_for_pair;
        cgidx_to_idx = other.cgidx_to_idx;
        cgpair_to_idx = other.cgpair_to_idx;
        total_cnrg = other.total_cnrg;
        total_ljnrg = other.total_ljnrg;
        needs_energy = other.needs_energy;
    }
    
    return *this;
}

/** Comparison operator */
bool CLJ14Group::operator==(const CLJ14Group &other) const
{
    return mol == other.mol and
           newmol == other.newmol and
           propmap == other.propmap;
}

/** Comparison operator */
bool CLJ14Group::operator!=(const CLJ14Group &other) const
{
    return not operator==(other);
}

const char* CLJ14Group::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJ14Group>() );
}

const char* CLJ14Group::what() const
{
    return CLJ14Group::typeName();
}

bool CLJ14Group::isNull() const
{
    return newmol.isNull();
}

QString CLJ14Group::toString() const
{
    if (isNull())
        return QObject::tr("CLJ14Group::null");
    else
        return QObject::tr("CLJ14Group( molecule = %1 )").arg(newmol.toString());
}

/** Return the molecule that is in this group */
const MoleculeView& CLJ14Group::molecule() const
{
    return newmol;
}

/** Return the property map used to find the properties needed to 
    calculate the 14 energy ("coordinates", "charge", "LJ"
    and "intrascale") */
PropertyMap CLJ14Group::propertyMap() const
{
    return propmap;
}

/** Return whether or not we are recalculating things from scratch */
bool CLJ14Group::recalculatingFromScratch() const
{
    return (mol.version() == newmol.version()) and needs_energy;
}

/** Set the flag to say that we must recalculate everything
    from scratch */
void CLJ14Group::mustNowRecalculateFromScratch()
{
    mol = newmol;
    total_cnrg = 0;
    total_ljnrg = 0;
    needs_energy = true;
}

/** Set the flag to ensure that the energy is really completely
    recalculated from scratch */
void CLJ14Group::mustReallyRecalculateFromScratch()
{
    mol = newmol;
    total_cnrg = 0;
    total_ljnrg = 0;
    data_for_pair.clear();
    cgidx_to_idx.clear();
    cgpair_to_idx.clear();
    needs_energy = true;
}

/** Add the passed molecule to this group */
void CLJ14Group::add(const MoleculeView &new_molecule)
{
    if (isNull())
    {
        mustReallyRecalculateFromScratch();
        newmol = new_molecule;
    }
    else
    {
        if (newmol.data().number() != mol.data().number())
            throw SireError::incompatible_error( QObject::tr(
                    "You cannot add the new molecule %1 as it is not the same "
                    "molecule as the old molecule %2.")
                        .arg(new_molecule.toString())
                        .arg(mol.toString()), CODELOC );

        this->add(new_molecule.selection());
        this->update(new_molecule);
    }
}

/** Add the passed selection onto this group */
void CLJ14Group::add(const AtomSelection &new_selection)
{
    AtomSelection selected_atoms = newmol.selection();
    selected_atoms = selected_atoms.select(new_selection);
    this->updateSelection(selected_atoms);
}

/** Update the selection to the new passed value */
void CLJ14Group::updateSelection(const AtomSelection &selection)
{
    if (isNull())
        return;

    else if (selection.selectedNone())
    {
        PropertyMap map = propmap;
        this->operator=( CLJ14Group() );
        propmap = map;
        return;
    }
    
    if (newmol.selection() != selection)
    {
        newmol = PartialMolecule(newmol.data(), selection);
        mustReallyRecalculateFromScratch();
    }
}

/** Update the contained molecule to the newest version */
void CLJ14Group::update(const MoleculeView &new_molecule)
{
    if (isNull())
        return;
    
    if (new_molecule.data().number() != newmol.number())
        throw SireError::incompatible_error( QObject::tr(
                    "You cannot update the new molecule %1 as it is not the same "
                    "molecule as the old molecule %2.")
                        .arg(new_molecule.toString())
                        .arg(mol.toString()), CODELOC );
    
    if (recalculatingFromScratch())
    {
        newmol = new_molecule;
        mol = new_molecule;
        needs_energy = true;
    }
    else
    {
        newmol = new_molecule;
        needs_energy = (newmol.version() != mol.version());
    }
}

/** Remove the passed selection from the group */
void CLJ14Group::remove(const AtomSelection &new_selection)
{
    if (not isNull())
    {
        AtomSelection selected_atoms = newmol.selection();
        selected_atoms = selected_atoms.deselect(new_selection);
        this->updateSelection(selected_atoms);
    }
}

/** Remove the passed molecule from this group */
void CLJ14Group::remove(const MoleculeView &new_molecule)
{
    if (not isNull())
    {
        this->remove(new_molecule.selection());
        this->update(new_molecule);
    }
}

/** Calculate and return the coulomb and LJ 14 energy */
boost::tuple<double,double> CLJ14Group::energy()
{
    if (recalculatingFromScratch())
    {
        if (data_for_pair.isEmpty())
        {
            this->reextract();
        }
        
        total_cnrg = 0;
        total_ljnrg = 0;
        
        for (int i=0; i<data_for_pair.count(); ++i)
        {
            double cnrg, ljnrg;
            this->calculateEnergy(newmol, data_for_pair.at(i), cnrg, ljnrg);
            total_cnrg += cnrg;
            total_ljnrg += ljnrg;
        }
    }
    else
    {
        //see if the charge, LJ or intrascale properties have changed.
        //If they have, then we need to really rebuild everything
        //from scratch
        const PropertyName chg_property = propmap["charge"];
        const PropertyName lj_property = propmap["LJ"];
        const PropertyName scl_property = propmap["intrascale"];
        
        if (newmol.version(chg_property) != mol.version(chg_property) or
            newmol.version(lj_property) != mol.version(lj_property) or
            newmol.version(scl_property) != mol.version(scl_property))
        {
            this->mustReallyRecalculateFromScratch();
            return this->energy();
        }

        const PropertyName coords_property = propmap["coordinates"];
        
        if (newmol.version(coords_property) == mol.version(coords_property))
        {
            //nothing has changed
            return boost::tuple<double,double>(total_cnrg,total_ljnrg);
        }

        //only the coordinates have changed. Find out which CutGroups have
        //changed
        if (newmol.data().info().nCutGroups() <= 1)
        {
            this->mustNowRecalculateFromScratch();
            return this->energy();
        }
        
        QSet<quint32> changed_cgroups;
        
        const AtomCoords &newcoords = newmol.property(coords_property).asA<AtomCoords>();
        const AtomCoords &oldcoords = mol.property(coords_property).asA<AtomCoords>();
        
        const CoordGroup *newarray = newcoords.constData();
        const CoordGroup *oldarray = oldcoords.constData();
        
        if (newarray != oldarray)
        {
            for (int i=0; i<newmol.data().info().nCutGroups(); ++i)
            {
                const Vector *newatoms = newarray[i].constData();
                const Vector *oldatoms = oldarray[i].constData();

                if (newatoms != oldatoms)
                {
                    for (int j=0; j<newmol.data().info().nAtoms(CGIdx(i)); ++j)
                    {
                        if (newatoms[j] != oldatoms[j])
                        {
                            changed_cgroups.insert( quint32(i) );
                            break;
                        }
                    }
                }
            }
        }
        
        if (changed_cgroups.count() >= int(0.4 * newmol.data().info().nCutGroups()))
        {
            this->mustNowRecalculateFromScratch();
            return this->energy();
        }
        
        //calculate the change in energy as a delta
        double delta_cnrg = 0;
        double delta_ljnrg = 0;
        
        foreach (quint32 cgroup, changed_cgroups)
        {
            QSet<quint32> bonded_groups = cgidx_to_idx.value(cgroup, QSet<quint32>());
            
            foreach (quint32 bonded_group, bonded_groups)
            {
                if (bonded_group <= cgroup or not changed_cgroups.contains(bonded_group))
                {
                    //process this pair only if we find it first (we don't process
                    //pairs that are greater to avoid double counting
                    int idx = -1;
                    
                    if (cgroup <= bonded_group)
                    {
                        idx = cgpair_to_idx.value(
                                    SireMM::detail::IDPair(cgroup,bonded_group), -1 );
                    }
                    else
                    {
                        idx = cgpair_to_idx.value(
                                    SireMM::detail::IDPair(bonded_group,cgroup), -1 );
                    }
                    
                    if (idx != -1)
                    {
                        double old_cnrg, old_ljnrg;
                        calculateEnergy(mol, data_for_pair.at(idx), old_cnrg, old_ljnrg);
                        
                        double new_cnrg, new_ljnrg;
                        calculateEnergy(mol, data_for_pair.at(idx), new_cnrg, new_ljnrg);

                        delta_cnrg += (new_cnrg - old_cnrg);
                        delta_ljnrg += (new_ljnrg - old_ljnrg);
                    }
                }
            }
        }
        
        total_cnrg += delta_cnrg;
        total_ljnrg += delta_ljnrg;
    }

    mol = newmol;
    needs_energy = false;

    return boost::tuple<double,double>(total_cnrg,total_ljnrg);
}
