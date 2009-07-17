/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "identityconstraint.h"
#include "system.h"

#include "SireMaths/linearap.h"

#include "SireMol/molecules.h"
#include "SireMol/molecule.h"
#include "SireMol/moleditor.h"
#include "SireMol/viewsofmol.h"

#include "SireVol/space.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireSystem;
using namespace SireFF;
using namespace SireMol;
using namespace SireBase;
using namespace SireMaths;
using namespace SireStream;

namespace SireSystem
{
namespace detail
{

class NullIdentityConstraintHelper;

/** This is the virtual base class of the IdentityConstraint helper
    classes - these allow the IdentityConstraint to use different
    strategies to solve the constraint depending on the ratio
    of the number of points to the number of molecules */
class IdentityConstraintPvt : public QSharedData
{
public:
    IdentityConstraintPvt();
    
    IdentityConstraintPvt(const MoleculeGroup &molgroup,
                          const PointRef &point,
                          const PropertyMap &map);
    
    IdentityConstraintPvt(const MoleculeGroup &molgroup,
                          const QVector<PointPtr> &points,
                          const PropertyMap &map);
    
    IdentityConstraintPvt(const IdentityConstraintPvt &other);
    
    virtual ~IdentityConstraintPvt();
    
    static const char* typeName()
    {
        return "SireSystem::detail::IdentityConstraintPvt";
    }
    
    virtual const char* what() const=0;
    
    virtual IdentityConstraintPvt* clone() const=0;
    
    virtual Molecules update(const System &system, bool new_system)=0;
    virtual Molecules update(const System &system, MolNum changed_mol,
                             bool new_system)=0;
    virtual Molecules update(const System &system, const Molecules &molecules,
                             bool new_system)=0;

    const MolGroupPtr& molGroupPtr() const;

    const MoleculeGroup& moleculeGroup() const;
    
    const QVector<PointPtr>& points() const;
    
    const PropertyMap& propertyMap() const;

    const Space& space() const;

    static const NullIdentityConstraintHelper& null();

protected:
    bool updatePoints(const System &system);
    bool updateGroup(const System &system);
    bool updateSpace(const System &system);

    void validateGroup(const MoleculeGroup &new_group) const;

    /** The molecule group containing the molecules whose identities
        are being constrained */
    MolGroupPtr molgroup;
    
    /** The set of points that provide the locations that
        identify the molecules. These 'n' points constrain
        the identity of the first 'n' molecule in 'molgroup'
        (with point 'i' constraining the identity of 
        molecule 'i') */
    QVector<PointPtr> identity_points;

    /** The space being used to calculate the distances */
    SpacePtr spce;

    /** The property map used to find the properties used
        by this constraint */
    PropertyMap map;
};

/** This is the null identity constraint helper used when
    there are no points against which the constraint is applied
    
    @author Christopher Woods
*/
class NullIdentityConstraintHelper : public IdentityConstraintPvt
{
public:
    NullIdentityConstraintHelper();
    
    NullIdentityConstraintHelper(const NullIdentityConstraintHelper &other);
    
    ~NullIdentityConstraintHelper();
    
    static const char* typeName()
    {
        return "SireSystem::detail::NullIdentityConstraintHelper";
    }
    
    const char* what() const;
    
    NullIdentityConstraintHelper* clone() const;

    Molecules update(const System &system, bool new_system);
    Molecules update(const System &system, MolNum changed_mol, bool new_system);
    Molecules update(const System &system, const Molecules &molecules, bool new_system);
};

/** This is the IdentityConstraint helper class that is used
    when the number of points is comparable to the number
    of molecules (or if the points are widely dispersed)
    
    In these cases it is best just to calculate all of the 
    distances to all of the points all of the time
    
    @author Christopher Woods
*/
class ManyPointsHelper : public IdentityConstraintPvt
{
public:
    ManyPointsHelper();
    
    ManyPointsHelper(const MoleculeGroup &molgroup,
                     const PropertyMap &map);
    
    ManyPointsHelper(const MoleculeGroup &molgroup,
                     const QVector<PointPtr> &points,
                     const PropertyMap &map);
                               
    ManyPointsHelper(const ManyPointsHelper &other);
    
    ~ManyPointsHelper();
    
    static const char* typeName()
    {
        return "SireSystem::detail::ManyPointsHelper";
    }
    
    const char* what() const;
    
    ManyPointsHelper* clone() const;

