/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include "sireglobal.h"

namespace SireMM { namespace detail { class CGIDQuad; } }

uint qHash(const SireMM::detail::CGIDQuad&);

#include "internalparameters.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireBase;
using namespace SireCAS;
using namespace SireMol;
using namespace SireFF;

using namespace SireStream;

//////////
////////// Implementation of SireMM::detail::CGIDQuad
//////////

uint qHash(const CGIDQuad &idquad)
{
    return idquad.hash();
}

//////////
////////// Implementation of InternalParameters
//////////

/** Return the index of the group that contains the potential and forces
    for the internals that act only within the CutGroup with index 'cgidx0'.
    
    This returns -1 if there is no such group 
*/
qint32 InternalParameters::getIndex(CGIdx cgidx0) const
{
    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it = groups_by_cgidx.constFind(cgidx0);
                                    
    if (it != groups_by_cgidx.constEnd())
    {
        const GroupInternalParameters *group_params_array = group_params.constData();
        
        foreach (qint32 i, it.value())
        {
            if (group_params_array[i].isSingleCutGroup(cgidx0))
                return i;
        }
    }
    
    return -1;
}

/** Return the index of the group that contains the potential and forces
    for the internals that act only between the CutGroups with indicies 
    'cgidx0' and 'cgidx1'.
    
    This returns -1 if there is no such group 
*/
qint32 InternalParameters::getIndex(CGIdx cgidx0, CGIdx cgidx1) const
{
    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it0 = groups_by_cgidx.constFind(cgidx0);

    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it1 = groups_by_cgidx.constFind(cgidx1);
                                    
    if (it0 != groups_by_cgidx.constEnd() and
        it1 != groups_by_cgidx.constEnd())
    {
        const GroupInternalParameters *group_params_array = group_params.constData();
        
        QSet<qint32> shared_groups = it0.value();
        shared_groups.intersect(it1.value());
        
        foreach (qint32 i, shared_groups)
        {
            if (group_params_array[i].isDoubleCutGroup(cgidx0,cgidx1))
                return i;
        }
    }
    
    return -1;
}

/** Return the index of the group that contains the potential and forces
    for the internals that act only between the three CutGroups with indicies 
    'cgidx0', 'cgidx1' and 'cgidx2'.
    
    This returns -1 if there is no such group 
*/
qint32 InternalParameters::getIndex(CGIdx cgidx0, CGIdx cgidx1,
                                    CGIdx cgidx2) const
{
    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it0 = groups_by_cgidx.constFind(cgidx0);

    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it1 = groups_by_cgidx.constFind(cgidx1);

    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it2 = groups_by_cgidx.constFind(cgidx2);
                                    
    if (it0 != groups_by_cgidx.constEnd() and
        it1 != groups_by_cgidx.constEnd() and
        it2 != groups_by_cgidx.constEnd())
    {
        const GroupInternalParameters *group_params_array = group_params.constData();
        
        QSet<qint32> shared_groups = it0.value();
        shared_groups.intersect(it1.value());
        shared_groups.intersect(it2.value());
        
        foreach (qint32 i, shared_groups)
        {
            if (group_params_array[i].isTripleCutGroup(cgidx0,cgidx1,cgidx2))
                return i;
        }
    }
    
    return -1;
}

/** Return the index of the group that contains the potential and forces
    for the internals that act only between the four CutGroups with indicies 
    'cgidx0', 'cgidx1', 'cgidx2' and 'cgidx3'.
    
    This returns -1 if there is no such group 
*/
qint32 InternalParameters::getIndex(CGIdx cgidx0, CGIdx cgidx1,
                                    CGIdx cgidx2, CGIdx cgidx3) const
{
    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it0 = groups_by_cgidx.constFind(cgidx0);

    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it1 = groups_by_cgidx.constFind(cgidx1);

    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it2 = groups_by_cgidx.constFind(cgidx2);

    QSet< CGIdx, QSet<qint32> >::const_iterator 
                                    it3 = groups_by_cgidx.constFind(cgidx3);
                                    
    if (it0 != groups_by_cgidx.constEnd() and
        it1 != groups_by_cgidx.constEnd() and
        it2 != groups_by_cgidx.constEnd() and
        it3 != groups_by_cgidx.constEnd())
    {
        const GroupInternalParameters *group_params_array = group_params.constData();
        
        QSet<qint32> shared_groups = it0.value();
        shared_groups.intersect(it1.value());
        shared_groups.intersect(it2.value());
        shared_groups.intersect(it3.value());
        
        foreach (qint32 i, shared_groups)
        {
            if (group_params_array[i].isQuadrupleCutGroup(cgidx0,cgidx1,
                                                          cgidx2,cgidx3))
                return i;
        }
    }
    
    return -1;
}

