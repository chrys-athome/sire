########################################
#
# CMake file for Python wrapper:_Test
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

# Define the sources in SireTest
set ( WRAPTEST_SOURCES
      wrap_runtests.cpp
      wrap_testcast.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireTest ${WRAPTEST_SOURCES})

set_target_properties (PyWrap_SireTest PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireTest
                       SireTest
                       SireError
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireTest
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireTest wrap_SireTest.cpp)

target_link_libraries (PythonWrapping_SireTest
                       PyWrap_SireTest
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireTest
                       PROPERTIES
                         OUTPUT_NAME "Test"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Test )

install( TARGETS PythonWrapping_SireTest
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
