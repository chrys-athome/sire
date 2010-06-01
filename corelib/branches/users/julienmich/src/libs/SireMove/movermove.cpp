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

#include "movermove.h"

/*#include "zmatrix.h"*/
#include "ensemble.h"

#include "SireSystem/system.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/moleditor.h"
#include "SireMol/mover.hpp"
#include "SireMol/atomidx.h"
#include "SireMol/connectivity.h"
#include "SireMol/bondid.h"

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

static const RegisterMetaType<MoverMove> r_movermove;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MoverMove &movermove)
{
    writeHeader(ds, r_movermove, 2);
    
    SharedDataStream sds(ds);
    
    sds << movermove.smplr << movermove.sync_bonds 
	<< movermove.sync_angles
        << movermove.sync_dihedrals
        << static_cast<const MonteCarlo&>(movermove);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MoverMove &movermove)
{
    VersionID v = readHeader(ds, r_movermove);
    /*JM I probably do not need multple versions...*/
    if (v == 2)
    {
        SharedDataStream sds(ds);
        
        sds >> movermove.smplr >> movermove.sync_bonds 
	    >> movermove.sync_angles
            >> movermove.sync_dihedrals
            >> static_cast<MonteCarlo&>(movermove);
    }
    else if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> movermove.smplr >> static_cast<MonteCarlo&>(movermove);
            
        movermove.sync_bonds = false;
        movermove.sync_angles = false;
        movermove.sync_dihedrals = false;
    }
    else
        throw version_error(v, "1", r_movermove, CODELOC);
        
    return ds;
}

/** Null constructor */
MoverMove::MoverMove() 
         : ConcreteProperty<MoverMove,MonteCarlo>(),
           /*zmatrix_property( "z-matrix" ),*/
           sync_bonds(false), sync_angles(false),
           sync_dihedrals(false)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
}

/** Construct the mover move for the passed group of molecules */
MoverMove::MoverMove(const MoleculeGroup &molgroup)
         : ConcreteProperty<MoverMove,MonteCarlo>(),
           smplr( UniformSampler(molgroup) ),
           /*zmatrix_property( "z-matrix" ),*/
           sync_bonds(false), sync_angles(false),
           sync_dihedrals(false)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Construct the mover move that samples molecules from the
    passed sampler */
MoverMove::MoverMove(const Sampler &sampler)
         : ConcreteProperty<MoverMove,MonteCarlo>(),
           smplr(sampler),
           /*zmatrix_property( "z-matrix" ),*/
           sync_bonds(false), sync_angles(false),
           sync_dihedrals(false)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Copy constructor */
MoverMove::MoverMove(const MoverMove &other)
         : ConcreteProperty<MoverMove,MonteCarlo>(other),
           smplr(other.smplr),
           /*zmatrix_property(other.zmatrix_property),*/
           sync_bonds(other.sync_bonds), sync_angles(other.sync_angles),
           sync_dihedrals(other.sync_dihedrals)
{}

/** Destructor */
MoverMove::~MoverMove()
{}

/** Copy assignment operator */
MoverMove& MoverMove::operator=(const MoverMove &other)
{
    if (this != &other)
    {
        MonteCarlo::operator=(other);
        smplr = other.smplr;
        /*zmatrix_property = other.zmatrix_property;*/
        sync_bonds = other.sync_bonds;
        sync_angles = other.sync_angles;
        sync_dihedrals = other.sync_dihedrals;
    }
    
    return *this;
}

/** Comparison operator */
bool MoverMove::operator==(const MoverMove &other) const
{
    return MonteCarlo::operator==(other) and smplr == other.smplr and
           sync_bonds == other.sync_bonds and sync_angles == other.sync_angles and
      sync_dihedrals == other.sync_dihedrals ; 
    /*and zmatrix_property == other.zmatrix_property;*/
}

/** Comparison operator */
bool MoverMove::operator!=(const MoverMove &other) const
{
    return not MoverMove::operator==(other);
}

/** Return a string representation of this move */
QString MoverMove::toString() const
{
    return QObject::tr("MoverMove( nAccepted() = %1 nRejected() == %2 )")
                .arg( this->nAccepted() )
                .arg( this->nRejected() );
}

/** Set the sampler used to sample molecules for this move */
void MoverMove::setSampler(const Sampler &sampler)
{
    smplr = sampler;
    smplr.edit().setGenerator( this->generator() );
}

/** Set the sampler so that it draws molecules uniformly from the
    molecule group 'molgroup' */
void MoverMove::setSampler(const MoleculeGroup &molgroup)
{
    this->setSampler( UniformSampler(molgroup) );
}

/** Return the sampler used to sample molecules to move */
const Sampler& MoverMove::sampler() const
{
    return smplr;
}

/** Return the molecule group that is sampled for this move */
const MoleculeGroup& MoverMove::moleculeGroup() const
{
    return smplr->group();
}

/** Return the property used to find the z-matrix of each molecule */
/*const PropertyName& MoverMove::zmatrixProperty() const
{
    return zmatrix_property;
    }*/
    
/** Set the name of the property used to find the z-matrix of each molecule */
 /*void ZMatMove::setZMatrixProperty(const PropertyName &property)
{
    zmatrix_property = property;
}*/

/** Set whether or not to synchronise all motion for all molecules 
    in the group */
void MoverMove::setSynchronisedMotion(bool on)
{
    sync_bonds = on;
    sync_angles = on;
    sync_dihedrals = on;
}

/** Set whether or not to synchronise all bond moves for all molecules */
void MoverMove::setSynchronisedBonds(bool on)
{
    sync_bonds = on;
}

/** Set whether or not to synchronise all angle moves for all molecules */
void MoverMove::setSynchronisedAngles(bool on)
{
    sync_angles = on;
}

/** Set whether or not to synchronise all dihedral moves for all molecules */
void MoverMove::setSynchronisedDihedrals(bool on)
{
    sync_dihedrals = on;
}

/** Return whether or not all moves for all molecules are synchronised */
bool MoverMove::synchronisedMotion() const
{
    return sync_bonds and sync_angles and sync_dihedrals;
}

/** Return whether or not all bond moves for all molecules
    are synchronised */
bool MoverMove::synchronisedBonds() const
{
    return sync_bonds;
}

/** Return whether or not all angle moves for all molecules
    are synchronised */
bool MoverMove::synchronisedAngles() const
{
    return sync_angles;
}

/** Return whether or not all dihedral moves for all molecules
    are synchronised */
bool MoverMove::synchronisedDihedrals() const
{
    return sync_dihedrals;
}

/** Set the random number generator used to generate the random
    number used for this move */
void MoverMove::setGenerator(const RanGenerator &rangenerator)
{
    MonteCarlo::setGenerator(rangenerator);
    smplr.edit().setGenerator(this->generator());
}

/** Internal function used to set the ensemble based on the
    passed temperature */
void MoverMove::_pvt_setTemperature(const Temperature &temperature)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(temperature) );
}

/* Internal function. not yet implemented */
void MoverMove::move(AtomIdx atom,
                    QHash< AtomIdx, tuple<Length,Angle,Angle> > &saved_deltas)
{
}
/** Actually perform 'nmoves' moves of the molecules in the 
    system 'system', optionally recording simulation statistics
    if 'record_stats' is true */
void MoverMove::move(System &system, int nmoves, bool record_stats)
{
    if (nmoves <= 0)
      return;

    MoverMove old_state(*this);
    System old_system_state(system);

    try
    {
        PropertyMap map;
        map.set("coordinates", this->coordinatesProperty());
    
	for (int i=0; i<nmoves; ++i)
	{
	  double old_nrg = system.energy( this->energyComponent() );
	  System old_system(system);
	  SamplerPtr old_sampler(smplr);
      
	  double old_bias = 1;
	  double new_bias = 1;
	  
	  //move one molecule
	  //update the sampler with the latest version of the molecules
	  smplr.edit().updateFrom(system);
	  //this will randomly select one molecule
	  tuple<PartialMolecule,double> mol_and_bias = smplr.read().sample();
	  
	  const PartialMolecule &oldmol = mol_and_bias.get<0>();
	  old_bias = mol_and_bias.get<1>();
	
	  const Connectivity &connectivity = oldmol.property( map["connectivity"] ).asA<Connectivity>();
	  // select the bonds in one molecule and move them 
	  QList<BondID> bonds = connectivity.getBonds();
	  Length bond_delta = 0.1*angstrom;
	  foreach (const BondID &bond, bonds)
	    {
	      oldmol = oldmol.move().change(bond,bond_delta).commit();
	    }
	  Molecule newmol = oldmol.molecule();
	  //update the system with the new coordinates
	  system.update(newmol);
	  //get the new bias on this molecule
	  smplr.edit().updateFrom(system);
	  
	  new_bias = smplr.read().probabilityOf( PartialMolecule(newmol,
								 oldmol.selection()) );
	  //calculate the energy of the system
	  double new_nrg = system.energy( this->energyComponent() );
	
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

const char* MoverMove::typeName()
{
    return QMetaType::typeName( qMetaTypeId<MoverMove>() );
}
