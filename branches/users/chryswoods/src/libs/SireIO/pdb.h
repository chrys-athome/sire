/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREIO_PDB_H
#define SIREIO_PDB_H

#include "iobase.h"

SIRE_BEGIN_HEADER

namespace SireIO
{
class PDB;
}

QDataStream& operator<<(QDataStream&, const SireIO::PDB&);
QDataStream& operator>>(QDataStream&, SireIO::PDB&);

namespace SireIO
{

/** This is a IOBase object that has been specialised to read and 
    write PDB format files.

    @author Christopher Woods
*/
class SIREIO_EXPORT PDB : public SireBase::ConcreteProperty<PDB,IOBase>
{
public:
    PDB();
    PDB(const PDB &other);
    
    ~PDB();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<PDB>() );
    }

    const char* what() const
    {
        return PDB::typeName();
    }

    PDB& operator=(const PDB &other);
    
    bool operator==(const PDB &other) const;
    bool operator!=(const PDB &other) const;

protected:
    MoleculeGroup readMols(const QByteArray &data,
                           const PropertyMap &map) const;

    QByteArray writeMols(const MolGroup &molgroup,
                         const PropertyMap &map) const;

    QByteArray writeMols(const Molecules &molecules,
                         const PropertyMap &map) const;
};

}

Q_DECLARE_METATYPE( SireIO::PDB );

SIRE_END_HEADER

#endif
