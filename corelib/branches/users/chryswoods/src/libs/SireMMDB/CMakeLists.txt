########################################
#
# CMake file for library:SireDB
#
########################################

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireDB
set ( SIREDB_HEADERS
      assign_atoms.h
      assign_parameters.h
      assignbase.h
      assigninstruction.h
      atomdb.h
      atomparameter.hpp
      atomtable.h
      atomtable.hpp
      atomtype.h
      atomtypedb.h
      atomtypetable.h
      dbbase.h
      errors.h
      funcdb.h
      matchatom.h
      matchatomtype.h
      matchatomtypedata.h
      matchdata.h
      matchhelpers.h
      matchmol.h
      matchmr.h
      matchmra.h
      matchmradata.h
      matchmrdata.h
      matchres.h
      multimatch.hpp
      namedb.h
      nameidset.h
      overwriteparams.h
      parameterdb.h
      parametergroup.hpp
      parameterstore.hpp
      parametertable.h
      relateatomtypedata.h
      relateatomtypedatalist.h
      relateatomtypedb.h
      relatedb.h
      relateidmap.h
      relatemradata.h
      relatemradatalist.h
      relatemradb.h
      relatemrdata.h
      relatemrdb.h
      relationshipdb.h
      smarts.h
      tablebase.h
      term12db.h
      term13db.h
      term14db.h
      using_database.h
      using_parameters.hpp
      using_relationships.hpp
    )

# Define the sources in SireDB
set ( SIREDB_SOURCES
      assign_atoms.cpp         
      assignbase.cpp           
      assign_parameters.cpp    
      atomdb.cpp               
      atomtable.cpp            
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
      
      ${SIREDB_HEADERS}
    )

add_library (SireDB ${SIREDB_SOURCES})

set_target_properties (SireDB PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireDB
                       SireMol
                       SireCAS
                       )

# installation
install( TARGETS SireDB EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREDB_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireDB )
