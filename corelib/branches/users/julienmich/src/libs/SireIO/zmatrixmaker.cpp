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

#include "zmatrixmaker.h"

#include "SireMol/moleculegroup.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

#include "SireError/errors.h"
#include "SireIO/errors.h"
#include "SireMol/errors.h"

using namespace SireIO;
using namespace SireMol;
using namespace SireStream;


//
// Implementation of ZmatrixLine
//

ZmatrixLine::ZmatrixLine(const QString &atom , const QString &bond , const QString &angle , 
			 const QString &dihedral , const double &bondDelta , const double &angleDelta , 
			 const double &dihedralDelta ) 
  : atom (atom), bond(bond), angle(angle), dihedral(dihedral), 
    bondDelta(bondDelta), angleDelta(angleDelta), dihedralDelta(dihedralDelta)
{}

bool ZmatrixLine::has(const QString &atom)
{
  if ( this->atom == atom )
    return true;
  else
    return false;
}

const QString ZmatrixLine::getAtom()
{
  return this->atom;
}

const QString ZmatrixLine::getBond()
{
  return this->bond;
}

const QString ZmatrixLine::getAngle()
{
  return this->angle;
}

const QString ZmatrixLine::getDihedral()
{
  return this->dihedral;
}

const double ZmatrixLine::getBondDelta()
{
  return this->bondDelta;
}

const double ZmatrixLine::getAngleDelta()
{
  return this->angleDelta;
}

const double ZmatrixLine::getDihedralDelta()
{
  return this->dihedralDelta;
}

void ZmatrixLine::setBondDelta( double bondDelta )
{
  this->bondDelta = bondDelta;
}

void ZmatrixLine::setAngleDelta( double angleDelta )
{
  this->angleDelta = angleDelta;
}

void ZmatrixLine::setDihedralDelta( double dihedralDelta )
{
  this->dihedralDelta = dihedralDelta;
}

QString ZmatrixLine::toString()
{
  return QObject::tr("ZmatrixLine: %1 %2 %3 %4 %5 %6 %7")
    .arg(this->getAtom())
    .arg(this->getBond())
    .arg(this->getAngle())
    .arg(this->getDihedral())
    .arg(this->getBondDelta())
    .arg(this->getAngleDelta())
    .arg(this->getDihedralDelta());
}

//
// Implementation of ZmatrixTemplate
//

ZmatrixTemplate::ZmatrixTemplate(const QString &name) 
  : name(name)
{}

QList<ZmatrixLine> ZmatrixTemplate::getZmatrix()
{
  QList<ZmatrixLine> zmatrix;
  foreach (ZmatrixLine zmatline, this->zmatrix)
    zmatrix.append(zmatline);
  return zmatrix;
}

ZmatrixLine ZmatrixTemplate::getZmatrixLine(const QString &atom)
{
  if ( not this->zmatrix.contains(atom) )
    throw SireError::invalid_key(atom, CODELOC);
  else
    return this->zmatrix.value(atom);
}

const QString ZmatrixTemplate::getName()
{
  return this->name;
}

void ZmatrixTemplate::addZmatrixLine( const QString &atom, const ZmatrixLine &zmatline)
{
  if ( this->zmatrix.contains(atom) )
    throw SireError::invalid_key(atom, CODELOC);
  else
    this->zmatrix[atom] = zmatline;
}

void ZmatrixTemplate::setBondDelta( const QString &atom, const QString &bond, double bondDelta )
{
  if ( not this->zmatrix.contains(atom) )
    throw SireError::invalid_key(atom, CODELOC);
  //Also check that the bond atom is correct
  else if ( bond != zmatrix[atom].getBond() )
    throw SireError::invalid_key(bond, CODELOC);
  else
    this->zmatrix[atom].setBondDelta(bondDelta);
}

void ZmatrixTemplate::setAngleDelta( const QString &atom, const QString &bond, const QString &angle, double angleDelta )
{
  if ( not this->zmatrix.contains(atom) )
    throw SireError::invalid_key(atom, CODELOC);
  else if ( bond != zmatrix[atom].getBond() )
    throw SireError::invalid_key(bond, CODELOC);
  else if ( angle != zmatrix[atom].getAngle() )
    throw SireError::invalid_key(angle, CODELOC);
  else
    this->zmatrix[atom].setAngleDelta(angleDelta);
}

