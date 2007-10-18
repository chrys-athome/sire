/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SPIER_CPKREP_H
#define SPIER_CPKREP_H

#include "moleculerep.h"

SIRE_BEGIN_HEADER

namespace Spier
{

/**
This class holds a CPK (balls and sticks) representation of a molecule.
 
@author Christopher Woods
*/
class SPIER_EXPORT CPKRep : public MoleculeRep
{
public:
    CPKRep(const MolDisplayDataPtr &ptr);

    ~CPKRep();

    const BondList& renderBonds() const;

    void select(const AtomIndex &atom);
    void deselect(const AtomIndex &atom);

protected:

    void buildRep();

    void draw();
    void selectDraw();

    SelectID select(int x, int y);
    void highlight(const Atom &atom);

    /** Vector of spheres for each atom in the representation */
    QVector<GLSphere> atomspheres;
    
    /** All of the bonds that are to be rendered from this molecule
        (though note that the atoms in the bonds must also be in 'renderatms' */
    BondList renderbnds;

    /** Vector of cylinders for each half bond */
    QVector<GLCylinder> bondcylder0;
    QVector<GLCylinder> bondcylder1;

};

/** Return the list of bonds that are rendered in this representation */
inline const BondList& CPKRep::renderBonds() const
{
    return renderbnds;
}

}

SIRE_END_HEADER

#endif
