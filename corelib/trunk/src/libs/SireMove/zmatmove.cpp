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

#include "zmatmove.h"

#include "zmatrix.h"
#include "ensemble.h"

#include "SireSystem/system.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/moleditor.h"
#include "SireMol/mover.hpp"
#include "SireMol/atomidx.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/temperature.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include <QDebug>
#include <QTime>

using namespace SireMove;
using namespace SireMol;
using namespace SireSystem;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

static const RegisterMetaType<ZMatMove> r_zmatmove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const ZMatMove &zmatmove)
{
    writeHeader(ds, r_zmatmove, 1);
    
    SharedDataStream sds(ds);
    
    sds << zmatmove.smplr << zmatmove.zmatrix_property
        << static_cast<const MonteCarlo&>(zmatmove);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, ZMatMove &zmatmove)
{
    VersionID v = readHeader(ds, r_zmatmove);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> zmatmove.smplr >> zmatmove.zmatrix_property
            >> static_cast<MonteCarlo&>(zmatmove);
    }
    else
        throw version_error(v, "1", r_zmatmove, CODELOC);
        
    return ds;
}

/** Null constructor */
ZMatMove::ZMatMove() 
         : ConcreteProperty<ZMatMove,MonteCarlo>(),
           zmatrix_property( "z-matrix" )
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
}

/** Construct the z-matrix move for the passed group of molecules */
ZMatMove::ZMatMove(const MoleculeGroup &molgroup)
         : ConcreteProperty<ZMatMove,MonteCarlo>(),
           smplr( UniformSampler(molgroup) ),
           zmatrix_property( "z-matrix" )
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Construct the z-matrix move that samples molecules from the
    passed sampler */
ZMatMove::ZMatMove(const Sampler &sampler)
         : ConcreteProperty<ZMatMove,MonteCarlo>(),
           smplr(sampler),
           zmatrix_property( "z-matrix" )
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Copy constructor */
ZMatMove::ZMatMove(const ZMatMove &other)
         : ConcreteProperty<ZMatMove,MonteCarlo>(other),
           smplr(other.smplr),
           zmatrix_property(other.zmatrix_property)
{}

/** Destructor */
ZMatMove::~ZMatMove()
{}

/** Copy assignment operator */
ZMatMove& ZMatMove::operator=(const ZMatMove &other)
{
    MonteCarlo::operator=(other);
    smplr = other.smplr;
    zmatrix_property = other.zmatrix_property;
    
    return *this;
}

/** Comparison operator */
bool ZMatMove::operator==(const ZMatMove &other) const
{
    return MonteCarlo::operator==(other) and smplr == other.smplr and
           zmatrix_property == other.zmatrix_property;
}

/** Comparison operator */
bool ZMatMove::operator!=(const ZMatMove &other) const
{
    return MonteCarlo::operator!=(other) or smplr != other.smplr or
           zmatrix_property != other.zmatrix_property;
}

/** Return a string representation of this move */
QString ZMatMove::toString() const
{
    return QObject::tr("ZMatMove( nAccepted() = %1 nRejected() == %2 )")
                .arg( this->nAccepted() )
                .arg( this->nRejected() );
}

/** Set the sampler used to sample molecules for this move */
void ZMatMove::setSampler(const Sampler &sampler)
{
    smplr = sampler;
    smplr.edit().setGenerator( this->generator() );
}

/** Set the sampler so that it draws molecules uniformly from the
    molecule group 'molgroup' */
void ZMatMove::setSampler(const MoleculeGroup &molgroup)
{
    this->setSampler( UniformSampler(molgroup) );
}

/** Return the sampler used to sample molecules to move */
const Sampler& ZMatMove::sampler() const
{
    return smplr;
}

/** Return the molecule group that is sampled for this move */
const MoleculeGroup& ZMatMove::moleculeGroup() const
{
    return smplr->group();
}

/** Return the property used to find the z-matrix of each molecule */
const PropertyName& ZMatMove::zmatrixProperty() const
{
    return zmatrix_property;
}
    
/** Set the name of the property used to find the z-matrix of each molecule */
void ZMatMove::setZMatrixProperty(const PropertyName &property)
{
    zmatrix_property = property;
}