void ZmatrixTemplate::setDihedralDelta( const QString &atom, const QString &bond, 
					const QString &angle, const QString &dihedral, double dihedralDelta )
{
  if ( not this->zmatrix.contains(atom) )
    throw SireError::invalid_key(atom, CODELOC);
  else if ( bond != zmatrix[atom].getBond() )
    throw SireError::invalid_key(bond, CODELOC);
  else if ( angle != zmatrix[atom].getAngle() )
    throw SireError::invalid_key(angle, CODELOC);
  else if ( dihedral != zmatrix[atom].getDihedral() )
    throw SireError::invalid_key(dihedral, CODELOC);
  else
    this->zmatrix[atom].setDihedralDelta(dihedralDelta);
}

//
// Implementation of ZmatrixChain
//

ZmatrixChain::ZmatrixChain(const QString &name) 
  : ZmatrixTemplate(name)
{}

QStringList ZmatrixChain::getBBatoms()
{
  return this->bbatoms;
}

void ZmatrixChain::addBBatom( const QString &bbAtom)
{
  this->bbatoms.append(bbAtom);
}

//
// Implementation of ZmatrixResidue
//

ZmatrixResidue::ZmatrixResidue(const QString &name) 
  : ZmatrixTemplate(name)
{}

void ZmatrixResidue::setRotation(const double rot)
{
  this->rotate = rot;
}

void ZmatrixResidue::setTranslation(const double trans)
{
  this->translate = trans;
}

const double ZmatrixResidue::getRotation()
{
  return this->rotate;
}

const double ZmatrixResidue::getTranslation()
{
  return this->translate;
}

void ZmatrixResidue::addChain( const QString &name, const ZmatrixChain &chain)
{
  if ( this->backbone.contains(name ) )
    throw SireError::invalid_key(name, CODELOC);
  else
    this->backbone[name] = chain;
}

const ZmatrixChain ZmatrixResidue::getChain(const QString &name)
{
  if ( not this->backbone.contains(name) )
    throw SireError::invalid_key(name, CODELOC);
  else
    return this->backbone[name];
}

QList<ZmatrixChain> ZmatrixResidue::getChains()
{
  QList<ZmatrixChain> chains;
  foreach (ZmatrixChain chain, this->backbone)
    chains.append(chain);
  return chains;
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
// Implementation of ZmatrixMaker
//
static const RegisterMetaType<ZmatrixMaker> r_zmatrixmaker;

/** Serialise to a binary datastream */
QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds, const ZmatrixMaker &zmatrixmaker)
{
    writeHeader(ds, r_zmatrixmaker, 1);
    
    SharedDataStream sds(ds);
    
    //sds << protoms.paramfiles << protoms.protoms_exe;
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, ZmatrixMaker &zmatrixmaker)
{
    VersionID v = readHeader(ds, r_zmatrixmaker);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        //sds >> protoms.paramfiles >> protoms.protoms_exe;
    }
    else
        throw version_error( v, "1", r_zmatrixmaker, CODELOC );
        
    return ds;
}

/** Default constructor */
ZmatrixMaker::ZmatrixMaker()
{}

//** Destructor */
ZmatrixMaker::~ZmatrixMaker()
{}

const char* ZmatrixMaker::typeName()
{
    return QMetaType::typeName( qMetaTypeId<ZmatrixMaker>() );
}

