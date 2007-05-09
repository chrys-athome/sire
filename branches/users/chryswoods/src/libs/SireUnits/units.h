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

#ifndef SIREUNITS_CONSTANTS_H
#define SIREUNITS_CONSTANTS_H

#include <limits>

#include "SireMaths/constants.h"

SIRE_BEGIN_HEADER

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
// First, the base unit class                  //
/////////////////////////////////////////////////

namespace Dimension
{

/** This is the base class of all units of a dimension - this provides
    the interface to the functions used to convert values
    to and from this unit */
class Unit
{
public:
      virtual ~Unit()
      {}

      virtual double convertFromInternal(double val) const
      {
          return val / relativeScale();
      }

      virtual double convertToInternal(double val) const
      {
          return val * relativeScale();
      }

      virtual double relativeScale() const
      {
          return sclfac;
      }

protected:
      Unit(double scale_factor) : sclfac(scale_factor)
      {}

      Unit(const Unit &other) : sclfac(other.sclfac)
      {}

private:
      double sclfac;
};

/** This class is used to represent a derived unit - a derived
    unit is one that is formed by combining together other units. */
class DerivedUnit : public Unit
{
public:
      DerivedUnit() : Unit(1)
      {}

      explicit DerivedUnit(double scale) : Unit(scale)
      {}

      explicit DerivedUnit(const Unit &unit)  : Unit(unit)
      {}

      DerivedUnit(const DerivedUnit &other)
              : Unit(other)
      {}

      ~DerivedUnit()
      {}

      DerivedUnit multiply(const Unit &unit) const
      {
          return DerivedUnit( relativeScale() * unit.relativeScale() );
      }

      DerivedUnit divide(const Unit &unit) const
      {
          return DerivedUnit( relativeScale() / unit.relativeScale() );
      }
};

inline double operator*(double val, const DerivedUnit &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const DerivedUnit &unit)
{
    return unit.convertFromInternal(val);
}

inline DerivedUnit operator*(const Unit &unit0, const Unit &unit1)
{
    return DerivedUnit(unit0).multiply(unit1);
}

inline DerivedUnit operator/(const Unit &unit0, const Unit &unit1)
{
    return DerivedUnit(unit0).divide(unit1);
}

} // end of namespace Dimension

/////////////////////////////////////////////////
// First, basic, dimensionless constants       //
/////////////////////////////////////////////////

namespace Dimension
{

class Quantity : protected Unit
{
public:
    Quantity(double scale_factor) : Unit(scale_factor)
    {}

    explicit Quantity(const DerivedUnit &unit) : Unit(unit)
    {}

    ~Quantity()
    {}

    double convertFromInternal(double val) const
    {
        return Unit::convertFromInternal(val);
    }

    double convertToInternal(double val) const
    {
        return Unit::convertToInternal(val);
    }

    double relativeScale() const
    {
        return Unit::relativeScale();
    }
};

inline double operator*(double val, const Quantity &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const Quantity &unit)
{
    return unit.convertFromInternal(val);
}

inline Quantity operator*(const Quantity &unit, const Quantity &amount)
{
    return Quantity( unit.relativeScale() * amount.relativeScale() );
}

inline Quantity operator/(const Quantity &unit, const Quantity &amount)
{
    return Quantity( unit.relativeScale() / amount.relativeScale() );
}

inline DerivedUnit operator*(const DerivedUnit &unit, const Quantity &amount)
{
    return DerivedUnit( unit.relativeScale() * amount.relativeScale() );
}

inline DerivedUnit operator/(const DerivedUnit &unit, const Quantity &amount)
{
    return DerivedUnit( unit.relativeScale() / amount.relativeScale() );
}

inline DerivedUnit operator*(const Quantity &amount, const DerivedUnit &unit)
{
    return DerivedUnit( unit.relativeScale() * amount.relativeScale() );
}

inline DerivedUnit operator/(const Quantity &amount, const DerivedUnit &unit)
{
    return DerivedUnit( amount.relativeScale() / unit.relativeScale() );
}

} // end of namespace Dimension

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

namespace Dimension
{

class Length : public Unit
{
public:
    Length(double scale_factor) : Unit(scale_factor)
    {}

    explicit Length(const DerivedUnit &unit) : Unit(unit)
    {}

    ~Length()
    {}
};

inline double operator*(double val, const Length &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const Length &unit)
{
    return unit.convertFromInternal(val);
}

inline Length operator*(const Length &unit, const Quantity &amount)
{
    return Length( unit.relativeScale() * amount.relativeScale() );
}

inline Length operator/(const Length &unit, const Quantity &amount)
{
    return Length( unit.relativeScale() / amount.relativeScale() );
}

inline Length operator*(const Quantity &amount, const Length &unit)
{
    return Length( unit.relativeScale() * amount.relativeScale() );
}

inline Length operator/(const Quantity &amount, const Length &unit)
{
    return Length( amount.relativeScale() / unit.relativeScale() );
}

} // end of namespace Dimension

/** Convert angstroms into internal units */
const Dimension::Length angstrom(1);

/** Convert nanometers into internal units */
const Dimension::Length nanometer( 10 * angstrom );

/** Convert picometers into internal units */
const Dimension::Length picometer( 1000 * nanometer );

/** Convert meters into internal units */
const Dimension::Length meter( 1.0e9 * nanometer );

/** Convert Bohr radii into internal units */
const Dimension::Length bohr_radii( 0.529177249 * angstrom );

const Dimension::Length centimeter( 0.01 * meter );
const Dimension::Length kilometer( 1000 * meter );

const Dimension::Length inch( 2.54 * centimeter );
const Dimension::Length foot( 12 * inch );
const Dimension::Length yard( 3 * foot );
const Dimension::Length mile( 1760 * yard );

/////////////////////////////////////////////////
// Units of time. Internal unit = femtoseconds //
/////////////////////////////////////

namespace Dimension
{

class Time : public Unit
{
public:
    Time(double scale_factor) : Unit(scale_factor)
    {}

    explicit Time(const DerivedUnit &unit) : Unit(unit)
    {}

    ~Time()
    {}
};

inline double operator*(double val, const Time &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const Time &unit)
{
    return unit.convertFromInternal(val);
}

inline Time operator*(const Time &unit, const Quantity &amount)
{
    return Time( unit.relativeScale() * amount.relativeScale() );
}

inline Time operator/(const Time &unit, const Quantity &amount)
{
    return Time( unit.relativeScale() / amount.relativeScale() );
}

inline Time operator*(const Quantity &amount, const Time &unit)
{
    return Time( unit.relativeScale() * amount.relativeScale() );
}

inline Time operator/(const Quantity &amount, const Time &unit)
{
    return Time( amount.relativeScale() / unit.relativeScale() );
}

} // end of namespace Dimension

/** Convert femtoseconds into internal units */
const Dimension::Time femtosecond(1);

/** Convert picoseconds into internal units */
const Dimension::Time picosecond(1000 * femtosecond);

/** Convert nanoseconds into internal units */
const Dimension::Time nanosecond(1000 * picosecond);

const Dimension::Time microsecond(1000 * nanosecond);
const Dimension::Time millisecond(1000 * microsecond);

/** Convert seconds into internal units */
const Dimension::Time second(1000 * millisecond);

const Dimension::Time minute( 60 * second );
const Dimension::Time hour( 60 * minute );
const Dimension::Time day( 24 * hour );

///////////////////////////////////////////////////////
// Units of mass. Internal unit = g mol-1            //
///////////////////////////////////////////////////////

namespace Dimension
{

class Mass : public Unit
{
public:
    Mass(double scale_factor) : Unit(scale_factor)
    {}

    explicit Mass(const DerivedUnit &unit) : Unit(unit)
    {}

    ~Mass()
    {}
};

inline double operator*(double val, const Mass &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const Mass &unit)
{
    return unit.convertFromInternal(val);
}

inline Mass operator*(const Mass &unit, const Quantity &amount)
{
    return Mass( unit.relativeScale() * amount.relativeScale() );
}

inline Mass operator/(const Mass &unit, const Quantity &amount)
{
    return Mass( unit.relativeScale() / amount.relativeScale() );
}

inline Mass operator*(const Quantity &amount, const Mass &unit)
{
    return Mass( unit.relativeScale() * amount.relativeScale() );
}

inline Mass operator/(const Quantity &amount, const Mass &unit)
{
    return Mass( amount.relativeScale() / unit.relativeScale() );
}

} // end of namespace Dimension

/** Convert g mol-1 to internal units */
const Dimension::Mass g_per_mol(1);
const Dimension::Mass gram( mole * g_per_mol );
const Dimension::Mass kilogram( 1000 * gram );
const Dimension::Mass kg_per_mol( 1000 * g_per_mol );

///////////////////////////////////////////////////////
// Units of Charge. Internal unit = |electron| mol-1 //
///////////////////////////////////////////////////////

namespace Dimension
{

class Charge : public Unit
{
public:
    Charge(double scale_factor) : Unit(scale_factor)
    {}

    explicit Charge(const DerivedUnit &unit) : Unit(unit)
    {}

    ~Charge()
    {}
};

inline double operator*(double val, const Charge &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const Charge &unit)
{
    return unit.convertFromInternal(val);
}

inline Charge operator*(const Charge &unit, const Quantity &amount)
{
    return Charge( unit.relativeScale() * amount.relativeScale() );
}

inline Charge operator/(const Charge &unit, const Quantity &amount)
{
    return Charge( unit.relativeScale() / amount.relativeScale() );
}

inline Charge operator*(const Quantity &amount, const Charge &unit)
{
    return Charge( unit.relativeScale() * amount.relativeScale() );
}

inline Charge operator/(const Quantity &amount, const Charge &unit)
{
    return Charge( amount.relativeScale() / unit.relativeScale() );
}

} // end of namespace Dimension

/** Convert electron charges to internal units
    ( 1 |e| is the charge in coulombs of one mole of electrons) */
const Dimension::Charge mod_electrons(1);

/** Convert Coulombs to internal units */
const Dimension::Charge coulomb( (1.0/1.60217646263e-19) * mod_electrons );

/** Unit charge, 1.602176462(63) * 10-19 C */
const Dimension::Charge e_charge(1);

/////////////////////////////////////////////////
// Units of Energy. Internal unit = kcal mol-1 //
/////////////////////////////////////////////////

namespace Dimension
{

class Energy : public Unit
{
public:
    Energy(double scale_factor) : Unit(scale_factor)
    {}

    explicit Energy(const DerivedUnit &unit) : Unit(unit)
    {}

    ~Energy()
    {}
};

inline double operator*(double val, const Energy &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const Energy &unit)
{
    return unit.convertFromInternal(val);
}

inline Energy operator*(const Energy &unit, const Quantity &amount)
{
    return Energy( unit.relativeScale() * amount.relativeScale() );
}

inline Energy operator/(const Energy &unit, const Quantity &amount)
{
    return Energy( unit.relativeScale() / amount.relativeScale() );
}

inline Energy operator*(const Quantity &amount, const Energy &unit)
{
    return Energy( unit.relativeScale() * amount.relativeScale() );
}

inline Energy operator/(const Quantity &amount, const Energy &unit)
{
    return Energy( amount.relativeScale() / unit.relativeScale() );
}

} // end of namespace Dimension

/** Conversion factor from kcal mol-1 (thermal) to internal units (kcal mol-1).  */
const Dimension::Energy kcal_per_mol(1);
const Dimension::Energy kcal( mole * kcal_per_mol );

