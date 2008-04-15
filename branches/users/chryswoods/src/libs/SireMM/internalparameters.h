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

#ifndef SIREMM_INTERNALPARAMETERS_H
#define SIREMM_INTERNALPARAMETERS_H

namespace SireMM
{

class InternalParameters;
class GroupInternalParameters;

class AtomFunction;
class TwoAtomFunction;
class ThreeAtomFunction;
class FourAtomFunction;

}

namespace SireMM
{

/** This class holds all of the internal parameters for one group
    combination within a molecule
    
    There are several types of internal parameters, defined
    by the type of internal used to provide the coordinates,
    and the quantities calculated from those coordinates that
    can be used in the function
    
    Bond            : Input Bond - function uses interatomic distance (1-2), r
    Angle           : Input Angle - function uses angle (1-2-3), theta
    Dihedral        : Input Dihedral - function uses torsion (1-2-3-4), phi
    
    Improper        : Input Improper - function uses either torsion angle
                                       (1-3-4-2), phi, or out of plane angle, theta
                                       
    Urey-Bradley    : Input Angle - function uses distance (1-3), r
    
    Stretch-Stretch : Input Angle - function uses distances (1-2), r12, and 
                                    (3-2), r32
                                    
    Stretch-Bend    : Input Angle - function uses distances angle (1-2-3), theta,
                                    and distances (1-2), r12, and (3-2) r32
                                    
    Bend-Bend       : Input Improper - function uses angles (1-2-3), (3-2-4), (4-2-1),
                                        theta123, theta324, theta421 
    
    Stretch-Bend    : Input Dihedral - function uses torsion (1-2-3-4), phi,
      -Torsion                         distances (1-2), (2-3), (3-4),
                                       r12, r23, r34, and angles
                                       (1-2-3) and (2-3-4), theta123, theta234
    
    @author Christopher Woods
*/
class SIREMM_EXPORT GroupInternalParameters
{
public:


private:
    /** The array of bond parameters */
    QVector<TwoAtomFunction> bond_params;
    
    /** The array of angle parameters */
    QVector<ThreeAtomFunction> angle_params;
    
    /** The array of dihedral parameters */
    QVector<FourAtomFunction> dihedral_params;
    
    /** The array of improper parameters */
    QVector<FourAtomFunction> improper_params;
    
    /** The array of Urey-Bradley parameters */
    QVector<TwoAtomFunction> ub_params;
    
    /** The array of stretch-stretch parameters */
    QVector<ThreeAtomFunction> stretch_stretch_params;
    
    /** The array of stretch-bend parameters */
    QVector<ThreeAtomFunction> stretch_bend_params;
    
    /** The array of bend-bend parameters */
    QVector<FourAtomFunction> bend_bend_params;
    
    /** The array of stretch-bend-torsion parameters */
    QVector<FourAtomFunction> stretch_bend_torsion_params;
};

}

#endif