/** Load the zmatrix templates specified in &templatefile*/
void ZmatrixMaker::loadTemplates( const QString &templatefile)
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

  /** Holds the dictionnary of chain and residues read from the templates*/
  QHash< QString, ZmatrixChain > chains;
  QHash< QString, ZmatrixResidue > residues;

  enum { CHAIN=0,
	 RESIDUE=1
  };
  QString current = " ";// the chain/residue currently read
  int currentType=CHAIN;

  /** Now read rest of the file */
  while ( not line.isNull() )
    {
      line = ts.readLine();
      QStringList words = line.split(" ", QString::SkipEmptyParts);
      qDebug() << line;
      //qDebug() << " current chain/residue " << current << " type " << currentType;
      
      if ( line.startsWith("chain") )
	{
	  ZmatrixChain chain = ZmatrixChain ( words[1] );
	  current = chain.getName();
	  chains[current] = chain;
	  currentType = CHAIN;
	}
      else if ( line.startsWith("residue") )
	{
	  ZmatrixResidue residue = ZmatrixResidue ( words[1] );
	  current = residue.getName();
	  residues[current] = residue;
	  currentType = RESIDUE;
	}
      else if ( line.startsWith("bbatom") )
	{
	  if ( currentType != CHAIN )
	    throw SireError::io_error( QObject::tr(
       	         "There is a problem with the input file %1, "
       		 " a bbatom line should only be defined for a chain template").arg(templatefile), CODELOC );
	  chains[current].addBBatom( words[1] );
	}
      else if ( line.startsWith("zmatrix") )
       	{
	  ZmatrixLine zmatline = ZmatrixLine( words[1], words[2], words[3], words[4] );
       	  if ( currentType == CHAIN )
	    chains[current].addZmatrixLine( words[1], zmatline);
       	  else if ( currentType == RESIDUE )
	    residues[current].addZmatrixLine( words[1], zmatline);
       	}
      else if ( line.startsWith("bond") )
       	{
      	  if ( currentType == CHAIN )
	    chains[current].setBondDelta( words[1], words[2], words[4].toDouble() );
       	  else if ( currentType == RESIDUE )
	    residues[current].setBondDelta( words[1], words[2], words[4].toDouble() );
       	}
      else if ( line.startsWith("angle") )
       	{
       	  if ( currentType == CHAIN )
	    chains[current].setAngleDelta( words[1], words[2], words[3], words[5].toDouble() );
      	  else if ( currentType == RESIDUE )
	     residues[current].setAngleDelta( words[1], words[2], words[3], words[5].toDouble() );
       	}
       else if ( line.startsWith("dihedral") )
	 {
	   if ( currentType == CHAIN )
	     chains[current].setDihedralDelta( words[1], words[2], words[3], words[4], words[6].toDouble() );
	   else if ( currentType == RESIDUE )
	     residues[current].setDihedralDelta( words[1], words[2], words[3], words[4], words[6].toDouble() );
       	}
       else if ( line.startsWith("rigidbody") )
       	{
       	  if ( currentType != RESIDUE )
	    throw SireError::io_error( QObject::tr(
                   " There is a problem with the input file %1, "
		   " a rigidbody line should only be defined for a residue template").arg(templatefile), CODELOC );
	  residues[current].setRotation( words[2].toDouble() );
	  residues[current].setTranslation( words[4].toDouble() );
       	}
       else if ( line.startsWith("backbone") )
       	{
       	  if ( currentType != RESIDUE )
	    throw SireError::io_error( QObject::tr(
                   " There is a problem with the input file %1, "
		   " a backbone line should only be defined for a residue template").arg(templatefile), CODELOC );
	  /** The appropriate chain can be defined for a variety of environments */
	  residues[current].addChain( words[1], chains[words[2]] );
	  residues[current].addChain( words[3], chains[words[4]] );
	  residues[current].addChain( words[5], chains[words[6]] );	  
       	}
    }

  /** Store the new residues */
  foreach (ZmatrixChain chain, chains)
    {
      qDebug() << " CHAIN " << chain.getName();
      QList<ZmatrixLine> zmatrix = chain.getZmatrix();
      foreach ( ZmatrixLine zmatline, zmatrix)
	qDebug() << zmatline.toString();
    }
  foreach (ZmatrixResidue residue, residues)
    {
      qDebug() << " RESIDUE " << residue.getName();
      qDebug() << " Rotate " << residue.getRotation();
      qDebug() << " Translate " << residue.getTranslation();
      qDebug() << " CHAINS " ;
      QList<ZmatrixChain> chains = residue.getChains();
      foreach (ZmatrixChain chain, chains)
	qDebug() << chain.getName();
      QList<ZmatrixLine> zmatrix = residue.getZmatrix();
      foreach ( ZmatrixLine zmatline, zmatrix)
	qDebug() << zmatline.toString();
      QString resname = residue.getName();
      if ( not this->residues.contains(resname))
	this->residues[resname] = residue;
    }

}


Molecule ZmatrixMaker::applyTemplates( Molecule &molecule)
{

  PropertyName zmatrix_property = PropertyName("z-matrix");

  // Does it already have a zmatrix_property?
  
  /** For each residue in that molecule*/
  // look for a matching template
  // for each atom in that residue
  // find zmatrix line entry in the template and use it to update the molecule zmatrix
  // add zmatrix_property, zmatrix to molecule

  return molgroup;
}
