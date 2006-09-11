########################################
#
# CMake file for library:SireIO
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireIO
set ( SIREIO_SOURCES
      iobase.cpp
      mct.cpp
      mol2.cpp
      pdb.cpp
      protoms.cpp
      protomsws.cpp
      psf.cpp
    )

add_library (SireIO SHARED ${SIREIO_SOURCES})

set_target_properties (SireIO PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireIO
                       SireMM
                       SireMol
                       )
