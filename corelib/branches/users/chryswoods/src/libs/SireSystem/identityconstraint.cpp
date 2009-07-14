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

#include "SireMol/molecules.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

namespace SireSystem
{
namespace detail
{

/** This class is used to hold the cost (price) of 
    a molecule with respect to a point */
class IdentityPrice
{
public:
    IdentityPrice(const DistVector &distvector=DistVector(), 
                  MolNum molnum, MGNum mgnum);
    
    IdentityPrice(const IdentityPrice &other);
    
    ~IdentityPrice();
    
    IdentityPrice& operator=(const IdentityPrice &other);
    
    bool operator==(const IdentityPrice &other) const;
    bool operator!=(const IdentityPrice &other) const;
    
    bool operator<(const IdentityPrice &other) const;
    bool operator<=(const IdentityPrice &other) const;
    
    bool operator>(const IdentityPrice &other) const;
    bool operator>=(const IdentityPrice &other) const;
    
    double distance() const;
    
private:
    /** The minimum image vector from the molecule to the 
        point, including the distance between the molecule
        and the point */
    DistVector distvec;
    
    /** The number of the molecule */
    MolNum molnum;
    
    /** The number of the molecule group */
    MGNum mgnum;
};

} // end of namespace detail
} // end of namespace SireSystem

using namespace SireSystem::detail;

/////////
///////// Implementation of IdentityPrice
/////////

/** Constructor */
IdentityPrice::IdentityPrice(const DistVector &dvec, MolNum mol_num, MGNum mg_num)
              : distvec(dvec), molnum(mol_num), mgnum(mg_num)
{}

/** Copy constructor */
IdentityPrice::IdentityPrice(const IdentityPrice &other)
              : distvec(other.distvec), molnum(other.molnum), mgnum(other.mgnum)
{}

/** Destructor */
IdentityPrice::~IdentityPrice()
{}

/** Copy assignment operator */
IdentityPrice& IdentityPrice::operator=(const IdentityPrice &other)
{
    distvec = other.distvec;
    molnum = other.molnum;
    mgnum = other.mgnum;
    return *this;
}

/** Equality comparison */
bool IdentityPrice::operator==(const IdentityPrice &other) const
{
    return distvec == other.distvec and 
           molnum == other.molnum and
           mgnum == other.mgnum;
}

/** Inequality comparison */
bool IdentityPrice::operator!=(const IdentityPrice &other) const
{
    return distvec != other.distvec or 
           molnum != other.molnum or
           mgnum != other.mgnum;
}

/** Compare two prices - this is used to provide a unique ordering
    of the costs of each molecule with respect to each point. This
    first compares using the distance, then if the distances are
    equal, then it compares using the x delta, then the y delta,
    then the z delta, and then if these are equal (so the molecules
    have identical minimum image coordinates), if then compares
    the indicies of the molecule groups in the IdentityConstraint
    that contain the molecules, and then if these are equal, then
    it then compares the index of the molecules in the molecule
    group - this ensures that there is a 100% unique ordering
    of prices for all molecules, regardless of the  */
bool IdentityPrice::operator<(const IdentityPrice &other) const
{
    if (distvec.length() < other.distvec.length())
        return true;
        
    else if (distvec.length() == other.distvec.length())
    {
        if (distvec.x() < other.distvec.x())
            return true;

        else if (distvec.x() == other.distvec.x())
        {
            if (distvec.y() < other.distvec.y())
                return true;
                
            else if (distvec.y() == other.distvec.y())
            {
                if (distvec.z() < other.distvec.z())
                    return true;
                
                else if (distvec.z() == other.distvec.z())
                {
                    if (molnum.value() < other.molnum.value())
                        return true;
                        
                    else if (molnum.value() == other.molnum.value())
                        return mgnum.value() < other.mgnum.value();
                }
            }
        }
    }

    return false;
}

/** Compare two prices - see IdentityPrice::operator<(const IdentityPrice&) const
    for details on how prices are compared */
bool IdentityPrice::operator>(const IdentityPrice &other) const
{
    if (distvec.length() > other.distvec.length())
        return true;
        
    else if (distvec.length() == other.distvec.length())
    {
        if (distvec.x() > other.distvec.x())
            return true;
            
        else if (distvec.x() == other.distvec.x())
        {
            if (distvec.y() > other.distvec.y())
                return true;
                
            else if (distvec.y() == other.distvec.y())
            {
                if (distvec.z() > other.distvec.z())
                    return true;
                    
                else if (distvec.z() == other.distvec.z())
                {
                    if (molnum.value() > other.molnum.value())
                        return true;
                    
                    else if (molnum.value() == other.molnum.value())
                        return mgnum.value() > other.mgnum.value();
                }
            }
        }
    }
    
    return false;
}

