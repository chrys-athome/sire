/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SIREMM_RESTRAINTPARAMETERS_H
#define SIREMM_RESTRAINTPARAMETERS_H

#include <QSet>

#include "SireCAS/symbol.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class RestraintParameters;
class RestraintParameters3D;
}

QDataStream& operator<<(QDataStream&, const SireMM::RestraintParameters&);
QDataStream& operator>>(QDataStream&, SireMM::RestraintParameters&);

QDataStream& operator<<(QDataStream&, const SireMM::RestraintParameters3D&);
QDataStream& operator>>(QDataStream&, SireMM::RestraintParameters3D&);

namespace SireMM
{

using SireCAS::Symbol;

/** This class holds all of the symbols that are permitted
    to exist in a restraint expression */
class SIREMM_EXPORT RestraintSymbols
{
public:
    RestraintSymbols();
    ~RestraintSymbols();
    
    const Symbol& r() const;
    
    const Symbol& theta() const;
    
    const Symbol& phi() const;
    
    operator QSet<Symbol>() const
    {
        return symbols;
    }

protected:    
    /** All of the available restraint symbols */
    QSet<Symbol> symbols;
    
private:
    /** The symbol for a distance in a distance-based restraint */
    Symbol r_;
    
    /** The symbol for an angle in an angle-based restraint */
    Symbol theta_;
    
    /** The symbol for a dihedral in a dihedral-based restraint */
    Symbol phi_;
};

/** This class hold all of the restraint parameters for a molecule

    @author Christopher Woods
*/
class RestraintParameters

   Restraints mol_restraints;


class RestraintParameters3D

    Restraints3D mol_restraints;

}

Q_DECLARE_METATYPE( SireMM::RestraintParameters )
Q_DECLARE_METATYPE( SireMM::RestraintParameters3D )

SIRE_EXPOSE_CLASS( SireMM::RestraintParameters )
SIRE_EXPOSE_CLASS( SireMM::RestraintParameters3D )

SIRE_END_HEADER

#endif
