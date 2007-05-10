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

#ifndef SIREUNITS_UNITS_H
#define SIREUNITS_UNITS_H

#include <limits>

#include "dimensions.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{

/** This file defines physical constants, in internal units of this program

    energy = kcal mol-1  (thermal)
    length = Angstroms
    time = femtoseconds
    mass = g mol-1
    charge = unit electrons

*/

/** Avogadro's number */
const Dimension::Quantity mole( 6.0221419947e23 );

/////////////////////////////////////////////////
// Units of angle. Internal unit = radians     //
/////////////////////////////////////////////////

/** Convert radians into internal units */
const double radians = 1.0;

/** Convert degrees into internal units */
const double degrees = pi / 180.0;

/////////////////////////////////////////////////
// Units of length. Internal unit = Angstroms  //
/////////////////////////////////////////////////

const Dimension::Length angstrom(1);

const Dimension::Length picometer( 0.01 * angstrom );
const Dimension::Length nanometer( 1000 * picometer );
const Dimension::Length micrometer( 1000 * nanometer );
const Dimension::Length millimeter( 1000 * micrometer );
const Dimension::Length centimeter( 10 * millimeter );
const Dimension::Length meter( 100 * centimeter );
const Dimension::Length kilometer( 1000 * meter );

const Dimension::Length bohr_radii( 0.529177249 * angstrom );

const Dimension::Length inch( 2.54 * centimeter );
const Dimension::Length foot( 12 * inch );
const Dimension::Length yard( 3 * foot );
const Dimension::Length mile( 1760 * yard );

/////////////////////////////////////////////////
// Units of time. Internal unit = femtoseconds //
/////////////////////////////////////

const Dimension::Time femtosecond(1);
const Dimension::Time picosecond(1000 * femtosecond);
const Dimension::Time nanosecond(1000 * picosecond);
const Dimension::Time microsecond(1000 * nanosecond);
const Dimension::Time millisecond(1000 * microsecond);
const Dimension::Time second(1000 * millisecond);

const Dimension::Time minute( 60 * second );
const Dimension::Time hour( 60 * minute );
const Dimension::Time day( 24 * hour );

///////////////////////////////////////////////////////
// Units of mass. Internal unit = g mol-1            //
///////////////////////////////////////////////////////

const Dimension::Mass g_per_mol(1);

const Dimension::Mass gram( mole * g_per_mol );
const Dimension::Mass kilogram( 1000 * gram );
const Dimension::Mass tonne( 1000 * kilogram );

const Dimension::Mass milligram( 0.001 * gram );
const Dimension::Mass microgram( 0.001 * milligram );
const Dimension::Mass nanogram( 0.001 * microgram );
const Dimension::Mass picogram( 0.001 * nanogram );
const Dimension::Mass femtogram( 0.001 * picogram );

const Dimension::Mass kg_per_mol( 1000 * g_per_mol );
const Dimension::Mass tonne_per_mol( 1000 * kg_per_mol );
const Dimension::Mass mg_per_mol( 0.001 * g_per_mol );
const Dimension::Mass ug_per_mol( 0.001 * mg_per_mol );
const Dimension::Mass ng_per_mol( 0.001 * ug_per_mol );
const Dimension::Mass pg_per_mol( 0.001 * ng_per_mol );
const Dimension::Mass fg_per_mol( 0.001 * pg_per_mol );

///////////////////////////////////////////////////////
// Units of Charge. Internal unit = |electron| mol-1 //
///////////////////////////////////////////////////////

const Dimension::Charge mod_electrons(1);

const Dimension::Charge coulomb( (1.0/1.60217646263e-19) * mod_electrons );

const Dimension::Charge e_charge(mod_electrons);

/////////////////////////////////////////////////
// Units of Energy. Internal unit = kcal mol-1 //
/////////////////////////////////////////////////

const Dimension::Energy kcal_per_mol(1);
const Dimension::Energy kcal( mole * kcal_per_mol );

const Dimension::Energy cal_per_mol( 0.001 * kcal_per_mol );
const Dimension::Energy cal( 0.001 * kcal );

const Dimension::Energy kJ_per_mol( (1.0/4.184) * kcal_per_mol );
const Dimension::Energy kilojoule( mole * kJ_per_mol );

const Dimension::Energy MJ_per_mol( 1000 * kJ_per_mol );
const Dimension::Energy megajoule( 1000 * kilojoule );