/** Compare two prices - see IdentityPrice::operator<(const IdentityPrice&) const
    for details on how prices are compared */
bool IdentityPrice::operator<=(const IdentityPrice &other) const
{
    return this->operator<(other) or this->operator==(other);
}

/** Compare two prices - see IdentityPrice::operator<(const IdentityPrice&) const
    for details on how prices are compared */
bool IdentityPrice::operator>=(const IdentityPrice &other) const
{
    return this->operator>(other) or this->operator==(other);
}

/////////
///////// Implementation of IdentityConstraint
/////////

/** Internal function used to recalculate all of the prices
    for the ith point */
void IdentityConstraint::recalculatePointPrices(int i, const System &system)
{
    const Point &point = identity_points.at(i).read();
    
    int ngroups = molgroups.count();
    
    const int max_nprices = identity_points.count() + nbuffer;
    
    QMap<IdentityPrice,void*> prices;
    
    IdentityPrice highest_price( DistVector(0), MolNum(0), MGNum(0) );
    
    for (int mgidx=0; mgidx<ngroups; ++mgidx)
    {
        const MoleculeGroup &molgroup = molgroups.at(i).read();
        const PropertyMap &map = property_maps.at(i);
        
        const Space &space = system.property(map["space"]).asA<Space>();
        
        for (Molecules::const_iterator it = molgroup.constBegin();
             it != molgroup.constEnd();
             ++it)
        {
            const ViewsOfMol &mol = *it;
            
            DistVector distance = space.calcDistVector( point(), 
                                                        mol.evaluate().center(map) );
                                
            IdentityPrice price(distance, mol.number(), group.mgNum());
                                                                     
            if (price > highest_price)
            {
                if (prices.count() >= max_nprices)
                    //no need to record this price
                    continue;
                    
                else
                {
                    //record this price, and the fact that it is 
                    //now the highest price
                    prices.insert( price, 0 );
                    highest_price = price;
                }
            }
            else
            {
                //add this price, as it is not the highest
                prices.insert( price, 0 );
                
                if (prices.count() > max_nprices)
                {
                    while (prices.count() > max_nprices)
                        removeHighestPrice(prices);
                        
                    highest_price = highestPrice(prices);
                }
            }
        }
    }
    
    identity_prices[i] = prices;
}

/** Internal function used to recalculate some of the prices
    when only the molecules in 'molecules' have changed */
void IdentityConstraint::recalculateMoleculePrices(const Molecules &molecules,
                                                   const System &system)
{
    if (molecules.isEmpty())
        return;
        
    else if (molecules.count() == 1)
    {
        this->recalculateMoleculePrices( molecules.constBegin()->number(), system );
        return;
    }

    int ngroups = molgroups.count();
    
    int npoints = identity_points.count();
    const PointPtr *points_array = identity_points.constData();
    
    QMap<IdentityPrice,void*> *prices_array = identity_prices.data();
    
    QSet<int> recalculated_point_prices;
    
    const int max_nprices = npoints + nbuffer;
    
    for (int mgidx=0; mgidx<ngroups; ++mgidx)
    {
        const MoleculeGroup &molgroup = molgroups.at(i).read();
        const PropertyMap &map = property_maps.at(i);
        
        const Space &space = system.property( map["space"] ).asA<Space>();
        
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            if (recalculated_point_prices.count() == npoints)
                //all points have been recalculated
                return;
        
            else if (molgroup.contains(it->number()))
            {
                //get the molecule as it exists in this molecule group
                const ViewsOfMol &mol = molgroup[ it->number() ];

                for (int i=0; i<npoints; ++i)
                {
                    if (recalculated_point_prices.contains(i))
                        //we've already completely recalculated this point
                        continue;
                        
                    const Point &point = points_array[i].read();
                    
                    QHash<IdentityPrice,void*> &prices = prices_array[i];
                    
                    DistVector distance = space.calcDistVector( point(),
                                                         mol.evaluate().center(map) );
                                                         
                    IdentityPrice new_price(distance, molnum, group.mgNum());
                
                    IdentityPrice old_price = takePrice(prices, molnum, mgnum);
                    
                    if (old_price.isNull())
                    {
                        if (new_price < highestPrice(prices))
                        {
                            //this molecule has moved into contention
                            prices.insert(new_price, 0);
                            
                            while (prices.count() > max_nprices)
                                removeHighestPrice(prices);
                        }
                    }
                    else
                    {
                        //this molecule is already in contention - has this changed?
                        if (new_price < old_price or new_price < highestPrice(prices))
                            //this can't have changed
                            prices.insert(new_price, 0);
                        
                        else
                        {
                            //this molecule may have moved out of contention - 
                            //we need to recalculate prices for this point
                            this->recalculatePointPrices(i, system);
                            recalculated_point_prices.insert(i);
                        
                            if (recalculated_point_prices.count() == npoints)
                                //all points have been recalculated
                                return;
                        }
                    }
                }
            }
        }
    }
}