    Molecules update(const System &system, bool new_system);
    Molecules update(const System &system, MolNum changed_mol, bool new_system);
    Molecules update(const System &system, const Molecules &molecules, bool new_system);

private:
    /** The (squared) distances between all molecules and all points */
    QHash< MolNum, QVector<double> > point_distances;
};

/** This is the identity constraint helper used when the number
    of points is significantly less than the number of molecules,
    and the points are close together. In this case, the 
    constraint can be simplified by only calculating distances
    between molecules which are close to the points */
class FewPointsHelper : public IdentityConstraintPvt
{
public:
    FewPointsHelper();
    FewPointsHelper(const MoleculeGroup &molgroup,
                    const QVector<PointPtr> &points,
                    const PropertyMap &map);
                               
    FewPointsHelper(const FewPointsHelper &other);
    
    ~FewPointsHelper();
    
    static const char* typeName()
    {
        return "SireSystem::detail::FewPointsHelper";
    }
    
    const char* what() const;
    
    FewPointsHelper* clone() const;

    Molecules update(const System &system, bool new_system);
    Molecules update(const System &system, MolNum changed_mol, bool new_system);
    Molecules update(const System &system, const Molecules &molecules, bool new_system);
};

/** This is the identity constraint helper class used when there
    is only a single point against which the molecule is constrained.
    This is the easiest case, as the constraint just has to look
    for the closest molecule */
class SinglePointHelper : public IdentityConstraintPvt
{
public:
    SinglePointHelper();
    
    SinglePointHelper(const MoleculeGroup &molgroup,
                      const PointRef &point,
                      const PropertyMap &map);
    
    SinglePointHelper(const MoleculeGroup &molgroup,
                      const QVector<PointPtr> &points,
                      const PropertyMap &map);
                               
    SinglePointHelper(const SinglePointHelper &other);
    
    ~SinglePointHelper();
    
    static const char* typeName()
    {
        return "SireSystem::detail::SinglePointHelper";
    }
    
    const char* what() const;
    
    SinglePointHelper* clone() const;

    Molecules update(const System &system, bool new_system);
    Molecules update(const System &system, MolNum changed_mol, bool new_system);
    Molecules update(const System &system, const Molecules &molecules, bool new_system);

private:
    void recalculateDistances();
    void recalculateDistances(MolNum changed_mol);
    void recalculateDistances(const Molecules &molecules);
    
    Molecules applyConstraint() const;

    /** The number of the current molecule closest to the point */
    MolNum closest_molnum;
    
    /** The distance^2 between the closest molecule and the point */
    double closest_distance2;
};


} // end of namespace detail
} // end of namespace SireSystem

using namespace SireSystem::detail;

/////////
///////// Implementation of IdentityConstraintPvt
/////////

void IdentityConstraintPvt::validateGroup(const MoleculeGroup &new_group) const
{
    //run through each molecule and ensure that the coordinate properties
    //of them all are compatible
    
    if (new_group.nMolecules() < 2)
        return;
    
    const PropertyName &coords_property = map["coordinates"];
    
    const ViewsOfMol &first_mol = new_group.moleculeAt(0);
    
    const AtomCoords &coords = first_mol.data().property(coords_property)
                                               .asA<AtomCoords>();
    
    for ( Molecules::const_iterator it = new_group.constBegin(); 
          it != new_group.constEnd(); 
          ++it )
    {
        if (not coords.isCompatibleWith(it->data().info()))
            throw SireError::incompatible_error( QObject::tr(
                    "The coordinates property (%1) of molecule number %2 "
                    "is not compatible with that of molecule number %3. "
                    "Only molecules with completely compatible coordinates "
                    "properties can be used in an IdentityConstraint.")
                        .arg(coords_property.toString())
                        .arg(first_mol.number())
                        .arg(it.key()), CODELOC );
    }
}

/** Constructor */
IdentityConstraintPvt::IdentityConstraintPvt() : QSharedData()
{}

/** Constructor */
IdentityConstraintPvt::IdentityConstraintPvt(const MoleculeGroup &group,
                                             const QVector<PointPtr> &points,
                                             const PropertyMap &property_map)
                      : QSharedData(),
                        molgroup(group), identity_points(points), map(property_map)
{
    this->validateGroup(group);
    identity_points.squeeze();
}

/** Constructor */
IdentityConstraintPvt::IdentityConstraintPvt(const MoleculeGroup &group,
                                             const PointRef &point,
                                             const PropertyMap &property_map)
                      : QSharedData(),
                        molgroup(group), map(property_map)
{
    this->validateGroup(group);
    identity_points.append( PointPtr(point) );
    identity_points.squeeze();
}

/** Copy constructor */
IdentityConstraintPvt::IdentityConstraintPvt(const IdentityConstraintPvt &other)
                      : QSharedData(),
                        molgroup(other.molgroup), 
                        identity_points(other.identity_points), 
                        spce(other.spce), map(other.map)
{}

/** Destructor */
IdentityConstraintPvt::~IdentityConstraintPvt()
{}

/** Return the shared pointer to the molecule group */
const MolGroupPtr& IdentityConstraintPvt::molGroupPtr() const
{
    return molgroup;
}

/** Return the molecule group operated on by this constraint */
const MoleculeGroup& IdentityConstraintPvt::moleculeGroup() const
{
    return molgroup.read();
}

/** Return the points used to identify the molecules */
const QVector<PointPtr>& IdentityConstraintPvt::points() const
{
    return identity_points;
}

/** Return the space used to calculate distances between the molecules
    and the identity points */
const Space& IdentityConstraintPvt::space() const
{
    return spce.read();
}

/** Return the property map used to find the properties required
    by this constraint */
const PropertyMap& IdentityConstraintPvt::propertyMap() const
{
    return map;
}

/** Update the space used to calculate the distances between
    the points and the molecules - this returns whether or 
    not the space has changed */
bool IdentityConstraintPvt::updateSpace(const System &system)
{
    const Space &new_space = system.property(map["space"]).asA<Space>();
    
    if (spce != new_space)
    {
        spce = new_space;
        return true;
    }
    else
        return false;
}

/** Update the points in this constraint from the passed system - 
    this returns whether or not this changes any points */
bool IdentityConstraintPvt::updatePoints(const System &system)
{
    int npoints = identity_points.count();
    
    const PointPtr *const_points_array = identity_points.constData();
    
    bool need_update = false;
    
    for (int i=0; i<npoints; ++i)
    {
        if (const_points_array[i].read().usesMoleculesIn(system))
        {
            need_update = true;
            break;
        }
    }
    
    if (need_update)
    {
        bool changed = false;
        
        PointPtr *points_array = identity_points.data();
        
        for (int i=0; i<npoints; ++i)
        {
            bool this_changed = points_array[i].edit().update(system);
            
            changed = changed or this_changed;
        }
        
        return changed;
    }
    else
        return false;
}

/** Update the molecule group whose molecules are constrained to equal
    the version from the passed system. Note that all of the molecules
    in the molecule group *must* have exactly the same atomic
    layout, as the coordinates are just straight swapped */
bool IdentityConstraintPvt::updateGroup(const System &system)
{
    const MoleculeGroup &old_group = molgroup.read();

    if (not system.contains(old_group.number()))
        return false;
        
    const MoleculeGroup &new_group = system[old_group.number()];
    
    if (new_group.version() == old_group.version())
        return false;
        
    else if (new_group.version().majorVersion() != old_group.version().majorVersion())
    {
        //the group's contents have changed - ensure that all
        //of the molecules are still compatible
        this->validateGroup(new_group);
    }
    
    molgroup = new_group;
    return true;
}

/////////
///////// Implementation of NullIdentityConstraintHelper
/////////

/** Constructor */
NullIdentityConstraintHelper::NullIdentityConstraintHelper()
                             : IdentityConstraintPvt()
{}

/** Copy constructor */
NullIdentityConstraintHelper::NullIdentityConstraintHelper(
                                        const NullIdentityConstraintHelper&)
                             : IdentityConstraintPvt()
{}

/** Destructor */
NullIdentityConstraintHelper::~NullIdentityConstraintHelper()
{}

const char* NullIdentityConstraintHelper::what() const
{
    return NullIdentityConstraintHelper::typeName();
}

NullIdentityConstraintHelper* NullIdentityConstraintHelper::clone() const
{
    return new NullIdentityConstraintHelper(*this);
}

Molecules NullIdentityConstraintHelper::update(const System&, bool)
{
    return Molecules();
}

Molecules NullIdentityConstraintHelper::update(const System&, MolNum, bool)
{
    return Molecules();
}

Molecules NullIdentityConstraintHelper::update(const System&, const Molecules&, bool)
{
    return Molecules();
}

Q_GLOBAL_STATIC( NullIdentityConstraintHelper, nullHelper );

/** Return the global null helper */
const NullIdentityConstraintHelper& IdentityConstraintPvt::null()
{
    return *(nullHelper());
}

/////////
///////// Implementation of SinglePointHelper
/////////

/** Constructor */
SinglePointHelper::SinglePointHelper() 
                  : IdentityConstraintPvt(),
                    closest_distance2( std::numeric_limits<double>::max() )
{}

/** Constructor */
SinglePointHelper::SinglePointHelper(const MoleculeGroup &molgroup,
                                     const PointRef &point,
                                     const PropertyMap &map)
                  : IdentityConstraintPvt(molgroup, point, map),
                    closest_distance2( std::numeric_limits<double>::max() )
{}

