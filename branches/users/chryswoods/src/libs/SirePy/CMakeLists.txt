########################################
#
# CMake file for library:SirePy
#
########################################

# Third Party dependencies of this module
include_directories(${PYTHON_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SirePy
set ( SIREPY_HEADERS
      indexer.hpp
      python2qt.h
      python2wrappers.h
      pythonerror.h
      pythonparser.h
      pythonscript.h
    )

# Define the sources in SirePy
set ( SIREPY_SOURCES
      python2qt.cpp
      pythonerror.cpp
      pythonparser.cpp
      pythonscript.cpp
    
      ${SIREPY_HEADERS}
    )

set ( SIREPY_MOCS
      pythonparser.h
    )

qt4_wrap_cpp( SIREPY_MOC_SRCS ${SIREPY_MOCS} )

add_library (SirePy ${SIREPY_SOURCES} ${SIREPY_MOC_SRCS})

set_target_properties (SirePy PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                      )

# Link to other Sire libraries
target_link_libraries (SirePy
                       SireError
                       ${PYTHON_LIBRARY}
                       ${BOOST_PYTHON_LIBRARY}
                       )

# installation
install( TARGETS SirePy
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREPY_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SirePy )
