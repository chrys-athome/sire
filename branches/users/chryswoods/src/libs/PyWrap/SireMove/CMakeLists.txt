########################################
#
# CMake file for Python wrapper:_Move
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

# Define the sources in SireMove
set ( WRAPMOVE_SOURCES
      rigidmolecules.py.cpp
      flexiblemolecules.py.cpp
     )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireMove ${WRAPMOVE_SOURCES})

set_target_properties (PyWrap_SireMove PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
           )

target_link_libraries (PyWrap_SireMove
                       SireMove
		       PyWrap_SireMol
                       ${BOOST_PYTHON_LIBRARY}
          )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireMove
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireMove wrap_SireMove.cpp)

target_link_libraries (PythonWrapping_SireMove
                       PyWrap_SireMove
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireMove
                       PROPERTIES
                         OUTPUT_NAME "Move"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Move )

install( TARGETS PythonWrapping_SireMove
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )

