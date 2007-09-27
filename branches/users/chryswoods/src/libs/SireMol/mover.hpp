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

#ifndef SIREMOL_MOVER_HPP
#define SIREMOL_MOVER_HPP

#include "mover.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This manipulator class is used to move the atoms in 
    a molecule view object. 
    
    @author Christopher Woods
*/
template<class T>
class Mover : public T, public MoverBase
{
public:
    Mover(const T &view);
    Mover(const T &view, const AtomSelection &movable_atoms);
    
    Mover(const Mover<T> &other);
    
    ~Mover();
    
    Mover<T>& operator=(const Mover<T> &other);
    Mover<T>& operator=(const T &other);
    
    T commit() const;
    
    Mover<T>& mapInto(const Space &space,
                      const PropertyMap &map = PropertyMap());
    
    Mover<T>& translate(const Vector &delta,
                        const PropertyMap &map = PropertyMap());
    
    Mover<T>& rotate(const Quaternion &quat, const Vector &point,
                     const PropertyMap &map = PropertyMap());
                     
    Mover<T>& rotate(const Matrix &rotmat, const Vector &point,
                     const PropertyMap &map = PropertyMap());
                                 
    Mover<T>& change(const BondID &bond, SireUnits::Dimension::Length delta,
                     const PropertyMap &map = PropertyMap());
                     
    Mover<T>& change(const AngleID &angle, SireUnits::Dimension::Angle delta,
                     const PropertyMap &map = PropertyMap());
                     
    Mover<T>& change(const DihedralID &dihedral, SireUnits::Dimension::Angle delta,
                     const PropertyMap &map = PropertyMap());
    
    Mover<T>& change(const BondID &bond, SireUnits::Dimension::Angle delta,
                     const PropertyMap &map = PropertyMap());
                                                       
    Mover<T>& change(const ImproperID &improper, SireUnits::Dimension::Angle delta,
                     const PropertyMap &map = PropertyMap());
                     
    Mover<T>& set(const BondID &bond, SireUnits::Dimension::Length value,
                  const PropertyMap &map = PropertyMap());
                  
    Mover<T>& set(const AngleID &angle, SireUnits::Dimension::Angle value,
                  const PropertyMap &map = PropertyMap());
                  
    Mover<T>& set(const DihedralID &dihedral, SireUnits::Dimension::Angle value,
                  const PropertyMap &map = PropertyMap());
                  
    Mover<T>& setAll(const DihedralID &dihedral, SireUnits::Dimension::Angle value,
                     const PropertyMap &map = PropertyMap());
                     
    Mover<T>& set(const ImproperID &improper, SireUnits::Dimension::Angle value,
                  const PropertyMap &map = PropertyMap());

    Mover<T>& align(const MoleculeView &other, 
                    const AtomAliases &aliases = AtomAliases());
                    
    Mover<T>& align(const MoleculeView &other,
                    const AtomSelection &aligning_atoms,
                    const AtomAliases &aliases = AtomAliases());
};

/** Construct a mover that can move all of the atoms
    in the view 'view' */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>::Mover(const T &view) : T(view), MoverBase(view.selectedAtoms())
{}

/** Construct a mover that can move the 'movable_atoms' of the 
    view 'view' (note that movable_atoms really should be a subset
    of 'view', but that this class does not check that this is the case) */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>::Mover(const T &view, const AtomSelection &movable_atoms)
         : T(view), MoverBase(movable_atoms)
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>::Mover(const Mover<T> &other)
         : T(other) : MoverBase(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>::~Mover()
{}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::operator=(const Mover<T> &other)
{
    T::operator=(other);
    MoverBase::operator=(other);
    return *this;
}

/** Copy assignment operator */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::operator=(const T &other)
{
    T::operator=(other);
    MoverBase::setMovableAtoms(other.selectedAtoms());
    
    return *this;
}

/** Commit this Mover - this returns the new copy of
    the Molecule after it has been moved */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
T Mover<T>::commit()
{
    return *this;
}

/** Map the coordinates of the *whole* molecule into the space
    'space', using the supplied property map to find the required
    properties. Note that this is the only Mover function that moves
    *all* atoms of the molecule, regardless of whether or not they
    are specified as being movable, or whether or not they are
    in the parent view */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::mapInto(const Space &space, const PropertyMap &map)
{
    MoverBase::mapInto(*d, space, map);
    return *this;
}

/** Translate the movable atoms by 'delta', using the supplied
    property map to find the necessary properties */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::translate(const Vector &delta, const PropertyMap &map)
{
    MoverBase::translate(*d, delta, map);
    return *this;
}

/** Rotate the movable atoms using the quaternion 'quat' about the 
    point 'point', using the supplied map to find the necessary 
    properties */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::rotate(const Quaternion &quat, const Vector &point,
                           const PropertyMap &map)
{
    MoverBase::rotate(*d, quat, point, map);
    return *this;
}
                 
/** Rotate the movable atoms using the rotation matrix 'rotmat' about
    the point 'point', using the supplied map to find the necessary
    properties */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::rotate(const Matrix &rotmat, const Vector &point,
                           const PropertyMap &map)
{
    MoverBase::rotate(*d, rotmat, point, map);
    return *this;
}
    
/** Change the bond identified by 'bond' by the amount 'delta',
    by only moving the movable atoms in this view, using the 
    supplied map to find the necessary properties.
    
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/                         
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::change(const BondID &bond, double delta,
                           const PropertyMap &map)
{
    MoverBase::change(*d, bond, delta, map);
    return *this;
}
                 
/** Change the angle identified by 'angle' by the amount 'delta',
    by only moving the movable atoms in this view, using the 
    supplied map to find the necessary properties
    
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::change(const AngleID &angle, SireUnits::Dimension::Angle delta,
                           const PropertyMap &map)
{
    MoverBase::change(*d, angle, delta, map);
    return *this;
}
                 
/** Change the dihedral identified by 'dihedral' by the amount 'delta',
    by only moving the movable atoms in this view, using the 
    supplied map to find the necessary properties
    
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::change(const DihedralID &dihedral, 
                           SireUnits::Dimension::Angle delta,
                           const PropertyMap &map)
{
    MoverBase::change(*d, dihedral, delta, map);
    return *this;
}

/** Rotate all of the atoms around the bond identified by 'bond' 
    by the amount 'delta', by only moving the movable atoms in 
    this view, using the supplied map to find the necessary properties
    
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::change(const BondID &bond, SireUnits::Dimension::Angle delta,
                           const PropertyMap &map)
{
    MoverBase::change(*d, bond, delta, map);
    return *this;
}
                                                   
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::change(const ImproperID &improper, SireUnits::Dimension::Angle delta,
                 const PropertyMap &map = PropertyMap());
                 
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::set(const BondID &bond, double value,
              const PropertyMap &map = PropertyMap());
              
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::set(const AngleID &angle, SireUnits::Dimension::Angle value,
              const PropertyMap &map = PropertyMap());
              
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::set(const DihedralID &dihedral, SireUnits::Dimension::Angle value,
              const PropertyMap &map = PropertyMap());
              
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::setAll(const DihedralID &dihedral, SireUnits::Dimension::Angle value,
                 const PropertyMap &map = PropertyMap());
                 
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::set(const ImproperID &improper, SireUnits::Dimension::Angle value,
              const PropertyMap &map = PropertyMap());

template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::align(const MoleculeView &other, 
                const AtomAliases &aliases = AtomAliases());
                
template<class T>
SIRE_OUTOFLINE_TEMPLATE
Mover<T>& Mover<T>::align(const MoleculeView &other,
                const AtomSelection &aligning_atoms,
                const AtomAliases &aliases = AtomAliases());


}

SIRE_END_HEADER

#endif
