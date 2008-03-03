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

#ifndef SIREFF_DETAIL_FFMOLECULES3D_H
#define SIREFF_DETAIL_FFMOLECULES3D_H

#include "ffmolecules.h"

#include "SireVol/coordgroup.h"
#include "SireVol/aabox.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
namespace detail
{
template<class PTNL>
class FFMolecule3D;
template<class PTNL>
class FFMolecules3D;
}
}

namespace SireBase
{
class PropertyName;
}

namespace SireFF
{

namespace detail
{

using SireVol::CoordGroup;
using SireVol::CoordGroupArray;
using SireVol::CoordGroupArrayArray;

using SireVol::AABox;

using SireBase::PropertyName;

/** This is the base class of all parameters objects for 3D potentials.
    It holds the coordinates property (and is responsible for extracting
    the coordinates property)
    
    @author Christopher Woods
*/
class SIREFF_EXPORT FFParameters3D
{
public:
    FFParameters3D();
    
    FFParameters3D(const PartialMolecule &molecule,
                   const PropertyName &coords_property);
    
    FFParameters3D(const CoordGroupArray &coords);
    
    template<class T>
    FFParameters3D(const PartialMolecule &molecule,
                   const T &parameternames);
                   
    FFParameters3D(const FFParameters3D &other);
    
    ~FFParameters3D();
    
    FFParameters3D& operator=(const FFParameters3D &other);
    
    bool operator==(const FFParameters3D &other);
    bool operator!=(const FFParameters3D &other);
    
    const CoordGroupArray& coordinates() const;
    
    void setCoordinates(const CoordGroupArray &coords);
    
    bool changedAllGroups(const FFParameters3D &params) const;
    
    QSet<quint32> getChangedGroups(const FFParameters3D &params) const;
    
    void addChangedGroups(const FFParameters3D &params,
                          QSet<quint32> &changed_groups) const;
    
    FFParameters3D applyMask(const QSet<quint32> &idxs) const;
    
protected:
    static bool selectedAll(quint32 n);
    
private:
    /** The 3D coordinates of all of the atoms in this forcefield,
        arranged by CutGroup */
    CoordGroupArray coords;
};

/** This class holds 3D parameters, one for each atom 

    @author Christopher Woods
*/
template<class PARAM>
class AtomicParameters : public FFParameters3D
{
public:
    AtomicParameters();
    
    AtomicParameters(const PartialMolecule &molecule,
                     const PropertyName &coords_property);
     
    AtomicParameters(const FFParameters3D &params3d,
                     const PackedArray2D<PARAM> &parameters);
    
    AtomicParameters(const AtomicParameters<PARAM> &other);
    
    ~AtomicParameters();
    
    AtomicParameters<PARAM>& operator=(const AtomicParameters<PARAM> &other);
    
    bool operator==(const AtomicParameters<PARAM> &other) const;
    bool operator!=(const AtomicParameters<PARAM> &other) const;

    const PackedArray2D<PARAM>& cljParameters() const;

    void setAtomicParameters(const PackedArray2D<PARAM> &parameters);

    bool changedAllGroups(const AtomicParameters<PARAM> &params) const;
    
    QSet<quint32> getChangedGroups(const AtomicParameters<PARAM> &params) const;
    
    void addChangedGroups(const AtomicParameters<PARAM> &params,
                          QSet<quint32> &changed_groups) const;
    
    AtomicParameters<PARAM> applyMask(const QSet<quint32> &idxs) const;

    void assertCompatible(const PackedArray2D<PARAM> &params) const;

protected:
    /** The atomic parameters, arranged by CutGroup */
    PackedArray2D<PARAM> cljparams;
};

/** This class holds a 3D molecule for a 3n D potential energy
    surface (each atom has 3 degrees of freedom, x, y, z). This
    include the 3D atomic coordinates of the molecule, together
    with a 3D axis-aligned bounding box that encompasses all
    of the atoms in this view
    
    PTNL is the underlying potential energy class that 
    is used to calculate the energy of the group
    of molecules (e.g. CLJFunctional)
    
    @author Christopher Woods
*/
template<class PTNL>
class FFMolecule3D : public FFMolecule<PTNL>
{

friend class FFMolecules3D<PTNL>;  //so can call setCoordinates(...)

public:
    typedef typename FFMolecule<PTNL>::Parameters Parameters;
    typedef typename FFMolecule<PTNL>::ParameterNames ParameterNames;

    FFMolecule3D();
    
    FFMolecule3D(const PartialMolecule &molecule,
                 PTNL &forcefield,
                 const ParameterNames &parameters);
    
    FFMolecule3D(const FFMolecule3D<PTNL> &other);
    
    ~FFMolecule3D();
    
    FFMolecule3D<PTNL> operator=(const FFMolecule3D<PTNL> &other);
    
    bool operator==(const FFMolecule3D<PTNL> &other) const;
    bool operator!=(const FFMolecule3D<PTNL> &other) const;
    
    const CoordGroupArray& coordinates() const;
    
    const AABox& aaBox() const;
    
    bool change(const PartialMolecule &molecule,
                PTNL &forcefield,
                const ParameterNames &parameternames);
    
    bool change(const PartialMolecule &molecule,
                const ParameterNames &new_paramnames,
                PTNL &forcefield,
                const ParameterNames &old_paramnames);

    bool add(const AtomSelection &selected_atoms,
             PTNL &forcefield,
             const ParameterNames &parameternames);

    bool remove(const AtomSelection &selected_atoms,
                PTNL &forcefield,
                const ParameterNames &parameternames);

    FFMolecule3D<PTNL> getDifferences(const FFMolecule<PTNL> &other) const;

protected:
    FFMolecule3D(const FFMolecule<PTNL> &other);
    
    void setCoordinates(const CoordGroupArray &coords);
    
private:
    /** The bounding box that fully encompasses these coordinates */
    AABox aabox;
};

/** This class holds a group of 3D molecules - these are molecules
    that use 3D atomic coordinates and are bounded by 3D axis-aligned
    bounding boxes
    
    @author Christopher Woods
*/
template<class PTNL>
class FFMolecules3D : public FFMolecules<PTNL>
{
public:
    typedef typename FFMolecules<PTNL>::Molecule Molecule;
    typedef typename FFMolecules<PTNL>::ChangedMolecule ChangedMolecule;
    typedef typename FFMolecules<PTNL>::ParameterNames ParameterNames;

    FFMolecules3D();
    
    FFMolecules3D(const FFMolecules3D<PTNL> &other);
    
    ~FFMolecules3D();
    
    FFMolecules3D<PTNL>& operator=(const FFMolecules3D<PTNL> &other);
    
    bool operator==(const FFMolecules3D<PTNL> &other) const;
    bool operator!=(const FFMolecules3D<PTNL> &other) const;
    
    const QVector<AABox>& aaBoxesByIndex() const;
    
    void packCoordinates();

    ChangedMolecule change(const PartialMolecule &molecule,
                           PTNL &forcefield,
                           bool record_changes = true);
                           
    ChangedMolecule change(const PartialMolecule &molecule,
                           const ParameterNames &paramnames,
                           PTNL &forcefield,
                           bool record_changes = true);
                           
    ChangedMolecule add(const PartialMolecule &molecule,
                        const ParameterNames &paramnames,
                        PTNL &forcefield,
                        bool record_changes = true);
                        
    ChangedMolecule remove(const PartialMolecule &molecule,
                           PTNL &forcefield,
                           bool record_changes = true);
    
private:
    void updateAABox(MolNum molnum);

    /** The bounding boxes that completely encompasses all of  
        the atoms in each molecule */
    QVector<AABox> aaboxes_by_idx;
};

///////
/////// Implementation of FFParameters3D
///////

/** Construct from a molecule and the passed parameter property sources */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
FFParameters3D::FFParameters3D(const PartialMolecule &molecule,
                               const T &properties)
{
    this->operator=( FFParameters3D(molecule, properties.coordinates()) );
}

/** Return the 3D coordinates of the atoms in the forcefield */
inline const CoordGroupArray& FFParameters3D::coordinates() const
{
    return coords;
}

///////
/////// Implementation of FFMolecule3D<PTNL>
///////

/** Null constructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule3D<PTNL>::FFMolecule3D() : FFMolecule<PTNL>()
{}

/** Construct for the passed molecule 

    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule3D<PTNL>::FFMolecule3D(const PartialMolecule &molecule,
                                 PTNL &forcefield,
                                 const ParameterNames &parameters)
                   : FFMolecule<PTNL>(molecule, forcefield, parameters)
{
    //get the AABox
    aabox = this->params.coordinates().aaBox();
}

/** Private constructor used to create from an FFMolecule<PTNL> */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule3D<PTNL>::FFMolecule3D(const FFMolecule<PTNL> &ffmol)
                   : FFMolecule<PTNL>(ffmol)
{
    aabox = this->params.coordinates().aaBox();
}

/** Copy constructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule3D<PTNL>::FFMolecule3D(const FFMolecule3D<PTNL> &other)
                   : FFMolecule<PTNL>(other),
                     aabox(other.aabox)
{}

/** Destructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule3D<PTNL>::~FFMolecule3D()
{}

/** Copy assignment operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule3D<PTNL> FFMolecule3D<PTNL>::operator=(const FFMolecule3D<PTNL> &other)
{
    if (this != &other)
    {
        FFMolecule<PTNL>::operator=(other);
        aabox = other.aabox;
    }
    
    return *this;
}

/** Comparison operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule3D<PTNL>::operator==(const FFMolecule3D<PTNL> &other) const
{
    return FFMolecule<PTNL>::operator==(other);
}

/** Comparison operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule3D<PTNL>::operator!=(const FFMolecule3D<PTNL> &other) const
{
    return FFMolecule<PTNL>::operator!=(other);
}

/** Return the coordinate of the atoms of this molecule, arranged
    into CoordGroups. Note that only CoordGroups that contain at
    least one selected atom are included in this list */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
const CoordGroupArray& FFMolecule3D<PTNL>::coordinates() const
{
    return this->params.coordinates();
}

/** Return the AABox that encloses all of the atoms whose coordinates
    are returned in 'coordinates()' (this includes atoms that are in
    the same CutGroup as selected atoms, but are not themselves selected) */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
const AABox& FFMolecule3D<PTNL>::aaBox() const
{
    return aabox;
}

/** Change this molecule so that it is equal to 'new_molecule'.

    Note that you can only change a molecule's molecule layout ID
    is *all* of the molecule is contained in this forcefield.
    If only part of the molecule is contained, then you will
    need to remove it, and then add it again to be able to
    change its layout ID.

    The names of the properties used to originally get the 
    parameters for this molecule are in 'parameternames'
    
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule3D<PTNL>::change(const PartialMolecule &molecule,
                                PTNL &forcefield,
                   const typename FFMolecule3D<PTNL>::ParameterNames &parameternames)
{
    if (FFMolecule<PTNL>::change(molecule, forcefield, parameternames))
    {
        //we need to update the global AABox
        aabox = this->params.coordinates().aaBox();
        
        return true;
    }
    else
        return false;
}

/** Change this molecule so that it is equal to 'new_molecule' and
    so that it gets its parameters from 'new_parameternames'

    The names of the properties used to originally get the 
    parameters for this molecule are in 'parameternames'
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule3D<PTNL>::change(const PartialMolecule &molecule,
            const typename FFMolecule3D<PTNL>::ParameterNames &new_paramnames,
                                PTNL &forcefield,
            const typename FFMolecule3D<PTNL>::ParameterNames &old_paramnames)
{
    if (FFMolecule<PTNL>::change(molecule, new_paramnames,
                                 forcefield, old_paramnames))
    {
        //we need to update the aabox
        aabox = this->params.coordinates().aaBox();
        return true;
    }
    else
        return false;
}

/** Add the selected atoms in 'added_atoms' to this view.

    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule3D<PTNL>::add(const AtomSelection &selected_atoms,
                             PTNL &forcefield,
                const typename FFMolecule3D<PTNL>::ParameterNames &parameternames)
{   
    if (FFMolecule<PTNL>::add(selected_atoms, forcefield, parameternames))
    {
        //update the aabox
        aabox = this->params.coordinates().aaBox();
        return true;
    }
    else
        return false;
}

/** Remove the selected atoms 'removed_atoms' from this molecule.

    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecule3D<PTNL>::remove(const AtomSelection &selected_atoms,
                                PTNL &forcefield,
                    const typename FFMolecule<PTNL>::ParameterNames &parameternames)
{
    if (FFMolecule<PTNL>::remove(selected_atoms, forcefield, parameternames))
    {
        //update the aabox
        aabox = this->params.coordinates().aaBox();
        return true;
    }
    else
        return false;
}

/** Return the differences between this molecule and 'newmol'. This returns
    the parts of this molecule that are different to 'newmol'. 
    
    \throw SireError::incompatible_error
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecule3D<PTNL> 
FFMolecule3D<PTNL>::getDifferences(const FFMolecule<PTNL> &newmol) const
{
    return FFMolecule3D<PTNL>( FFMolecule<PTNL>::getDifferences(newmol) );
}

/** Set the coordinates - this is used by FFMolecules3D when it 
    packs the coordinates together into a single contiguous block of memory */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
void FFMolecule3D<PTNL>::setCoordinates(const CoordGroupArray &coords)
{
    this->params.setCoordinates(coords);
}

///////
/////// Implementation of FFMolecules3D<PTNL>
///////

/** Null constructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecules3D<PTNL>::FFMolecules3D() : FFMolecules<PTNL>()
{}

/** Copy constructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecules3D<PTNL>::FFMolecules3D(const FFMolecules3D<PTNL> &other)
                    : FFMolecules<PTNL>(other),
                      aaboxes_by_idx(other.aaboxes_by_idx)
{}

/** Destructor */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecules3D<PTNL>::~FFMolecules3D()
{}

/** Copy assignment operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
FFMolecules3D<PTNL>& FFMolecules3D<PTNL>::operator=(const FFMolecules3D<PTNL> &other)
{
    FFMolecules<PTNL>::operator=(other);
    aaboxes_by_idx = other.aaboxes_by_idx;
    return *this;
}

/** Comparison operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecules3D<PTNL>::operator==(const FFMolecules3D<PTNL> &other) const
{
    return FFMolecules<PTNL>::operator==(other);
}

/** Comparison operator */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
bool FFMolecules3D<PTNL>::operator!=(const FFMolecules3D<PTNL> &other) const
{
    return FFMolecules<PTNL>::operator!=(other);
}

/** Return the array of AABoxes that enclose each molecule in this group,
    ordered in the same order as the molecules in this group */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
const QVector<AABox>& FFMolecules3D<PTNL>::aaBoxesByIndex() const
{
    return aaboxes_by_idx;
}

/** Pack the coordinates of all of the molecules in this group together
    into a single array. This doesn't change the coordinates - it just
    moves them all to occupy a contiguous block of memory. This may 
    improve the speed of iterating over the coordinates, particularly
    if during the course of a long simulation they have all become
    fragmented over a wide range of memory */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
void FFMolecules3D<PTNL>::packCoordinates()
{
    //first collect the coordinates together into an array
    int nmols = this->mols_by_idx.count();
    
    if (nmols == 0)
        return;
        
    typename FFMolecules3D<PTNL>::Molecule *mols_array = this->mols_by_idx.data();
    
    QVector<CoordGroupArray> coords_array(nmols);
    
    for (int i=0; i<nmols; ++i)
    {
        coords_array[nmols] = mols_array[i].coordinates();
    }
    
    //now convert the array of CoordGroupArrays into a single
    //CoordGroupArrayArray
    CoordGroupArrayArray all_coords( coords_array );
    
    const CoordGroupArray *all_coords_array = all_coords.constData();

    BOOST_ASSERT(all_coords.count() == nmols);
    
    //give each molecule its packed CoordGroupArray
    for (int i=0; i<nmols; ++i)
    {
        mols_array[i].setCoordinates(all_coords_array[i]);
    }
}

/** Update the AABox for the molecule with number 'molnum'. This does
    nothing if this molecule is not in this group */
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
void FFMolecules3D<PTNL>::updateAABox(MolNum molnum)
{
    QHash<MolNum,quint32>::const_iterator it = FFMoleculesBase::indexesByMolNum()
                                                        .constFind(molnum);
                                                        
    if (it != FFMoleculesBase::indexesByMolNum().constEnd())
    {
        aaboxes_by_idx.data()[*it] = this->mols_by_idx.constData()[*it].aaBox();
    }
}

/** Change the molecule 'new_molecule' (which is in the forcefield 'forcefield').
    This does nothing if this molecule is not in this group. If 
    'record_changes' is true then this returns a ChangedMolecule that
    records the change from the current version of the molecule 
    to 'new_molecule'. If there is no change, or record_changes is false,
    then a null (isEmpty()) ChangedMolecule is returned

    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
typename FFMolecules3D<PTNL>::ChangedMolecule 
FFMolecules3D<PTNL>::change(const PartialMolecule &molecule,
                            PTNL &forcefield, bool record_changes)
{
    ChangedMolecule changed_mol = FFMolecules<PTNL>::change(molecule, forcefield,
                                                            record_changes);
                                                            
    //update the AABox for this molecule
    this->updateAABox(molecule.number());
    
    return changed_mol;
}
                       
/** Change the molecule 'new_molecule' (which is in the forcefield 'forcefield'),
    also changing the source of its parameters to 'new_params'.
    This does nothing if this molecule is not in this group. If 
    'record_changes' is true then this returns a ChangedMolecule that
    records the change from the current version of the molecule 
    to 'new_molecule'. If there is no change, or record_changes is false,
    then a null (isEmpty()) ChangedMolecule is returned

    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
typename FFMolecules3D<PTNL>::ChangedMolecule 
FFMolecules3D<PTNL>::change(const PartialMolecule &molecule,
                      const typename FFMolecules3D<PTNL>::ParameterNames &paramnames,
                            PTNL &forcefield, bool record_changes)
{
    ChangedMolecule changed_mol = FFMolecules<PTNL>::change(molecule, paramnames,
                                                            forcefield, record_changes);
                                                            
    //update the AABox for this molecule
    this->updateAABox(molecule.number());
    
    return changed_mol;
}

/** Add the molecule 'molecule' to this group, getting the parameters
    for the molecule using the parameter names listed in 'paramnames'.
    
    If this molecule already exists in this group then this will
    add any parts of 'molecule' that don't already exist, but this
    will *not* update the existing molecule so that it is at the 
    same version. If you want to add to the same version, then
    you must call change(molecule, ffield) then add(molecule, ffield).
    
    Note also that you *cannot* add this molecule if the existing
    version has a different molecule layout ID.
    
    Note also that you *cannot* change the source of the parameters
    if this molecule exists already in the forcefield. To change
    parameter sources for a molecule that is already in the forcefield
    you must remove the molecule and then add it from scratch.

    If 'record_changes' is true then this returns a ChangedMolecule
    that records the change. If there is no change, or if 'record_changes'
    is false, then a null ChangedMolecule is returned
    
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/                                                        
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
typename FFMolecules3D<PTNL>::ChangedMolecule
FFMolecules3D<PTNL>::add(const PartialMolecule &molecule,
                    const typename FFMolecules3D<PTNL>::ParameterNames &paramnames,
                         PTNL &forcefield, bool record_changes)
{
    ChangedMolecule changed_mol = FFMolecules<PTNL>::add(molecule, paramnames,
                                                         forcefield, record_changes);
                                                         
    if (changed_mol.oldMolecule().isEmpty())
    {
        //the molecule has been added from scratch - create space
        //for the AABox
        BOOST_ASSERT( FFMoleculesBase::molNumsByIndex().count() ==
                      aaboxes_by_idx.count() + 1 );
                      
        aaboxes_by_idx.append( changed_mol.newMolecule().aaBox() );
    }
    else
    {
        //the molecule has just been altered
        this->updateAABox(molecule.number());
    }
    
    return changed_mol;
}

/** Remove the molecule 'molecule' from this forcefield. This only
    removes the selected atoms in 'molecule', and this molecule must
    have the same molecule layout version. Note that this does not
    update the version of the molecule in this set to be the 
    same as 'molecule'
    
    If 'record_changes' is true then this records the change
    caused by the removal of the atoms and returns it in the
    returned ChangedMolecule. If there is no change, or if
    'record_changes' is false, then a null ChangedMolecule
    is returned.
    
    \throw SireError::incompatible_error
    \throw SireError::invalid_cast
    \throw SireBase::missing_property
*/        
template<class PTNL>
SIRE_OUTOFLINE_TEMPLATE
typename FFMolecules3D<PTNL>::ChangedMolecule
FFMolecules3D<PTNL>::remove(const PartialMolecule &molecule,
                            PTNL &forcefield, bool record_changes)
{
    //get the index of this molecule
    QHash<MolNum,quint32>::const_iterator it = FFMoleculesBase::indexesByMolNum()
                                                    .constFind(molecule.number());
                                                    
    if (it == FFMoleculesBase::indexesByMolNum().constEnd())
        //this molecule is not in this forcefield!
        return ChangedMolecule();

    quint32 idx = *it;

    ChangedMolecule changed_mol = FFMolecules<PTNL>::remove(molecule, forcefield,
                                                            record_changes);
                                                       
    if (changed_mol.newMolecule().isEmpty())
    {
        //the molecule has been completely removed - remove the AABox
        aaboxes_by_idx.remove(idx);
        
        BOOST_ASSERT( aaboxes_by_idx.count() ==
                      FFMoleculesBase::molNumsByIndex().count() );
    }
    
    return changed_mol;
}

} // end of namespace SireFF::detail

} // end of namespace SireFF

SIRE_END_HEADER

#endif
