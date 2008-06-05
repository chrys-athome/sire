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

#ifndef SIREMM_UREYBRADLEYDB_H
#define SIREMM_UREYBRADLEYDB_H

#include "SireDB/term13db.h"

#include "SireCAS/symbol.h"

SIRE_BEGIN_HEADER

namespace SireMM
{

using SireCAS::Expression;
using SireCAS::Symbol;

using SireDB::AssertMatch;
using SireDB::RelateID;
using SireDB::ParamID;
using SireDB::RelateIDMap;

/**
The UreyBradleyDB is the database that holds Urey-Bradley functions. These are functions that act between the 1-3 atoms of an angle. The database can hold a wide variety of functions as it holds generic functions and their parameters, rather than parameters for standard harmonics.

Note that the UreyBradleyDB is not strictly necessary as it is possible to add 1-3 terms directly to the angle functions loaded into AngleDB. UreyBradleyDB is useful as it allows the 1-3 terms to be loaded and assigned separately from the angle terms. This is also useful if used with a forcefield that calculates the Urey-Bradley energy separately from the Angle energy (if not, then the UB terms are combined with the angle terms).

@author Christopher Woods
*/
class SIREMM_EXPORT UreyBradleyDB : public SireDB::Term13DB
{
public:
    /** The UreyBradleyDB stores Expression objects */
    typedef Expression parameter_type;
    
    UreyBradleyDB();
    UreyBradleyDB(const UreyBradleyDB &other);

    ~UreyBradleyDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireMM::UreyBradleyDB";
    }

    const char* what() const
    {
        return UreyBradleyDB::typeName();
    }

    void addUreyBradley(const QString &userid, const Expression &ubfunc);
    Expression getUreyBradley(const QString &userid, bool *foundub = 0);
    
    void relateUreyBradley(const AssertMatch<3> &matchtriple, const QString &userid);
    void relateUreyBradley(const AssertMatch<3> &matchtriple, const Expression &ubfunc);
    
    void relateUreyBradley(RelateID relateid, const QString &userid);
    void relateUreyBradley(RelateID relateid, const Expression &ubfunc);
    
    Expression getUreyBradley(RelateID relateid, bool *foundub = 0);
    Expression getUreyBradley(const RelateIDMap &relateids, bool *foundub = 0);

    const Symbol& r() const;

protected:
    void prepareToDump();
    void postLoad();
  
private:
    /** The symbol used to represent the distance between 
        the 1-3 atoms of the angle */
    Symbol _r;
};

/** Return the symbol used to represent the distance between the 
    1-3 atoms of the angle (the Urey-Bradley distance) */
inline const Symbol& UreyBradleyDB::r() const
{
    return _r;
}

}

Q_DECLARE_METATYPE(SireMM::UreyBradleyDB)

SIRE_END_HEADER

#endif
