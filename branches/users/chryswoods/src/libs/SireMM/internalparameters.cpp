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

#include "SireMol/partialmolecule.h"

using namespace SireMM;
using namespace SireMM::detail;

using namespace SireBase;
using namespace SireCAS;
using namespace SireMol;

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
    QHash< CGIdx, QSet<qint32> >::const_iterator 
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
    QHash< CGIdx, QSet<qint32> >::const_iterator 
                                    it0 = groups_by_cgidx.constFind(cgidx0);

    QHash< CGIdx, QSet<qint32> >::const_iterator 
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
    QHash< CGIdx, QSet<qint32> >::const_iterator 
                                    it0 = groups_by_cgidx.constFind(cgidx0);

    QHash< CGIdx, QSet<qint32> >::const_iterator 
                                    it1 = groups_by_cgidx.constFind(cgidx1);

    QHash< CGIdx, QSet<qint32> >::const_iterator 
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
    QHash< CGIdx, QSet<qint32> >::const_iterator 
                                    it0 = groups_by_cgidx.constFind(cgidx0);

    QHash< CGIdx, QSet<qint32> >::const_iterator 
                                    it1 = groups_by_cgidx.constFind(cgidx1);

    QHash< CGIdx, QSet<qint32> >::const_iterator 
                                    it2 = groups_by_cgidx.constFind(cgidx2);

    QHash< CGIdx, QSet<qint32> >::const_iterator 
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

static const GroupInternalParameters shared_empty_group_params;

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
InternalParameters::getGroup(const CGIDQuad &idquad, 
                             QHash<CGIDQuad,qint32> &cached_groups)
{
    qint32 i = cached_groups.value(idquad, -1);
    
    if (i < 0)
    {
        i = this->addGroup(idquad);

        cached_groups.insert(idquad, i);
    }
    
    return group_params.data()[i];
}

/** This adds all of the bond parameters and forces in 'bondparams'
    to the list of parameters */
void InternalParameters::addBonds(const TwoAtomFunctions &bondparams,
                                  QHash<CGIDQuad,qint32> &cached_groups)
{
    if (bondparams.isEmpty())
        return;

    //assert that these are functions only of the bond length
    this->assertContainsOnly(this->symbols().bond(), bondparams.symbols());
    
    //get the potential and forces
    QVector<TwoAtomFunction> potentials = bondparams.potentials();
    QVector<TwoAtomFunction> forces = bondparams.forces(this->symbols().bond().r());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<TwoAtomFunction> > group_potentials;
    QHash< CGIDQuad, QVector<TwoAtomFunction> > group_forces;
    
    const TwoAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const TwoAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    //do forces separately as not all potentials may have a force
    //(as potential could be constant with r)
    const TwoAtomFunction *forces_array = forces.constData();
    n = forces.count();

    for (int i=0; i<n; ++i)
    {
        const TwoAtomFunction &force = forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup());
                        
        group_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<TwoAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<TwoAtomFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setBondPotential( it.value(), group_force );
    }
}

/** This adds all of the angle parameters and forces in 'angleparams'
    to the list of parameters */
void InternalParameters::addAngles(const ThreeAtomFunctions &angleparams,
                                   QHash<CGIDQuad,qint32> &cached_groups)
{
    if (angleparams.isEmpty())
        return;

    //assert that these are functions only of the angle (theta)
    this->assertContainsOnly(this->symbols().angle(), angleparams.symbols());
    
    //get the potential and forces
    QVector<ThreeAtomFunction> potentials = angleparams.potentials();
    QVector<ThreeAtomFunction> forces = angleparams.forces(this->symbols()
                                                            .angle().theta());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_potentials;
    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_forces;
    
    const ThreeAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    const ThreeAtomFunction *forces_array = forces.constData();
    n = forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &force = forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup());
                        
        group_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<ThreeAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<ThreeAtomFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setAnglePotential( it.value(), group_force );
    }
}

/** This adds all of the dihedral parameters and forces in 'dihedralparams'
    to the list of parameters */
void InternalParameters::addDihedrals(const FourAtomFunctions &dihedralparams,
                                      QHash<CGIDQuad,qint32> &cached_groups)
{
    if (dihedralparams.isEmpty())
        return;

    //assert that these are functions only of the torsion (phi)
    this->assertContainsOnly(this->symbols().dihedral(), dihedralparams.symbols());
    
    //get the potential and forces
    QVector<FourAtomFunction> potentials = dihedralparams.potentials();
    QVector<FourAtomFunction> forces = dihedralparams.forces(this->symbols()
                                                            .dihedral().phi());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_potentials;
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_forces;
    
    const FourAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup(),
                        potential.atom3().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    const FourAtomFunction *forces_array = forces.constData();
    n = forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_forces[idquad].append(force);                        
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<FourAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<FourAtomFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setDihedralPotential( it.value(), group_force );
    }
}