/** Return the index of the group with ID 'idquad'. This returns -1
    if there is no group with this ID */
qint32 InternalParameters::getIndex(const CGIDQuad &idquad) const
{
    if (idquad.isSingleCutGroup())
        return this->getIndex(idquad.cgIdx0());
    
    else if (idquad.isDoubleCutGroup())
        return this->getIndex(idquad.cgIdx0(), idquad.cgIdx1());
        
    else if (idquad.isTripleCutGroup())
        return this->getIndex(idquad.cgIdx0(), idquad.cgIdx1(),
                              idquad.cgIdx2());
                              
    else
        return this->getIndex(idquad.cgIdx0(), idquad.cgIdx1(),
                              idquad.cgIdx2(), idquad.cgIdx3());
}

/** Return the potential and force parameters for all of the internals
    that involve only atoms in the CutGroup at index 'cgidx0'
    
    This returns an empty set of parameters if there are no internals
    that involve this CutGroup
*/
const GroupInternalParameters& InternalParameters::getGroup(CGIdx cgidx0) const
{
    qint32 i = this->getIndex(cgidx0);
    
    if (i >= 0)
        return group_params.at(i);
    else
        return shared_empty_group_params;
}

/** Return the potential and force parameters for all of the internals
    that involve the two CutGroups at indicies 'cgidx0' and 'cgidx1'
    
    This returns an empty set of parameters if there are no internals
    that involve this CutGroup
*/
const GroupInternalParameters& InternalParameters::getGroup(CGIdx cgidx0, 
                                                            CGIdx cgidx1) const
{
    qint32 i = this->getIndex(cgidx0, cgidx1);
    
    if (i >= 0)
        return group_params.at(i);
    else
        return shared_empty_group_params;
}

/** Return the potential and force parameters for all of the internals
    that involve the three CutGroups at indicies 'cgidx0', 'cgidx1'
    and 'cgidx2'.
    
    This returns an empty set of parameters if there are no internals
    that involve this CutGroup
*/
const GroupInternalParameters& InternalParameters::getGroup(CGIdx cgidx0, 
                                                            CGIdx cgidx1,
                                                            CGIdx cgidx2) const
{
    qint32 i = this->getIndex(cgidx0, cgidx1, cgidx2);
    
    if (i >= 0)
        return group_params.at(i);
    else
        return shared_empty_group_params;
}

/** Return the potential and force parameters for all of the internals
    that involve the four CutGroups at indicies 'cgidx0', 'cgidx1',
    'cgidx2' and 'cgidx3'.
    
    This returns an empty set of parameters if there are no internals
    that involve this CutGroup
*/
const GroupInternalParameters& InternalParameters::getGroup(CGIdx cgidx0, 
                                                            CGIdx cgidx1,
                                                            CGIdx cgidx2, 
                                                            CGIdx cgidx3) const
{
    qint32 i = this->getIndex(cgidx0, cgidx1, cgidx2, cgidx3);
    
    if (i >= 0)
        return group_params.at(i);
    else
        return shared_empty_group_params;
}

