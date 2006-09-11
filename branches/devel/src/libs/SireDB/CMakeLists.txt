########################################
#
# CMake file for library:SireDB
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireDB
set ( SIREDB_SOURCES
      assign_atoms.cpp         
      assignbase.cpp           
      assign_parameters.cpp    
      atomdb.cpp               
      atomtable.cpp            
      atomtabledata.cpp        
      atomtype.cpp             
      atomtypedb.cpp           
      atomtypetable.cpp        
      dbbase.cpp               
      errors.cpp               
      funcdb.cpp               
      matchatom.cpp            
      matchatomtype.cpp      
      matchatomtypedata.cpp 
      matchdata.cpp	      
      matchhelpers.cpp      
      matchmol.cpp	      
      matchmra.cpp	      
      matchmradata.cpp      
      matchmr.cpp	      
      matchmrdata.cpp       
      matchres.cpp	      
      namedb.cpp	      
      nameidset.cpp	      
      parameterdb.cpp       
      parametertable.cpp    
      relateatomtypedata.cpp
      relateatomtypedb.cpp
      relatedb.cpp
      relatemradata.cpp
      relatemradb.cpp
      relatemrdata.cpp
      relatemrdb.cpp
      relationshipdb.cpp
      smarts.cpp
      tablebase.cpp
      term12db.cpp
      term13db.cpp
      term14db.cpp
      using_database.cpp
    )

add_library (SireDB SHARED ${SIREDB_SOURCES})

set_target_properties (SireDB PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireDB
                       SireMol
		       SireCAS
		       SqliteDriver
                       ${QT_QTCORE_LIBRARY}
                       ${QT_QTSQL_LIBRARY}
                       )

