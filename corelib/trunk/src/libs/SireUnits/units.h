#ifndef SIREUNITS_CONSTANTS_H
#define SIREUNITS_CONSTANTS_H

#include <limits>

#include "SireMaths/constants.h"

namespace SireUnits
{

using SireMaths::pi;

/** This file defines physical constants, in internal units of this program
 
    energy = kcal mol-1  (thermal)
    length = Angstroms
    time = femtoseconds
    mass = g mol-1
    charge = unit electrons
 
*/

/////////////////////////////////////////////////
// First, basic, dimensionless constants       //
/////////////////////////////////////////////////

/** Avogadro's number */
const double mole = 6.0221419947e23;

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

/** Convert angstroms into internal units */
const double angstrom = 1.0;

/** Convert nanometers into internal units */
const double nanometer = angstrom * 10.0;

/** Convert picometers into internal units */
const double picometer = nanometer * 1000.0;

/** Convert meters into internal units */
const double meter = nanometer * 1.0e9;

/////////////////////////////////////////////////
// Units of time. Internal unit = femtoseconds //
/////////////////////////////////////////////////

/** Convert femtoseconds into internal units */
const double femtosecond = 1.0;

/** Convert picoseconds into internal units */
const double picosecond = 1000.0 * femtosecond;

/** Convert nanoseconds into internal units */
const double nanosecond = 1000.0 * picosecond;

/** Convert seconds into internal units */
const double second = nanosecond * 1.0e9;

///////////////////////////////////////////////////////
// Units of mass. Internal unit = g mol-1            //
///////////////////////////////////////////////////////

/** Convert g mol-1 to internal units */
const double g_per_mol = 1.0;

/** Convert kilogrammes to internal units */
const double kilogram = 1000.0 * g_per_mol * mole;

///////////////////////////////////////////////////////
// Units of Charge. Internal unit = |electron| mol-1 //
///////////////////////////////////////////////////////

/** Convert electron charges to internal units 
    ( 1 |e| is the charge in coulombs of one mole of electrons) */
const double mod_electrons = 1.0;

/** Convert Coulombs to internal units */
const double coulomb = mod_electrons / 1.60217646263e-19;

/////////////////////////////////////////////////
// Units of Energy. Internal unit = kcal mol-1 //
/////////////////////////////////////////////////

/** Conversion factor from kcal mol-1 (thermal) to internal units (kcal mol-1).  */
const double kcal_per_mol = 1.0;

/** Conversion factor from kcal to kcal mol-1 */
const double kcal = kcal_per_mol * mole;

/** Conversion factor from cal mol-1 to internal units (kcal mol-1).  */
const double cal_per_mol = 0.001 * kcal_per_mol;

/** Conversion factor from cal to internal units, assuming that one mole produced this energy */
const double cal = 0.001 * kcal;

/** Conversion factor from kJ mol-1 to internal units (kcal mol-1).  */
const double kJ_per_mol = kcal_per_mol / (4.184);

/** Conversion factor from J mol-1 to internal units (kcal mol-1).  */
const double J_per_mol = 0.001 * kJ_per_mol;

/** Convert from kJ */
const double kilojoule = kJ_per_mol * mole;

/** Convert from J */
const double joule = 0.001 * kilojoule;

/** Conversion factor from international kcal mol-1 to internal units  */
const double int_kcal_per_mol = 4.1868 * kJ_per_mol;

/** Conversion factor from international cal mol-1 to internal units (kcal mol-1). */
const double int_cal_per_mol = 0.001 * int_kcal_per_mol;

/** Convert from international kcal */
const double int_kcal = int_kcal_per_mol * mole;

/** Convert from international cal */
const double int_cal = 0.001 * int_kcal;

//////////////////////////////////////////////////////////
// Units of force. Internal units = kcal mol-1 A-1      //
//////////////////////////////////////////////////////////

/** Convert force in Newtons_per_mol (J mol-1 m-1) to internal units (kcal mol-1 A-1) */
const double newton_per_mol = J_per_mol / meter;

/** Convert a force in Newtons to internal units */
const double newton = joule / meter;

//////////////////////////////////////////////////////////
// Units of pressure. Internal units = kcal mol-1 A-3   //
//////////////////////////////////////////////////////////

/** Convert pressure in Pascals (N m-2) to internal units (where does the mol-1 come from..?) */
const double pascal = newton / (meter*meter);

/** Convert atmospheres to internal units */
const double atm = 101325.0 * pascal;

//////////////////////////////////////////////////////////
// Units of temperature. Internal units = Kelvin        //
//////////////////////////////////////////////////////////

/** Convert Kelvin into internal units */
const double kelvin = 1.0;

//////////////////////////////////////////////////////////
// Now some miscellaneous units                         //
//////////////////////////////////////////////////////////

/** Convert the units of current (amps) */
const double amp = coulomb / second;
/** Volts */
const double volt = joule / coulomb;
/** Convert the units of capacitance (farads) */
const double farad = coulomb / volt;
/** Convert power in Watts */
const double watt = joule / second;


///////////////////////////////////////////////////////////
// Now physical constants converted into internal units. //
//  The values of these are taken from the 1998 CODATA   //
//  values - see fundemental_constants.pdf in techdocs   //
///////////////////////////////////////////////////////////

/** Speed of light in a vacuum */
const double c = 299792458.0 * meter / second;

/** Epsilon_0 (electrostatic constant) 8.854187817e-12 F m-1,
    converted to internal units, |e|^2 J-1 mol A-1 */
const double epsilon0 = 8.854187817e-12 * farad / meter;

/** 4 * pi * epsilon_0 */
const double four_pi_eps0 = 4.0 * SireMaths::pi * epsilon0;

/** 1 / (4 * pi * epsilon0) */
const double one_over_four_pi_eps0 = 1.0 / four_pi_eps0;

/** Gas constant (8.3143510 J mol-1 K-1) */
const double gasr = 8.31447215 * J_per_mol / kelvin;

/** Boltzmann constant */
const double k_boltz = 1.380650324e-23 * joule / kelvin;

/** Magnetic constant, mu0, 4pi * 10-7 N A-2 */
const double mu0 = 4.0 * pi * newton / (amp*amp);

/** Newton's gravitational constant, 6.673(10) m^3 kg-1 s-2 */
const double G_newton = 6.67310 * (meter*meter*meter) / (kilogram * second*second);

/** Planck's constant, 6.62606876(52) J s */
const double h_planck = 6.6260687652 * joule * second;

/** Plank / 2pi */
const double h_slash = h_planck / (2.0*pi);

/** Unit charge, 1.602176462(63) * 10-19 C */
const double e_charge = 1.60217646263e-19 * coulomb;

/** Mass of an electron */
const double electron_mass = 9.1093818872e-31 * kilogram;

/** Mass of a proton */
const double proton_mass = 1.6726215813e-27 * kilogram;

/** Mass of a neutron */
const double neutron_mass = 1.6749271613e-27 * kilogram;

/** Atomic mass constant */
const double atomic_mass_constant = 1.6605387313e-27 * kilogram;

/** Hartree energy */
const double hartree = 4.3597438134e-18 * joule;

/** Faraday constant */
const double faraday = 96485.341539 * coulomb / mole;

/** Molar volume of an ideal gas */
const double molar_volume = 22.41399639e-3 * meter*meter*meter / mole;

}

#endif
