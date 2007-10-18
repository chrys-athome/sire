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

#ifndef SIREDB_SMARTS_H
#define SIREDB_SMARTS_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/** This class will eventually provide Smarts string functionality
    for atom searching (of Smiles strings). For now, this is just 
    a placeholder.
    
    It is very much my intention to provide a Smiles and Smarts
    parser/searcher so that it will be possible to describe
    an atom using a Smarts string, and to use this string to 
    automatically assign basic parameters, e.g. atom types,
    LJ parameters and semi-empirical QM parameters.
    
    Building a Smiles/Smarts system into the code may also
    provide other interesting opportunities for use, e.g.
    searching in Spier. It will however require a significant
    investment of time, and also perhaps some changes to
    the Molecule and EditMol classes (as neither of these
    have concepts of bond type, nor of the formal 
    charge of their constituent atoms).
    
    @author Christopher Woods
*/
class SIREDB_EXPORT Smarts
{
public:
    Smarts();
    
    Smarts(const Smarts &other);
    
    ~Smarts();

    bool operator==(const Smarts &other) const;

    QString toString() const;

    bool isEmpty() const;

};

}

SIRE_END_HEADER

#endif
