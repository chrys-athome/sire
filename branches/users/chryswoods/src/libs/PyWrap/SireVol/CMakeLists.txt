########################################
#
# CMake file for Python wrapper:_Vol
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

# Define the sources in SireVol
set ( WRAPVOL_SOURCES
      coordgroup.py.cpp

      wrap_aabox.cpp
      wrap_simvolume.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireVol ${WRAPVOL_SOURCES})

set_target_properties (PyWrap_SireVol PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireVol
                       SireVol
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireVol
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireVol wrap_SireVol.cpp)

target_link_libraries (PythonWrapping_SireVol
                       PyWrap_SireVol
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireVol
                       PROPERTIES
                         OUTPUT_NAME "Vol"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Vol )

install( TARGETS PythonWrapping_SireVol
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
