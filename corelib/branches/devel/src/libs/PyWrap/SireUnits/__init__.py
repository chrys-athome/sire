#########################
#
# This module contains a collection
# of units.
#
# The units are copied from
# SireUnits/units.h
#
########################

import math

#/////////////////////////////////////////////////
#// Conversion functions                        //
#/////////////////////////////////////////////////

def convert( value, from_units, to_units ):
    """Convert 'value' from units of 'from_units' to
       units of 'to_units' """
    return value * (from_units / to_units)

def convertFrom( value, from_units ):
    """Convert 'value' from units of 'from_units' to
       the internal units used by Sire"""
    return value * from_units

def convertTo( value, to_units ):
    """Convert 'value' from the internal units used
       by Sire to units of 'to_units' """
    return value / to_units

#/////////////////////////////////////////////////
#// First, basic, dimensionless constants       //
#/////////////////////////////////////////////////

# Avogadro's number
mole = 6.0221419947e23

#/////////////////////////////////////////////////
#// Units of angle. Internal unit = radians     //
#/////////////////////////////////////////////////

# define pi
pi = math.pi

# Convert radians into internal units
radians = 1.0

# Convert degrees into internal units
degrees = pi / 180.0

#/////////////////////////////////////////////////
#// Units of length. Internal unit = Angstroms  //
#/////////////////////////////////////////////////

# Convert angstroms into internal units
angstrom = 1.0

# Convert nanometers into internal units
nanometer = angstrom * 10.0

# Convert picometers into internal units
picometer = nanometer * 1000.0

# Convert meters into internal units
meter = nanometer * 1.0e9

# Convert bohr radii into internal units
bohr_radii = 0.529177249

#/////////////////////////////////////////////////
#// Units of time. Internal unit = femtoseconds //
#/////////////////////////////////////////////////

# Convert femtoseconds into internal units
femtosecond = 1.0

# Convert picoseconds into internal units
picosecond = 1000.0 * femtosecond

# Convert nanoseconds into internal units
nanosecond = 1000.0 * picosecond

# Convert seconds into internal units
second = nanosecond * 1.0e9

#///////////////////////////////////////////////////////
#// Units of mass. Internal unit = g mol-1            //
#///////////////////////////////////////////////////////

# Convert g mol-1 to internal units
g_per_mol = 1.0

# Convert kilogrammes to internal units
kilogram = 1000.0 * g_per_mol * mole

#///////////////////////////////////////////////////////
#// Units of Charge. Internal unit = |electron| mol-1 //
#///////////////////////////////////////////////////////

# Convert electron charges to internal units
# ( 1 |e| is the charge in coulombs of one mole of electrons)
mod_electrons = 1.0

# Convert Coulombs to internal units
coulomb = mod_electrons / 1.60217646263e-19

#/////////////////////////////////////////////////
#// Units of Energy. Internal unit = kcal mol-1 //
#/////////////////////////////////////////////////

# Conversion factor from kcal mol-1 (thermal) to internal units (kcal mol-1).
kcal_per_mol = 1.0

# Conversion factor from kcal to kcal mol-1
kcal = kcal_per_mol * mole

# Conversion factor from cal mol-1 to internal units (kcal mol-1).
cal_per_mol = 0.001 * kcal_per_mol

# Conversion factor from cal to internal units, assuming that one mole produced this energy
cal = 0.001 * kcal

# Conversion factor from kJ mol-1 to internal units (kcal mol-1).
kJ_per_mol = kcal_per_mol / (4.184)

# Conversion factor from J mol-1 to internal units (kcal mol-1).
J_per_mol = 0.001 * kJ_per_mol

# Convert from kJ
kilojoule = kJ_per_mol * mole

# Convert from J
joule = 0.001 * kilojoule

# Conversion factor from international kcal mol-1 to internal units
int_kcal_per_mol = 4.1868 * kJ_per_mol

# Conversion factor from international cal mol-1 to internal units (kcal mol-1).
int_cal_per_mol = 0.001 * int_kcal_per_mol

# Convert from international kcal
int_kcal = int_kcal_per_mol * mole

# Convert from international cal
int_cal = 0.001 * int_kcal

#//////////////////////////////////////////////////////////
#// Units of force. Internal units = kcal mol-1 A-1      //
#//////////////////////////////////////////////////////////

# Convert force in Newtons_per_mol (J mol-1 m-1) to internal units (kcal mol-1 A-1)
newton_per_mol = J_per_mol / meter

# Convert a force in Newtons to internal units
newton = joule / meter

#//////////////////////////////////////////////////////////
#// Units of pressure. Internal units = kcal mol-1 A-3   //
#//////////////////////////////////////////////////////////

# Convert pressure in Pascals (N m-2) to internal units (where does the mol-1 come from..?)
pascal = newton / (meter*meter)

# Convert atmospheres to internal units
atm = 101325.0 * pascal

#//////////////////////////////////////////////////////////
#// Units of temperature. Internal units = Kelvin        //
#//////////////////////////////////////////////////////////

# Convert Kelvin into internal units
kelvin = 1.0

#//////////////////////////////////////////////////////////
#// Now some miscellaneous units                         //
#//////////////////////////////////////////////////////////

# Convert the units of current (amps)
amp = coulomb / second

# Volts
volt = joule / coulomb
# Convert the units of capacitance (farads)
farad = coulomb / volt
# Convert power in Watts
watt = joule / second

#///////////////////////////////////////////////////////////
#// Now physical constants converted into internal units. //
#//  The values of these are taken from the 1998 CODATA   //
#//  values - see fundemental_constants.pdf in techdocs   //
#///////////////////////////////////////////////////////////

# Speed of light in a vacuum
c = 299792458.0 * meter / second

# Epsilon_0 (electrostatic constant) 8.854187817e-12 F m-1,
#  converted to internal units, |e|^2 J-1 mol A-1
epsilon0 = 8.854187817e-12 * farad / meter

# 4 * pi * epsilon_0
four_pi_eps0 = 4.0 * pi * epsilon0

# 1 / (4 * pi * epsilon0)
one_over_four_pi_eps0 = 1.0 / four_pi_eps0

# Gas constant (8.3143510 J mol-1 K-1)
gasr = 8.31447215 * J_per_mol / kelvin

# Boltzmann constant
k_boltz = 1.380650324e-23 * joule / kelvin

# Magnetic constant, mu0, 4pi * 10-7 N A-2
mu0 = 4.0 * pi * newton / (amp*amp)

# Newton's gravitational constant, 6.673(10) m^3 kg-1 s-2
G_newton = 6.67310 * (meter*meter*meter) / (kilogram * second*second)

# Planck's constant, 6.62606876(52) J s
h_planck = 6.6260687652 * joule * second

# Plank / 2pi
h_slash = h_planck / (2.0*pi)

# Unit charge, 1.602176462(63) * 10-19 C
e_charge = 1.60217646263e-19 * coulomb

# Mass of an electron
electron_mass = 9.1093818872e-31 * kilogram

# Mass of a proton
proton_mass = 1.6726215813e-27 * kilogram

# Mass of a neutron
neutron_mass = 1.6749271613e-27 * kilogram

# Atomic mass constant
atomic_mass_constant = 1.6605387313e-27 * kilogram

# Hartree energy
hartree = 4.3597438134e-18 * joule

# Faraday constant
faraday = 96485.341539 * coulomb / mole

# Molar volume of an ideal gas
molar_volume = 22.41399639e-3 * meter*meter*meter / mole

