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

/**
  * This file contains the boost::python wrapping of the parameter database classes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/dbbase.h"
#include "SireDB/namedb.h"
#include "SireDB/funcdb.h"
#include "SireDB/relatedb.h"
#include "SireDB/atomdb.h"
#include "SireDB/term12db.h"
#include "SireDB/term13db.h"
#include "SireDB/term14db.h"
#include "SireDB/relateidmap.h"

#include "SireCAS/expression.h"
#include "SireCAS/function.h"

#include "SirePy/str.hpp"

using namespace boost::python;
using namespace SirePy;
using namespace SireCAS;

namespace SireDB
{

Expression wrap_getFunc_QString2(FuncDB &db, const QString &userid, bool &foundfunc)
{
    return db.getFunc(userid, &foundfunc);
}

Expression wrap_getFunc_QString1(FuncDB &db, const QString &userid)
{
    return db.getFunc(userid);
}

Expression wrap_getFunc_Function2(FuncDB &db, const Function &func, bool &foundfunc)
{
    return db.getFunc(func, &foundfunc);
}

Expression wrap_getFunc_Function1(FuncDB &db, const Function &func)
{
    return db.getFunc(func);
}

Expression wrap_getFunc_RelateID2(FuncDB &db, RelateID relateid, bool &foundfunc)
{
    return db.getFunc(relateid, &foundfunc);
}

Expression wrap_getFunc_RelateID1(FuncDB &db, RelateID relateid)
{
    return db.getFunc(relateid);
}

Expression wrap_getFunc_RelateIDMap2(FuncDB &db, const RelateIDMap &relateids, 
                                     bool &foundfunc)
{
    return db.getFunc(relateids, &foundfunc);
}

Expression wrap_getFunc_RelateIDMap1(FuncDB &db, const RelateIDMap &relateids)
{
    return db.getFunc(relateids);
}

void  
SIREDB_EXPORT
export_DB()
{
    
    class_<DBBase, boost::noncopyable>("DBBase", no_init)
        
        .def( "__str__", &__str__<DBBase> )
        
        .def( "toString", &DBBase::toString )
        
        .def( "typeName", &DBBase::typeName ).staticmethod("typeName")
        
        .def( "what", &DBBase::what )
        
        //wrap isA() and asA() via python
    ;
    
    class_<RelateDB, bases<DBBase>, boost::noncopyable>("RelateDB", no_init)
    
        .def( "typeName", &RelateDB::typeName ).staticmethod("typeName")
  
    ;
    
    class_<AtomDB, bases<RelateDB>, boost::noncopyable>("AtomDB", no_init)
    
        .def( "typeName", &RelateDB::typeName ).staticmethod("typeName")
    
    ;
    
    class_<FuncDB, bases<RelateDB>, boost::noncopyable>("FuncDB", no_init)
    
        .def( "typeName", &FuncDB::typeName ).staticmethod("typeName")
    
        .def( "getFunc", &wrap_getFunc_QString2 )
        .def( "getFunc", &wrap_getFunc_QString1 )
        
        .def( "getFunc", &wrap_getFunc_Function2 )
        .def( "getFunc", &wrap_getFunc_Function1 )
        
        .def( "getFunc", &wrap_getFunc_RelateID2 )
        .def( "getFunc", &wrap_getFunc_RelateID1 )
        
        .def( "getFunc", &wrap_getFunc_RelateIDMap2 )
        .def( "getFunc", &wrap_getFunc_RelateIDMap1 )
    
        .def( "resolveTerms", &FuncDB::resolveTerms )
    
    ;
    
    class_<Term12DB, bases<FuncDB>, boost::noncopyable>("Term12DB", no_init)
    
        .def( "typeName", &Term12DB::typeName ).staticmethod("typeName")
    ;
    
    class_<Term13DB, bases<FuncDB>, boost::noncopyable>("Term13DB", no_init)
    
        .def( "typeName", &Term13DB::typeName ).staticmethod("typeName")
    ;
    
    class_<Term14DB, bases<FuncDB>, boost::noncopyable>("Term14DB", no_init)
    
        .def( "typeName", &Term14DB::typeName ).staticmethod("typeName")
    ;
    
    class_<NameDB, bases<DBBase> >("NameDB", init<>())
        
        .def( "addAtomName", &NameDB::addAtomName )
        .def( "addResidueName", &NameDB::addResidueName )
        .def( "addMoleculeName", &NameDB::addMoleculeName )
        .def( "addName", &NameDB::addName )
        
        .def( "caseSensitive", &NameDB::caseSensitive )
        
        .def( "caseSensitiveMoleculeNames", &NameDB::caseSensitiveMoleculeNames )
        .def( "caseSensitiveResidueNames", &NameDB::caseSensitiveResidueNames )
        .def( "caseSensitiveAtomNames", &NameDB::caseSensitiveAtomNames )
    
        .def( "getAtomName", &NameDB::getAtomName )
        .def( "getAtomNameID", &NameDB::getAtomNameID )
        
        .def( "getResidueNameID", &NameDB::getResidueNameID )
        .def( "getResidueName", &NameDB::getResidueName )
        
        .def( "getMoleculeName", &NameDB::getMoleculeName )
        .def( "getMoleculeNameID", &NameDB::getMoleculeNameID )

        .def( "getName", (QString (NameDB::*)(const QString&, NameID))
                              &NameDB::getName )
        .def( "getNameID", &NameDB::getNameID )

        .def( "nNames", &NameDB::nNames )
        
        .def( "setCaseInsensitive", &NameDB::setCaseInsensitive )
        .def( "setCaseInsensitiveAtomNames", &NameDB::setCaseInsensitiveAtomNames )
        .def( "setCaseInsensitiveResidueNames", &NameDB::setCaseInsensitiveResidueNames )
        .def( "setCaseInsensitiveMoleculeNames", &NameDB::setCaseInsensitiveMoleculeNames )
    
        .def( "setCaseSensitive", &NameDB::setCaseSensitive )
        .def( "setCaseSensitiveAtomNames", &NameDB::setCaseSensitiveAtomNames )
        .def( "setCaseSensitiveResidueNames", &NameDB::setCaseSensitiveResidueNames )
        .def( "setCaseSensitiveMoleculeNames", &NameDB::setCaseSensitiveMoleculeNames )
    
        .def( "typeName", &NameDB::typeName ).staticmethod("typeName")
    ;
}

}