/** Constructor */
SinglePointHelper::SinglePointHelper(const MoleculeGroup &molgroup,
                                     const QVector<PointPtr> &points,
                                     const PropertyMap &map)
                  : IdentityConstraintPvt(molgroup, points, map),
                    closest_distance2( std::numeric_limits<double>::max() )
{
    if (points.count() > 1)
        throw SireError::program_bug( QObject::tr(
                "It is wrong to try and create a SinglePointHelper using "
                "%1 points!!!").arg(points.count()), CODELOC );
}

/** Copy constructor */
SinglePointHelper::SinglePointHelper(const SinglePointHelper &other)
                  : IdentityConstraintPvt(other),
                    closest_molnum(other.closest_molnum),
                    closest_distance2(other.closest_distance2)
{}

/** Destructor */
SinglePointHelper::~SinglePointHelper()
{}

const char* SinglePointHelper::what() const
{
    return SinglePointHelper::typeName();
}

SinglePointHelper* SinglePointHelper::clone() const
{
    return new SinglePointHelper(*this);
}

/** Recalulate all of the distances from scratch to find the closest
    molecule to the identity constraint point */
void SinglePointHelper::recalculateDistances()
{
    if (identity_points.isEmpty())
        return;

    //loop through every molecule in the group and calculate
    //its distance to the constraint point
    closest_distance2 = std::numeric_limits<double>::max();
    closest_molnum = MolNum();

    const MoleculeGroup &molgroup = this->moleculeGroup();
    
    const Vector &point = identity_points.at(0).read().point();
    
    const PropertyName &coords_property = map["coordinates"];
    
    for (Molecules::const_iterator it = molgroup.constBegin();
         it != molgroup.constEnd();
         ++it)
    {
        //get the coordinates property of this molecule
        const AtomCoords &coords = it->data().property(coords_property)
                                             .asA<AtomCoords>();
    
        double new_dist2 = space().calcDist2( point, coords.array().aaBox().center() );
        
        if (new_dist2 < closest_distance2)
        {
            closest_distance2 = new_dist2;
            closest_molnum = it.key();
        }
        else if (new_dist2 == closest_distance2)
        {
            //choose the molecule with the lowest number
            if (it.key() < closest_molnum)
            {
                closest_molnum = it.key();
            }
        }
    }
}

/** Recalculate the distances to find the identity constraint point,
    using the hint that only the molecule with number 'changed_mol' 
    has moved */
void SinglePointHelper::recalculateDistances(MolNum changed_mol)
{
    if (closest_molnum.isNull())
    {
        this->recalculateDistances();
        return;
    }

    if (not this->moleculeGroup().contains(changed_mol))
    {
        this->recalculateDistances();
        return;
    }

    //get the coordinates property of this molecule
    const PropertyName &coords_property = map["coordinates"];
    
    const ViewsOfMol &molecule = this->moleculeGroup()[changed_mol];
    
    const AtomCoords &coords = molecule.data().property(coords_property)
                                              .asA<AtomCoords>();

    const Vector &point = identity_points.at(0).read().point();

    double new_dist2 = space().calcDist2( point, coords.array().aaBox().center() );
    
    if (closest_molnum == changed_mol)
    {
        if (new_dist2 > closest_distance2)
            //we need to recalculate all of the distances
            this->recalculateDistances();
        else
            closest_distance2 = new_dist2;
    }
    else if (new_dist2 < closest_distance2)
    {
        closest_distance2 = new_dist2;
        closest_molnum = changed_mol;
    }
    else if (new_dist2 == closest_distance2)
    {
        //choose the molecule with the lowest molecule number
        if (changed_mol < closest_molnum)
            closest_molnum = changed_mol;
    }
}

/** Recalculate the distances to find the identity constraint point,
    using the hint that only the molecules in 'molecules' have changed */
void SinglePointHelper::recalculateDistances(const Molecules &molecules)
{
    if (closest_molnum.isNull())
    {
        this->recalculateDistances();
        return;
    }

    const PropertyName &coords_property = map["coordinates"];
    const Vector &point = identity_points.at(0).read().point();

    if (molecules.contains(closest_molnum))
    {
        //check the current closest molecule first, as if this
        //has moved away, then we need to recalculate from scratch

        if (not this->moleculeGroup().contains(closest_molnum))
        {
            this->recalculateDistances(system);
            return;
        }

        const ViewsOfMol &molecule = this->moleculeGroup()[closest_molnum];
    
        const AtomCoords &coords = molecule.data().property(coords_property)
                                                  .asA<AtomCoords>();

        double new_dist2 = space().calcDist2( point, coords.array().aaBox().center() );

        if (new_dist2 > closest_distance2)
        {
            this->recalculateDistances();
            return;
        }
        else
            closest_distance2 = new_dist2;
    }   

    MolNum old_closest_molnum = closest_molnum;

    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        if (it.key() == old_closest_molnum)
            continue;

        if (not this->moleculeGroup().contains(it.key()))
            continue;
    
        const ViewsOfMol &molecule = this->moleculeGroup()[it.key()];
    
        //get the coordinates property of this molecule
        const AtomCoords &coords = molecule.data().property(coords_property)
                                           .asA<AtomCoords>();

        double new_dist2 = space().calcDist2( point, coords.array().aaBox().center() );
    
        if (new_dist2 < closest_distance2)
        {
            closest_distance2 = new_dist2;
            closest_molnum = it.key();
        }
        else if (new_dist2 == closest_distance2)
        {
            //choose the molecule with the lowest molecule number
            if (it.key() < closest_molnum)
                closest_molnum = it.key();
        }
    }
}