/** Add the group that contains the potential and forces for the 
    internals that act only between the atoms between the four
    CutGroups at indicies 'cgidx0', 'cgidx1', 'cgidx2' and 'cgidx3',
    and then return the index of the added group.
*/
qint32 InternalParameters::addGroup(const CGIDQuad &idquad)
{
    qint32 i = this->getIndex(idquad);
    
    if (i < 0)
    {
        //the group doesn't exist - it must be added
        group_params.append( GroupInternalParameters(idquad) );
        
        i = group_params.count() - 1;
        
        if (idquad.isSingleCutGroup())
        {
            groups_by_cgidx[idquad.cgIdx0()].insert(i);
        }
        else if (idquad.isDoubleCutGroup())
        {
            groups_by_cgidx[idquad.cgIdx0()].insert(i);
            groups_by_cgidx[idquad.cgIdx1()].insert(i);
        }
        else if (idquad.isTripleCutGroup())
        {
            groups_by_cgidx[idquad.cgIdx0()].insert(i);
            groups_by_cgidx[idquad.cgIdx1()].insert(i);
            groups_by_cgidx[idquad.cgIdx2()].insert(i);
        }
        else
        {
            groups_by_cgidx[idquad.cgIdx0()].insert(i);
            groups_by_cgidx[idquad.cgIdx1()].insert(i);
            groups_by_cgidx[idquad.cgIdx2()].insert(i);
            groups_by_cgidx[idquad.cgIdx3()].insert(i);
        }
    }
    
    return i;
}

GroupInternalParameters&
InternalParameters:getGroup(CGIdx cgidx0, CGIdx cgidx1, CGIdx cgidx2,
                            CGIdx cgidx3, QHash<IDQuad,qint32> &cached_groups)
{
    IDQuad idquad(cgidx0, cgidx1, cgidx2, cgidx3);
    
    qint32 i = cached_groups.value(idquad, -1);
    
    if (i < 0)
    {
        i = this->addGroup(idquad);

        cached_groups.insert(idquad, i);
    }
    
    return groups_params.data()[i];
}

/** This adds all of the bond parameters and forces in 'bondparams'
    to the list of parameters */
void InternalParameters::addBonds(const TwoAtomFunctions &bondparams,
                                  QHash<CGIDQuad,qint32> &cached_groups)
{
    //assert that these are functions only of the bond length
    this->assertContainsOnly(this->symbols().bond(), bondparams.symbols());
    
    //get the potential and forces
    QVector<TwoBodyFunction> potentials = bondparams.potentials();
    QVector<TwoBodyFunction> forces = bondparams.forces(this->symbols().bond().r());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<TwoBodyFunction> > group_potentials;
    QHash< CGIDQuad, QVector<TwoBodyFunction> > group_forces;
    
    const TwoBodyFunction *potentials_array = potential.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const TwoBodyFunction &potential = potentials_array[i]
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    //do forces separately as not all potentials may have a force
    //(as potential could be constant with r)
    const TwoBodyFunction *forces_array = forces.constData();
    n = forces.count();

    NEED TO MAKE THIS CHANGE TO THREE AND FOUR ATOM FUNCTIONS

    for (int i=0; i<n; ++i)
    {
        const TwoBodyFunction &force = forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup());
                        
        group_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< IDQuad,QVector<TwoBodyFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<TwoBodyFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->addGroup(it.key(), cached_groups)
                        .setBondPotential( it.value(), group_force );
    }
}

/** This adds all of the angle parameters and forces in 'angleparams'
    to the list of parameters */
void InternalParameters::addAngles(const ThreeAtomFunctions &angleparams,
                                   QHash<CGIDQuad,qint32> &cached_groups)
{
    //assert that these are functions only of the angle (theta)
    this->assertContainsOnly(this->symbols().angle(), angleparams.symbols());
    
    //get the potential and forces
    QVector<ThreeBodyFunction> potentials = angleparams.potentials();
    QVector<ThreeBodyFunction> forces = angleparams.forces(this->symbols()
                                                            .angle().theta());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<ThreeBodyFunction> > group_potentials;
    QHash< CGIDQuad, QVector<ThreeBodyFunction> > group_forces;
    
    const ThreeBodyFunction *potentials_array = potential.constData();
    const ThreeBodyFunction *forces_array = forces.constData();
    
    int n = potentials.count();
    BOOST_ASSERT(n == forces.count();
    
    for (int i=0; i<npotentials; ++i)
    {
        const ThreeBodyFunction &potential = potentials_array[i]
        const ThreeBodyFunction &force = forces_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup());
                        
        BOOST_ASSERT( idquad == CGIDQuad(force.atom0().cutGroup(),
                                         force.atom1().cutGroup(),
                                         force.atom2().cutGroup()) );
                                         
        group_potentials[idquad].append(potential);
        group_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< IDQuad,QVector<ThreeBodyFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<ThreeBodyFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->addGroup(it.key(), cached_groups)
                        .setAnglePotential( it.value(), group_force );
    }
}