/** Internal function used to recalculate some of the prices,
    when only the molecule 'molnum' has changed */
void IdentityConstraint::recalculateMoleculePrices(MolNum molnum, 
                                                   const System &system,
                                                   QSet<int> &recalculated_points)
{
    int ngroups = molgroups.count();
    
    int npoints = identity_points.count();
    const PointPtr *points_array = identity_points.constData();
    
    const int max_nprices = npoints + nbuffer;
    
    for (int mgidx=0; mgidx<ngroups; ++mgidx)
    {
        const MoleculeGroup &molgroup = molgroups.at(i).read();
        const PropertyMap &map = property_maps.at(i);
        
        const Space &space = system.property( map["space"] ).asA<Space>();
        
        if (molgroup.contains(molnum))
        {
            const ViewsOfMol &mol = molgroup[molnum];
        
            for (int i=0; i<npoints; ++i)
            {
                if (recalculated_points.contains(i))
                    //this point has already been completely recalculated
                    continue;
            
                const Point &point = points_array[i].read();
            
                QHash<IdentityPrice,void*> &prices = identity_prices[i];
            
                //calculate the new distance to this point
                DistVector distance = space.calcDistVector( point(),
                                                            mol.evaluate().center(map) );
            
                IdentityPrice new_price(distance, molnum, group.mgNum());
            
                IdentityPrice old_price = takePrice(prices, molnum, mgnum);
                
                if (old_price.isNull())
                {
                    if (new_price < highestPrice(prices))
                    {
                        //this molecule has moved into contention
                        prices.insert(new_price, 0);
                        
                        while (prices.count() > max_nprices)
                            removeHighestPrice(prices);
                    }
                }
                else
                {
                    //this molecule is already in contention - has this changed?
                    if (new_price < old_price or new_price < highestPrice(prices))
                        //this can't have changed
                        prices.insert(new_price, 0);
                    
                    else
                    {
                        //this molecule may have moved out of contention - 
                        //we need to recalculate prices for this point
                        this->recalculatePointPrices(i, system);
                        recalculated_points.insert(i);
                    
                        if (recalculated_points.count() == npoints)
                            //all points have been recalculated
                            return;
                    }
                }
            }
        }
    }
}

/** Internal function used to recalculate the prices of all 
    molecules against all points */
void IdentityConstraint::recalculatePrices(const System &system)
{
    int npoints = identity_points.count();
    
    for (int i=0; i<npoints; ++i)
    {
        this->recalculatePointPrices(i, system);
    }
}

/** Internal function used to update all of the molecule
    groups to the same version used in the passed system */
bool IdentityConstraint::updateGroups(const System &system)
{
    int ngroups = molgroups.count();
    IdentityGroup *groups_array = molgroups.data();
    
    for (int i=0; i<ngroups; ++i)
    {
        const IdentityGroup &old_group = groups_array[i];
        const MoleculeGroup &new_group = system[ old_group.mgNum() ];
        
        if (old_group.version() == new_group.version())
            //nothing has changed
            continue;
            
        else if (old_group.version().minorVersion() 
                                    == new_group.version().minorVersion())
        {
            //only the coordinates of the molecules have changed
            // - there is no need to revalidate the group
            old_group.update(new_group);
        }
        else
        {
            //there has been a major version change - this means that
            //molecules may have been added or removed - we need to
            //revalidate that this molecule group is right for this
            //constraint
            this->validateGroup(new_group);
            
            old_group.update(new_group);
        }
    }
}

/** Internal function used to update all of the points - this
    returns the indicies of points that have moved */
QSet<int> IdentityConstraint::updatePoints(const System &system)
{
    int npoints = identity_points.count();
    
    PointPtr *const_points_array = identity_points.constData();
    
    bool need_update = false;
    
    for (int i=0; i<npoints; ++i)
    {
        if (const_points_array[i].read().usesMoleculesIn(system))
        {
            need_update = true;
            break;
        }
    }
    
    bool points_changed = false;

    QSet<int> changed_points;

    if (need_update)
    {
        PointPtr *points_array = identity_points.data();
    
        for (int i=0; i<npoints; ++i)
        {
            if (points_array[i].read().usesMoleculesIn(system))
            {
                if (points_array[i].edit().update(system))
                {
                    changed_points.insert(i);
                }
            }
        }
    }
    
    return changed_points;
}

