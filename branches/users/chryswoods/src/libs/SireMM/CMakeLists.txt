########################################
#
# CMake file for library:SireMM
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireMM
set ( SIREMM_HEADERS
      atompairs.hpp
      cljcomponent.h
      cljpotential.h
      cljnbpairs.h
      errors.h
      intercljff.h
      intercoulombff.h
      interljff.h
      intracljff.h
      ljpair.h
      ljparameter.h
      ljparameterdb.h
      qhash_siremm.h
      switchingfunction.h

      detail/intrascaledatomicparameters.hpp
    )

# Define the sources in SireMM
set ( SIREMM_SOURCES
      cljcomponent.cpp
      cljpotential.cpp
      cljnbpairs.cpp
      errors.cpp
      intercljff.cpp
      intercoulombff.cpp
      interljff.cpp
      intracljff.cpp
      ljpair.cpp
      ljparameter.cpp
      ljparameterdb.cpp
      switchingfunction.cpp
    
      ${SIREMM_HEADERS}
    )

add_library (SireMM ${SIREMM_SOURCES})

set_target_properties (SireMM PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireMM
                       SireFF
                       SireMol
                       SireCAS
                       SireBase
                       SireMaths
                       SireUnits
                       )

# installation
install( TARGETS SireMM
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMM_HEADERS} DESTINATION ${SIRE_INCLUDES}/SireMM )
