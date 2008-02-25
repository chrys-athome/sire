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

    /** Functionals must also supply a Molecule class. It is easiest if
        you inherit from TwoBodyFunctional::Molecule<MolParams> */
    template<class MolParams>
    class Molecule;

protected:
    TwoBodyFunctional();
    
    TwoBodyFunctional(const TwoBodyFunctional &other);


};

template<class MolParams>
class TwoBodyFunctional::Molecule : public FF::Molecule
{
public:
    ~TwoBodyMolecule();
   
protected:
    TwoBodyMolecule();

    TwoBodyMolecule(const CoordGroupArray &coords,
                    const MolParams &params);
    
    TwoBodyMolecule(const TwoBodyMolecule &other);

private:
    /** The coordinates of the atoms in this view */
    CoordGroupArray coords;
    
    /** An AABox that encases all of the atoms in this view */
    AABox aabox;
    
    /** The parameters for all of the parts of this view */
    MolParams params;
};

}

#endif