/** Actually apply the constraint - if the first molecule is not
    the closest to the point, then this swaps the coordinates of 
    the closest molecule with the first molecule */
Molecules SinglePointHelper::applyConstraint() const
{
    if (this->moleculeGroup().isEmpty())
        return Molecules();
        
    else if (this->moleculeGroup().molNumAt(0) == closest_molnum)
        //this constraint is already satisfied
        return Molecules();
        
    else
    {
        //we need to swap the coordinates of the first molecule
        //in the group with the closest molecule to the constraint
        //point
        Molecules changed_mols;
        
        Molecule first_mol = this->moleculeGroup().moleculeAt(0).molecule();
        Molecule closest_mol = this->moleculeGroup()[closest_molnum].molecule();
        
        const PropertyName &coords_property = map["coordinates"];
        
        AtomCoords first_mol_coords = first_mol.property(coords_property)
                                               .asA<AtomCoords>();
                                               
        AtomCoords closest_mol_coords = closest_mol.property(coords_property)
                                                   .asA<AtomCoords>();
                                                   
        first_mol = first_mol.edit().setProperty(coords_property, closest_mol_coords);
        closest_mol = closest_mol.edit().setProperty(coords_property, first_mol_coords);
        
        changed_mols.add(first_mol);
        changed_mols.add(closest_mol);
        
        return changed_mols;
    }
}

/** Update this constraint, returning what needs to be changed */
Molecules SinglePointHelper::update(const System &system, bool new_system)
{
    if (identity_points.isEmpty())
        return Molecules();
        
    bool new_group = this->updateGroup(system);
    bool new_points = this->updatePoints(system);
    bool new_space = false;
    
    if (new_system)
        new_space = this->updateSpace(system);

    if (new_points or new_group or new_space)
        //recalculate all distances to find the closest molecule
        this->recalculateDistances();

    return this->applyConstraint();
}

/** Update this constraint, returning what needs to be changed */
Molecules SinglePointHelper::update(const System &system, MolNum changed_mol,
                                    bool new_system)
{
    if (identity_points.isEmpty())
        return Molecules();

    bool new_group = this->updateGroup(system);
    bool new_points = this->updatePoints(system);

    bool new_space = false;
    
    if (new_system)
        new_space = this->updateSpace(system);

    if (new_points or new_space)
    {
        //points have changed - need to recalculate all distances
        this->recalculateDistances();
    }
    else if (new_group)
    {
        this->recalculateDistances(changed_mol);
    }

    return this->applyConstraint();
}

/** Update this constraint, returning what needs to be changed */
Molecules SinglePointHelper::update(const System &system, const Molecules &molecules,
                                    bool new_system)
{
    if (identity_points.isEmpty())
        return Molecules();

    bool new_group = this->updateGroup(system);
    bool new_points = this->updatePoints(system);

    bool new_space = false;
    
    if (new_system)
        new_space = this->updateSpace(system);

    if (new_points or new_space)
    {
        //points have changed - need to recalculate all distances
        this->recalculateDistances();
    }
    else if (new_group)
    {
        this->recalculateDistances(molecules);
    }

    return this->applyConstraint();
}

/////////
///////// Implementation of FewPointsHelper
/////////


/////////
///////// Implementation of ManyPointsHelper
/////////

/** Constructor */
ManyPointsHelper::ManyPointsHelper() : IdentityConstraintPvt()
{}

/** Construct to constrain the identity of all of the molecules of the passed group
     - this automatically generates the points from the current configuration */
ManyPointsHelper::ManyPointsHelper(const MoleculeGroup &molgroup,
                                   const PropertyMap &map)
                 : IdentityConstraintPvt()
{
    if (molgroup.isEmpty())
        return;

    int nmols = molgroup.nMolecules();

    QVector<PointPtr> points( nmols );
    points.squeeze();
    
    PointPtr *points_array = points.data();
    
    const PropertyName &coords_property = map["coordinates"];
    
    for (int i=0; i<nmols; ++i)
    {
        const ViewsOfMol &mol = molgroup.moleculeAt(i);
        
        const AtomCoords &coords = mol.data().property(coords_property)
                                             .asA<AtomCoords>();
                                             
        points_array[i] = new VectorPoint(coords.array().aaBox().center());
    }
    
    this->validateGroup(molgroup);
}

/** Construct to constrain the molecules in 'molgroup' to keep their
    identities using the points in 'points' */
ManyPointsHelper::ManyPointsHelper(const MoleculeGroup &molgroup,
                                   const QVector<PointPtr> &points,
                                   const PropertyMap &map)
                 : IdentityConstraintPvt(molgroup, points, map)
{}

/** Copy constructor */                          
ManyPointsHelper::ManyPointsHelper(const ManyPointsHelper &other)
                 : IdentityConstraintPvt(other),
                   point_distances(other.point_distances)
{}

/** Destructor */
ManyPointsHelper::~ManyPointsHelper()
{}

const char* ManyPointsHelper::what() const
{
    return ManyPointsHelper::typeName();
}

ManyPointsHelper* ManyPointsHelper::clone() const
{
    return new ManyPointsHelper(*this);
}

/** Recalculate all of the distances between all of the molecules
    and all of the points */
void ManyPointsHelper::recalculateDistances(const System &system)
{
    point_distances = QHash< MolNum,QVector<double> >();
    
    const Molecules &molecules = this->moleculeGroup().molecules();
    
    point_distances.reserve(molecules.nMolecules());
    
    int npoints = identity_points.count();
    const PointPtr *const_points_array = identity_points.constData();
    
    QVector< Vector > points(npoints);
    Vector *points_array = points.data();
    
    for (int i=0; i<npoints; ++i)
    {
        points_array[i] = const_points_array[i].read().point();
    }
    
    const Space &space = system.property(map["space"]).asA<Space>();
    const PropertyName &coords_property = map["coordinates"];
    
    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        const AtomCoords &coords = it->data().property(coords_property)
                                             .asA<AtomCoords>();
                                             
        Vector center = coords.array().aaBox().center();
        
        QVector<double> distances(npoints);
        double *distances_array = distances.data();
        
        for (int i=0; i<npoints; ++i)
        {
            double dist2 = space.calcDist2(center, points_array[i]);
            distances_array[i] = dist2;
        }
        
        point_distances.insert( it.key(), distances );
    }
}

/** Update this constraint, returning what needs to change */
Molecules ManyPointsHelper::update(const System &system, bool new_space)
{
    if (identity_points.isEmpty())
        return Molecules();

    bool new_group = this->updateGroup(system);
    bool new_points = this->updatePoints(system);

    bool new_space = false;
    
    if (new_system)
        new_space = this->updateSpace(system);

    if (new_group or new_points or new_space)
    {
        this->recalculateDistances(system);
        return this->applyConstraint();
    }
    else
        return Molecules();
}

/** Update this constraint, returning what needs to change */
Molecules ManyPointsHelper::update(const System &system, MolNum changed_mol,
                                   bool new_system)
{
    if (identity_points.isEmpty())
        return Molecules();

    else if (point_distances.isEmpty())
        return this->update(system);

    bool new_group = this->updateGroup(system);
    bool new_points = this->updatePoints(system);

    bool new_space = false;
    
    if (new_system)
        new_space = this->updateSpace(system);

    if (new_points or new_space)
    {
        this->recalculateDistances(system);
        return this->applyConstraint();
    }
    else if (new_group)
    {
        if (this->recalculateDistances(system, changed_mol))
            return this->applyConstraint();
    }

    return Molecules();
}

/** Update this constraint, returning what needs to change */
Molecules ManyPointsHelper::update(const System &system, const Molecules &molecules,
                                   bool new_system)
{
    if (identity_points.isEmpty())
        return Molecules();

    else if (point_distances.isEmpty())
        return this->update(system);

    bool new_group = this->updateGroup(system);
    bool new_points = this->updatePoints(system);

    bool new_space = false;
    
    if (new_system)
        new_space = this->updateSpace(system);

    if (new_points or new_space)
    {
        this->recalculateDistances(system);
        return this->applyConstraint();
    }
    else if (new_group)
    {
        if (this->recalculateDistances(system, molecules))
            return this->applyConstraint();
    }

    return Molecules();
}

/////////
///////// Implementation of IdentityConstraint
/////////

static const RegisterMetaType<IdentityConstraint> r_identityconstraint;

