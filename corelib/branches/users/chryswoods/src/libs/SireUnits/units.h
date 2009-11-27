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
#include "generalunit.h"

SIRE_BEGIN_HEADER

namespace SireUnits
{
extern const Dimension::Quantity SIREUNITS_EXPORT mole;
extern const Dimension::Quantity SIREUNITS_EXPORT dozen;

extern const Dimension::Angle SIREUNITS_EXPORT radians;
extern const Dimension::Angle SIREUNITS_EXPORT radian;
extern const Dimension::Angle SIREUNITS_EXPORT degrees;
extern const Dimension::Angle SIREUNITS_EXPORT degree;
extern const Dimension::Angle SIREUNITS_EXPORT angle_minute;
extern const Dimension::Angle SIREUNITS_EXPORT angle_minutes;
extern const Dimension::Angle SIREUNITS_EXPORT angle_second;
extern const Dimension::Angle SIREUNITS_EXPORT angle_seconds;
extern const Dimension::Angle SIREUNITS_EXPORT octant;
extern const Dimension::Angle SIREUNITS_EXPORT octants;
extern const Dimension::Angle SIREUNITS_EXPORT sextant;
extern const Dimension::Angle SIREUNITS_EXPORT sextants;
extern const Dimension::Angle SIREUNITS_EXPORT quadrant;
extern const Dimension::Angle SIREUNITS_EXPORT quadrants;
extern const Dimension::Angle SIREUNITS_EXPORT gradian;
extern const Dimension::Angle SIREUNITS_EXPORT gradians;
extern const Dimension::Angle SIREUNITS_EXPORT grad;
extern const Dimension::Angle SIREUNITS_EXPORT gon;
extern const Dimension::Angle SIREUNITS_EXPORT revolution;
extern const Dimension::Angle SIREUNITS_EXPORT revolutions;
extern const Dimension::Angle SIREUNITS_EXPORT revs;
extern const Dimension::Angle SIREUNITS_EXPORT circumference;

extern const Dimension::Length SIREUNITS_EXPORT angstrom;
extern const Dimension::Length SIREUNITS_EXPORT angstroms;
extern const Dimension::Length SIREUNITS_EXPORT picometer;
extern const Dimension::Length SIREUNITS_EXPORT picometers;
extern const Dimension::Length SIREUNITS_EXPORT nanometer;
extern const Dimension::Length SIREUNITS_EXPORT nanometers;
extern const Dimension::Length SIREUNITS_EXPORT micrometer;
extern const Dimension::Length SIREUNITS_EXPORT micrometers;
extern const Dimension::Length SIREUNITS_EXPORT millimeter;
extern const Dimension::Length SIREUNITS_EXPORT millimeters;
extern const Dimension::Length SIREUNITS_EXPORT centimeter;
extern const Dimension::Length SIREUNITS_EXPORT centimeters;
extern const Dimension::Length SIREUNITS_EXPORT meter;
extern const Dimension::Length SIREUNITS_EXPORT meters;
extern const Dimension::Length SIREUNITS_EXPORT kilometer;
extern const Dimension::Length SIREUNITS_EXPORT kilometers;
extern const Dimension::Length SIREUNITS_EXPORT bohr_radius;
extern const Dimension::Length SIREUNITS_EXPORT bohr_radii;
extern const Dimension::Length SIREUNITS_EXPORT inch;
extern const Dimension::Length SIREUNITS_EXPORT inches;
extern const Dimension::Length SIREUNITS_EXPORT foot;
extern const Dimension::Length SIREUNITS_EXPORT feet;
extern const Dimension::Length SIREUNITS_EXPORT yard;
extern const Dimension::Length SIREUNITS_EXPORT yards;
extern const Dimension::Length SIREUNITS_EXPORT mile;
extern const Dimension::Length SIREUNITS_EXPORT miles;

extern const Dimension::Area SIREUNITS_EXPORT angstrom2;
extern const Dimension::Area SIREUNITS_EXPORT picometer2;
extern const Dimension::Area SIREUNITS_EXPORT nanometer2;
extern const Dimension::Area SIREUNITS_EXPORT meter2;

extern const Dimension::Volume SIREUNITS_EXPORT angstrom3;
extern const Dimension::Volume SIREUNITS_EXPORT picometer3;
extern const Dimension::Volume SIREUNITS_EXPORT nanometer3;
extern const Dimension::Volume SIREUNITS_EXPORT meter3;

extern const Dimension::MolarMass SIREUNITS_EXPORT g_per_mol;

extern const Dimension::Mass SIREUNITS_EXPORT gram;
extern const Dimension::Mass SIREUNITS_EXPORT grams;
extern const Dimension::Mass SIREUNITS_EXPORT kilogram;
extern const Dimension::Mass SIREUNITS_EXPORT kilograms;
extern const Dimension::Mass SIREUNITS_EXPORT tonne;
extern const Dimension::Mass SIREUNITS_EXPORT tonnes;
extern const Dimension::Mass SIREUNITS_EXPORT milligram;
extern const Dimension::Mass SIREUNITS_EXPORT milligrams;
extern const Dimension::Mass SIREUNITS_EXPORT microgram;
extern const Dimension::Mass SIREUNITS_EXPORT micrograms;
extern const Dimension::Mass SIREUNITS_EXPORT nanogram;
extern const Dimension::Mass SIREUNITS_EXPORT nanograms;
extern const Dimension::Mass SIREUNITS_EXPORT picogram;
extern const Dimension::Mass SIREUNITS_EXPORT picograms;
extern const Dimension::Mass SIREUNITS_EXPORT femtogram;
extern const Dimension::Mass SIREUNITS_EXPORT femtograms;

extern const Dimension::MolarMass SIREUNITS_EXPORT kg_per_mol;
extern const Dimension::MolarMass SIREUNITS_EXPORT tonne_per_mol;
extern const Dimension::MolarMass SIREUNITS_EXPORT mg_per_mol;
extern const Dimension::MolarMass SIREUNITS_EXPORT ug_per_mol;
extern const Dimension::MolarMass SIREUNITS_EXPORT ng_per_mol;
extern const Dimension::MolarMass SIREUNITS_EXPORT pg_per_mol;
extern const Dimension::MolarMass SIREUNITS_EXPORT fg_per_mol;

extern const Dimension::Charge SIREUNITS_EXPORT mod_electron;
extern const Dimension::Charge SIREUNITS_EXPORT mod_electrons;
extern const Dimension::Charge SIREUNITS_EXPORT coulomb;
extern const Dimension::Charge SIREUNITS_EXPORT coulombs;
extern const Dimension::Charge SIREUNITS_EXPORT e_charge;

extern const Dimension::MolarCharge SIREUNITS_EXPORT coulombs_per_mol;
extern const Dimension::MolarCharge SIREUNITS_EXPORT faraday;
extern const Dimension::MolarCharge SIREUNITS_EXPORT faradays;

extern const Dimension::Energy SIREUNITS_EXPORT kcal;
extern const Dimension::Energy SIREUNITS_EXPORT cal;
extern const Dimension::Energy SIREUNITS_EXPORT kilojoule;
extern const Dimension::Energy SIREUNITS_EXPORT kilojoules;
extern const Dimension::Energy SIREUNITS_EXPORT megajoule;
extern const Dimension::Energy SIREUNITS_EXPORT megajoules;
extern const Dimension::Energy SIREUNITS_EXPORT joule;
extern const Dimension::Energy SIREUNITS_EXPORT joules;
extern const Dimension::Energy SIREUNITS_EXPORT int_kcal;
extern const Dimension::Energy SIREUNITS_EXPORT int_cal;
extern const Dimension::Energy SIREUNITS_EXPORT hartree;
extern const Dimension::Energy SIREUNITS_EXPORT hartrees;

extern const Dimension::MolarEnergy SIREUNITS_EXPORT kcal_per_mol;
extern const Dimension::MolarEnergy SIREUNITS_EXPORT cal_per_mol;
extern const Dimension::MolarEnergy SIREUNITS_EXPORT kJ_per_mol;
extern const Dimension::MolarEnergy SIREUNITS_EXPORT MJ_per_mol;
extern const Dimension::MolarEnergy SIREUNITS_EXPORT J_per_mol;
extern const Dimension::MolarEnergy SIREUNITS_EXPORT int_kcal_per_mol;
extern const Dimension::MolarEnergy SIREUNITS_EXPORT int_cal_per_mol;

extern const Dimension::Time SIREUNITS_EXPORT akma_time;

extern const Dimension::Time SIREUNITS_EXPORT second;
extern const Dimension::Time SIREUNITS_EXPORT seconds;
extern const Dimension::Time SIREUNITS_EXPORT millisecond;
extern const Dimension::Time SIREUNITS_EXPORT milliseconds;
extern const Dimension::Time SIREUNITS_EXPORT microsecond;
extern const Dimension::Time SIREUNITS_EXPORT microseconds;
extern const Dimension::Time SIREUNITS_EXPORT nanosecond;
extern const Dimension::Time SIREUNITS_EXPORT nanoseconds;
extern const Dimension::Time SIREUNITS_EXPORT picosecond;
extern const Dimension::Time SIREUNITS_EXPORT picoseconds;
extern const Dimension::Time SIREUNITS_EXPORT femtosecond;
extern const Dimension::Time SIREUNITS_EXPORT femtoseconds;
extern const Dimension::Time SIREUNITS_EXPORT minute;
extern const Dimension::Time SIREUNITS_EXPORT minutes;
extern const Dimension::Time SIREUNITS_EXPORT hour;
extern const Dimension::Time SIREUNITS_EXPORT hours;
extern const Dimension::Time SIREUNITS_EXPORT day;
extern const Dimension::Time SIREUNITS_EXPORT days;
extern const Dimension::Time SIREUNITS_EXPORT week;
extern const Dimension::Time SIREUNITS_EXPORT weeks;
extern const Dimension::Time SIREUNITS_EXPORT fortnight;
extern const Dimension::Time SIREUNITS_EXPORT fortnights;

extern const Dimension::Force SIREUNITS_EXPORT newton;
extern const Dimension::Force SIREUNITS_EXPORT newtons;
extern const Dimension::Force SIREUNITS_EXPORT ounce;
extern const Dimension::Force SIREUNITS_EXPORT ounces;
extern const Dimension::Force SIREUNITS_EXPORT pound;
extern const Dimension::Force SIREUNITS_EXPORT pounds;
extern const Dimension::Force SIREUNITS_EXPORT stone;
extern const Dimension::Force SIREUNITS_EXPORT stones;
extern const Dimension::Force SIREUNITS_EXPORT hundredweight;

extern const Dimension::Pressure SIREUNITS_EXPORT pascal;
extern const Dimension::Pressure SIREUNITS_EXPORT pascals;
extern const Dimension::Pressure SIREUNITS_EXPORT bar;
extern const Dimension::Pressure SIREUNITS_EXPORT atm;
extern const Dimension::Pressure SIREUNITS_EXPORT psi;
extern const Dimension::Pressure SIREUNITS_EXPORT mmHg;

extern const Dimension::Temperature SIREUNITS_EXPORT kelvin;

extern const Dimension::Current SIREUNITS_EXPORT amp;
extern const Dimension::Current SIREUNITS_EXPORT amps;
extern const Dimension::Potential SIREUNITS_EXPORT volt;
extern const Dimension::Potential SIREUNITS_EXPORT volts;
extern const Dimension::Capacitance SIREUNITS_EXPORT farad;
extern const Dimension::Capacitance SIREUNITS_EXPORT farads;
extern const Dimension::Power SIREUNITS_EXPORT watt;
extern const Dimension::Power SIREUNITS_EXPORT watts;
extern const Dimension::MolarPower SIREUNITS_EXPORT watts_per_mol;

extern const Dimension::Velocity SIREUNITS_EXPORT c;
extern const Dimension::Acceleration SIREUNITS_EXPORT g_accel;
extern const Dimension::Mass SIREUNITS_EXPORT electron_mass;
extern const Dimension::Mass SIREUNITS_EXPORT proton_mass;
extern const Dimension::Mass SIREUNITS_EXPORT neutron_mass;
extern const Dimension::Mass SIREUNITS_EXPORT atomic_mass_constant;
extern const Dimension::MolarVolume SIREUNITS_EXPORT molar_volume;

extern const Dimension::GeneralUnit SIREUNITS_EXPORT epsilon0;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT four_pi_eps0;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT one_over_four_pi_eps0;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT gasr;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT k_boltz;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT mu0;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT G_newton;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT h_planck;
extern const Dimension::GeneralUnit SIREUNITS_EXPORT h_slash;

}

SIRE_END_HEADER

#endif
