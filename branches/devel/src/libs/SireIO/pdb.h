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

/**
This is a IOBase object that has been specialised to read and write PDB format files.

@author Christopher Woods
*/
class SIREIO_EXPORT PDB : public IOBase
{
public:
    PDB();
    ~PDB();

protected:
    /** Read and return some molecules from the data */
    QList<Molecule> readMols(const QByteArray &data,
                             const CuttingFunction &cutfunc) const;

    /** Write the list of molecules to a bytearray and return it */
    QByteArray writeMols(const QList<Molecule> &molecules) const;

    QByteArray writeMols(const QList<EditMol> &molecules) const;

};

}

SIRE_END_HEADER

#endif
