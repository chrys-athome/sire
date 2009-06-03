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

#include "SireSystem/system.h"
#include "SireVol/space.h"

#include "SireMol/molecule.h"
#include "SireMol/mover.hpp"

#include "SireUnits/dimensions.h"
#include "SireUnits/units.h"
#include "SireUnits/temperature.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireVol;
using namespace SireSystem;
using namespace SireBase;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

static const RegisterMetaType<VolumeMove> r_volmove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const VolumeMove &volmove)
{
    writeHeader(ds, r_volmove, 1);
    
    ds << double(volmove.maxchange) 
       << static_cast<const MonteCarlo&>(volmove);
       
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, VolumeMove &volmove)
{
    VersionID v = readHeader(ds, r_volmove);
    
    if (v == 1)
    {
        double maxchange;
        ds >> maxchange >> static_cast<MonteCarlo&>(volmove);
        
        volmove.maxchange = Volume(maxchange);
    }
    else
        throw version_error( v, "1", r_volmove, CODELOC );
        
    return ds;
}

/** Construct a volume move that can be used to generate the ensemble
    for a temperature of 25 C, pressure of 1 atm, and with a maximum 
    change of 100 A^3 */
VolumeMove::VolumeMove()
           : ConcreteProperty<VolumeMove,MonteCarlo>(),
             maxchange(100)
{
    MonteCarlo::setEnsemble( Ensemble::NPT( 25 * celsius, 1 * atm ) );
}

/** Construct a volume move that can be used to generate the ensemble
    for a temperature of 25 C, pressure of 1 atm, 
    and with a maximum change of 'maxchange' */
VolumeMove::VolumeMove(const Volume &maxvolchange)
           : ConcreteProperty<VolumeMove,MonteCarlo>(),
             maxchange(maxvolchange)
{
    MonteCarlo::setEnsemble( Ensemble::NPT( 25 * celsius, 1 * atm ) );
}

/** Copy constructor */
VolumeMove::VolumeMove(const VolumeMove &other)
           : ConcreteProperty<VolumeMove,MonteCarlo>(other),
             maxchange(other.maxchange)
{}

/** Destructor */
VolumeMove::~VolumeMove()
{}

/** Copy assignment operator */
VolumeMove& VolumeMove::operator=(const VolumeMove &other)
{
    maxchange = other.maxchange;
    MonteCarlo::operator=(other);
    
    return *this;
}

/** Comparison operator */
bool VolumeMove::operator==(const VolumeMove &other) const
{
    return maxchange == other.maxchange and
           MonteCarlo::operator==(other);
}

/** Comparison operator */
bool VolumeMove::operator!=(const VolumeMove &other) const
{
    return maxchange != other.maxchange or
           MonteCarlo::operator!=(other);
}

/** Return a string representation of this move */
QString VolumeMove::toString() const
{
    return QObject::tr("VolumeMove( maximumVolumeChange() = %1 A^3 "
                       "nAccepted() = %2 nRejected() = %3 )")
                  .arg(this->maximumVolumeChange().to(angstrom3))
                  .arg(this->nAccepted())
                  .arg(this->nRejected());
}

/** Internal function called to set the temperature */
void VolumeMove::_pvt_setTemperature(const Temperature &temperature)
{
    MonteCarlo::setEnsemble( Ensemble::NPT( temperature, this->pressure() ) );
}

/** Internal function called to set the pressure */
void VolumeMove::_pvt_setPressure(const Pressure &pressure)
{
    MonteCarlo::setEnsemble( Ensemble::NPT( this->temperature(), pressure ) );
}

/** Set the maximum change in volume */
void VolumeMove::setMaximumVolumeChange(const Volume &delta)
{
    maxchange = delta;
}

/** Return the maximum change of volume attempted by a move */
const Volume& VolumeMove::maximumVolumeChange() const
{
    return maxchange;
}

/** Perform 'nmoves' volume moves on the passed system, optionally
    recording simulation statistics if 'record_stats' is true */
void VolumeMove::move(System &system, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
        return;

    System old_system_state(system);
    VolumeMove old_state(*this);
    
    try
    {
        PropertyMap map;
        map.set("coordinates", this->coordinatesProperty());

        for (int i=0; i<nmoves; ++i)
        {
            System old_system(system);
        
            const Space &old_space = system.property( this->spaceProperty() )
                                           .asA<Space>();

            //calculate the old energy and volume
            double old_nrg = system.energy( this->energyComponent() );
            Volume old_vol = old_space.volume();
            
            Volume new_vol = old_vol + Volume( generator().rand(-maxchange, maxchange) );
            
            //create the new space
            SpacePtr new_space = old_space.setVolume(new_vol);
            
            //read the new volume - it may be slightly different
            //due to round off error
            new_vol = new_space.read().volume();
            
            //set the new space
            if (this->spaceProperty().hasSource())
                system.setProperty( this->spaceProperty().source(), new_space );
            
            //now get all of the molecules in the system
            Molecules all_mols = system.molecules();
            
            //map all of the molecules into the new space
            for (Molecules::const_iterator it = all_mols.constBegin();
                 it != all_mols.constEnd();
                 ++it)
            {
                Molecule mol = it->molecule()
                                        .move().changeSpace(old_space, new_space, map);

                all_mols.update(mol);
            }
            
            //update all of the molecules
            system.mustNowRecalculateFromScratch();
            system.update(all_mols);
            
            //calculate the new energy
            double new_nrg = system.energy();
            
            if (not this->test(new_nrg, old_nrg, all_mols.nMolecules(),
                               new_vol, old_vol))
            {
                //move failed - go back to the last step
                system = old_system;
            }
            
            if (record_stats)
            {
                system.collectStats();
            }
        }
    }
    catch(...)
    {
        system = old_system_state;
        this->operator=(old_state);
        throw;
    }
    
}

const char* VolumeMove::typeName()
{
    return QMetaType::typeName( qMetaTypeId<VolumeMove>() );
}