/** This adds all of the improper parameters and forces in 'improperparams'
    to the list of parameters */
void InternalParameters::addImpropers(const FourAtomFunctions &improperparams,
                                      QHash<CGIDQuad,qint32> &cached_groups)
{
    if (improperparams.isEmpty())
        return;

    //assert that these are functions only of the angle (theta or phi)
    this->assertContainsOnly(this->symbols().improper(), improperparams.symbols());
    
    //get the potential and forces
    QVector<FourAtomFunction> potentials = improperparams.potentials();
    
    QVector<FourAtomFunction> theta_forces = improperparams.forces(this->symbols()
                                                            .improper().theta());
    QVector<FourAtomFunction> phi_forces = improperparams.forces(this->symbols()
                                                            .improper().phi());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_potentials;
    
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_theta_forces;
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_phi_forces;
    
    const FourAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup(),
                        potential.atom3().cutGroup());
                        
        group_potentials[idquad].append(potential);
    }

    const FourAtomFunction *theta_forces_array = theta_forces.constData();
    n = theta_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = theta_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_theta_forces[idquad].append(force);
    }
    
    const FourAtomFunction *phi_forces_array = phi_forces.constData();
    n = phi_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = phi_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_phi_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<FourAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<FourAtomFunction> &group_theta_force = group_theta_forces[it.key()];
        QVector<FourAtomFunction> &group_phi_force = group_phi_forces[it.key()];
        
        group_theta_force.squeeze();
        group_phi_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setImproperPotential( it.value(), 
                                               group_theta_force, group_phi_force );
    }
}

/** This adds all of the bond parameters and forces in 'bondparams'
    to the list of parameters */
void InternalParameters::addUBs(const TwoAtomFunctions &ubparams,
                                QHash<CGIDQuad,qint32> &cached_groups)
{
    if (ubparams.isEmpty())
        return;

    //assert that these are functions only of the bond length
    this->assertContainsOnly(this->symbols().ureyBradley(), ubparams.symbols());
    
    //get the potential and forces
    QVector<TwoAtomFunction> potentials = ubparams.potentials();
    QVector<TwoAtomFunction> forces = ubparams.forces(this->symbols()
                                                    .ureyBradley().r());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<TwoAtomFunction> > group_potentials;
    QHash< CGIDQuad, QVector<TwoAtomFunction> > group_forces;
    
    const TwoAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const TwoAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    //do forces separately as not all potentials may have a force
    //(as potential could be constant with r)
    const TwoAtomFunction *forces_array = forces.constData();
    n = forces.count();

    for (int i=0; i<n; ++i)
    {
        const TwoAtomFunction &force = forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup());
                        
        group_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<TwoAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<TwoAtomFunction> &group_force = group_forces[it.key()];
        group_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setUreyBradleyPotential( it.value(), group_force );
    }
}

/** This adds all of the stretch-stretch parameters and forces in 'ssparams'
    to the list of parameters */
void InternalParameters::addSSs(const ThreeAtomFunctions &ssparams,
                                QHash<CGIDQuad,qint32> &cached_groups)
{
    if (ssparams.isEmpty())
        return;

    //assert that these are functions only of the angle (theta)
    this->assertContainsOnly(this->symbols().stretchStretch(), 
                             ssparams.symbols());
    
    //get the potential and forces
    QVector<ThreeAtomFunction> potentials = ssparams.potentials();

    QVector<ThreeAtomFunction> r01_forces = ssparams.forces(this->symbols()
                                                      .stretchStretch().r01());
    QVector<ThreeAtomFunction> r21_forces = ssparams.forces(this->symbols()
                                                      .stretchStretch().r21());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_potentials;

    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_r01_forces;
    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_r21_forces;
    
    const ThreeAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    const ThreeAtomFunction *r01_forces_array = r01_forces.constData();
    n = r01_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &force = r01_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup());
                        
        group_r01_forces[idquad].append(force);
    }

    const ThreeAtomFunction *r21_forces_array = r21_forces.constData();
    n = r21_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &force = r21_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup());
                        
        group_r21_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<ThreeAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<ThreeAtomFunction> &group_r01_force = group_r01_forces[it.key()];
        QVector<ThreeAtomFunction> &group_r21_force = group_r21_forces[it.key()];
        
        group_r01_force.squeeze();
        group_r21_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setStretchStretchPotential( it.value(), 
                                            group_r01_force, group_r21_force );
    }
}

