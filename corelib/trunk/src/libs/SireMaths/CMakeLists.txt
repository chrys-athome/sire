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

# Define the sources in ThirdParty
set ( SIREMATHS_SOURCES
      angle.cpp
      axisset.cpp
      complex.cpp
      errors.cpp
      line.cpp
      matrix4.cpp
      matrix.cpp
      plane.cpp
      quaternion.cpp
      rational.cpp
      sphere.cpp
      torsion.cpp
      triangle.cpp
      vector.cpp
    )

add_library (SireMaths ${SIREMATHS_SOURCES})

set_target_properties (SireMaths PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireMaths
                       SireError
                       ${GSL_LIBRARIES}
                       ${QT_QTCORE_LIBRARY}
                       )

# installation
install( TARGETS SireMaths
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
