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

#include "ensemble.h"

#include "SireSystem/system.h"

#include "SireMol/partialmolecule.h"
#include "SireMol/molecule.h"
#include "SireMol/moleditor.h"
#include "SireMol/mover.hpp"
#include "SireMol/atomidx.h"
#include "SireMol/connectivity.h"
#include "SireMol/bondid.h"
#include "SireMol/angleid.h"
#include "SireMol/dihedralid.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/units.h"
#include "SireUnits/temperature.h"
#include "SireUnits/units.h"

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
    
    sds << movermove.smplr 
	<< movermove.bonds << movermove.angles << movermove.dihedrals
	<< static_cast<const MonteCarlo&>(movermove);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MoverMove &movermove)
{
    SharedDataStream sds(ds);
    
    sds >> movermove.smplr 
	>> movermove.bonds >> movermove.angles >> movermove.dihedrals 
	>> static_cast<MonteCarlo&>(movermove);
        
    return ds;
}

/** Null constructor */
MoverMove::MoverMove() 
         : ConcreteProperty<MoverMove,MonteCarlo>()
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
}

/** Construct the mover move for the passed group of molecules */
MoverMove::MoverMove(const MoleculeGroup &molgroup)
         : ConcreteProperty<MoverMove,MonteCarlo>(),
           smplr( UniformSampler(molgroup) )
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Construct the mover move that samples molecules from the
    passed sampler */
MoverMove::MoverMove(const Sampler &sampler)
         : ConcreteProperty<MoverMove,MonteCarlo>(),
           smplr(sampler)
{
    MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
    smplr.edit().setGenerator( this->generator() );
}

/** Copy constructor */
MoverMove::MoverMove(const MoverMove &other)
         : ConcreteProperty<MoverMove,MonteCarlo>(other),
           smplr(other.smplr),
	   bonds(other.bonds),angles(other.angles),dihedrals(other.dihedrals)
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
	bonds = other.bonds;
	angles = other.angles;
	dihedrals = other.dihedrals;
    }
    
    return *this;
}

/** Comparison operator */
bool MoverMove::operator==(const MoverMove &other) const
{
  return MonteCarlo::operator==(other) and smplr == other.smplr 
    and bonds == other.bonds and angles == other.angles and 
    dihedrals == other.dihedrals;
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
/* Set the list of variable bonds */
void MoverMove::setBonds(const QList<BondID> &bonds)
{
  this->bonds = bonds;
}
/* Set the list of variable angles */
void MoverMove::setAngles(const QList<AngleID> &angles)
{
  this->angles = angles;
}
/* Set the list of variable dihedrals */
void MoverMove::setDihedrals(const QList<DihedralID> &dihedrals)
{
  this->dihedrals = dihedrals;
}
/* Return the list of variable bonds */
const QList<BondID>& MoverMove::getBonds()
{
  return this->bonds;
}
/* Return the list of variable angles*/
const QList<AngleID>& MoverMove::getAngles()
{
  return this->angles;
}
/* Return the list of variable dihedrals */
const QList<DihedralID>& MoverMove::getDihedrals()
{
  return this->dihedrals;
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
	  
	  PartialMolecule oldmol = mol_and_bias.get<0>();
	  old_bias = mol_and_bias.get<1>();
	
	  //const Connectivity &connectivity = oldmol.property( map["connectivity"] ).asA<Connectivity>();
	  Mover<Molecule> mol_mover = oldmol.molecule().move();

	  /**TODO 
	     - TEST ON MODEL MOLECULES THAT DETAILED BALANCE HOLDS
	       ongoing...

	     - CACHE THE BONDS/ANGLES/DIHEDRALS FOR SPEED 
             - ADD OPTIONS TO NOT SAMPLE BONDS OR ANGLES OR TORSIONS
             - ADD ABILITY TO FREEZE SELECTED DOFS
	         For the time being, the last three options are done by letting the user set 
		 the bonds/angles/dihedrals to be sampled. 
		 This concept however does not seem very general because it basically assumes 
                 that MoverMove operates on only one molecule

	     - CORRELATE TORSION CHANGES AROUND A BOND FOR EFFICIENCY
	     will do that when testing ethane, benzene and indole

	     - PASS A DICTIONARY OF DELTA VALUES TO THE CONSTRUCTOR FOR THE BONDS, ANGLES, DIHEDRALS
	       will do that in a while, when everything else is working and I want to optimize the 
	       acceptance rate
	  */

	  // move the bonds 
	  //QList<BondID> bonds = connectivity.getBonds();
	  Length bond_delta;
	  foreach (const BondID &bond, this->bonds)
	    {
	      bond_delta = Length( this->generator().rand(-0.02*angstrom, 0.02*angstrom ) );
	      mol_mover.change(bond,bond_delta);
	    }

	  // and the angles
	  //QList<AngleID> angles = connectivity.getAngles();
	  Angle angle_delta;
	  foreach (const AngleID &angle,this->angles)
	    {
	      angle_delta = Angle( this->generator().rand(-0.5*degrees,0.5*degrees) );	      
	      mol_mover.change(angle,angle_delta);
	    }
	    // and the torsions
	  //QList<DihedralID> dihedrals = connectivity.getDihedrals();
	  Angle dihedral_delta;
 	  foreach (const DihedralID &dihedral,this->dihedrals)
	    {
	      //dihedral_delta = Angle( this->generator().rand(-15.0*degrees,15.0*degrees) );
	      // We rotate by picking the central bond of the dihedral to handle concerted motions
	      BondID centralbond;
	      centralbond = BondID(dihedral.atom1(), dihedral.atom2());
	      dihedral_delta = Angle (15.0*degrees);
	      mol_mover.change(centralbond,dihedral_delta);
	    }

	  //update the system with the new coordinates
	  Molecule newmol = mol_mover.commit();
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