/** This adds all of the stretch-bend parameters and forces in 'sbparams'
    to the list of parameters */
void InternalParameters::addSBs(const ThreeAtomFunctions &sbparams,
                                QHash<CGIDQuad,qint32> &cached_groups)
{
    if (sbparams.isEmpty())
        return;

    //assert that these are functions only of the angle (theta)
    this->assertContainsOnly(this->symbols().stretchBend(), 
                             sbparams.symbols());
    
    //get the potential and forces
    QVector<ThreeAtomFunction> potentials = sbparams.potentials();

    QVector<ThreeAtomFunction> theta_forces = sbparams.forces(this->symbols()
                                                      .stretchBend().theta());
    QVector<ThreeAtomFunction> r01_forces = sbparams.forces(this->symbols()
                                                      .stretchBend().r01());
    QVector<ThreeAtomFunction> r21_forces = sbparams.forces(this->symbols()
                                                      .stretchBend().r21());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_potentials;

    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_theta_forces;
    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_r01_forces;
    QHash< CGIDQuad, QVector<ThreeAtomFunction> > group_r21_forces;
    
    const ThreeAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    const ThreeAtomFunction *theta_forces_array = theta_forces.constData();
    n = theta_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &force = theta_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup());
                        
        group_theta_forces[idquad].append(force);
    }

    const ThreeAtomFunction *r01_forces_array = r01_forces.constData();
    n = r01_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &force = r01_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup());
                        
        group_r01_forces[idquad].append(force);
    }

    const ThreeAtomFunction *r21_forces_array = r21_forces.constData();
    n = r21_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const ThreeAtomFunction &force = r21_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup());
                        
        group_r21_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<ThreeAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<ThreeAtomFunction> &group_theta_force = group_theta_forces[it.key()];
        QVector<ThreeAtomFunction> &group_r01_force = group_r01_forces[it.key()];
        QVector<ThreeAtomFunction> &group_r21_force = group_r21_forces[it.key()];
        
        group_theta_force.squeeze();
        group_r01_force.squeeze();
        group_r21_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setStretchBendPotential( it.value(), group_theta_force,
                                            group_r01_force, group_r21_force );
    }
}

/** This adds all of the bend-bend parameters and forces in 'bbparams'
    to the list of parameters */
void InternalParameters::addBBs(const FourAtomFunctions &bbparams,
                                QHash<CGIDQuad,qint32> &cached_groups)
{
    if (bbparams.isEmpty())
        return;

    //assert that these are functions only of the angle (theta)
    this->assertContainsOnly(this->symbols().bendBend(), 
                             bbparams.symbols());
    
    //get the potential and forces
    QVector<FourAtomFunction> potentials = bbparams.potentials();

    QVector<FourAtomFunction> theta012_forces = bbparams.forces(this->symbols()
                                                      .bendBend().theta012());
    QVector<FourAtomFunction> theta213_forces = bbparams.forces(this->symbols()
                                                      .bendBend().theta213());
    QVector<FourAtomFunction> theta310_forces = bbparams.forces(this->symbols()
                                                      .bendBend().theta310());
    
    //sort the internals into groups
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_potentials;

    QHash< CGIDQuad, QVector<FourAtomFunction> > group_theta012_forces;
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_theta213_forces;
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_theta310_forces;
    
    const FourAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup(),
                        potential.atom3().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    const FourAtomFunction *theta012_forces_array = theta012_forces.constData();
    n = theta012_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = theta012_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_theta012_forces[idquad].append(force);
    }

    const FourAtomFunction *theta213_forces_array = theta213_forces.constData();
    n = theta213_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = theta213_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_theta213_forces[idquad].append(force);
    }

    const FourAtomFunction *theta310_forces_array = theta310_forces.constData();
    n = theta310_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = theta310_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_theta310_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<FourAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<FourAtomFunction> &group_theta012_force 
                                        = group_theta012_forces[it.key()];
        QVector<FourAtomFunction> &group_theta213_force 
                                        = group_theta213_forces[it.key()];
        QVector<FourAtomFunction> &group_theta310_force 
                                        = group_theta310_forces[it.key()];
        
        group_theta012_force.squeeze();
        group_theta213_force.squeeze();
        group_theta310_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setBendBendPotential( it.value(), group_theta012_force,
                                        group_theta213_force, group_theta310_force );
    }
}

/** This adds all of the stretch-bend-torsion parameters and forces in 'sbtparams'
    to the list of parameters */
void InternalParameters::addSBTs(const FourAtomFunctions &sbtparams,
                                QHash<CGIDQuad,qint32> &cached_groups)
{
    if (sbtparams.isEmpty())
        return;

    //assert that these are functions only of the angle (theta)
    this->assertContainsOnly(this->symbols().stretchBendTorsion(), 
                             sbtparams.symbols());
    
    //get the potential and forces
    QVector<FourAtomFunction> potentials = sbtparams.potentials();

    QVector<FourAtomFunction> phi_forces = sbtparams.forces(this->symbols()
                                                  .stretchBendTorsion().phi());

    QVector<FourAtomFunction> theta012_forces = sbtparams.forces(this->symbols()
                                                  .stretchBendTorsion().theta012());
    QVector<FourAtomFunction> theta321_forces = sbtparams.forces(this->symbols()
                                                  .stretchBendTorsion().theta321());

    QVector<FourAtomFunction> r01_forces = sbtparams.forces(this->symbols()
                                                  .stretchBendTorsion().r01());
    QVector<FourAtomFunction> r12_forces = sbtparams.forces(this->symbols()
                                                  .stretchBendTorsion().r12());
    QVector<FourAtomFunction> r32_forces = sbtparams.forces(this->symbols()
                                                  .stretchBendTorsion().r32());

    //sort the internals into groups
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_potentials;

    QHash< CGIDQuad, QVector<FourAtomFunction> > group_phi_forces;

    QHash< CGIDQuad, QVector<FourAtomFunction> > group_theta012_forces;
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_theta321_forces;

    QHash< CGIDQuad, QVector<FourAtomFunction> > group_r01_forces;
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_r12_forces;
    QHash< CGIDQuad, QVector<FourAtomFunction> > group_r32_forces;
    
    const FourAtomFunction *potentials_array = potentials.constData();
    int n = potentials.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &potential = potentials_array[i];
    
        CGIDQuad idquad(potential.atom0().cutGroup(),
                        potential.atom1().cutGroup(),
                        potential.atom2().cutGroup(),
                        potential.atom3().cutGroup());
                                         
        group_potentials[idquad].append(potential);
    }

    const FourAtomFunction *phi_forces_array = phi_forces.constData();
    n = phi_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = phi_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_phi_forces[idquad].append(force);
    }

    const FourAtomFunction *theta012_forces_array = theta012_forces.constData();
    n = theta012_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = theta012_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_theta012_forces[idquad].append(force);
    }

    const FourAtomFunction *theta321_forces_array = theta321_forces.constData();
    n = theta321_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = theta321_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_theta321_forces[idquad].append(force);
    }

    const FourAtomFunction *r01_forces_array = r01_forces.constData();
    n = r01_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = r01_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_r01_forces[idquad].append(force);
    }

    const FourAtomFunction *r12_forces_array = r12_forces.constData();
    n = r12_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = r12_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_r12_forces[idquad].append(force);
    }

    const FourAtomFunction *r32_forces_array = r32_forces.constData();
    n = r32_forces.count();
    
    for (int i=0; i<n; ++i)
    {
        const FourAtomFunction &force = r32_forces_array[i];
        
        CGIDQuad idquad(force.atom0().cutGroup(),
                        force.atom1().cutGroup(),
                        force.atom2().cutGroup(),
                        force.atom3().cutGroup());
                        
        group_r32_forces[idquad].append(force);
    }
    
    //add the groups into this set
    for (QHash< CGIDQuad,QVector<FourAtomFunction> >::iterator
                                        it = group_potentials.begin();
         it != group_potentials.end();
         ++it)
    {
        it.value().squeeze();
        
        QVector<FourAtomFunction> &group_phi_force = group_phi_forces[it.key()];
        
        QVector<FourAtomFunction> &group_theta012_force 
                                        = group_theta012_forces[it.key()];
        QVector<FourAtomFunction> &group_theta321_force 
                                        = group_theta321_forces[it.key()];

        QVector<FourAtomFunction> &group_r01_force = group_r01_forces[it.key()];
        QVector<FourAtomFunction> &group_r12_force = group_r12_forces[it.key()];
        QVector<FourAtomFunction> &group_r32_force = group_r32_forces[it.key()];
        
        group_phi_force.squeeze();

        group_theta012_force.squeeze();
        group_theta321_force.squeeze();

        group_r01_force.squeeze();
        group_r12_force.squeeze();
        group_r32_force.squeeze();
        
        this->getGroup(it.key(), cached_groups)
                        .setStretchBendTorsionPotential( it.value(), 
                                        group_phi_force,
                                        group_theta012_force, group_theta321_force,
                                        group_r01_force, group_r12_force,
                                        group_r32_force );
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
    QHash<CGIDQuad,qint32> cached_groups;
    
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
            this->addAngles(angleparams, cached_groups);
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
