########################################
#
# CMake file for library:SireIO
#
########################################

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireIO
set ( SIREIO_HEADERS
      errors.h
      iobase.h
      pdb.h
      protoms.h
      tinker.h
      trajectorymonitor.h
    )

# Define the sources in SireIO
set ( SIREIO_SOURCES

      register_sireio.cpp

      errors.cpp
      iobase.cpp
      pdb.cpp
      protoms.cpp
      tinker.cpp
      trajectorymonitor.cpp    

      ${SIREIO_HEADERS}
    )

add_library (SireIO ${SIREIO_SOURCES})

set_target_properties (SireIO PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireIO
                       SireMM
                       SireMove
                       SireMol
                       SireStream
                       )

# installation
install( TARGETS SireIO EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREIO_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireIO )
