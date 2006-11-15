########################################
#
# CMake file for library:SireMM
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireMM
set ( SIREMM_SOURCES
      angledb.cpp            
      anglegenerator.cpp     
      angletable.cpp         
      assign_angles.cpp      
      assign_bonds.cpp       
      assign_dihedrals.cpp   
      bonddb.cpp             
      bondgenerator.cpp      
      bondtable.cpp          
      chargedb.cpp           
      chargeparameter.cpp    
      chargetable.cpp       
      cljff.cpp	      
      cljpair.cpp	      
      cljparameter.cpp      
      combiningrules.cpp
      dihedraldb.cpp        
      dihedralgenerator.cpp 
      dihedraltable.cpp     
      errors.cpp	      
      intercljff.cpp        
      intergroupcljff.cpp   
      internaldatatypes.cpp 
      ljdb.cpp
      ljparameter.cpp
      ljtable.cpp
      mmdb.cpp
      molangleinfo.cpp
      molbondinfo.cpp
      moldihedralinfo.cpp
      resangleinfo.cpp
      resbondinfo.cpp
      resdihedralinfo.cpp
      switchingfunction.cpp
      tip4pff.cpp
      ureybradleydb.cpp

      detail/molcljinfo.cpp
    )

add_library (SireMM ${SIREMM_SOURCES})

set_target_properties (SireMM PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireMM
                       SireFF
                       SireDB
                       SireUnits
                       )

# installation
install( TARGETS SireMM
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