/** Serialise to a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator<<(QDataStream &ds,
                                          const IdentityConstraint &identityconstraint)
{
    writeHeader(ds, r_identityconstraint, 1);
    
    SharedDataStream sds(ds);
    
    sds << identityconstraint.d->molGroupPtr()
        << identityconstraint.d->points()
        << identityconstraint.d->propertyMap()
        << static_cast<const MoleculeConstraint&>(identityconstraint);
        
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIRESYSTEM_EXPORT &operator>>(QDataStream &ds,
                                          IdentityConstraint &identityconstraint)
{
    VersionID v = readHeader(ds, r_identityconstraint);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        MolGroupPtr molgroup;
        QVector<PointPtr> points;
        PropertyMap map;
        
        sds >> molgroup >> points >> map;
        
        IdentityConstraint new_constraint(points, molgroup, map);
        
        sds >> static_cast<MoleculeConstraint&>(new_constraint);
        
        identityconstraint = new_constraint;
    }
    else
        throw version_error( v, "1", r_identityconstraint, CODELOC );
        
    return ds;
}

/** Constructor */
IdentityConstraint::IdentityConstraint()
                   : ConcreteProperty<IdentityConstraint,MoleculeConstraint>(),
                     d( IdentityConstraintPvt::null() )
{}

/** Construct the constraint that constrains the identities of all
    of the molecules in the passed molecule group. This uses the current
    locations of the molecules to apply the constraint. The (optionally
    supplied) property map is used to find the properties required
    of this constraint */
IdentityConstraint::IdentityConstraint(const MoleculeGroup &molgroup,
                                       const PropertyMap &map)
                   : ConcreteProperty<IdentityConstraint,MoleculeConstraint>(),
                     d( new ManyPointsHelper(molgroup, map) )
{}

/** Construct the constraint that constrains the identity of a single
    molecule in the passed molecule group - this sets the identity 
    of the first molecule to be that of the one closest to the
    passed point. The (optionally supplied) property map is used to 
    find the properties required of this constraint */
IdentityConstraint::IdentityConstraint(const PointRef &point,
                                       const MoleculeGroup &molgroup,
                                       const PropertyMap &map)
                   : ConcreteProperty<IdentityConstraint,MoleculeConstraint>(),
                     d( new SinglePointHelper(molgroup, point, map) )
{}

/** Construct the constraint that constrains the identities of the 
    points.count() molecules from the passed molecule group so that
    the first molecule is identified by the first point, the second
    molecule is identified by the second point, and the nth molecule
    is identified by the nth point. The (optionally supplied) property 
    map is used to find the properties required of this constraint */
IdentityConstraint::IdentityConstraint(const QVector<PointPtr> &points,
                                       const MoleculeGroup &molgroup,
                                       const PropertyMap &map)
                   : ConcreteProperty<IdentityConstraint,MoleculeConstraint>()
{
    if (points.isEmpty())
        d = IdentityConstraintPvt::null();
    
    else if (points.count() == 1)
        d = new SinglePointHelper(molgroup, points.first(), map);
    
    else if (points.count() > 0.5 * molgroup.nMolecules())
        d = new ManyPointsHelper(molgroup, points, map);
        
    else
        d = new FewPointsHelper(molgroup, points, map);
}                  
  
/** Construct the constraint that constrains the identities of the 
    points.count() molecules from the passed molecule group so that
    the first molecule is identified by the first point, the second
    molecule is identified by the second point, and the nth molecule
    is identified by the nth point. The (optionally supplied) property 
    map is used to find the properties required of this constraint */
IdentityConstraint::IdentityConstraint(const QList<PointPtr> &points,
                                       const MoleculeGroup &molgroup,
                                       const PropertyMap &map)
                   : ConcreteProperty<IdentityConstraint,MoleculeConstraint>()
{
    if (points.isEmpty())
        d = IdentityConstraintPvt::null();
    
    else if (points.count() == 1)
        d = new SinglePointHelper(molgroup, points.first(), map);
    
    else
    {
        IdentityConstraint::operator=( IdentityConstraint(points.toVector(),
                                                          molgroup, map) );
    }
}

/** Copy constructor */
IdentityConstraint::IdentityConstraint(const IdentityConstraint &other)
                   : ConcreteProperty<IdentityConstraint,MoleculeConstraint>(other),
                     d(other.d)
{}

/** Destructor */
IdentityConstraint::~IdentityConstraint()
{}

/** Copy assignment operator */
IdentityConstraint& IdentityConstraint::operator=(const IdentityConstraint &other)
{
    MoleculeConstraint::operator=(other);
    d = other.d;
    
    return *this;
}

/** Comparison operator */
bool IdentityConstraint::operator==(const IdentityConstraint &other) const
{
    return this == &other or
           (MoleculeConstraint::operator==(other) and
            QLatin1String(d->what()) == QLatin1String(other.d->what()) and
            this->moleculeGroup() == other.moleculeGroup() and
            this->points() == other.points() and
            this->propertyMap() == other.propertyMap());
}

