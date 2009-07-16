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

#ifndef SIRESYSTEM_IDENTITYCONSTRAINT_H
#define SIRESYSTEM_IDENTITYCONSTRAINT_H

#include <QVector>

#include "constraint.h"

#include "SireBase/propertymap.h"

#include "SireMaths/vector.h"

#include "SireMol/moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class IdentityConstraint;
}

QDataStream& operator<<(QDataStream&, const SireSystem::IdentityConstraint&);
QDataStream& operator>>(QDataStream&, SireSystem::IdentityConstraint&);

namespace SireSystem
{

using SireBase::PropertyMap;

using SireMaths::Vector;

using SireMol::MoleculeGroup;
using SireMol::Molecules;
using SireMol::MolID;

namespace detail
{
class IdentityPrice;
}

/** An identity constraint provides a method of constraining
    the identity of molecules based on *where* they are located.
    
    For example, it can be useful to be able to identify a 
    water in a binding pocket. However, in a normal simulation,
    we don't identify waters by location, but by their index 
    (e.g. this is the first water, this is the second etc.).
    
    This means that the identity of the water in the binding
    pocket can change, e.g. it can start with the fifth water
    in the pocket, but during the simulation the fifth water
    may diffuse out of the pocket, and the twentieth water
    would diffuse in its place. The identity of the water
    in the binding pocket will thus have changed from the 
    fifth water to the twentieth water.
    
    An identity constraint works by constantly monitoring 
    the locations of the waters, and so it can detect when 
    the fifth water is displaced by the twentieth water. When
    it detects that this has occured, the constraint swaps
    the coordinates of the fifth and twentieth waters,
    thereby ensuring that the fifth water *stays* in the pocket.
    This doesn't affect the energy or the statistics of the 
    system, as waters are indistinguishable (there are N!
    equivalent configurations of N waters - we only see them
    as N! different configurations as we identify each water,
    when really they are indistinguishable).
    
    The idea of constraining the identity of molecules
    was first presented by M. Tyka, R. Sessions and A. Clarke in 
   
    "Absolute Free-Energy Calculations of Liquids Using 
                a Harmonic Reference State" 
                
    J. Chem. Phys. B,  2007, 111, 9571-9580
    
            doi://10.1021/jp072357w
            
    They used the method to constrain the identity of *all*
    molecules, so that harmonic restraints can be applied
    to them all.
    
    This identity constraint is more general, and allows
    the identification of a subset of molecules to be
    constrained (e.g. just the waters in a binding pocket).
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT IdentityConstraint
               : public SireBase::ConcreteProperty<IdentityConstraint,Constraint>
{

friend QDataStream& ::operator<<(QDataStream&, const IdentityConstraint&);
friend QDataStream& ::operator>>(QDataStream&, IdentityConstraint&);

public:
    IdentityConstraint();
    
    IdentityConstraint(const MoleculeGroup &molgroup,
                       const PropertyMap &map = PropertyMap());
    
    IdentityConstraint(const PointRef &point,
                       const MoleculeGroup &molgroup,
                       const PropertyMap &map = PropertyMap());
                       
    IdentityConstraint(const QList<PointPtr> &points,
                       const MoleculeGroup &molgroup,
                       const PropertyMap &map = PropertyMap());
    
    IdentityConstraint(const IdentityConstraint &other);
    
    ~IdentityConstraint();
    
    IdentityConstraint& operator=(const IdentityConstraint &other);
    
    bool operator==(const IdentityConstraint &other) const;
    bool operator!=(const IdentityConstraint &other) const;
    
    static const char* typeName();
    
    QString toString() const;
    
    const MoleculeGroup& moleculeGroup() const;
    
    QVector<PointPtr> points() const;

    const PropertyMap& propertyMap() const;
    
    bool involvesMolecule(MolNum molnum) const;
    bool involvesMoleculesFrom(const Molecules &molecules) const;
    
    Molecules update(const System &system);
    Molecules update(const System &system, MolNum changed_mol);
    Molecules update(const System &system, const Molecules &molecules);

private:
    /** The molecule group containing the molecules whose identities
        are being constrained */
    MolGroupPtr molgroup;
    
    /** The set of points that provide the locations that
        identify the molecules. These 'n' points constrain
        the identity of the first 'n' molecule in 'molgroup'
        (with point 'i' constraining the identity of 
        molecule 'i') */
    QVector<PointPtr> identity_points;

    /** The property map used to find the properties used
        by this constraint */
    PropertyMap map;
    
    /** The prices of best npoints+nbuffer molecules for
        each point (the prices contain the identities
        of the molecules) - a QMap is used to ensure that
        the prices are always sorted */
    QVector< QMap<detail::IdentityPrice,void*> > identity_prices;
    
    /** The number of buffer points to record - this is used
        as an optimisation (so that the prices of all molecules
        against all points don't need to be saved or recorded) */
    quint32 nbuffer;
};

}

Q_DECLARE_METATYPE( SireSystem::IdentityConstraint )

SIRE_EXPOSE_CLASS( SireSystem::IdentityConstraint )

SIRE_END_HEADER

#endif
