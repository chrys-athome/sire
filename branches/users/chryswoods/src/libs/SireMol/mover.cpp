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

#include "mover.h"

using namespace SireMol;

/** Constructor */
MoverBase::MoverBase()
{}

/** Copy constructor */
MoverBase::MoverBase(const MoverBase &other)
          : movable_atoms(other.movable_atoms)
{}

/** Destructor */
MoverBase::~MoverBase()
{}

/** Copy assignment operator */
MoverBase& MoverBase::operator=(const MoverBase &other)
{
    movable_atoms = other.movable_atoms;
    return *this;
}

/** Translate the selected atoms from 'coords' by 'delta'.
    This function assumes that 'selected_atoms' is compatible
    with 'coords' */
void MoverBase::translate(AtomicCoords &coords,
                          const AtomSelection &selected_atoms,
                          const Vector &delta)
{
    if (delta.isZero() or selected_atoms.selectedNone())
        return;

    CoordGroup *coords_array = coords.data();
    int ncg = coords.count();
    
    if (selected_atoms.selectedAll())
    {
        //we are moving everything!
        for (int i=0; i<ncg; ++i)
        {
            coords_array[i] = coords_array[i].edit().translate(delta);
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        //we are moving all of the CutGroups
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                coords_array[i] = coords_array[i].edit().translate(delta);
            }
            else
            {
                QSet<Index> atoms_to_move = selected_atoms.selectedAtoms(i);
                
                coords_array[i] = coords_array[i].edit().translate(atoms_to_move,
                                                                   delta);
            }
        }
    }
    else
    {
        //we are moving only some CutGroups
        QSet<CGIdx> cg_to_move = selected_atoms.selectedCutGroups();
        
        foreach (CGIdx i, cg_to_move)
        {
            if (selected_atoms.selectedAll(i))
            {
                coords_array[i] = coords_array[i].edit().translate(delta);
            }
            else
            {
                QSet<Index> atoms_to_move = selected_atoms.selectedAtoms(i);

                coords_array[i] = coords_array[i].edit().translate(atoms_to_move,
                                                                   delta);
            }
        }
    }
}

/** Rotate the coordinates (in 'coords') of the specified selected
    atoms using the rotation matrix 'rotmat' about the point 'point'.
    This function assumes that coords and selected_atoms are compatible */
void MoverBase::rotate(AtomicCoords &coords,
                       const AtomSelection &selected_atoms,
                       const Matrix &rotmat,
                       const Vector &point)
{
    if (selected_atoms.selectedNone())
        return;
    
    CoordGroup *coords_array = coords.data();
    int ncg = coords.count();
                
    if (selected_atoms.selectedAll())
    {
        //we are rotating everything
        for (int i=0; i<ncg; ++i)
        {
            coords_array[i] = coords_array[i].edit().rotate(rotmat,point);
        }
    }
    else if (selected_atoms.selectedAllCutGroups())
    {
        //we are rotating every CutGroup
        for (CGIdx i(0); i<ncg; ++i)
        {
            if (selected_atoms.selectedAll(i))
            {
                coords_array[i] = coords_array[i].edit().rotate(rotmat,point);
            }
            else
            {
                QSet<Index> atoms_to_move = selected_atoms.selectedAtoms(i);
                
                coords_array[i] = coords_array[i].edit().rotate(atoms_to_move,
                                                                rotmat, delta);
            }
        }
    }
    else
    {
        QSet<CGIdx> cg_to_move = selected_atoms.selectedCutGroups();
        
        foreach (CGIdx i, cg_to_move)
        {
            if (selected_atoms.selectedAll(i))
            {
                coords_array[i] = coords_array[i].edit().rotate(rotmat, point);
            }
            else
            {
                QSet<Index> atoms_to_move = selected_atoms.selectedAtoms(i);
                
                coords_array[i] = coords_array[i].edit().rotate(atoms_to_move,
                                                                rotmat, point);
            }
        }
    }
}

/** Translate the selected atoms in the molecule whose data is in 'moldata'
    by 'delta', using 'coord_property' to get the coordinates to 
    be translated. This function assumes that selected_atoms
    and moldata are compatible.
    
    \throw SireBase::missing_property
*/
void MoverBase::translate(MoleculeData &moldata 
                          const AtomSelection &selected_atoms,
                          const Vector &delta, 
                          const PropertyMap &map)
{
    //which property contains the coordinates?
    const PropertyName &coord_property = map["coordinates"];

    //get the current coordinates
    AtomicCoords coords = moldata.property(coord_property);
    
    //translate the coordinates of the selected atoms
    MoverBase::translate(coords, selected_atoms, delta);
    
    //set the new property
    view.setProperty(coord_property, coords);
}

/** Rotate the selected atoms in the molecule whose data
    is in 'moldata' using the rotation matrix 'rotmat' 
    around the point 'point', using 'coord_property'
    to find the property containing the coordinates 
    to be rotated.
    
    This function assumes that moldata and selected_atoms
    are compatible.
    
    \throw SireBase::missing_property
*/
void MoverBase::rotate(MoleculeData &moldata,
                       const AtomSelection &selected_atoms,
                       const Matrix &rotmat,
                       const Vector &point,
                       const PropertyMap &map)
{
    //get the name of the property that contains the coordinates
    const PropertyName &coord_property = map["coordinates"];

    //get the coordinates to be rotated
    AtomicCoords coords = moldata.property(coord_property);
    
    //rotate the coordinates
    MoverBase::rotate(coords, selected_atoms, rotmat, point);
    
    //set the new property
    moldata.setProperty(coord_property, coords);
}

/** Translate atoms we are allowed to move from the molecule whose
    data is in 'moldata' by 'delta', finding the coordinates
    using 'coord_property' 
    
    \throw SireBase::missing_property
*/
void MoverBase::translate(MoleculeData &moldata,      
                          const Vector &delta,
                          const PropertyMap &map) const
{
    MoverBase::translate(moldata, movable_atoms, delta, map);
}

/** Rotate the atoms we are allowed to move from the molecule whose
    data is in 'moldata' using the quaternion 'quat' about the 
    point 'point', finding the coordinates using 'coord_property'
    
    \throw SireBase::missing_property
*/
void MoverBase::rotate(MoleculeData &moldata,
                       const Quaternion &quat,
                       const Vector &point,
                       const PropertyMap &map) const
{
    MoverBase::rotate(moldata, movable_atoms, quat.toMatrix(),
                      point, map);
}

/** Rotate the atoms we are allowed to move from the molecule whose
    data is in 'moldata' using the rotation matrix 'rotmat' about the 
    point 'point', finding the coordinates using 'coord_property'
    
    \throw SireBase::missing_property
*/
void MoverBase::rotate(MoleculeData &moldata,
                       const Matrix &rotmat,
                       const Vector &point,
                       const PropertyMap &map) const
{
    MoverBase::rotate(moldata, movable_atoms, rotmat,
                      point, coord_property);
}

/** Change the length of the bond identified by 'bond' by 'delta',
    in the molecule whose data is in 'moldata', using the supplied
    PropertyMap to locate the necessary properties 
    
    This only moves the movable atoms in this view, and 
    an anchor_error is thrown if it is not possible to make
    this change without moving the unmovable atoms.
    
    \throw SireBase::missing_property
    \throw SireMol::anchor_error
    \throw SireMol::ring_error
*/
void MoverBase::change(MoleculeData &moldata, const BondID &bond,
                       SireUnits::Dimension::Length delta,
                       const PropertyMap &map) const
{
    if (delta == 0)
        return;

    //get the indicies of the atoms that comprise the bond
    tuple<AtomIdx,AtomIdx> atomidxs = bond.map(moldata.info());

    AtomIdx atom0 = atomidxs.get<0>();
    AtomIdx atom1 = atomidxs.get<1>();

    //get the connectivity property that is used to split
    //the molecule into two parts
    Connectivity connectivity = moldata.property(map["connectivity"]);

    //split the molecule into the two parts that are
    //going to move - the two groups are only able to
    //contain the atoms that are in 'movable_atoms'
    tuple<AtomSelection,AtomSelection> groups = 
                        connectivity.split(atom0, atom1, movable_atoms);

    const AtomSelection &group0 = groups.get<0>();
    const AtomSelection &group1 = groups.get<1>();

    //get the weighting function that is used to weight the two
    //sides of the move
    WeightFunction weightfunc = moldata.property(map["weight function"]);
    
    tuple<double,double> weights = weightfunc(moldata, group0, group1);
    
    double weight0 = weights.get<0>();
    double weight1 = weights.get<1>();
    
    //now get property containing the coordinates of the atoms
    PropertyName coord_property = map["coordinates"];
    
    AtomicCoords coords = moldata.property(coord_property);
    
    //use these coordinates to calculate the unit vector that 
    //points along the bond
    Vector unit_vec = (coords[moldata.info().cgAtomIdx(atom1)] - 
                       coords[moldata.info().cgAtomIdx(atom0)]).normalise();

    //now translate the groups along this vector by their weighted
    //amount of delta
    if (weight0 != 0)
        MoverBase::translate(coords, group0, -weight0 * delta);
        
    if (weight1 != 0)
        MoverBase::translate(coords, group1, weight1 * delta);
        
    //save the new coordinates
    moldata.setProperty(coord_property, coords);
}
