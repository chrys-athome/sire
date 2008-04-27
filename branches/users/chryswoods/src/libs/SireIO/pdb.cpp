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

#include <QTextStream>
#include <QByteArray>
#include <QString>
#include <QStringList>

#include "pdb.h"

#include "SireMol/mover.hpp"

#include "SireStream/datastream.h"

using namespace SireMol;
using namespace SireBase;
using namespace SireIO;
using namespace SireStream;

static const RegisterMetaType<PDB> r_pdb;

/** Serialise to a binary datastream */
QDataStream SIREIO_EXPORT &operator<<(QDataStream &ds, const PDB &pdb)
{
    writeHeader(ds, r_pdb, 1);
    
    ds << static_cast<const IOBase&>(pdb);
    
    return ds;
}

/** Extract from a binary datastream */
QDataStream SIREIO_EXPORT &operator>>(QDataStream &ds, PDB &pdb)
{
    VersionID v = readHeader(ds, r_pdb);
    
    if (v == 1)
    {
        ds >> static_cast<IOBase&>(pdb);
    }
    else
        throw version_error(v, "1", r_pdb, CODELOC);
        
    return ds;
}

/** Constructor */
PDB::PDB() : ConcreteProperty<PDB,IOBase>()
{}

/** Copy constructor */
PDB::PDB(const PDB &other) : ConcreteProperty<PDB,IOBase>(other)
{}

/** Destructor */
PDB::~PDB()
{}

/** Copy assignment operator */
PDB& PDB::operator=(const PDB &other)
{
    IOBase::operator=(other);
    return *this;
}

/** Comparison operator */
bool PDB::operator==(const PDB&) const
{
    return true;
}

/** Comparison operator */
bool PDB::operator!=(const PDB&) const
{
    return false;
}

/** Read a group of molecules from the data */
MoleculeGroup PDB::readMols(const QByteArray &data,
                            const PropertyMap &map) const
{

    return MoleculeGroup();
}

/** Write a group of molecules to a bytearray */
QByteArray PDB::writeMols(const MolGroup &molgroup,
                          const PropertyMap &map) const
{
    QByteArray data;
    QTextStream ts(&data, QIODevice::WriteOnly | QIODevice::Text);

    return data;
}

/** Write a group of molecules to a bytearray */
QByteArray PDB::writeMols(const Molecules &molecules,
                          const PropertyMap &map) const
{
    QByteArray data;
    QTextStream ts(&data, QIODevice::WriteOnly | QIODevice::Text);

    return data;
}
