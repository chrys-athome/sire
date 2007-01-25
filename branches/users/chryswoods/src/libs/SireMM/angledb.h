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

#ifndef SIREMM_ANGLEDB_H
#define SIREMM_ANGLEDB_H

#include "SireDB/term13db.h"
#include "SireCAS/symbol.h"

#include "molangleinfo.h"
#include "internaldb.hpp"
#include "angletable.h"

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
The AngleDB is the database component that holds angle parameters. The database can hold a vast range of Angle parameters, as it holds generic functions, and their parameters, rather than specific parameters for standard harmonic angles.
 
 
@author Christopher Woods
*/
class SIREMM_EXPORT AngleDB : public SireDB::Term13DB, public InternalDB<MolAngleInfo>
{
public:
    /** The AngleDB stores Expression objects */
    typedef Expression parameter_type;
    
    /** This uses an AngleTable to store angle parameters */
    typedef AngleTable table_type;

    AngleDB();
    AngleDB(const AngleDB &other);
    
    ~AngleDB();

    /** Return the full name of this database component */
    static const char* typeName()
    {
        return "SireMM::AngleDB";
    }

    const char* what() const
    {
        return AngleDB::typeName();
    }

    void addAngle(const QString &userid, const Expression &anglefunc);
    Expression getAngle(const QString &userid, bool *foundangle = 0);
    
    void relateAngle(const AssertMatch<3> &matchangle, const QString &userid);
    void relateAngle(const AssertMatch<3> &matchangle, const Expression &anglefunc);
    
    void relateAngle(RelateID relateid, const QString &userid);
    void relateAngle(RelateID relateid, const Expression &anglefunc);
    
    Expression getAngle(RelateID relateid, bool *foundangle = 0);
    Expression getAngle(const RelateIDMap &relateids, bool *foundangle = 0);

    const Symbol& r() const;
    const Symbol& theta() const;
    
    bool assignParameter(const Angle &angle, 
                         const RelateIDMap &relateids,
                         ParameterTable &param_table);

protected:
    void prepareToDump();
    void postLoad();
  
private:
    /** The symbol used to represent the angle */
    Symbol _theta;
    /** The symbol used to represent the distance between 
        the 1-3 atoms of the angle */
    Symbol _r;
};

/** Return the symbol used to represent the angle (theta) */
inline const Symbol& AngleDB::theta() const
{
    return _theta;
}

/** Return the symbol used to represent the distance between the 
    1-3 atoms of the angle */
inline const Symbol& AngleDB::r() const
{
    return _r;
}

}

Q_DECLARE_METATYPE(SireMM::AngleDB)

SIRE_END_HEADER

#endif