/** Update this constraint so that it uses the same version of 
    the molecules as that used in the passed system, and return
    the molecules that need to be changed to maintain the constaint
    
    Call this function when you don't know which molecules have
    changed since the last update, or when the contents of 
    molecule groups have changed
    
    \throw SireError::incompatible_error
    \throw SireSystem::unachievable_constraint
*/
Molecules IdentityConstraint::update(const System &system)
{
    this->assertCompatibleWith(system);
    
    if (system.version() == this->sysVersion())
        //nothing has changed
        return Molecules();

    this->updateGroups(system);
    this->updatePoints(system);
    this->recalculatePrices(system);
    
    //use the new prices to apply the constraint
    Molecules changed_mols = this->applyIdentityConstraint(system);
    
    //save the fact that we have been updated for this
    //version of the system
    MoleculeConstaint::updated(system);
    
    return changed_mols;
}

/** Update this constraint so that it uses the same version of 
    the molecules as that used in the passed system, and return
    the molecules that need to be changed to maintain the constaint

    Call this function when you *know* that only the molecule
    with number 'changed_mol' has changed since the last update.
    
    \throw SireError::incompatible_error
    \throw SireSystem::unachievable_constraint
*/
Molecules IdentityConstraint::update(const System &system, MolNum changed_mol)
{
    this->assertCompatibleWith(system);
    
    if (system.version() == this->sysVersion())
        //nothing has changed
        return Molecules();
    
    else if (system.version().majorVersion() != this->sysVersion().majorVersion())
        //something more than a molecule has changed
        return this->update(system);

    this->updateGroups(system);

    //update all of the points with the new system
    QSet<int> changed_points = this->updatePoints(system);

    if (changed_points.count() == this->nPoints())
        //we need to recalculate everything
        this->recalculatePrices(system);
    else
    {
        //recalculate prices for the changed points
        foreach (int changed_point, changed_points)
        {
            this->recalculatePointPrices(i, system);
        }
    
        //now recalculate prices for the changed molecule
        this->recalculateMoleculePrices(changed_mol, system, changed_points);
    }
    
    //use the new prices to apply the constraint
    Molecules changed_mols = this->applyIdentityConstraint(system);
    
    //save the fact that we have been updated for this
    //version of the system
    MoleculeConstaint::updated(system);
    
    return changed_mols;
}

/** Update this constraint so that it uses the same version of 
    the molecules as that used in the passed system, and return
    the molecules that need to be changed to maintain the constaint
    
    Call this function when you *know* that only the molecules
    in 'molecules' have changed since the last update
    
    \throw SireError::incompatible_error
    \throw SireSystem::unachievable_constraint
*/
Molecules IdentityConstraint::update(const System &system, const Molecules &molecules)
{
    this->assertCompatibleWith(system);
    
    if (system.version() == this->sysVersion())
        //nothing has changed
        return Molecules();
    
    else if (system.version().majorVersion() != this->sysVersion().majorVersion())
        //something more than a molecule has changed
        return this->update(system);

    this->updateGroups(system);

    //update all of the points with the new system
    QSet<int> changed_points = this->updatePoints(system);

    if (changed_points.count() == this->nPoints())
        //we need to recalculate everything
        this->recalculatePrices(system);
    else
    {
        //recalculate prices for the changed points
        foreach (int changed_point, changed_points)
        {
            this->recalculatePointPrices(i, system);
        }
    
        //now recalculate prices for the changed molecules
        this->recalculateMoleculePrices(molecules, system, changed_points);
    }
    
    //use the new prices to apply the constraint
    Molecules changed_mols = this->applyIdentityConstraint(system);
    
    //save the fact that we have been updated for this
    //version of the system
    MoleculeConstaint::updated(system);
    
    return changed_mols;
}

/** Return whether or not this constraint affects or uses information
    from the molecule with number 'molnum' */
bool IdentityConstraint::involvesMolecule(MolNum molnum) const
{
    const int npoints = identity_points.count();
    const PointPtr *points_array = identity_points.constData();
    
    for (int i=0; i<npoints; ++i)
    {
        if (points_array[i].read().contains(molnum))
            return true;
    }
    
    for (QList<MolGroupPtr>::const_iterator it = molgroups.constBegin();
         it != molgroups.constEnd();
         ++it)
    {
        if (it->read().contains(molnum))
            return true;
    }
    
    return false;
}

/** Return whether or not this constraint affects or uses information
    from any of the molecules in 'molecules' */
bool IdentityConstraint::involvesMoleculesFrom(const Molecules &molecules) const
{
    const int npoints = identity_points.count();
    const PointPtr *points_array = identity_points.constData();
    
    for (int i=0; i<npoints; ++i)
    {
        if (points_array[i].read().usesMoleculesIn(molecules))
            return true;
    }
    
    for (QList<MolGroupPtr>::const_iterator it = molgroups.constBegin();
         it != molgroups.constEnd();
         ++it)
    {
        for (Molecules::const_iterator it2 = molecules.constBegin();
             it2 != molecules.constEnd();
             ++it2)
        {
            if (it->read().contains(it2.key()))
                return true;
        }
    }
    
    return false;
}