/** Comparison operator */
bool IdentityConstraint::operator!=(const IdentityConstraint &other) const
{
    return not IdentityConstraint::operator==(other);
}

const char* IdentityConstraint::typeName()
{
    return QMetaType::typeName( qMetaTypeId<IdentityConstraint>() );
}

/** Return a string representation of this constraint */
QString IdentityConstraint::toString() const
{
    return QString("IdentityConstraint( nPoints() == %1, nMolecules() == %2 )")
                .arg(this->points().count())
                .arg(this->moleculeGroup().nMolecules());
}

/** Return the molecule group acted on by this constraint */
const MoleculeGroup& IdentityConstraint::moleculeGroup() const
{
    return d->moleculeGroup();
}

/** Return the points used to identify the molecules */
QVector<SireFF::PointPtr> IdentityConstraint::points() const
{
    return d->points();
}

/** Return the property map used to find the properties used
    by this constraint */
const PropertyMap& IdentityConstraint::propertyMap() const
{
    return d->propertyMap();
}

/** Return whether or not this constraint affects or uses information
    from the molecule with number 'molnum' */
bool IdentityConstraint::involvesMolecule(MolNum molnum) const
{
    const int npoints = d->points().count();
    const PointPtr *points_array = d->points().constData();
    
    for (int i=0; i<npoints; ++i)
    {
        if (points_array[i].read().contains(molnum))
            return true;
    }
    
    return d->moleculeGroup().contains(molnum);
}

/** Return whether or not this constraint affects or uses information
    from any of the molecules in 'molecules' */
bool IdentityConstraint::involvesMoleculesFrom(const Molecules &molecules) const
{
    const int npoints = d->points().count();
    const PointPtr *points_array = d->points().constData();
    
    for (int i=0; i<npoints; ++i)
    {
        if (points_array[i].read().usesMoleculesIn(molecules))
            return true;
    }

    const MoleculeGroup &molgroup = d->moleculeGroup();

    for (Molecules::const_iterator it = molecules.constBegin();
         it != molecules.constEnd();
         ++it)
    {
        if (molgroup.contains(it.key()))
            return true;
    }
    
    return false;
}

/** Update this constraint so that it is applied to the system 'system'
    and return the molecules from system that need to change to 
    ensure that this constraint is maintained */
Molecules IdentityConstraint::update(const System &system)
{
    if (system.UID() == this->sysUID() and
        system.version() == this->sysVersion())
    {
        return Molecules();
    }
    else
    {
        IdentityConstraint old_state(*this);

        bool new_system = (system.UID() != this->sysUID() or
                           system.version().majorVersion() 
                                            != this->sysVersion().majorVersion());
        
        try
        {
            this->updatedFrom(system);
            return d->update(system, new_system);
        }
        catch(...)
        {
            IdentityConstraint::operator=(old_state);
            throw;
        }
        
        return Molecules();
    }
}

/** Update this constraint so that it is applied to the system 'system'
    and return the molecules from system that need to change to 
    ensure that this constraint is maintained. This provides the
    hint that only the molecule with number 'changed_mol' has changed
    since the last update. */
Molecules IdentityConstraint::update(const System &system, MolNum changed_mol)
{
    if (system.UID() == this->sysUID() and
        system.version() == this->sysVersion())
    {
        return Molecules();
    }
    else
    {
        IdentityConstraint old_state(*this);
        
        bool new_system = (system.UID() != this->sysUID() or
                           system.version().majorVersion() 
                                            != this->sysVersion().majorVersion());
        
        try
        {
            this->updatedFrom(system);
            return d->update(system, changed_mol, new_system);
        }
        catch(...)
        {
            IdentityConstraint::operator=(old_state);
            throw;
        }
        
        return Molecules();
    }
}

/** Update this constraint so that it is applied to the system 'system'
    and return the molecules from system that need to change to 
    ensure that this constraint is maintained. This provides the
    hint that only the molecules in 'molecules' have changed
    since the last update. */
Molecules IdentityConstraint::update(const System &system, const Molecules &molecules)
{
    if (system.UID() == this->sysUID() and
        system.version() == this->sysVersion())
    {
        return Molecules();
    }
    else
    {
        IdentityConstraint old_state(*this);
        
        bool new_system = (system.UID() != this->sysUID() or
                           system.version().majorVersion() 
                                            != this->sysVersion().majorVersion());
        
        try
        {
            this->updatedFrom(system);
            return d->update(system, molecules, new_system);
        }
        catch(...)
        {
            IdentityConstraint::operator=(old_state);
            throw;
        }
        
        return Molecules();
    }
}
