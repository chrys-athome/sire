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

#include "flexibility.h"

#include <QList>

#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/bondid.h"
#include "SireMol/angleid.h"
#include "SireMol/dihedralid.h"
#include "SireMol/atomidx.h"

#include "SireUnits/convert.h"
#include "SireUnits/units.h"

#include "SireMove/errors.h"
#include "SireMol/errors.h"
#include "SireError/errors.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireMol;
using namespace SireUnits;
using namespace SireUnits::Dimension;
using namespace SireStream;

//
// Implementation of Flexibility
//
static const RegisterMetaType<Flexibility> r_flex;

/** Serialise to a binary datastream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const Flexibility &flex)
{
    writeHeader(ds, r_flex, 1);
    
    SharedDataStream sds(ds);
    
    sds << flex.molinfo << flex.maxtranslation 
	<< flex.maxrotation << flex.bonds 
	<< flex.angles << flex.dihedrals 
	<< flex.bond_deltas << flex.angle_deltas 
        << static_cast<const MoleculeProperty&>(flex);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, Flexibility &flex)
{
    VersionID v = readHeader(ds, r_flex);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        sds >> flex.molinfo >> flex.maxtranslation
	    >> flex.maxrotation >> flex.bonds
	    >> flex.angles >> flex.dihedrals
	    >> flex.bond_deltas >> flex.angle_deltas
            >> static_cast<MoleculeProperty&>(flex);
    }
    else
        throw version_error( v, "1", r_flex, CODELOC );
        
    return ds;
}

/** Null Constructor */
Flexibility::Flexibility()
{}

/** Constructor for the passed molecule*/
Flexibility::Flexibility(const Molecule &molecule)
  : ConcreteProperty<Flexibility,MoleculeProperty>(),
    molinfo(molecule.data().info())
{}

/** Copy constructor */
Flexibility::Flexibility(const Flexibility &other)
  : ConcreteProperty<Flexibility,MoleculeProperty>(),
    molinfo(other.molinfo),maxtranslation(other.maxtranslation),
    maxrotation(other.maxrotation),bonds(other.bonds),
    angles(other.angles),dihedrals(other.dihedrals),
    bond_deltas(other.bond_deltas),angle_deltas(other.angle_deltas)
{}

/** Destructor */
Flexibility::~Flexibility()
{}

/** Copy assignment operator */
Flexibility& Flexibility::operator=(const Flexibility &other)
{
  if (this != &other)
    {
      MoleculeProperty::operator=(other);
      molinfo = other.molinfo;
      maxtranslation = other.maxtranslation;
      maxrotation = other.maxrotation;
      bonds = other.bonds;
      angles = other.angles;
      dihedrals = other.dihedrals;
      bond_deltas = other.bond_deltas;
      angle_deltas = other.angle_deltas;
    }
  
  return *this;
}

/** Comparison operator */
bool Flexibility::operator==(const Flexibility &other) const
{
  return ( molinfo == other.molinfo and maxtranslation == other.maxtranslation 
	   and maxrotation == other.maxrotation and bonds == other.bonds 
	   and angles == other.angles and dihedrals == other.dihedrals and 
	   bond_deltas == other.bond_deltas and angle_deltas == other.angle_deltas );
}

/** Comparison operator */
bool Flexibility::operator!=(const Flexibility &other) const
{
  return not this->operator==(other);
}

/** Return the layout of the molecule whose flexibility is contained
    in this object */
const MoleculeInfoData& Flexibility::info() const
{
    if (molinfo.constData() == 0)
        return MoleculeInfoData::null();
    else
        return *molinfo;
}

/** Return a string representation of this flexibility */
QString Flexibility::toString() const
{
    QStringList lines;
    
    lines.append( QObject::tr("Flexibility ")  );
    lines.append( QObject::tr("Rotation: %1 , Translation: %2 ")
		  .arg(this->maxrotation.toString())
		  .arg(this->maxtranslation.toString()) );

    foreach (BondID bond, this->bonds)
	lines.append( QObject::tr("%1 %2")
		      .arg(bond.toString())
		      .arg(this->bond_deltas[bond].toString()) );

    foreach (AngleID angle, this->angles)
      lines.append( QObject::tr("%1 %2")
		    .arg(angle.toString())
		    .arg(this->angle_deltas[angle].toString()) );

    foreach (DihedralID dihedral, this->dihedrals)
      lines.append( QObject::tr("%1 %2")
		    .arg(dihedral.toString())
		    .arg(this->angle_deltas[dihedral].toString()) );

    return lines.join("\n");
}

/** *Return whether or not this flexibility is compatible with the molecule whose info is in 'molinfo'*/
bool Flexibility::isCompatibleWith(const SireMol::MoleculeInfoData &molinfo) const
{
    return info().UID() == molinfo.UID();
}

/** Set the maximum rotation of this flexibility*/
void Flexibility::setRotation(const Angle &rotation)
{
  this->maxrotation = rotation;
}

/** Set the maximum translation of this flexibility*/
void Flexibility::setTranslation(const Length &translation)
{
  this->maxtranslation = translation;
}

/** Return the maximum rotation of this flexibility*/
Angle Flexibility::rotation() const
{
  return this->maxrotation;
}

/** Return the maximum translation of this flexibility */
Length Flexibility::translation() const
{
  return this->maxtranslation;
}

/** Add bond with delta to this flexibility*/
void Flexibility::add( const BondID &bond, const Length &delta)
{
  // the bond could be between atoms that are not in this molecule --> throw SireMol::missing_atom
  // QList<AtomIdx> atoms = this->molinfo.getAtoms(); // Obviously I don't understand what a SharedDataPointer is
  if (not this->bonds.contains(bond) )
    this->bonds.append(bond);
  this->bond_deltas[bond] = delta;
}

/** Add angle with delta to this flexibility*/
void Flexibility::add( const AngleID &angle, const Angle &delta)
{
  if (not this->angles.contains(angle) )
    this->angles.append(angle);
  this->angle_deltas[angle] = delta;
}

/** Add dihedral with delta to this flexibility*/
void Flexibility::add( const DihedralID &dihedral, const Angle &delta)
{
  if (not this->dihedrals.contains(dihedral) )
    this->dihedrals.append(dihedral);
  this->angle_deltas[dihedral] = delta;
}

/** Remove bond from this flexibility*/
void Flexibility::remove( const BondID &bond)
{
  if ( this->bonds.contains(bond) )
    {
    this->bonds.removeAll(bond); 
    this->bond_deltas.remove(bond);
    }
}

/** Remove angle from this flexibility*/
void Flexibility::remove( const AngleID &angle)
{
  if ( this->angles.contains(angle) )
    {
    this->angles.removeAll(angle); 
    this->angle_deltas.remove(angle);
    }
}

/** Remove dihedral from this flexibility*/
void Flexibility::remove( const DihedralID &dihedral)
{
  if ( this->dihedrals.contains(dihedral) )
    {
    this->dihedrals.removeAll(dihedral); 
    this->angle_deltas.remove(dihedral);
    }
}

/** Check if bond is present in this flexibility */
bool Flexibility::contains(const BondID &bond) const
{
  if ( this->bonds.contains(bond) )
    return true;
  else
    return false;
}

/** Check if angle is present in this flexibility */
bool Flexibility::contains(const AngleID &angle) const
{
  if ( this->angles.contains(angle) )
    return true;
  else
    return false;
}

/** Check if angle is present in this flexibility */
bool Flexibility::contains(const DihedralID &dihedral) const
{
  if ( this->dihedrals.contains(dihedral) )
    return true;
  else
    return false;
}

/** set the delta value of bond to delta*/
void Flexibility::setDelta(const BondID &bond, const Length &delta)
{
  if ( not this->bonds.contains(bond) )
    throw SireMol::missing_bond( QObject::tr(
					     "There is no bond %1 in this flexibility")
                                 .arg(bond.toString()), CODELOC );
  this->bond_deltas[bond] = delta;
}

/** set the delta value of bond to delta*/
void Flexibility::setDelta(const AngleID &angle, const Angle &delta)
{
  if ( not this->angles.contains(angle) )
    throw SireMol::missing_angle( QObject::tr(
					     "There is no angle %1 in this flexibility")
                                 .arg(angle.toString()), CODELOC );
  this->angle_deltas[angle] = delta;
}

/** set the delta value of bond to delta*/
void Flexibility::setDelta(const DihedralID &dihedral, const Angle &delta)
{
  if ( not this->dihedrals.contains(dihedral) )
    throw SireMol::missing_dihedral( QObject::tr(
						 "There is no dihedral %1 in this flexibility")
				     .arg(dihedral.toString()), CODELOC );
  this->angle_deltas[dihedral] = delta;
}

/** Return the delta value of bond in this flexibility */
Length Flexibility::delta(const BondID &bond) const
{
  if ( not this->bonds.contains(bond) )
    throw SireMol::missing_bond( QObject::tr(
					     "There is no bond %1 in this flexibility")
                                 .arg(bond.toString()), CODELOC );
  return this->bond_deltas[bond];
}

/** Return the delta value of angle in this flexibility */
Angle Flexibility::delta(const AngleID &angle) const
{
  if ( not this->angles.contains(angle) )
    throw SireMol::missing_angle( QObject::tr(
					     "There is no angle %1 in this flexibility")
                                 .arg(angle.toString()), CODELOC );
  return this->angle_deltas[angle];
}

/** Return the delta value of angle in this flexibility */
Angle Flexibility::delta(const DihedralID &dihedral) const
{
  if ( not this->dihedrals.contains(dihedral) )
    throw SireMol::missing_angle( QObject::tr(
					     "There is no dihedral %1 in this flexibility")
                                 .arg(dihedral.toString()), CODELOC );
  return this->angle_deltas[dihedral];
}

const char* Flexibility::typeName()
{
  return QMetaType::typeName( qMetaTypeId<Flexibility>() );
}


