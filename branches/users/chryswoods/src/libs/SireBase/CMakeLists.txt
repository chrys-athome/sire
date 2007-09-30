########################################
#
# CMake file for library:SireBase
#
########################################

# Third Party dependencies of this module

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the header files for SireBase
set ( SIREBASE_HEADERS

      errors.h
      findexe.h
      idmajminversion.h
      idpair.h
      idtriple.h
      idversion.h
      increment.h
      incremint.h
      majminversion.h
      majversion.h
      md5sum.h
      pairmatrix.hpp
      properties.h
      property.h
      propertymap.h
      sharedpolypointer.hpp
      sharedpolypointer_cast.hpp
      sparsematrix.hpp
      version.h
    )

# Define the sources in SireBase
set ( SIREBASE_SOURCES

      errors.cpp
      findexe.cpp
      idmajminversion.cpp
      idpair.cpp
      idtriple.cpp
      idversion.cpp
      majminversion.cpp
      majversion.cpp
      md5sum.cpp
      properties.cpp
      property.cpp
      propertymap.cpp
      sharedpolypointer.cpp
      version.cpp
      
      ${SIREBASE_HEADERS}
    )

add_library (SireBase ${SIREBASE_SOURCES})

set_target_properties (SireBase PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireBase
                       SireStream
                       )

# installation
install( TARGETS SireBase
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREBASE_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireBase )
