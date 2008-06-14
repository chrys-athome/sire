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
      atomfunctions.h
      atomljs.h
      atompairs.hpp
      cljcomponent.h
      cljpotential.h
      cljnbpairs.h
      errors.h
      fouratomfunctions.h
      intercljff.h
      intercoulombff.h
      interljff.h
      internalcomponent.h
      internalff.h
      internalparameters.h
      intracljff.h
      ljpair.h
      ljparameter.h
      ljparameterdb.h
      qhash_siremm.h
      switchingfunction.h
      threeatomfunctions.h
      twoatomfunctions.h
    )

set ( SIREMM_DETAIL_HEADERS
      detail/intrascaledatomicparameters.hpp
    )

# Define the sources in SireMM
set ( SIREMM_SOURCES
      atomfunctions.cpp
      cljcomponent.cpp
      cljpotential.cpp
      cljnbpairs.cpp
      errors.cpp
      fouratomfunctions.cpp
      intercljff.cpp
      intercoulombff.cpp
      interljff.cpp
      internalcomponent.cpp
      internalff.cpp
      internalparameters.cpp
      intracljff.cpp
      ljpair.cpp
      ljparameter.cpp
      ljparameterdb.cpp
      switchingfunction.cpp
      threeatomfunctions.cpp
      twoatomfunctions.cpp    

      ${SIREMM_HEADERS}
      ${SIREMM_DETAIL_HEADERS}
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
install( FILES ${SIREMM_DETAIL_HEADERS} DESTINATION ${SIRE_INCLUDES}/SireMM/detail )
