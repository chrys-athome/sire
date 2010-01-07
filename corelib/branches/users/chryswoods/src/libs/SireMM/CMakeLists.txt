########################################
#
# CMake file for library:SireMM
#
########################################

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireMM
set ( SIREMM_HEADERS
      anglerestraint.h
      atomfunctions.h
      atomljs.h
      atompairs.hpp
      cljcomponent.h
      cljpotential.h
      cljnbpairs.h
      coulombpotential.h
      dihedralrestraint.h
      distancerestraint.h
      errors.h
      fouratomfunctions.h
      intercljff.h
      intercoulombff.h
      interljff.h
      internalcomponent.h
      internalff.h
      internalparameters.h
      internalperturbation.h
      intersoftcljff.h
      intracljff.h
      intracoulombff.h
      intraljff.h
      ljpair.h
      ljparameter.h
      ljparameterdb.h
      ljperturbation.h
      ljpotential.h
      restraint.h
      restraintcomponent.h
      restraintff.h
      softcljcomponent.h
      softcljpotential.h
      switchingfunction.h
      threeatomfunctions.h
      twoatomfunctions.h
    )

set ( SIREMM_DETAIL_HEADERS
      detail/intrascaledatomicparameters.hpp
    )

# Define the sources in SireMM
set ( SIREMM_SOURCES

      anglerestraint.cpp
      atomfunctions.cpp
      atomljs.cpp
      cljcomponent.cpp
      cljpotential.cpp
      cljnbpairs.cpp
      coulombpotential.cpp
      dihedralrestraint.cpp
      distancerestraint.cpp
      errors.cpp
      fouratomfunctions.cpp
      intercljff.cpp
      intercoulombff.cpp
      interljff.cpp
      internalcomponent.cpp
      internalff.cpp
      internalparameters.cpp
      internalperturbation.cpp
      intersoftcljff.cpp
      intracljff.cpp
      intracoulombff.cpp
      intraljff.cpp
      ljpair.cpp
      ljparameter.cpp
      ljparameterdb.cpp
      ljperturbation.cpp
      ljpotential.cpp
      restraint.cpp
      restraintcomponent.cpp
      restraintff.cpp
      softcljcomponent.cpp
      softcljpotential.cpp
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
                       SireStream
                       )

# installation
install( TARGETS SireMM EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMM_HEADERS} DESTINATION ${SIRE_INCLUDES}/SireMM )
install( FILES ${SIREMM_DETAIL_HEADERS} DESTINATION ${SIRE_INCLUDES}/SireMM/detail )
