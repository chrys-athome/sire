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

      wrap_SireTest.cpp

      wrap_runtests.cpp
      wrap_testcast.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireTest ${WRAPTEST_SOURCES})

set_target_properties (PythonWrapping_SireTest 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME Test
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
		       )
		       
target_link_libraries (PythonWrapping_SireTest
                       SireTest
                       SireError
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
		      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Test )

install( TARGETS PythonWrapping_SireTest
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
