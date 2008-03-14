########################################
#
# CMake file for library:SireMaths
#
########################################

# Third Party dependencies of this module

# GSL
include_directories(${GSL_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireMaths
set ( SIREMATHS_HEADERS
      axisset.h
      circularangle.h
      complex.h
      constants.h
      distvector.h
      errors.h
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
      vectorvector.h
    )

# Define the sources in SireMaths
set ( SIREMATHS_SOURCES
      axisset.cpp
      complex.cpp
      distvector.cpp
      errors.cpp
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
                       SireStream
                       SireUnits
                       ${GSL_LIBRARIES}
                       ${QT_QTCORE_LIBRARY}
                       )

# installation
install( TARGETS SireMaths
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMATHS_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMaths )
          
