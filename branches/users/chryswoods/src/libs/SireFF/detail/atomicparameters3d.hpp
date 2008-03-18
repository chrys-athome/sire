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

#ifndef SIREFF_DETAIL_ATOMICPARAMETERS3D_HPP
#define SIREFF_DETAIL_ATOMICPARAMETERS3D_HPP

#include "atomiccoords3d.h"
#include "atomicparameters.hpp"

SIRE_BEGIN_HEADER

namespace SireFF
{

namespace detail
{

/** This class holds 3D parameters, one for each atom 

    @author Christopher Woods
*/
template<class PARAM>
class AtomicParameters3D : public AtomicParameters<PARAM>,
                           public AtomicCoords3D
{
public:
    typedef typename AtomicParameters<PARAM>::Parameter Parameter;
    typedef typename AtomicParameters<PARAM>::Parameters Parameters;
    typedef typename AtomicParameters<PARAM>::Array Array;

    AtomicParameters3D();
    
    AtomicParameters3D(const PartialMolecule &molecule,
                       const PropertyName &coords_property);
     
    AtomicParameters3D(const AtomicCoords3D &coords,
                       const AtomicParameters<PARAM> &parameters);
    
    AtomicParameters3D(const AtomicParameters3D<PARAM> &other);
    
    ~AtomicParameters3D();
    
    AtomicParameters3D<PARAM>& operator=(const AtomicParameters3D<PARAM> &other);
    
    bool operator==(const AtomicParameters3D<PARAM> &other) const;
    bool operator!=(const AtomicParameters3D<PARAM> &other) const;
    
    void setAtomicParameters(const Parameters &parameters);
    void setAtomicParameters(const AtomicParameters<PARAM> &parameters);

    void setAtomicCoordinates(const CoordGroupArray &coordinates);
    void setAtomicCoordinates(const AtomicCoords3D &coordinates);

    bool changedAllGroups(const AtomicParameters3D<PARAM> &params) const;
    
    QSet<quint32> getChangedGroups(const AtomicParameters3D<PARAM> &params) const;
    
    void addChangedGroups(const AtomicParameters3D<PARAM> &params,
                          QSet<quint32> &changed_groups) const;
    
    AtomicParameters3D<PARAM> applyMask(const QSet<quint32> &idxs) const;

    void assertCompatible(const Parameters &parameters) const;
    void assertCompatible(const CoordGroupArray &coordinates) const;
};

/** Null constructor */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
AtomicParameters3D<PARAM>::AtomicParameters3D()
                          : AtomicParameters<PARAM>(),
                            AtomicCoords3D()
{}

/** Assert that the supplied parameters are compatible with the
    3D parameters already part of this object
    
    \throw SireError::incompatible_error
*/
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
void AtomicParameters3D<PARAM>::assertCompatible(
                    const typename AtomicParameters3D<PARAM>::Parameters &params) const
{
    if (params.count() == this->coordinates().count())
    {
        quint32 ngroups = params.count();
        
        const CoordGroup *cgroup_array = this->coordinates().constData();
        const typename Parameters::Array *params_array = params.constData();
        
        for (quint32 i=0; i<ngroups; ++i)
        {
            if (cgroup_array[i].count() != params_array[i].count())
            {
                SireFF::detail::throwAtomicParametersIncompatibleError(
                        i, cgroup_array[i].count(), params_array[i].count());
            }
        }
    }
    else
        SireFF::detail::throwAtomicParametersIncompatibleError(
                params.count(), this->coordinates().count());
}

/** Construct from the passed 3D coordinates and CLJ parameters */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
AtomicParameters3D<PARAM>::AtomicParameters3D(const AtomicCoords3D &coords3d,
                                              const AtomicParameters<PARAM> &parameters)
              : AtomicParameters<PARAM>(parameters),
                AtomicCoords3D(coords3d)
{
    this->assertCompatible( this->params );
}

/** Copy constructor */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
AtomicParameters3D<PARAM>::AtomicParameters3D(const AtomicParameters3D<PARAM> &other)
                          : AtomicParameters<PARAM>(other),
                            AtomicCoords3D(other)
{}

/** Destructor */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
AtomicParameters3D<PARAM>::~AtomicParameters3D()
{}

/** Copy assignment operator */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
AtomicParameters3D<PARAM>& AtomicParameters3D<PARAM>::operator=(
                                        const AtomicParameters3D<PARAM> &other)
{
    AtomicParameters<PARAM>::operator=(other);
    AtomicCoords3D::operator=(other);
    
    return *this;
}

/** Comparison operator */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
bool AtomicParameters3D<PARAM>::operator==(const AtomicParameters3D<PARAM> &other) const
{
    return AtomicParameters<PARAM>::operator==(other) and
           AtomicCoords3D::operator==(other);
}

/** Comparison operator */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
bool AtomicParameters3D<PARAM>::operator!=(const AtomicParameters3D<PARAM> &other) const
{
    return AtomicParameters<PARAM>::operator!=(other) or
           AtomicCoords3D::operator!=(other);
}

/** Return whether or not all of the groups have changed parameters
    compared to 'params' */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
bool AtomicParameters3D<PARAM>::changedAllGroups(
                                    const AtomicParameters3D<PARAM> &other) const
{
    if (AtomicCoords3D::changedAllGroups() or
        AtomicParameters<PARAM>::changedAllGroups())
    {
        return true;
    }
    
    //get the list of groups that have changed...
    QSet<quint32> changed_idxs = this->getChangedGroups();

    return SireFF::detail::selectedAll(changed_idxs, this->nGroups());
}

/** Add the changed groups from 'params' to the set 'changed_groups' */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
void AtomicParameters3D<PARAM>::addChangedGroups(
                                     const AtomicParameters3D<PARAM> &parameters,
                                     QSet<quint32> &changed_groups) const
{
    //first add on the groups that have changed coordinates
    AtomicCoords3D::addChangedGroups(parameters, changed_groups);

    //now add on the groups that have changed parameters
    if (not SireFF::detail::selectedAll(changed_groups, this->nGroups()))
        AtomicParameters<PARAM>::addChangedGroups(parameters, changed_groups);
}

/** Return the set of groups that have changed parameters */
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
QSet<quint32> AtomicParameters3D<PARAM>::getChangedGroups(
                                    const AtomicParameters3D<PARAM> &parameters) const
{
    QSet<quint32> changed_groups;
    changed_groups.reserve( this->nGroups() );
    
    this->addChangedGroups(parameters, changed_groups);
    
    return changed_groups;
}

/** Mask this set of parameters so that only the CutGroups at indicies 
    'idxs' are present */ 
template<class PARAM>
SIRE_OUTOFLINE_TEMPLATE
AtomicParameters3D<PARAM> AtomicParameters3D<PARAM>::applyMask(
                                            const QSet<quint32> &idxs) const
{
    AtomicCoords3D masked_coords = AtomicCoords3D::applyMask(idxs);
    
    if (masked_coords.atomicCoordinates().count() == 0)
        //all of the groups are masked
        return AtomicParameters3D<PARAM>();
    
    else if (masked_coords.atomicCoordinates().count() 
                    == this->atomicCoordinates().count())
        //none of the groups are masked
        return *this;
        
    return AtomicParameters3D<PARAM>( AtomicParameters<PARAM>::applyMask(idxs), 
                                      masked_coords );
}

} // end of namespace detail

} // end of namespace SireFF

SIRE_END_HEADER

#endif
