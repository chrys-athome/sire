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

#ifndef SIREMOL_MOVER_H
#define SIREMOL_MOVER_H

#include "moleculeview.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

/** This class provides the template-independent part
    of Mover<T>. This class is not designed to be used
    on its own!
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT MoverBase
{
public:
    MoverBase();

    MoverBase(const MoverBase &other);

    ~MoverBase();
    
    MoverBase& operator=(const MoverBase &other);
    
protected:
    void setMovableAtoms(const AtomSelection &movable_atoms);

    void translate(MoleculeData &data,      
                   const Vector &delta,
                   const PropertyMap &map) const;
                   
    void rotate(MoleculeData &data,
                const Quaternion &quat,
                const Vector &point,
                const PropertyMap &map) const;

    void rotate(MoleculeData &data,
                const Matrix &rotmat,
                const Vector &point,
                const PropertyMap &map) const;

    void change(MoleculeData &data, const BondID &bond,
                SireUnits::Dimension::Length delta, const PropertyMap &map); 

    static void translate(MoleculeView &view, 
                          const AtomSelection &selected_atoms,
                          const Vector &delta, 
                          const PropertyMap &map);

    static void rotate(MoleculeView &view,
                       const AtomSelection &selected_atoms,
                       const Matrix &rotmat,
                       const Vector &point,
                       const PropertyMap &map);

    static void translate(QVector<CoordGroup> &coords,
                          const AtomSelection &selected_atoms,
                          const Vector &delta);

    static void rotate(QVector<CoordGroup> &coords,
                       const AtomSelection &selected_atoms,
                       const Matrix &rotmat,
                       const Vector &point);

    /** The only atoms that can be moved by this Mover */
    AtomSelection movable_atoms;
};

}

SIRE_END_HEADER

#endif
