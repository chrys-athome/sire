########################################
#
# CMake file for library:SireIO
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTSQL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireIO
set ( SIREIO_HEADERS
      errors.h
      iobase.h
      pdb.h
#      protoms.h
#      protomsws.h
#      psf.h
    )

# Define the sources in SireIO
set ( SIREIO_SOURCES
      errors.cpp
      iobase.cpp
      pdb.cpp
#      protoms.cpp
#      protomsws.cpp
#      psf.cpp
    
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
                       SireMol
                       )

# installation
install( TARGETS SireIO
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREIO_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireIO )
