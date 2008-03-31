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
      angledb.h
      anglegenerator.h
      assign_angles.h
      assign_bonds.h
      assign_dihedrals.h
      assign_internals.hpp
      atompairs.hpp
      bonddb.h
      bondgenerator.h
      chargedb.h
      cljcomponent.h
      cljpotential.h
      cljnbpairs.h
      dihedraldb.h
      dihedralgenerator.h
      errors.h
      intercljff.h
      intercoulombff.h
      interljff.h
      internaldatatypes.h
      internaldb.hpp
      internalgenerator.hpp
      internaltable.hpp
      internaltablebase.hpp
      intracljff.h
      ljdb.h
      ljpair.h
      ljparameter.h
      ljparameterdb.h
      molangleinfo.h
      molbondinfo.h
      moldihedralinfo.h
      molinternalinfo.hpp
      qhash_siremm.h
      resangleinfo.h
      resbondinfo.h
      resdihedralinfo.h
      resinternalinfo.hpp
      switchingfunction.h
      ureybradleydb.h

      detail/intrascaledatomicparameters.hpp
    )

# Define the sources in SireMM
set ( SIREMM_SOURCES
      angledb.cpp
      anglegenerator.cpp
      assign_angles.cpp
      assign_bonds.cpp
      assign_dihedrals.cpp
      bonddb.cpp
      bondgenerator.cpp
      chargedb.cpp
      cljcomponent.cpp
      cljpotential.cpp
      cljnbpairs.cpp
      dihedraldb.cpp
      dihedralgenerator.cpp
      errors.cpp
      intercljff.cpp
      intercoulombff.cpp
      interljff.cpp
      internaldatatypes.cpp
      ljdb.cpp
      ljpair.cpp
      ljparameter.cpp
      ljparameterdb.cpp
      molangleinfo.cpp
      molbondinfo.cpp
      moldihedralinfo.cpp
      resangleinfo.cpp
      resbondinfo.cpp
      resdihedralinfo.cpp
      switchingfunction.cpp
      ureybradleydb.cpp
    
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
                       SireDB
                       SireUnits
                       )

# installation
install( TARGETS SireMM
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMM_HEADERS} DESTINATION ${SIRE_INCLUDES}/SireMM )
