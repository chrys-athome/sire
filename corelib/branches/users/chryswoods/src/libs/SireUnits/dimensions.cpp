/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#include "dimensions.h"
#include "units.h"
#include "temperature.h"
#include "generalunit.h"

#include "Siren/logger.h"
#include "Siren/tester.h"
#include "Siren/exception.h"

using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace Siren;


static const RegisterPrimitive<SireUnits::Dimension::Dimensionless> r_dimensionless;
static const RegisterPrimitive<SireUnits::Dimension::Mass> r_mass;
static const RegisterPrimitive<SireUnits::Dimension::MolarMass> r_molarmass;
static const RegisterPrimitive<SireUnits::Dimension::Length> r_length;
static const RegisterPrimitive<SireUnits::Dimension::Time> r_time;
static const RegisterPrimitive<SireUnits::Dimension::Charge> r_charge;
static const RegisterPrimitive<SireUnits::Dimension::MolarCharge> r_molarcharge;
static const RegisterPrimitive<SireUnits::Dimension::Temperature> r_temperature;
static const RegisterPrimitive<SireUnits::Dimension::Quantity> r_quantity;
static const RegisterPrimitive<SireUnits::Dimension::Angle> r_angle;
static const RegisterPrimitive<SireUnits::Dimension::Area> r_area;
static const RegisterPrimitive<SireUnits::Dimension::Volume> r_volume;
static const RegisterPrimitive<SireUnits::Dimension::MolarVolume> r_molarvolume;
static const RegisterPrimitive<SireUnits::Dimension::Velocity> r_velocity;
static const RegisterPrimitive<SireUnits::Dimension::AngularVelocity> r_angvelocity;
static const RegisterPrimitive<SireUnits::Dimension::Acceleration> r_acceleration;
static const RegisterPrimitive<SireUnits::Dimension::AngularAcceleration> r_angaccel;
static const RegisterPrimitive<SireUnits::Dimension::Energy> r_energy;
static const RegisterPrimitive<SireUnits::Dimension::MolarEnergy> r_molarenergy;
static const RegisterPrimitive<SireUnits::Dimension::Power> r_power;
static const RegisterPrimitive<SireUnits::Dimension::MolarPower> r_molarpower;
static const RegisterPrimitive<SireUnits::Dimension::Density> r_density;
static const RegisterPrimitive<SireUnits::Dimension::MolarDensity> r_molardensity;
static const RegisterPrimitive<SireUnits::Dimension::Force> r_force;
static const RegisterPrimitive<SireUnits::Dimension::Pressure> r_pressure;
static const RegisterPrimitive<SireUnits::Dimension::Capacitance> r_capacitance;
static const RegisterPrimitive<SireUnits::Dimension::Current> r_current;
static const RegisterPrimitive<SireUnits::Dimension::Potential> r_potential;

Unit::Unit(const detail::Temperature &temperature) 
     : sclfac( temperature.in(kelvin) )
{}

double Unit::convertToInternal(double value) const
{
    return value * sclfac;
}

double Unit::convertFromInternal(double value) const
{
    return value / sclfac;
}

uint Unit::hashCode(int M, int L, int T, int C, int t, int Q, int A) const
{
    return M + L + T + C + t + Q + A + *((uint*)(&sclfac));
}

bool Unit::test(Logger &logger,
                int M, int L, int T, int C, int t, int Q, int A) const
{
    Tester tester( QString("SireUnits::Dimension::PhysUnit<%1,%2,%3,%4,%5,%6,%7>")
                        .arg(M).arg(L).arg(T).arg(C).arg(t).arg(Q).arg(A), logger );

    #ifndef SIREN_DISABLE_TESTS
                        
    try
    {
        GeneralUnit unit(M, L, T, C, t, Q, A);
        
    }
    catch (const Siren::exception &e) 
    {
        tester.unexpected_error(e);
    }
        
    #endif // SIREN_DISABLE_TESTS
        
    return tester.allPassed();
}
