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
#include "flexibility.h"
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
    writeHeader(ds, r_movermove, 1);
    
    SharedDataStream sds(ds);
    
    sds << movermove.smplr 
	<< movermove.flexibility_property
	<< static_cast<const MonteCarlo&>(movermove);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MoverMove &movermove)
{
    VersionID v = readHeader(ds, r_movermove);

    if (v == 1)
      {
	SharedDataStream sds(ds);
    
	sds >> movermove.smplr 
	    >> movermove.flexibility_property
	    >> static_cast<MonteCarlo&>(movermove);
      }
    else
      throw version_error(v, "1", r_movermove, CODELOC);

    return ds;
}

/** Null constructor */
MoverMove::MoverMove(const PropertyMap &map) 
  : ConcreteProperty<MoverMove,MonteCarlo>(map)
{
  flexibility_property = map["flexibility"];
  MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
}

/** Construct the mover move for the passed group of molecules */
MoverMove::MoverMove(const MoleculeGroup &molgroup, const PropertyMap &map)
         : ConcreteProperty<MoverMove,MonteCarlo>(),
           smplr( UniformSampler(molgroup) )
{
  flexibility_property = map["flexibility"];
  MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
  smplr.edit().setGenerator( this->generator() );
}

/** Construct the mover move that samples molecules from the
    passed sampler */
MoverMove::MoverMove(const Sampler &sampler, const PropertyMap &map)
         : ConcreteProperty<MoverMove,MonteCarlo>(),
           smplr(sampler)
{
  flexibility_property = map["flexibility"];
  MonteCarlo::setEnsemble( Ensemble::NVT(25*celsius) );
  smplr.edit().setGenerator( this->generator() );
}

/** Copy constructor */
MoverMove::MoverMove(const MoverMove &other)
         : ConcreteProperty<MoverMove,MonteCarlo>(other),
           smplr(other.smplr),
	   flexibility_property(other.flexibility_property)
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
	flexibility_property = other.flexibility_property;
    }
    
    return *this;
}

/** Comparison operator */
bool MoverMove::operator==(const MoverMove &other) const
{
  return MonteCarlo::operator==(other) and smplr == other.smplr 
    and flexibility_property == other.flexibility_property;
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

/** Return the property used to find the flexibility of each molecule*/
const PropertyName& MoverMove::flexibilityProperty() const
{
  return flexibility_property;
}

/** Set the name of the property used to find the flexibility of each molecule */
void MoverMove::setFlexibilityProperty(const PropertyName &property)
{
  flexibility_property = property;
  Move::setProperty("flexibility", flexibility_property);
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
//void MoverMove::setBonds(const QList<BondID> &bonds)
//{
//  this->bonds = bonds;
//}
/* Set the list of variable angles */
//void MoverMove::setAngles(const QList<AngleID> &angles)
//{
//  this->angles = angles;
//}
/* Set the list of variable dihedrals */
//void MoverMove::setDihedrals(const QList<DihedralID> &dihedrals)
//{
//  this->dihedrals = dihedrals;
//}
/* Set the dictionnary of delta values for bonds*/
//void MoverMove::setBondDeltas(const QHash<DofID, SireUnits::Dimension::Length> &bond_deltas)
//{
//  this->bond_deltas = bond_deltas;
//}
/* Set the dictionnary of delta values for angles*/
//void MoverMove::setAngleDeltas(const QHash<DofID, SireUnits::Dimension::Angle> &angle_deltas)
//{
//  this->angle_deltas = angle_deltas;
//}

/* Return the list of variable bonds */
//const QList<BondID>& MoverMove::getBonds()
//{
//  return this->bonds;
//}
/* Return the list of variable angles*/
//const QList<AngleID>& MoverMove::getAngles()
//{
//  return this->angles;
//}
/* Return the list of variable dihedrals */
//const QList<DihedralID>& MoverMove::getDihedrals()
//{
//  return this->dihedrals;
//}
/* Return the dictionnary of delta values for bonds*/
//const QHash<DofID,SireUnits::Dimension::Length>& MoverMove::getBondDeltas()
//{
//  return this->bond_deltas;
//}
/* Return the dictionnary of delta values for angles*/
//const QHash<DofID,SireUnits::Dimension::Angle>& MoverMove::getAngleDeltas()
//{
//  return this->angle_deltas;
//}

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
	
	  Flexibility flex = oldmol.property(flexibility_property).asA<Flexibility>();

	  // Select the dofs to move. 
	  QList<BondID> moved_bonds;
	  QList<AngleID> moved_angles;
	  QList<DihedralID> moved_dihedrals;

	  int maxvar = flex.maximumVar();
	  int nbonds = flex.bonds.size();
	  int nangles = flex.angles.size();
	  int ndihedrals = flex.dihedrals.size();
	  int ndofs = nbonds + nangles + ndihedrals ;


	  //qDebug() << "There are " << nbonds << " bonds ";
	  //qDebug() << "There are " << nangles << " angles ";
	  //qDebug() << "There are " << ndihedrals << " dihedrals ";

	  if ( ndofs == 0 || maxvar < 0 || maxvar >= ndofs  )
	    {
	      // We move everything in these cases..
	      moved_bonds = flex.bonds;
	      moved_angles = flex.angles;
	      moved_dihedrals = flex.dihedrals;
	    }
	  else
	    {
	      // draw a random number [0, bonds.size()+angles.size()+dihedrals.size()) to find matching dof. 
	      // Add it to moved_bonds or moved_angles or moved_dihedrals if not already present	
	      int movecount = 0;
	      
	      while (movecount < maxvar)
		{
		  int rand = this->generator().randInt(0, ndofs - 1);
		  //qDebug() << " rand is " << rand;
		  if ( rand < nbonds )
		    {
		      // it is a bond...
		      const BondID &bond = flex.bonds.at( rand );
		      if ( not  moved_bonds.contains(bond) )
			{
			  //qDebug() << " adding bond " << rand;
			  moved_bonds.append(bond);
			  movecount++;
			}
		    }
		  else if ( rand < ( nbonds + nangles ) )
		    {
		      // it is an angle...
		      const AngleID &angle = flex.angles.at( rand - nbonds );
		      if ( not moved_angles.contains(angle) )
			{
			  //qDebug() << " adding angle " << rand - nbonds;
			  moved_angles.append(angle);
			  movecount++;
			}
		    }
		  else
		    {
		      // it is a dihedral...
		      const DihedralID &dihedral = flex.dihedrals.at( rand - nbonds - nangles );
		      if ( not moved_dihedrals.contains(dihedral) )
			{
			  //qDebug() << " adding dihedral " << rand - nbonds - nangles;
			  moved_dihedrals.append(dihedral);
			  movecount++;
			}
		    }
		}
	    }

	  // Now actually move the selected dofs

	  Mover<Molecule> mol_mover = oldmol.molecule().move();

	  // move the bonds of this molecule
	  Length bond_delta;
	  foreach (const BondID &bond, moved_bonds)
	    {
	      //const Length bond_delta_value = flex.bond_deltas[bond];
	      double bond_delta_value = flex.bond_deltas[bond].to(angstrom);
	      bond_delta = Length( this->generator().rand(-bond_delta_value, bond_delta_value) );
	      mol_mover.change(bond, bond_delta);
	    }

	  // and the angles
	  Angle angle_delta;
	  foreach (const AngleID &angle, moved_angles)
	    {
	      //const Angle angle_delta_value = flex.angle_deltas[angle];
	      double angle_delta_value = flex.angle_deltas[angle].to(radians);
	      angle_delta = Angle( this->generator().rand(-angle_delta_value,angle_delta_value) );	      
	      mol_mover.change(angle, angle_delta);
	    }
	  
	  // and the torsions
	  Angle dihedral_delta;
 	  foreach (const DihedralID &dihedral, moved_dihedrals)
	    {
	      // We rotate by picking the central bond of the dihedral to handle concerted motions
	      BondID centralbond;
	      centralbond = BondID(dihedral.atom1(), dihedral.atom2());
	      //const Angle angle_delta_value = flex.angle_deltas[dihedral];
	      double angle_delta_value = flex.angle_deltas[dihedral].to(radians);
	      dihedral_delta =  Angle( this->generator().rand(-angle_delta_value,angle_delta_value) );
	      mol_mover.change(centralbond, dihedral_delta);
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

/** Loop over all bond, angles and dihedrals. Multiply their delta value by scale 
with probability prob */
// void MoverMove::changeDeltas(float prob, float scale)
// {
//   /**upper and lower bounds for bond, angle and dihedrals */
//   Length lower_bond = Length(0.001*angstrom);
//   Length upper_bond = Length(0.1*angstrom);
//   Angle lower_angle = Angle(0.01*degree);
//   Angle upper_angle = Angle(5.0*degree);
//   Angle lower_dihedral = Angle(0.5*degree); 
//   Angle upper_dihedral = Angle(15.0*degree);

//   const RanGenerator ran;
 
//   foreach (const BondID &bond, this->bonds)
//     {
//       if (ran.rand() <= prob) 
// 	{
// 	  Length newdelta = Length(this->bond_deltas[bond].value()*scale);
// 	  if (newdelta < lower_bond)
// 	    newdelta = lower_bond;
// 	  else if (newdelta > upper_bond)
// 	    newdelta = upper_bond;
// 	  this->setDelta(bond, newdelta);
// 	}
//     }
//   foreach (const AngleID &angle, this->angles)
//     {
//       if (ran.rand() <= prob) 
// 	{
// 	  Angle newdelta = Angle(this->angle_deltas[angle].value()*scale);
// 	  if (newdelta < lower_angle)
// 	    newdelta = lower_angle;
// 	  else if (newdelta > upper_angle)
// 	    newdelta = upper_angle;
// 	  this->setDelta(angle, newdelta);
// 	}
//     }
//   foreach (const DihedralID &dihedral, this->dihedrals)
//     {
//       if (ran.rand() <= prob) 
// 	{
// 	  Angle newdelta = Angle(this->angle_deltas[dihedral].value()*scale);
// 	  if (newdelta < lower_dihedral)
// 	    newdelta = lower_dihedral;
// 	  else if (newdelta > upper_dihedral)
// 	    newdelta = upper_dihedral;
// 	  this->setDelta(dihedral, newdelta);
// 	}
//     }
// }
// /** Set the delta value of a bond*/
// void MoverMove::setDelta(const BondID &bond, SireUnits::Dimension::Length delta)
// {
//   //What to do if bond is not in bond_deltas?
//   this->bond_deltas[bond] = delta;
// }

// /** Set the delta value of an angle*/
// void MoverMove::setDelta(const AngleID &angle, SireUnits::Dimension::Angle delta)
// {
//   //What to do if angle is not in angle_deltas?
//   this->angle_deltas[angle] = delta;
// }

// /** Set the delta value of a dihedral*/
// void MoverMove::setDelta(const DihedralID &dihedral, SireUnits::Dimension::Angle delta)
// {
//   //What to do if dihedral is not in angle_deltas?
//   this->angle_deltas[dihedral] = delta;
// }

const char* MoverMove::typeName()
{
    return QMetaType::typeName( qMetaTypeId<MoverMove>() );
}

//////
////// Implementation of detail::DofID
//////

static const RegisterMetaType<DofID> r_dofid;

QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const DofID &dofid)
{

  writeHeader( ds, r_dofid, 2);
  SharedDataStream sds(ds);
  
  sds << dofid.idx0 << dofid.idx1
      << dofid.idx2 << dofid.idx3;
       
    return ds;
}

QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, DofID &dofid)
{
   VersionID v = readHeader(ds, r_dofid);

   if (v == 2)
   {
       SharedDataStream sds(ds);
   
       sds >> dofid.idx0 >> dofid.idx1
           >> dofid.idx2 >> dofid.idx3;
   }
   else
       throw version_error(v, "2", r_dofid, CODELOC);

   return ds;
}

//** Constructor from a set of 4 qtin32 atomic indices */
DofID::DofID(qint32 index0, qint32 index1, qint32 index2, qint32 index3) 
  : idx0(index0), idx1(index1), idx2(index2), idx3(index3)
{}

//** Constructor for a set of 2 AtomIDs*/
DofID::DofID(const AtomID &atom0, const AtomID &atom1)
  : idx0( atom0.asA<AtomIdx>().value() ), idx1(atom1.asA<AtomIdx>().value() ),
    idx2(-1), idx3(-1)
    {} 

//** Constructor for a set of 3 AtomIDs*/
DofID::DofID(const AtomID &atom0, const AtomID &atom1, const AtomID &atom2)
  : idx0( atom0.asA<AtomIdx>().value() ), idx1(atom1.asA<AtomIdx>().value() ),
    idx2( atom2.asA<AtomIdx>().value() ), idx3(-1)
    {} 

//** Constructor for a set of 4 AtomIDs*/
DofID::DofID(const AtomID &atom0, const AtomID &atom1, const AtomID &atom2, const AtomID &atom3)
  : idx0( atom0.asA<AtomIdx>().value() ), idx1(atom1.asA<AtomIdx>().value() ),
    idx2( atom2.asA<AtomIdx>().value() ), idx3(atom3.asA<AtomIdx>().value() )
    {} 
//** Constructor for a set of 2 AtomIdxs*/
DofID::DofID(const AtomIdx &atom0, const AtomIdx &atom1)
  : idx0( atom0.value() ), idx1(atom1.value() ),
    idx2(-1), idx3(-1)
    {} 

//** Constructor for a set of 3 AtomIdxs*/
DofID::DofID(const AtomIdx &atom0, const AtomIdx &atom1, const AtomIdx &atom2)
  : idx0( atom0.value() ), idx1(atom1.value() ),
    idx2( atom2.value() ), idx3(-1)
    {} 

//** Constructor for a set of 4 AtomIdxs*/
DofID::DofID(const AtomIdx &atom0, const AtomIdx &atom1, const AtomIdx &atom2, const AtomIdx &atom3)
  : idx0( atom0.value() ), idx1(atom1.value() ),
    idx2( atom2.value() ), idx3(atom3.value() )
    {} 


//** Constructor for one BondID*/
DofID:: DofID(const BondID &bond) 
  : idx0( bond.atom0().asA<AtomIdx>().value() ), idx1(bond.atom1().asA<AtomIdx>().value() ),
    idx2(-1), idx3(-1)
     {} 

//** Constructor for one AngleID*/
DofID:: DofID(const AngleID &angle) 
  : idx0( angle.atom0().asA<AtomIdx>().value() ), idx1(angle.atom1().asA<AtomIdx>().value() ),
    idx2( angle.atom2().asA<AtomIdx>().value() ), idx3(-1)
     {} 

//** Constructor for one DihedralID*/
DofID:: DofID(const DihedralID &dihedral) 
  : idx0( dihedral.atom0().asA<AtomIdx>().value() ), idx1(dihedral.atom1().asA<AtomIdx>().value() ),
    idx2( dihedral.atom2().asA<AtomIdx>().value() ), idx3(dihedral.atom3().asA<AtomIdx>().value() )
     {} 

DofID::DofID(const DofID &other)
       : idx0(other.idx0), idx1(other.idx1), 
         idx2(other.idx2), idx3(other.idx3)
{}

DofID::~DofID()
{}

DofID& DofID::operator=(const DofID &other)
{
    idx0 = other.idx0;
    idx1 = other.idx1;
    idx2 = other.idx2;
    idx3 = other.idx3;
    
    return *this;
}

bool DofID::operator==(const DofID &other) const
{
    return idx0 == other.idx0 and idx1 == other.idx1 and
           idx2 == other.idx2 and idx3 == other.idx3;
}

bool DofID::operator!=(const DofID &other) const
{
    return idx0 != other.idx0 or idx1 != other.idx1 or
           idx2 != other.idx2 or idx3 != other.idx3;
}

/** Return a hash of this identifier */
//uint DofID::hash() const
//{
//  return (idx0 << 24) | 
//       ( (idx1 << 16) & 0x00FF0000) |
//       ( (idx2 << 8)  & 0x0000FF00) |
//       (idx3 & 0x000000FF);
//}
const char* DofID::typeName()
{
    return QMetaType::typeName( qMetaTypeId<DofID>() );
}
