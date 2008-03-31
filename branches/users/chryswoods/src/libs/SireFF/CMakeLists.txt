########################################
#
# CMake file for library:SireFF
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireFF
set ( SIREFF_HEADERS
      errors.h
      ff.h
      ff3d.h
      ffcomponent.h
      ffgroupid.h
      ffid.h
      ffidentifier.h
      ffidx.h
      ffmolgroup.h
      ffname.h
      ffnum.h
      forcefield.h
      forcefields.h
      forcefieldsbase.h
      forcetable.h
      g1ff.h
      g2ff.h
      inter2b2g3dff.hpp
      inter2b2gff.hpp
      inter2b3dff.hpp
      inter2bff.hpp

      detail/atomiccoords3d.h
      detail/atomicparameters.hpp
      detail/atomicparameters3d.hpp
      detail/ffmolecules.h
      detail/ffmolecules3d.h

      qhash_sireff.h
    )

# Define the sources in SireFF
set ( SIREFF_SOURCES
      errors.cpp
      ff.cpp
      ff3d.cpp
      ffcomponent.cpp
      ffgroupid.cpp
      ffid.cpp
      ffidentifier.cpp
      ffmolgroup.cpp
      forcefield.cpp
      forcefields.cpp
      forcefieldsbase.cpp
      forcetable.cpp
      g1ff.cpp
      g2ff.cpp

      detail/atomiccoords3d.cpp
      detail/ffmolecules.cpp
      detail/ffmolecules3d.cpp   

      ${SIREFF_HEADERS}
    )

add_library (SireFF ${SIREFF_SOURCES})

set_target_properties (SireFF PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireFF
                       SireDB
                       SireCluster
                       SireBase
                       ${QT_QTSQL_LIBRARY}
                       )

# installation
install( TARGETS SireFF
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREFF_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireFF )
