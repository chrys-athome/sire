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
#include <cmath>

#include "dimensions.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{
extern const Dimension::Quantity mole;
extern const Dimension::Quantity dozen;

extern const Dimension::Angle radians;
extern const Dimension::Angle radian;
extern const Dimension::Angle degrees;
extern const Dimension::Angle degree;
extern const Dimension::Angle angle_minute;
extern const Dimension::Angle angle_minutes;
extern const Dimension::Angle angle_second;
extern const Dimension::Angle angle_seconds;
extern const Dimension::Angle octant;
extern const Dimension::Angle octants;
extern const Dimension::Angle sextant;
extern const Dimension::Angle sextants;
extern const Dimension::Angle quadrant;
extern const Dimension::Angle quadrants;
extern const Dimension::Angle gradian;
extern const Dimension::Angle gradians;
extern const Dimension::Angle grad;
extern const Dimension::Angle gon;
extern const Dimension::Angle revolution;
extern const Dimension::Angle revolutions;
extern const Dimension::Angle revs;
extern const Dimension::Angle circumference;

extern const Dimension::Length angstrom;
extern const Dimension::Length angstroms;
extern const Dimension::Length picometer;
extern const Dimension::Length picometers;
extern const Dimension::Length nanometer;
extern const Dimension::Length nanometers;
extern const Dimension::Length micrometer;
extern const Dimension::Length micrometers;
extern const Dimension::Length millimeter;
extern const Dimension::Length millimeters;
extern const Dimension::Length centimeter;
extern const Dimension::Length centimeters;
extern const Dimension::Length meter;
extern const Dimension::Length meters;
extern const Dimension::Length kilometer;
extern const Dimension::Length kilometers;
extern const Dimension::Length bohr_radius;
extern const Dimension::Length bohr_radii;
extern const Dimension::Length inch;
extern const Dimension::Length inches;
extern const Dimension::Length foot;
extern const Dimension::Length feet;
extern const Dimension::Length yard;
extern const Dimension::Length yards;
extern const Dimension::Length mile;
extern const Dimension::Length miles;

extern const Dimension::Area angstrom2;
extern const Dimension::Area picometer2;
extern const Dimension::Area nanometer2;
extern const Dimension::Area meter2;

extern const Dimension::Volume angstrom3;
extern const Dimension::Volume picometer3;
extern const Dimension::Volume nanometer3;
extern const Dimension::Volume meter3;

extern const Dimension::MolarMass g_per_mol;

extern const Dimension::Mass gram;
extern const Dimension::Mass grams;
extern const Dimension::Mass kilogram;
extern const Dimension::Mass kilograms;
extern const Dimension::Mass tonne;
extern const Dimension::Mass tonnes;
extern const Dimension::Mass milligram;
extern const Dimension::Mass milligrams;
extern const Dimension::Mass microgram;
extern const Dimension::Mass micrograms;
extern const Dimension::Mass nanogram;
extern const Dimension::Mass nanograms;
extern const Dimension::Mass picogram;
extern const Dimension::Mass picograms;
extern const Dimension::Mass femtogram;
extern const Dimension::Mass femtograms;

extern const Dimension::MolarMass kg_per_mol;
extern const Dimension::MolarMass tonne_per_mol;
extern const Dimension::MolarMass mg_per_mol;
extern const Dimension::MolarMass ug_per_mol;
extern const Dimension::MolarMass ng_per_mol;
extern const Dimension::MolarMass pg_per_mol;
extern const Dimension::MolarMass fg_per_mol;

extern const Dimension::Charge mod_electron;
extern const Dimension::Charge mod_electrons;
extern const Dimension::Charge coulomb;
extern const Dimension::Charge coulombs;
extern const Dimension::Charge e_charge;

extern const Dimension::MolarCharge coulombs_per_mol;
extern const Dimension::MolarCharge faraday;
extern const Dimension::MolarCharge faradays;

extern const Dimension::Energy kcal;
extern const Dimension::Energy cal;
extern const Dimension::Energy kilojoule;
extern const Dimension::Energy kilojoules;
extern const Dimension::Energy megajoule;
extern const Dimension::Energy megajoules;
extern const Dimension::Energy joule;
extern const Dimension::Energy joules;
extern const Dimension::Energy int_kcal;
extern const Dimension::Energy int_cal;
extern const Dimension::Energy hartree;
extern const Dimension::Energy hartrees;

extern const Dimension::MolarEnergy kcal_per_mol;
extern const Dimension::MolarEnergy cal_per_mol;
extern const Dimension::MolarEnergy kJ_per_mol;
extern const Dimension::MolarEnergy MJ_per_mol;
extern const Dimension::MolarEnergy J_per_mol;
extern const Dimension::MolarEnergy int_kcal_per_mol;
extern const Dimension::MolarEnergy int_cal_per_mol;

extern const Dimension::Time akma_time;

extern const Dimension::Time second;
extern const Dimension::Time seconds;
extern const Dimension::Time millisecond;
extern const Dimension::Time milliseconds;
extern const Dimension::Time microsecond;
extern const Dimension::Time microseconds;
extern const Dimension::Time nanosecond;
extern const Dimension::Time nanoseconds;
extern const Dimension::Time picosecond;
extern const Dimension::Time picoseconds;
extern const Dimension::Time femtosecond;
extern const Dimension::Time femtoseconds;
extern const Dimension::Time minute;
extern const Dimension::Time minutes;
extern const Dimension::Time hour;
extern const Dimension::Time hours;
extern const Dimension::Time day;
extern const Dimension::Time days;
extern const Dimension::Time week;
extern const Dimension::Time weeks;
extern const Dimension::Time fortnight;
extern const Dimension::Time fortnights;

extern const Dimension::Force newton;
extern const Dimension::Force newtons;
extern const Dimension::Force ounce;
extern const Dimension::Force ounces;
extern const Dimension::Force pound;
extern const Dimension::Force pounds;
extern const Dimension::Force stone;
extern const Dimension::Force stones;
extern const Dimension::Force hundredweight;

extern const Dimension::Pressure pascal;
extern const Dimension::Pressure pascals;
extern const Dimension::Pressure bar;
extern const Dimension::Pressure atm;
extern const Dimension::Pressure psi;
extern const Dimension::Pressure mmHg;

extern const Dimension::Temperature kelvin;

extern const Dimension::Current amp;
extern const Dimension::Current amps;
extern const Dimension::Potential volt;
extern const Dimension::Potential volts;
extern const Dimension::Capacitance farad;
extern const Dimension::Capacitance farads;
extern const Dimension::Power watt;
extern const Dimension::Power watts;
extern const Dimension::MolarPower watts_per_mol;

extern const Dimension::Velocity c;
extern const Dimension::Acceleration g_accel;
extern const Dimension::Mass electron_mass;
extern const Dimension::Mass proton_mass;
extern const Dimension::Mass neutron_mass;
extern const Dimension::Mass atomic_mass_constant;
extern const Dimension::MolarVolume molar_volume;

extern const double epsilon0;
extern const double four_pi_eps0;
extern const double one_over_four_pi_eps0;
extern const double gasr;
extern const double k_boltz;
extern const double mu0;
extern const double G_newton;
extern const double h_planck;
extern const double h_slash;

}

SIRE_END_HEADER

#endif
