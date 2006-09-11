########################################
#
# CMake file for library:SirePy
#
########################################

# Third Party dependencies of this module
include_directories(${PYTHON_INCLUDE_PATH})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SirePy
set ( SIREPY_SOURCES
      python2qt.cpp
      pythonerror.cpp
      pythonparser.cpp
      pythonscript.cpp
    )

add_library (SirePy SHARED ${SIREPY_SOURCES})

set_target_properties (SirePy PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SirePy
                       SireError
                       ${PYTHON_LIBRARIES}
                       )