const Dimension::Energy J_per_mol( 0.001 * kJ_per_mol );
const Dimension::Energy joule( 0.001 * kilojoule );

/** Conversion factor from international kcal mol-1 to internal units  */
const Dimension::Energy int_kcal_per_mol( 4.1868 * kJ_per_mol );
const Dimension::Energy int_cal_per_mol( 0.001 * int_kcal_per_mol );

const Dimension::Energy int_kcal( mole * int_kcal_per_mol );
const Dimension::Energy int_cal( 0.001 * int_kcal );

const Dimension::Energy hartree(4.3597438134e-18 * joule);

//////////////////////////////////////////////////////////
// Units of force. Internal units = kcal mol-1 A-1      //
//////////////////////////////////////////////////////////

/** Convert force in Newtons_per_mol (J mol-1 m-1) to internal units (kcal mol-1 A-1) */
const Dimension::Force newton_per_mol( J_per_mol / meter );

/** Convert a force in Newtons to internal units */
const Dimension::Force newton( joule / meter );

//////////////////////////////////////////////////////////
// Units of pressure. Internal units = kcal mol-1 A-2   //
//////////////////////////////////////////////////////////

/** Convert pressure in Pascals (N m-2) to internal units (where does the mol-1 come from..?) */
const Dimension::Pressure pascal( newton / (meter*meter) );

/** Convert atmospheres to internal units */
const Dimension::Pressure atm( 101325.0 * pascal );

//////////////////////////////////////////////////////////
// Units of temperature. Internal units = Kelvin        //
//////////////////////////////////////////////////////////

const Dimension::Temperature kelvin(1);

// other temperature units defined in temperature.h

//////////////////////////////////////////////////////////
// Now some miscellaneous units                         //
//////////////////////////////////////////////////////////

/** Convert the units of current (amps) */
const Dimension::ElecCurrent amp( coulomb / second );

/** Volts */
const Dimension::ElecPotential volt( joule / coulomb );

/** Convert the units of capacitance (farads) */
const Dimension::PhysUnit<-1,-2,2,2,0> farad( coulomb / volt );

/** Convert power in Watts */
const Dimension::Power watt( joule / second );

///////////////////////////////////////////////////////////
// Now physical constants converted into internal units. //
//  The values of these are taken from the 1998 CODATA   //
//  values - see fundemental_constants.pdf in techdocs   //
///////////////////////////////////////////////////////////

/** Speed of light in a vacuum */
const Dimension::Velocity c( 299792458.0 * (meter / second) );

/** Epsilon_0 (electrostatic constant) 8.854187817e-12 F m-1,
    converted to internal units, |e|^2 J-1 mol A-1 */
const double epsilon0 = 8.854187817e-12 * (farad / meter);

/** 4 * pi * epsilon_0 */
const double four_pi_eps0 = 4.0 * SireMaths::pi * epsilon0;

/** 1 / (4 * pi * epsilon0) */
const double one_over_four_pi_eps0 = 1.0 / four_pi_eps0;

/** Gas constant (8.31447215 J mol-1 K-1) */
const double gasr = 8.31447215 * (J_per_mol / kelvin);

/** Boltzmann constant */
const double k_boltz = 1.380650524e-23 * (joule / kelvin);

/** Magnetic constant, mu0, 4pi * 10-7 N A-2 */
const double mu0 = 4.0 * pi * (newton / (amp*amp));

/** Newton's gravitational constant, 6.673(10) m^3 kg-1 s-2 */
const double G_newton = 6.67310 * ((meter*meter*meter) / (kilogram * second*second));

/** Planck's constant, 6.62606876(52) J s */
const double h_planck = 6.6260687652 * (joule * second);

/** Plank / 2pi */
const double h_slash = h_planck / (2.0*pi);

/** Mass of an electron */
const Dimension::Mass electron_mass( 9.1093818872e-31 * kilogram );

/** Mass of a proton */
const Dimension::Mass proton_mass( 1.6726215813e-27 * kilogram );

/** Mass of a neutron */
const Dimension::Mass neutron_mass( 1.6749271613e-27 * kilogram );

/** Atomic mass constant */
const Dimension::Mass atomic_mass_constant( 1.6605387313e-27 * kilogram );

/** Faraday constant */
const Dimension::Charge faraday( (96485.341539 * coulomb) / mole );

/** Molar volume of an ideal gas */
const Dimension::Volume molar_volume( (22.41399639e-3 * (meter*meter*meter)) / mole );

}

SIRE_END_HEADER

#endif
