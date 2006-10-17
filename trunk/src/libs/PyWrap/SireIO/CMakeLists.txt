########################################
#
# CMake file for Python wrapper:_IO
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

# Define the sources in SireIO
set ( WRAPIO_SOURCES
      wrap_iobase.cpp
      wrap_param.cpp
      wrap_protoms.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireIO ${WRAPIO_SOURCES})

set_target_properties (PyWrap_SireIO PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireIO
                       SireIO
                       PyWrap_SireMM
                       PyWrap_SireMol
		       PyWrap_SireQt
                       ${BOOST_PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireIO
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireIO wrap_SireIO.cpp)

target_link_libraries (PythonWrapping_SireIO
                       PyWrap_SireIO
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireIO
                       PROPERTIES
                         OUTPUT_NAME "IO"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/IO )

install( TARGETS PythonWrapping_SireIO
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
