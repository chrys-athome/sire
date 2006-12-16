########################################
#
# CMake file for Python wrapper:_Base
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

# Define the sources in SireBase
set ( WRAPBASE_SOURCES
      idversion.py.cpp
      wrap_md5sum.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireBase ${WRAPBASE_SOURCES})

set_target_properties (PyWrap_SireBase PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireBase
                       SireBase
                       PyWrap_SireQt
                       ${BOOST_PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireBase
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireBase wrap_SireBase.cpp)

target_link_libraries (PythonWrapping_SireBase
                       PyWrap_SireBase
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireBase
                       PROPERTIES
                         OUTPUT_NAME "Base"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Base )

install( TARGETS PythonWrapping_SireBase
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
