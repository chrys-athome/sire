########################################
#
# CMake file for Python wrapper:_System
#
########################################

# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs
                    ${CMAKE_SOURCE_DIR}/src/libs/PyWrap)

# Define the sources in SireSystem
set ( WRAPSYSTEM_SOURCES
      ffexpression.py.cpp
      moleculegroup.py.cpp
      move.py.cpp
      moves.py.cpp
      system.py.cpp
      systemdata.py.cpp
     )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireSystem ${WRAPSYSTEM_SOURCES})

set_target_properties (PyWrap_SireSystem PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
           )

target_link_libraries (PyWrap_SireSystem
                       SireSystem
                       PyWrap_SireFF
                       PyWrap_SireMol
                       ${BOOST_PYTHON_LIBRARY}
          )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireSystem
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireSystem wrap_SireSystem.cpp)

target_link_libraries (PythonWrapping_SireSystem
                       PyWrap_SireSystem
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireSystem
                       PROPERTIES
                         OUTPUT_NAME "System"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/System )

install( TARGETS PythonWrapping_SireSystem
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
