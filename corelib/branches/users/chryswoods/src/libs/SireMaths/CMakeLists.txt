########################################
#
# CMake file for library:SireMaths
#
########################################

# SireMaths depends on >= GSL 1.6
find_package(GSL 1.6 REQUIRED)

message( STATUS "GSL paths ${GSL_LINK_DIRECTORIES} "
                "${GSL_LIBRARIES} | ${GSL_INCLUDE_DIR}" )

include_directories( BEFORE ${GSL_INCLUDE_DIR})

save_sire_variable( "SIRE_GSL_INCLUDE_PATH" "${GSL_INCLUDE_DIR}" )

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireMaths
set ( SIREMATHS_HEADERS
      accumulator.h
      axisset.h
      complex.h
      constants.h
      distvector.h
      errors.h
      freeenergyaverage.h
      histogram.h
      line.h
      maths.h
      matrix.h
      matrix4.h
      plane.h
      quaternion.h
      rangenerator.h
      rational.h
      rotate.h
      sphere.h
      torsion.h
      triangle.h
      vector.h
    )

# Define the sources in SireMaths
set ( SIREMATHS_SOURCES

      register_siremaths.cpp

      accumulator.cpp
      axisset.cpp
      complex.cpp
      distvector.cpp
      errors.cpp
      freeenergyaverage.cpp
      histogram.cpp
      line.cpp
      matrix4.cpp
      matrix.cpp
      plane.cpp
      quaternion.cpp
      rangenerator.cpp
      rational.cpp
      sphere.cpp
      torsion.cpp
      triangle.cpp
      vector.cpp
    
      ${SIREMATHS_HEADERS}
    )

add_library (SireMaths ${SIREMATHS_SOURCES})

set_target_properties (SireMaths PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireMaths
                       SireID
                       SireBase
                       SireUnits
                       SireStream
                       ${GSL_LIBRARIES}
                       )

save_sire_variable( "SIREMATHS_EXTRA_LIBRARIES" "${GSL_LIBRARIES}" )

# installation
install( TARGETS SireMaths EXPORT SireLibraries
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMATHS_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMaths )
          
