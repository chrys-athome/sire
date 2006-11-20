
/**
  * This file contains the boost::python wrapping of the relationship databases classes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/atomtype.h"
#include "SireDB/parameterdb.h"
#include "SireDB/parametertable.h"
#include "SireDB/dbbase.h"
#include "SireDB/atomtypedb.h"
#include "SireDB/namedb.h"
#include "SireDB/relatemrdb.h"
#include "SireDB/relatemradb.h"
#include "SireDB/relateatomtypedb.h"
#include "SireDB/matchmradata.h"
#include "SireDB/matchatomtypedata.h"
#include "SireDB/matchmra.h"

#include "SireMol/atomindex.h"
#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"

using namespace boost::python;
using namespace SireMol;

namespace SireDB
{

void  
SIREDB_EXPORT
export_RelateDB()
{
    class_<RelationshipDB, bases<DBBase>, boost::noncopyable>("RelationshipDB", no_init)
    
        .def( "search", &RelationshipDB::search<AtomIndex> )
        .def( "search", &RelationshipDB::search<Bond> )
        .def( "search", &RelationshipDB::search<Angle> )
        .def( "search", &RelationshipDB::search<Dihedral> )
        .def( "search", &RelationshipDB::search<Improper> )
                    
        .def( "typeName", &RelationshipDB::typeName ).staticmethod("typeName")
    
    ;

    class_<RelateMRDB, bases<DBBase> >( "RelateMRDB", init<>() )
        
        .def( "add", &RelateMRDB::add )
            
        .def( "addMoleculeGroup", &RelateMRDB::addMoleculeGroup )
        .def( "addMoleculeState", &RelateMRDB::addMoleculeState )
        .def( "addResidueAlias", &RelateMRDB::addResidueAlias )
        
        .def( "alias", &RelateMRDB::alias )
        
        .def( "caseSensitiveMoleculeGroups", 
                            &RelateMRDB::caseSensitiveMoleculeGroups )
        .def( "caseSensitiveMoleculeStates", 
                            &RelateMRDB::caseSensitiveMoleculeStates )
        .def( "caseSensitiveResidueAliases", 
                            &RelateMRDB::caseSensitiveResidueAliases )
        
        .def( "get", (tuple<RelateID,int> (RelateMRDB::*)(const MatchMR&))
                        &RelateMRDB::get )
        .def( "get", (MatchMR (RelateMRDB::*)(RelateID))
                        &RelateMRDB::get )
        
        .def( "getMoleculeGroup", &RelateMRDB::getMoleculeGroup )
        .def( "getMoleculeState", &RelateMRDB::getMoleculeState )
        .def( "getResidueAlias", &RelateMRDB::getResidueAlias )
        
        .def( "getMoleculeGroupID", &RelateMRDB::getMoleculeGroupID )
        .def( "getMoleculeStateID", &RelateMRDB::getMoleculeStateID )
        .def( "getResidueAliasID", &RelateMRDB::getResidueAliasID )
        
        .def( "search", (RelateIDMap (RelateMRDB::*)(const MatchMR&))
                        &RelateMRDB::search )
        
        .def( "setCaseInsensitiveMoleculeGroups", 
                            &RelateMRDB::setCaseInsensitiveMoleculeGroups )
        .def( "setCaseInsensitiveMoleculeStates", 
                            &RelateMRDB::setCaseInsensitiveMoleculeStates )
        .def( "setCaseInsensitiveResidueAliases", 
                            &RelateMRDB::setCaseInsensitiveResidueAliases )
    
        .def( "setCaseSensitiveMoleculeGroups", 
                            &RelateMRDB::setCaseSensitiveMoleculeGroups )
        .def( "setCaseSensitiveMoleculeStates", 
                            &RelateMRDB::setCaseSensitiveMoleculeStates )
        .def( "setCaseSensitiveResidueAliases", 
                            &RelateMRDB::setCaseSensitiveResidueAliases )

        .def( "typeName", &RelateMRDB::typeName ).staticmethod("typeName")
    ;

    class_<RelateMRADB, bases<RelationshipDB> >( "RelateMRADB", init<>() )
    
        .def( "typeName", &RelateMRADB::typeName ).staticmethod("typeName")
        
        .def( "add", (RelateID (RelateMRADB::*)(const MatchMRAData&, uint))
                          &RelateMRADB::add )
        .def( "add", (RelateID (RelateMRADB::*)(const MatchMRA&))
                          &RelateMRADB::add )
    
        .def( "get", (RelateID (RelateMRADB::*)(const MatchMRAData&, uint))
                          &RelateMRADB::get )
        .def( "get", (RelateID (RelateMRADB::*)(const MatchMRA&))
                          &RelateMRADB::get )
        .def( "get", (MatchMRAData (RelateMRADB::*)(RelateID, uint))
                          &RelateMRADB::get )
    ;
    
    class_<RelateAtomTypeDB, bases<RelationshipDB> >( "RelateAtomTypeDB", init<>() )
        
        .def( "add", (RelateID (RelateAtomTypeDB::*)(const MatchAtomTypeData&, uint))
                          &RelateAtomTypeDB::add )
        .def( "add", (RelateID (RelateAtomTypeDB::*)(const MatchAtomType&))
                          &RelateAtomTypeDB::add )
        
        .def( "addAtomType", &RelateAtomTypeDB::addAtomType )
        
        .def( "caseSensitiveAtomTypes", &RelateAtomTypeDB::caseSensitiveAtomTypes )
        
        .def( "get", (RelateID (RelateAtomTypeDB::*)(const MatchAtomTypeData&, uint))
                          &RelateAtomTypeDB::get )
        .def( "get", (RelateID (RelateAtomTypeDB::*)(const MatchAtomType&))
                          &RelateAtomTypeDB::get )
        .def( "get", (MatchAtomTypeData (RelateAtomTypeDB::*)(RelateID, uint))
                          &RelateAtomTypeDB::get )
    
        .def( "getAtomType", &RelateAtomTypeDB::getAtomType )
        .def( "getAtomTypeID", &RelateAtomTypeDB::getAtomTypeID )
    
        .def( "setCaseInsensitiveAtomTypes", 
                        &RelateAtomTypeDB::setCaseInsensitiveAtomTypes )
        .def( "setCaseSensitiveAtomTypes", 
                        &RelateAtomTypeDB::setCaseSensitiveAtomTypes )
    
        .def( "typeName", &RelateAtomTypeDB::typeName ).staticmethod("typeName")
    ;
}

}
