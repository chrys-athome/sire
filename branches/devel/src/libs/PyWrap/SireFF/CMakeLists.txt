########################################
#
# CMake file for Python wrapper:_FF
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

# Define the sources in SireFF
set ( WRAPFF_SOURCES
      ffbase.py.cpp
      ffcomponent.py.cpp
      ffprocessor.py.cpp
      ffworker.py.cpp
      forcefield.py.cpp
      parametermap.py.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireFF ${WRAPFF_SOURCES})

set_target_properties (PyWrap_SireFF PROPERTIES
                       VERSION ${SIRE_VERSION}
               SOVERSION ${SIRE_VERSION_MAJOR}
           )

target_link_libraries (PyWrap_SireFF
                       SireFF
                       PyWrap_SireMol
                       PyWrap_SireDB
                       PyWrap_SireCAS
                       ${BOOST_PYTHON_LIBRARY}
          )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireFF
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireFF wrap_SireFF.cpp)

target_link_libraries (PythonWrapping_SireFF
                       PyWrap_SireFF
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireFF
                       PROPERTIES
                         OUTPUT_NAME "FF"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/FF )

install( TARGETS PythonWrapping_SireFF
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
