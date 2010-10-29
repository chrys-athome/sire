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
#include <QFile>
#include <QTextStream>

#include "flexibilitymaker.h"

#include "SireMol/atom.h"
#include "SireMol/atomeditor.h"
#include "SireMol/molecule.h"
#include "SireMol/moleditor.h"
#include "SireMol/selector.hpp"
#include "SireMol/bondid.h"
#include "SireMol/angleid.h"
#include "SireMol/dihedralid.h"

#include "SireUnits/convert.h"
#include "SireUnits/units.h"
#include "SireUnits/dimensions.h"

#include "SireMove/movermove.h"

#include "SireError/errors.h"
#include "SireIO/errors.h"
#include "SireMol/errors.h"

using namespace SireIO;
using namespace SireMol;
using namespace SireMove;
using namespace SireStream;
using namespace SireUnits;

//
// Implementation of FlexibilityTemplate
//
FlexibilityTemplate::FlexibilityTemplate(const QString &name)
  : name(name)
{}

const QString FlexibilityTemplate::getName()
{
  return this->name;
}

void FlexibilityTemplate::setRotation(double rotation)
{
  this->rotation = rotation;
}

void FlexibilityTemplate::setTranslation(double translation)
{
  this->translation = translation;
}

const double FlexibilityTemplate::getRotation()
{
  return this->rotation;
}

const double FlexibilityTemplate::getTranslation()
{
  return this->translation;
}

void FlexibilityTemplate::addBond(QStringList words)
{
  this->bonds.append(words);
}

void FlexibilityTemplate::addAngle(QStringList words)
{
  this->angles.append(words);
}

void FlexibilityTemplate::addDihedral(QStringList words)
{
  this->dihedrals.append(words);
}

const QList< QStringList > FlexibilityTemplate::getBonds()
{
  return this->bonds;
}

const QList< QStringList > FlexibilityTemplate::getAngles()
{
  return this->angles;
}

const QList< QStringList > FlexibilityTemplate::getDihedrals()
{
  return this->dihedrals;
}

//
// Helper functions to parse a templates input file
//

static int processVersionLine( QString& line)
{
  QStringList words = line.split(" ", QString::SkipEmptyParts);
  bool ok;
  int version = words[1].toInt(&ok);
  if (not ok)
    throw SireError::program_bug( QObject::tr(
	"Unexpected error while trying to read the version line of the zmatrix template file"), CODELOC);
  return version;
}

//
// Implementation of FlexibilityMaker
//
static const RegisterMetaType<FlexibilityMaker> r_flexibilitymaker;

/** Serialise to a binary datastream */
QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds, const FlexibilityMaker &flexibilitymaker)
{
    writeHeader(ds, r_flexibilitymaker, 1);
    
    SharedDataStream sds(ds);
    
    //    sds << flexibilitymaker.templates;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, FlexibilityMaker &flexibilitymaker)
{
    VersionID v = readHeader(ds, r_flexibilitymaker);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
	//      sds >> flexibilitymaker.templates;
    }
    else
        throw version_error( v, "1", r_flexibilitymaker, CODELOC );
        
    return ds;
}

/** Default constructor */
FlexibilityMaker::FlexibilityMaker()
{}

//** Destructor */
FlexibilityMaker::~FlexibilityMaker()
{}

const char* FlexibilityMaker::typeName()
{
    return QMetaType::typeName( qMetaTypeId<FlexibilityMaker>() );
}