const Dimension::Energy cal_per_mol( 0.001 * kcal_per_mol );
const Dimension::Energy cal( 0.001 * kcal );

const Dimension::Energy kJ_per_mol( (1.0/4.184) * kcal_per_mol );
const Dimension::Energy kilojoule( mole * kJ_per_mol );

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
const Dimension::DerivedUnit newton_per_mol( J_per_mol / meter );

/** Convert a force in Newtons to internal units */
const Dimension::DerivedUnit newton( joule / meter );

//////////////////////////////////////////////////////////
// Units of pressure. Internal units = kcal mol-1 A-2   //
//////////////////////////////////////////////////////////

/** Convert pressure in Pascals (N m-2) to internal units (where does the mol-1 come from..?) */
const Dimension::DerivedUnit pascal( newton / (meter*meter) );

/** Convert atmospheres to internal units */
const Dimension::DerivedUnit atm( 101325.0 * pascal );

//////////////////////////////////////////////////////////
// Units of temperature. Internal units = Kelvin        //
//////////////////////////////////////////////////////////

namespace Dimension
{

class Temperature : public Unit
{
public:
    ~Temperature()
    {}

protected:
    Temperature(double scale_factor) : Unit(scale_factor)
    {}

    explicit Temperature(const DerivedUnit &unit) : Unit(unit)
    {}
};

inline double operator*(double val, const Temperature &unit)
{
    return unit.convertToInternal(val);
}

inline double operator/(double val, const Temperature &unit)
{
    return unit.convertFromInternal(val);
}

} // end of namespace Dimension

class Kelvin : public Dimension::Temperature
{
public:
    Kelvin() : Dimension::Temperature(1)
    {}

    explicit Kelvin(const Dimension::DerivedUnit &unit)
              : Dimension::Temperature(unit)
    {}

    ~Kelvin()
    {}
};

class Celsius : public Dimension::Temperature
{
public:
    Celsius() : Dimension::Temperature(1)
    {}

    explicit Celsius(const Dimension::DerivedUnit &unit)
              : Dimension::Temperature(unit)
    {}

    ~Celsius()
    {}

    double convertFromInternal(double val) const
    {
        //internal units are kelvin
        return val - 273.15;
    }

    double convertToInternal(double val) const
    {
        //internal units are kelvin
        return val + 273.15;
    }
};

class Fahrenheit : public Dimension::Temperature
{
public:
    Fahrenheit() : Dimension::Temperature(1.0 / 1.8)
    {}

    ~Fahrenheit()
    {}

    explicit Fahrenheit(const Dimension::DerivedUnit &unit)
                : Dimension::Temperature(unit)
    {}

    double convertFromInternal(double val) const
    {
        return (val*1.8) - 459.67;
    }

    double convertToInternal(double val) const
    {
        return (val + 459.67) / 1.8;
    }
};

const Kelvin kelvin;
const Celsius celsius;
const Fahrenheit fahrenheit;

//////////////////////////////////////////////////////////
// Now some miscellaneous units                         //
//////////////////////////////////////////////////////////

/** Convert the units of current (amps) */
const Dimension::DerivedUnit amp( coulomb / second );
/** Volts */
const Dimension::DerivedUnit volt( joule / coulomb );
/** Convert the units of capacitance (farads) */
const Dimension::DerivedUnit farad( coulomb / volt );
/** Convert power in Watts */
const Dimension::DerivedUnit watt( joule / second );

///////////////////////////////////////////////////////////
// Now physical constants converted into internal units. //
//  The values of these are taken from the 1998 CODATA   //
//  values - see fundemental_constants.pdf in techdocs   //
///////////////////////////////////////////////////////////

/** Speed of light in a vacuum */
const Dimension::DerivedUnit c( 299792458.0 * (meter / second) );

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
const Dimension::DerivedUnit molar_volume(
                                  (22.41399639e-3 * (meter*meter*meter)) / mole );

}

SIRE_END_HEADER

#endif
