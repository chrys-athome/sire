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

#ifndef SIREFF_TWOBODYFUNCTIONAL_HPP
#define SIREFF_TWOBODYFUNCTIONAL_HPP

#include "ff.h"

namespace SireFF
{

namespace detail
{

/** This is the template-independent base class of FFMolecule3D */
class SIREFF_EXPORT FFMolecule3DBase : public FFMolecule
{
protected:
    FFMolecule3DBase();

    FFMolecule3DBase(const PartialMolecule &molecule,
                     const ParameterMap &map);
                     
    FFMolecule3DBase(const FFMolecule3DBase &other);
    
    ~FFMolecule3DBase();
    
    FFMolecule3DBase& operator=(const FFMolecule3DBase &other);
    
private:
    /** The 3D coordinates of all of the atoms in this molecule 
        arranged into CutGroup */
    CoordGroupArray coords;
    
    /** The name of the property to use to get the coordinates */
    PropertyName coords_property;
    
    /** The AABox that completely encloses this molecule */
    AABox aabox;
};

/** This implementation class holds the necessary information
    about a molecule in a three dimensional forcefield (of type T).
    
    @author Christopher Woods
*/
template<class T>
class FFMolecule3D : public FFMolecule3DBase
{
public:
    FFMolecule3D();

    FFMolecule3D(const PartialMolecule &molecule,
                 const T &forcefield,
                 const ParameterMap &map);
    
    FFMolecule3D(const FFMolecule3D<T> &other);

    ~FFMolecule3D();

    FFMolecule3D<T> change(const PartialMolecule &newmol,
                           T &forcefield ) const;

    FFMolecule3D<T> add(const PartialMolecule &newparts,
                        T &forcefield) const;

    FFMolecule3D<T> remove(const PartialMolecule &molecule,
                           T &forcefield) const;

    FFMolecule3D<T> changeParameters(const ParameterMap &map,
                                     T &forcefield) const;
                                        
    FFMolecule3D<T> getDifferences(const FFMolecule3D<T> &other) const;

   
private:
    /** The parameters for all of the atoms of this view */
    typename T::Parameters params;
};

/** This holds a collection of molecules */
template<class T>
class FFMolecules3D
{
public:

    typename T::ChangedMolecule change(const PartialMolecule &molecule,
                                       T &forcefield);
                                       
    typename T::ChangedMolecule add(const PartialMolecule &molecule,
                                    T &forcefield);
                                    
    typename T::ChangedMolecule remove(const PartialMolecule &molecule,
                                       T &forcefield);

    const QVector<MolNum>& molNumsByIndex() const;
    const QVector<AABox>& aaBoxesByIndex() const;

    const typename QVector<T::Molecule>& moleculesByIndex() const;

    const typename QVector<T::Parameters>& parameterNamesByIndex() const;
    
    quint32 indexOf(MolNum molnum) const;
    
    bool contains(MolNum molnum) const;
    
private:
    /** The numbers of all of the molecules in this group */
    QVector<MolNum> molnums_by_index;
    
    /** The AABoxes that enclose each molecule in this group */
    QVector<AABox> aaboxes_by_idx;
    
    /** All of the molecules in this group */
    typename QVector<T::Molecule> mols_by_idx;
    
    /** The current parameters locations used for each molecule */
    typename QVector<T::Parameters> parameter_names;
    
    /** Index mapping the number of a molecule to its position
        in the above array */
    QHash<MolNum,quint32> mols_by_num;
};

}

/** This is the base class of all two-body potential functions. These
    provide a potential energy surface that is composed from just
    two-body terms that act between pairs of atoms.
  
    This is a curiously backwards template class, as a lot of
    the extra definitions required for this functional are provided
    by the class that inherits from it!
        
    @author Christopher Woods
*/
template<class TwoBodyFunc>
class TwoBodyFunctional
{

public:    
    ~TwoBodyFunctional();
    
    /** Two body functionals must implement the following functions */
    //
    //
    //
    //
    
    /** Two body 3D functionals must implement the following functions */
    //
    //
    //
    //

protected:
    TwoBodyFunctional();
    
    TwoBodyFunctional(const TwoBodyFunctional &other);


};


}

#endif
