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

#ifndef SIREIO_PROTOMS_H
#define SIREIO_PROTOMS_H

#include <QString>
#include <QStringList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireIO
{
class ProtoMS;
}

QDataStream& operator<<(QDataStream&, const SireIO::ProtoMS&);
QDataStream& operator>>(QDataStream&, SireIO::ProtoMS&);

namespace SireMol
{
class Molecule;
class Molecules;
class MolEditor;
}

namespace SireBase
{
class TempDir;
}

namespace SireIO
{

using SireMol::Molecule;
using SireMol::Molecules;
using SireMol::MolEditor;

/** This class is used to read in ProtoMS parameter files and
    parameterise passed molecules.
 
    @author Christopher Woods
*/
class SIREIO_EXPORT ProtoMS
{

friend QDataStream& ::operator<<(QDataStream&, const SireIO::ProtoMS&);
friend QDataStream& ::operator>>(QDataStream&, SireIO::ProtoMS&);

public:
    enum { PROTEIN = 1,     // a ProtoMS protein molecule
           SOLUTE  = 2,     // a ProtoMS solute molecule
           SOLVENT = 3  };  // a ProtoMS solvent molecule

    ProtoMS();
    ProtoMS(const QString &protoms);
    
    ~ProtoMS();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ProtoMS>() );
    }

    const char* what() const
    {
        return ProtoMS::typeName();
    }
    
    ProtoMS* clone() const
    {
        return new ProtoMS(*this);
    }

    void setExecutable(const QString &protoms);

    void addParameterFile(const QString &paramfile);
    
    QStringList parameterFiles() const;
    
    Molecule parameterise(const Molecule &molecule, int type);

    Molecules parameterise(const Molecules &molecules, int type);

private:
    QString writeShellFile(const SireBase::TempDir &tempdir,
                           const QString &cmdfile) const;
    QString writeCommandFile(const SireBase::TempDir &tempdir, 
                             const Molecule &molecule, int type) const;
    
    void processZMatrixLine(const QStringList &words, 
                            MolEditor &editmol, int type) const;
    void processAtomLine(const QStringList &words,
                         MolEditor &editmol, int type) const;
    
    Molecule runProtoMS(const Molecule &molecule, int type) const;

    /** The list of parameter files that will be used to 
        parameterise the molecules */
    QStringList paramfiles;
    
    /** The full path to the ProtoMS executable that will
        be used to perform the parameterisation */
    QString protoms_exe;
};

}

Q_DECLARE_METATYPE( SireIO::ProtoMS )

SIRE_EXPOSE_CLASS( SireIO::ProtoMS )

SIRE_END_HEADER

#endif
