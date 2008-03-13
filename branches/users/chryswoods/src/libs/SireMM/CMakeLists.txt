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
      angletable.h
      angletable.hpp
      assign_angles.h
      assign_bonds.h
      assign_dihedrals.h
      assign_internals.hpp
      atomiccharges.h
      atomicljs.h
      atompairs.hpp
      bonddb.h
      bondgenerator.h
      bondtable.h
      bondtable.hpp
      chargedb.h
      chargeparameter.h
      chargetable.h
      cljcomponent.h
      cljff.h
      cljfunctional.h
      cljnbpairs.h
      cljpair.h
      cljparameter.h
      combiningrules.h
      coulombff.h
      dihedraldb.h
      dihedralgenerator.h
      dihedraltable.h
      dihedraltable.hpp
      errors.h
      harmonicff.h
      inter2bodyff.hpp
      intercljff.h
      intercoulombff.h
      intergroup2bodyff.hpp
      intergroupcljff.h
      intergroupcoulombff.h
      intergroupljff.h
      interljff.h
      internaldatatypes.h
      internaldb.hpp
      internalgenerator.hpp
      internaltable.hpp
      internaltablebase.hpp
      intra2bodyff.hpp
      intracljff.h
      intragroup2bodyff.hpp
      ljdb.h
      ljff.h
      ljpair.h
      ljparameter.h
      ljtable.h
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
      angletable.cpp
      assign_angles.cpp
      assign_bonds.cpp
      assign_dihedrals.cpp
      atomiccharges.cpp
      atomicljs.cpp
      bonddb.cpp
      bondgenerator.cpp
      bondtable.cpp
      chargedb.cpp
      chargeparameter.cpp
      chargetable.cpp
      cljcomponent.cpp
      cljff.cpp
      cljfunctional.cpp
      cljnbpairs.cpp
      cljpair.cpp
      cljparameter.cpp
      combiningrules.cpp
      coulombff.cpp
      dihedraldb.cpp
      dihedralgenerator.cpp
      dihedraltable.cpp
      errors.cpp
      harmonicff.cpp
      intercljff.cpp
      intercoulombff.cpp
      intergroupcoulombff.cpp
      intergroupcljff.cpp
      intergroupljff.cpp
      interljff.cpp
      internaldatatypes.cpp
      ljdb.cpp
      ljff.cpp
      ljpair.cpp
      ljparameter.cpp
      ljtable.cpp
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