/** This adds all of the dihedral parameters and forces in 'dihedralparams'
    to the list of parameters */
void InternalParameters::addDihedrals(const FourAtomFunctions &dihedraparams,
                                      QHash<CGIDQuad,qint32> &cached_groups)
{
    //assert that these are functions only of the torsion (phi)
    this->assertContainsOnly(this->symbols().dihedral(), dihedralparams.symbols());
    
    //get the potential and forces
    QVector<FourBodyFunction> potentials = dihedralparams.potentials();
    QVector<FourBodyFunction> forces = dihedralparams.forces(this->symbols()
                                                            .dihedral().phi());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<FourBodyFunction> > group_potentials;
    QHash< CGIDQuad, QVector<FourBodyFunction> > group_forces;
    
    const FourBodyFunction *potentials_array = potential.constData();
    const FourBodyFunction *forces_array = forces.constData();
    
    int n = potentials.count();
    BOOST_ASSERT(n == forces.count();
    
    for (int i=0; i<npotentials; ++i)
    {
        const FourBodyFunction &potential = potentials_array[i]
        const FourBodyFunction &force = forces_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup(),
                        potential.atom3().cutGroup());
                        
        BOOST_ASSERT( idquad == CGIDQuad(force.atom0().cutGroup(),
                                         force.atom1().cutGroup(),
                                         force.atom2().cutGroup(),
                                         force.atom3().cutGroup()) );
                                         
        group_potentials[idquad].append(potential);
        group_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< IDQuad,QVector<FourBodyFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<FourBodyFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->addGroup(it.key(), cached_groups)
                        .setDihedralPotential( it.value(), group_force );
    }
}

/** This adds all of the improper parameters and forces in 'improperparams'
    to the list of parameters */
void InternalParameters::addImpropers(const FourAtomFunctions &improperparams,
                                      QHash<CGIDQuad,qint32> &cached_groups)
{
    //assert that these are functions only of the angle (theta or phi)
    this->assertContainsOnly(this->symbols().improper(), improperparams.symbols());
    
    //get the potential and forces
    QVector<FourBodyFunction> potentials = improperparams.potentials();
    QVector<FourBodyFunction> theta_forces = improperparams.forces(this->symbols()
                                                            .improper().theta());
    QVector<FourBodyFunction> phi_forces = improperparams.forces(this->symbols()
                                                            .improper().phi());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<ThreeBodyFunction> > group_potentials;
    QHash< CGIDQuad, QVector<ThreeBodyFunction> > group_forces;
    
    const ThreeBodyFunction *potentials_array = potential.constData();
    const ThreeBodyFunction *forces_array = forces.constData();
    
    int n = potentials.count();
    BOOST_ASSERT(n == forces.count();
    
    for (int i=0; i<npotentials; ++i)
    {
        const ThreeBodyFunction &potential = potentials_array[i]
        const ThreeBodyFunction &force = forces_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup());
                        
        BOOST_ASSERT( idquad == CGIDQuad(force.atom0().cutGroup(),
                                         force.atom1().cutGroup(),
                                         force.atom2().cutGroup()) );
                                         
        group_potentials[idquad].append(potential);
        group_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< IDQuad,QVector<ThreeBodyFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<ThreeBodyFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->addGroup(it.key(), cached_groups)
                        .setAnglePotential( it.value(), group_force );
    }
}

/** Construct the parameters from the specified properties of 
    the molecule 'molecule'. If 'isstrict' is true, then this
    includes only internals that are wholely contained within
    the selected atoms of the molecule. Otherwise, this contains
    internals from which at least one atom is in the selected
    atoms of the molecule */
InternalParameters::InternalParameters(const PartialMolecule &molecule,
                                       const PropertyName &bond_params,
                                       const PropertyName &angle_params,
                                       const PropertyName &dihedral_params,
                                       const PropertyName &improper_params,
                                       const PropertyName &ub_params,
                                       const PropertyName &ss_params,
                                       const PropertyName &sb_params,
                                       const PropertyName &bb_params,
                                       const PropertyName &sbt_params,
                                       bool isstrict
                                       )
{
    QHash<IDQuad,qint32> cached_groups;
    
    if (not bond_params.isNull())
    {
        const TwoAtomFunctions &bondparams = molecule.property(bond_params)
                                                        ->asA<TwoAtomFunctions>();

        if (molecule.selection().selectedAll())
            this->addBonds(bondparams, cached_groups);
        else
            this->addBonds(bondparams.includeOnly(molecule.selection(), isstrict),
                           cached_groups);
    }
    
    if (not angle_params.isNull())
    {
        const ThreeAtomFunctions &angleparams = molecule.property(angle_params)
                                                        ->asA<ThreeAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addAngless(angleparams, cached_groups);
        else
            this->addAngles(angleparams.includeOnly(molecule.selection(), isstrict),
                            cached_groups);
    }
    
    if (not dihedral_params.isNull())
    {
        const FourAtomFunctions &dihedralparams = molecule.property(dihedral_params)
                                                        ->asA<FourAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addDihedrals(dihedralparams, cached_groups);
        else
            this->addDihedrals(dihedralparams.includeOnly(molecule.selection(), 
                                                          isstrict), cached_groups);
    }
    
    if (not improper_params.isNull())
    {
        const FourAtomFunctions &improperparams = molecule.property(improper_params)
                                                        ->asA<FourAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addImpropers(improperparams, cached_groups);
        else
            this->addImpropers(improperparams.includeOnly(molecule.selection(), 
                                                          isstrict), cached_groups);
    }
    
    if (not ub_params.isNull())
    {
        const TwoAtomFunctions &ubparams = molecule.property(ub_params)
                                                        ->asA<TwoAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addUBs(ubparams, cached_groups);
        else
            this->addUBs(ubparams.includeOnly(molecule.selection(), isstrict),
                         cached_groups);
    }
    
    if (not ss_params.isNull())
    {
        const ThreeAtomFunctions &ssparams = molecule.property(ss_params)
                                                        ->asA<ThreeAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addSSs(ssparams, cached_groups);
        else
            this->addSSs(ssparams.includeOnly(molecule.selection(), isstrict),
                         cached_groups);
    }
    
    if (not sb_params.isNull())
    {
        const ThreeAtomFunctions &sbparams = molecule.property(sb_params)
                                                        ->asA<ThreeAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addSBs(sbparams, cached_groups);
        else
            this->addSBs(sbparams.includeOnly(molecule.selection(), isstrict),
                         cached_groups);
    }
        
    if (not bb_params.isNull())
    {
        const FourAtomFunctions &bbparams = molecule.property(bb_params)
                                                        ->asA<FourAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addBBs(bbparams, cached_groups);
        else
            this->addBBs(bbparams.includeOnly(molecule.selection(), isstrict),
                         cached_groups);
    }
        
    if (not sbt_params.isNull())
    {
        const FourAtomFunctions &sbtparams = molecule.property(sbt_params)
                                                        ->asA<FourAtomFunctions>();
                                                        
        if (molecule.selection().selectedAll())
            this->addSBTs(sbtparams, cached_groups);
        else
            this->addSBTs(sbtparams.includeOnly(molecule.selection(), isstrict),
                          cached_groups);
    }

}

InternalParameters::InternalParameters(const InternalParameters &other);

InternalParameters::~InternalParameters();

InternalParameters& InternalParameters::operator=(const InternalParameters &other);

bool InternalParameters::operator==(const InternalParameters &other) const;
bool InternalParameters::operator!=(const InternalParameters &other) const;

bool InternalParameters::changedAllGroups(const InternalParameters &other) const;

void InternalParameters::addChangedGroups(const InternalParameters &other, 
                      const QSet<CGIdx> &changed_groups) const;
                      
QSet<CGIdx> InternalParameters::getChangedGroups(const InternalParameters &other) const;

InternalParameters InternalParameters::includeOnly(const AtomSelection &selected_atoms) const;

InternalParameters InternalParameters::applyMask(const QSet<CGIdx> &cgidxs) const;

const QVector<GroupInternalParameters>& InternalParameters::groupParameters() const;

QVector<GroupInternalParameters> InternalParameters::groupParameters(CGIdx cgidx) const;
QVector<GroupInternalParameters> InternalParameters::groupParameters(const QSet<CGIdx> &cgidxs) const;