void FlexibilityMaker::loadTemplates( const QString &templatefile)
{
  QFile template_f(templatefile);
  
  if ( not (template_f.exists() and template_f.open(QIODevice::ReadOnly) ) )
    {
      throw SireError::file_error(template_f, CODELOC);
    }
  
  QTextStream ts(&template_f);
  
  QString line = ts.readLine();
  /** The first line contains the version*/
  int version = ::processVersionLine(line);
  if (version != 1)
    throw SireError::process_error( QObject::tr(
    "Invalid version of the template, got '%1' but only support '1' ").arg(version), CODELOC);
  
  QString current = " ";// the template currently being read
  QHash < QString, FlexibilityTemplate > templates; 

  /** Now read rest of the file */
  while ( not line.isNull() )
    {
      line = ts.readLine();
      QStringList words = line.split(" ", QString::SkipEmptyParts);
      qDebug() << line;

      if ( line.startsWith("molecule") )
	{
	  // create a new flexibilitytemplate
	  FlexibilityTemplate flextemplate = FlexibilityTemplate( words[1] );
	  current = flextemplate.getName();
	  templates[current] = flextemplate;
	}
      else if ( line.startsWith("rigidbody") )
	{
	  templates[current].setRotation( words[2].toDouble() );
	  templates[current].setTranslation( words[4].toDouble() );
	}
      else if ( line.startsWith("bond") )
	{
	  QStringList bonddata;
	  bonddata << words[1] << words[2] << words[4] ;
	  templates[current].addBond( bonddata );
	}
      else if ( line.startsWith("angle") )
	{
	  QStringList angledata;
	  angledata << words[1] << words[2] << words[3] << words[5] ;
	  templates[current].addAngle( angledata );
	}
      else if ( line.startsWith("dihedral") )
	{
	  QStringList dihedraldata;
	  dihedraldata << words[1] << words[2] << words[3] << words[4] << words[6] ;
	  templates[current].addDihedral( dihedraldata );
	}
    }

  foreach (FlexibilityTemplate templ, templates)
    {
      QString templname = templ.getName();
      if ( not this->templates.contains(templname) )
	this->templates[templname] = templ;
      qDebug() << " TEMPLATE " << templ.getName() ;
      qDebug() << " ROT " << templ.getRotation() << " TRANS " << templ.getTranslation();
      QList< QStringList > bonds = templ.getBonds();
      foreach ( QStringList bond, bonds)
	  qDebug() << bond.join(" ");
      QList< QStringList > angles = templ.getAngles();
      foreach ( QStringList angle, angles)
	  qDebug() << angle.join(" ");
      QList< QStringList > dihedrals = templ.getDihedrals();
      foreach ( QStringList dihedral, dihedrals)
	  qDebug() << dihedral.join(" ");
    }
}

Molecule FlexibilityMaker::applyTemplates( Molecule &molecule)
{
  
  PropertyName flexibility_property = PropertyName("flexibility");
  
  if ( molecule.hasProperty(flexibility_property) )
    return molecule;
  
  MolEditor editmol = molecule.edit();
  
  //Flexibility &flexibility = Flexibility( editmol );
  
  MolName moleculename = editmol.name();

  if ( not this->templates.contains(moleculename) )
    throw SireError::invalid_key(moleculename, CODELOC);

  FlexibilityTemplate templ = this->templates[moleculename];

  double rotate = templ.getRotation();
  double translate = templ.getTranslation();

  // Add to flexibility

  QList< QStringList > templbonds = templ.getBonds();
  foreach ( QStringList templbond, templbonds)
    {
      Atom atom0 = editmol.select( AtomName(templbond[0]) );
      Atom atom1 = editmol.select( AtomName(templbond[1]) );
      BondID bondid = BondID(atom0.index() , atom1.index() );
      //DofID dofidbond = DofID(bondid);
      ::Dimension::Length bonddelta = templbond[2].toDouble() * angstrom ; 
      // Add to flexibility bonds and bondDeltas
    }

  QList< QStringList > templangles = templ.getAngles();
  foreach ( QStringList templangle, templangles)
    {
      Atom atom0 = editmol.select( AtomName(templangle[0]) );
      Atom atom1 = editmol.select( AtomName(templangle[1]) );
      Atom atom2 = editmol.select( AtomName(templangle[2]) );
      AngleID angleid = AngleID(atom0.index() , atom1.index(), atom2.index() );
      //DofID dofidangle = DofID(angleid);
      ::Dimension::Angle angledelta = templangle[3].toDouble() * degrees; 
      // Add to flexibility angles and angleDeltas
    }

  QList< QStringList > templdihedrals = templ.getDihedrals();
  foreach ( QStringList templdihedral, templdihedrals)
    {
      Atom atom0 = editmol.select( AtomName(templdihedral[0]) );
      Atom atom1 = editmol.select( AtomName(templdihedral[1]) );
      Atom atom2 = editmol.select( AtomName(templdihedral[2]) );
      Atom atom3 = editmol.select( AtomName(templdihedral[3]) );
      DihedralID dihedralid = DihedralID(atom0.index() , atom1.index(), atom2.index(), atom3.index() );
      //DofID dofiddihedral = DofID(dihedralid);
      ::Dimension::Angle angledelta = templdihedral[4].toDouble() * degrees; 
      // Add to flexibility dihedrals and angleDeltas
    }

  //editmol.setProperty( flexibility_property, flexibility);

  return editmol.commit();
}