/** Set the random number generator used to generate the random
    number used for this move */
void ZMatMove::setGenerator(const RanGenerator &rangenerator)
{
    MonteCarlo::setGenerator(rangenerator);
    smplr.edit().setGenerator(this->generator());
}

/** Internal function used to set the ensemble based on the
    passed temperature */
void ZMatMove::_pvt_setTemperature(const Temperature &temperature)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(temperature) );
}

/** Internal function used to move the bond, angle and dihedral
    that is used to build the atom 'atom' in the z-matrix 'zmatrix' */
void ZMatMove::move(AtomIdx atom, ZMatrixCoords &zmatrix)
{
    Length bonddelta = zmatrix.bondDelta(atom);
    Angle angledelta = zmatrix.angleDelta(atom);
    Angle dihedraldelta = zmatrix.dihedralDelta(atom);
    
    if (bonddelta.value() != 0)
    {
        zmatrix.moveBond(atom, Length(this->generator().rand( -bonddelta.value(),
                                                               bonddelta.value() ) ) );
    }
    
    if (angledelta.value() != 0)
    {
        zmatrix.moveAngle(atom, Angle(this->generator().rand( -angledelta.value(),
                                                               angledelta.value() ) ) );
    }

    if (dihedraldelta.value() != 0)
    {
        zmatrix.moveDihedral(atom, Angle(this->generator().rand( 
                                                      -dihedraldelta.value(),
                                                       dihedraldelta.value() ) ) );
    }
}

/** Actually perform 'nmoves' moves of the molecules in the 
    system 'system', optionally recording simulation statistics
    if 'record_stats' is true */
void ZMatMove::move(System &system, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
        return;
      
    //save our, and the system's, current state
    ZMatMove old_state(*this);
    
    System old_system_state(system);
    
    try
    {
        PropertyMap map;
        map.set("coordinates", this->coordinatesProperty());
        map.set("z-matrix", this->zmatrixProperty());
        
        for (int i=0; i<nmoves; ++i)
        {
            //get the old energy of the system
            double old_nrg = system.energy( this->energyComponent() );
            
            //save the old system and sampler
            System old_system(system);
            SamplerPtr old_sampler(smplr);
    
            //update the sampler with the latest version of the molecules
            smplr.edit().updateFrom(system);

            //randomly select a molecule to move
            tuple<PartialMolecule,double> mol_and_bias = smplr.read().sample();

            const PartialMolecule &oldmol = mol_and_bias.get<0>();
            double old_bias = mol_and_bias.get<1>();

            ZMatrixCoords zmatrix( oldmol, map );

            //move the internal coordinates of selected atoms in the 
            //z-matrix
            AtomSelection selected_atoms = oldmol.selection();
            
            if (selected_atoms.selectedAll())
            {
                //move everything
                for (QHash<AtomIdx,int>::const_iterator 
                                                it = zmatrix.index().constBegin();
                     it != zmatrix.index().constEnd();
                     ++it)
                {
                    this->move(it.key(), zmatrix);
                }
            }
            else
            {
                //move only the selected atoms
                for (QHash<AtomIdx,int>::const_iterator 
                                                it = zmatrix.index().constBegin();
                     it != zmatrix.index().constEnd();
                     ++it)
                {
                    if (selected_atoms.selected(it.key()))
                        this->move(it.key(), zmatrix);
                }
            }

            Molecule newmol = oldmol.molecule().edit()
                                    .setProperty( map["coordinates"], 
                                                  zmatrix.toCartesian() )
                                    .commit();
            
            //update the system with the new coordinates
            system.update(newmol);

            //calculate the energy of the system
            double new_nrg = system.energy( this->energyComponent() );

            //get the new bias on this molecule
            smplr.edit().updateFrom(system);
        
            double new_bias = smplr.read().probabilityOf( PartialMolecule(newmol,
                                                              oldmol.selection()) );

            //accept or reject the move based on the change of energy
            //and the biasing factors
            if (not this->test(new_nrg, old_nrg, new_bias, old_bias))
            {
                //the move has been rejected - reset the state
                smplr = old_sampler;
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

const char* ZMatMove::typeName()
{
    return QMetaType::typeName( qMetaTypeId<ZMatMove>() );
}
