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

#include "qmmmelecembedpotential.h"

#include "SireStream/datastream.h"

using namespace Squire;
using namespace SireMM;
using namespace SireStream;

template class QMMMPotential<QMPotential,InterCoulombPotential>;

static const RegisterMetaType<QMMMElecEmbedPotential> r_qmmm( MAGIC_ONLY,
                                                "Squire::QMMMElecEmbedPotential" );
                                                
/** Serialise to a binary datastream */
QDataStream SQUIRE_EXPORT &operator<<(QDataStream &ds,
                                      const QMMMElecEmbedPotential &qmmm)
{
    writeHeader(ds, r_qmmm, 1);
    ds << static_cast<const QMMMPotential<QMPotential,InterCoulombPotential>&>(qmmm);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SQUIRE_EXPORT &operator>>(QDataStream &ds,
                                      QMMMElecEmbedPotential &qmmm)
{
    VersionID v = readHeader(ds, r_qmmm);
    
    if (v == 1)
    {
        ds >> static_cast<QMMMPotential<QMPotential,InterCoulombPotential>&>(qmmm);
    }
    else
        throw version_error(v, "1", r_qmmm, CODELOC);
    
    return ds;
}

/** Constructor */
QMMMElecEmbedPotential::QMMMElecEmbedPotential()
                       : QMMMPotential<QMPotential,InterCoulombPotential>()
{}

/** Copy constructor */
QMMMElecEmbedPotential::QMMMElecEmbedPotential(const QMMMElecEmbedPotential &other)
                       : QMMMPotential<QMPotential,InterCoulombPotential>(other)
{}

/** Destructor */
QMMMElecEmbedPotential::~QMMMElecEmbedPotential()
{}

/** Copy assignment operator */
QMMMElecEmbedPotential& 
QMMMElecEmbedPotential::operator=(const QMMMElecEmbedPotential &other)
{
    QMMMPotential<QMPotential,InterCoulombPotential>::operator=(other);
    
    return *this;
}

bool QMMMElecEmbedPotential::setProperty(const QString &name, const Property &property)
{
    return false;
}

const Property& QMMMElecEmbedPotential::property(const QString &name) const
{
    return QMPotential::property(name);
}

bool QMMMElecEmbedPotential::containsProperty(const QString &name) const
{
    return false;
}

const Properties& QMMMElecEmbedPotential::properties() const
{
    return QMPotential::properties();
}

QString QMMMElecEmbedPotential::energyCommandFile(
                          const QMMMElecEmbedPotential::QMMolecules &qmmols,
                          const QMMMElecEmbedPotential::MMMolecules &mmmols) const
{
    return QString();
}
                          
QString QMMMElecEmbedPotential::forceCommandFile(
                          const QMMMElecEmbedPotential::QMMolecules &qmmols,
                          const QMMMElecEmbedPotential::MMMolecules &mmmols) const
{
    return QString();
}
