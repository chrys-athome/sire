########################################
#
# CMake file for library:SireBase
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the header files for SireBase
set ( SIREBASE_HEADERS

      array2d.hpp
      array2d.h
      chunkedhash.hpp
      chunkedvector.hpp
      combineproperties.h
      countflops.h
      errors.h
      findexe.h
      increment.h
      incremint.h
      linktoproperty.h
      meminfo.h
      packedarray2d.h
      packedarray2d.hpp
      pairmatrix.hpp
      process.h
      properties.h
      property.h
      propertymap.h
      quickcopy.hpp
      savestate.h
      sharedversion.h
      sparsematrix.hpp
      streambase.h
      stringmangler.h
      tempdir.h
      trigarray2d.h
      trigarray2d.hpp
      vector3d.hpp
      version.h
      versionregistry.hpp
    )

# Define the sources in SireBase
set ( SIREBASE_SOURCES

      array2d.cpp
      chunkedhash.cpp
      chunkedvector.cpp
      combineproperties.cpp
      countflops.cpp
      errors.cpp
      findexe.cpp
      linktoproperty.cpp
      meminfo.cpp
      packedarray2d.cpp
      process.cpp
      properties.cpp
      property.cpp
      propertymap.cpp
      savestate.cpp
      sharedversion.cpp
      streambase.cpp
      stringmangler.cpp     
      tempdir.cpp
      trigarray2d.cpp 
      version.cpp

      ${SIREBASE_HEADERS}
    )

add_library (SireBase ${SIREBASE_SOURCES})

set_target_properties (SireBase PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireBase
                       SireID
                       SireStream
                       )

# installation
install( TARGETS SireBase EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREBASE_HEADERS} 
         DESTINATION ${SIRE_INCLUDES}/SireBase )
