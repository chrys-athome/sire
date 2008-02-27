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

#include "detail/ffmolecules.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

namespace detail
{

/** This class holds a 3D molecule for a 3D forcefield. This
    include the 3D atomic coordinates of the molecule, together
    with a 3D axis-aligned bounding box that encompasses all
    of the atoms in this view
    
    @author Christopher Woods
*/
template<class FFIELD>
class FFMolecule3D : FFMolecule<FFIELD>
{
public:
    FFMolecule3D();
    
    FFMolecule3D(const PartialMolecule &molecule,
                 FFIELD &forcefield,
                 const ParameterNames &parameters);
    
    FFMolecule3D(const FFMolecule3D<FFIELD> &other);
    
    ~FFMolecule3D();
    
    FFMolecule3D<FFIELD> operator=(const FFMolecule3D<FFIELD> &other);
    
    bool operator==(const FFMolecule3D<FFIELD> &other) const;
    bool operator!=(const FFMolecule3D<FFIELD> &other) const;
    
    const CoordGroupArray& coordinates() const;
    
    const AABox& aaBox() const;
    
    void change(const PartialMolecule &molecule,
                FFIELD &forcefield,
                const FFIELD::ParameterNames &parameters);
    
private:
    /** The bounding box that fully encompasses these coordinates */
    AABox aabox;

    /** The 3D coordinates of all of the atoms in this view */
    CoordGroupArray coords;
};

/** This class holds a group of 3D molecules - these are molecules
    that use 3D atomic coordinates and are bounded by 3D axis-aligned
    bounding boxes
    
    @author Christopher Woods
*/
template<class FFIELD>
class FFMolecules3D : public FFMolecules<FFIELD>
{
public:
    FFMolecules3D();
    
    const QVector<AABox>& aaBoxesByIndex() const;
    
    void packCoordinates();
    
private:
    /** The bounding boxes that completely encompasses all of  
        the atoms in each molecule */
    QVector<AABox> aaboxes_by_idx;
};

}

}

SIRE_END_HEADER

#endif
