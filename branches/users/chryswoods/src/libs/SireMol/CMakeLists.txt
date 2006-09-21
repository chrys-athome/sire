########################################
#
# CMake file for library:SireMol
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireMol
set ( SIREMOL_SOURCES
      angle.cpp	     
      atom.cpp	     
      atomidgroup.cpp      
      atomindex.cpp	     
      atominfo.cpp
      atominfogroup.cpp
      bond.cpp	     
      cgatomid.cpp
      convertfunction.cpp  
      cutgroup.cpp	     
      dihedral.cpp	     
      editmol.cpp	     
      editmoldata.cpp    
      editres.cpp	   
      elementdb.cpp	   
      errors.cpp	   
      groupindexid.cpp   
      improper.cpp	   
      indexinfo.cpp	   
      molcgatomid.cpp    
      molcutgroupid.cpp  
      moleculebonds.cpp  
      molecule.cpp
      moleculedata.cpp
      moleculeinfo.cpp
      moleculeversion.cpp
      residatomid.cpp
      residue.cpp
      residuebonds.cpp
      residuecutting.cpp
      residueinfo.cpp
      resnumatomid.cpp
      resnumindexid.cpp
      splitmolecule.cpp
      weightfunction.cpp
    )

add_library (SireMol ${SIREMOL_SOURCES})

set_target_properties (SireMol PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireMol
                       SireStream
                       )

# installation
install( TARGETS SireMol
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
