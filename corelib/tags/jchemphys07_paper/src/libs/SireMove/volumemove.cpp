/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "volumemove.h"

#include "SireSystem/simsystem.h"
#include "SireSystem/checkpoint.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;
using namespace SireUnits;

static const RegisterMetaType<VolumeMove> r_volmove;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds,
                                        const VolumeMove &volmove)
{
    writeHeader(ds, r_volmove, 1);

    SharedDataStream sds(ds);

    sds << volmove.mapfunc << volmove.volchanger << volmove.p
        << static_cast<const MonteCarlo&>(volmove);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds,
                                        VolumeMove &volmove)
{
    VersionID v = readHeader(ds, r_volmove);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> volmove.mapfunc >> volmove.volchanger
            >> volmove.p >> static_cast<MonteCarlo&>(volmove);
    }
    else
        throw version_error(v, "1", r_volmove, CODELOC);

    return ds;
}

/** Null constructor */
VolumeMove::VolumeMove(const RanGenerator &generator)
           : MonteCarlo(generator), p( 1 * atm )
{}

/** Construct a VolumeMove that moves the molecules in the passed
    mapping function */
VolumeMove::VolumeMove(const MolMappingFunction &mappingfunction,
                       const RanGenerator &generator)
           : MonteCarlo(generator), mapfunc(mappingfunction),
             p( 1 * atm )
{}

/** Construct a VolumeMove that moves the molecules in the passed
    mapping function using the passed volume changing function */
VolumeMove::VolumeMove(const MolMappingFunction &mappingfunction,
                       const VolChangingFunction &changefunc,
                       const RanGenerator &generator)
           : MonteCarlo(generator), mapfunc(mappingfunction),
             volchanger(changefunc), p( 1 * atm )
{}

/** Copy constructor */
VolumeMove::VolumeMove(const VolumeMove &other)
           : MonteCarlo(other), mapfunc(other.mapfunc),
             volchanger(other.volchanger), p(other.p)
{}

/** Destructor */
VolumeMove::~VolumeMove()
{}

/** Copy assignment operator */
VolumeMove& VolumeMove::operator=(const VolumeMove &other)
{
    if (this != &other)
    {
        MonteCarlo::operator=(other);
        mapfunc = other.mapfunc;
        volchanger = other.volchanger;
        p = other.p;
    }

    return *this;
}

/** Set the pressure that this volume move will target */
void VolumeMove::setPressure(SireUnits::Dimension::Pressure pressure)
{
    p = pressure;
}

/** Return the pressure that this volume move is targetting */
SireUnits::Dimension::Pressure VolumeMove::pressure() const
{
    return SireUnits::Dimension::Pressure(p);
}

/** Set the mapping function that is used to map the molecules from
    one space to another - this function also contains all of the molecules
    that will be mapped */
void VolumeMove::setMappingFunction(const MolMappingFunction &mappingfunction)
{
    mapfunc = mappingfunction;
}

/** Return the mapping function that is used to map the system molecules
    from one space to another */
const MolMappingFunction& VolumeMove::mappingFunction() const
{
    return mapfunc;
}

/** Set the function that is used to change the volume of the space */
void VolumeMove::setVolumeChangingFunction(const VolChangingFunction &changefunc)
{
    volchanger = changefunc;
}

/** Return the function that is used to change the volume of the space */
const VolChangingFunction& VolumeMove::volumeChangingFunction() const
{
    return volchanger;
}

/** The actual NPT Monte Carlo test */
bool VolumeMove::nptTest(double new_nrg, double old_nrg,
                         double new_volume, double old_volume,
                         int nmolecules)
{
    double p_deltav = p * (new_volume - old_volume);

    double vratio = nmolecules * std::log(new_volume / old_volume);

    double x = std::exp( -beta * (new_nrg - old_nrg + p_deltav) + vratio );

    if ( x > 1 or x > rangenerator.rand() )
    {
        ++naccept;
        return true;
    }
    else
    {
        ++nreject;
        return false;
    }
}

/** Perform a volume Monte Carlo move */
void VolumeMove::move(SimSystem &system)
{
    //get the old energy and old space
    double old_nrg = energy(system);
    Space old_space = system.space();

    //save the old configuration
    CheckPoint checkpoint = system.checkPoint();

    try
    {
        //change the volume
        Space new_space = volchanger.change(old_space, rangenerator);

        //change any molecules that need to be mapped
        Molecules mapped_mols = mapfunc.map(system, new_space);

        //now change the system
        system.setSpace(new_space, mapped_mols);

        //get the new energy
        double new_nrg = energy(system);

        //use the NPT MC test
        if ( not nptTest(new_nrg, old_nrg,
                         new_space.volume(), old_space.volume(),
                         system.nMolecules()) )
        {
            //go back to the old system
            system.rollBack(checkpoint);
        }
    }
    catch(...)
    {
        system.rollBack(checkpoint);
        throw;
    }
}

/** Assert that this move is compatible with the passed system */
void VolumeMove::assertCompatibleWith(QuerySystem &system) const
{
    mapfunc.assertCompatibleWith(system);
    volchanger.assertCompatibleWith(system.space());

    MonteCarlo::assertCompatibleWith(system);
}
