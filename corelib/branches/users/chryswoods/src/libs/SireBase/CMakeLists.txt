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
      countflops.h
      errors.h
      findexe.h
      increment.h
      incremint.h
      majorminorversion.h
      meminfo.h
      packedarray2d.h
      packedarray2d.hpp
      pairmatrix.hpp
      process.h
      properties.h
      property.h
      propertymap.h
      quickcopy.hpp
      qvariant_metatype.h
      shareddatapointer.hpp
      sharedpolypointer.hpp
      sharedpolypointer_cast.hpp
      sparsematrix.hpp
      stringmangler.h
      tempdir.h
      vector3d.hpp
    )

# Define the sources in SireBase
set ( SIREBASE_SOURCES

      register_sirebase.cpp

      array2d.cpp
      chunkedhash.cpp
      chunkedvector.cpp
      countflops.cpp
      errors.cpp
      findexe.cpp
      majorminorversion.cpp
      meminfo.cpp
      packedarray2d.cpp
      process.cpp
      properties.cpp
      property.cpp
      propertymap.cpp
      sharedpolypointer.cpp
      stringmangler.cpp     
      tempdir.cpp 

      ${SIREBASE_HEADERS}
    )

add_library (SireBase ${SIREBASE_SOURCES})

set_target_properties (SireBase PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SireBase
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
