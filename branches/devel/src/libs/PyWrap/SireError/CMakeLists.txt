########################################
#
# CMake file for Python wrapper:_Error
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

# Define the sources in SireError
set ( WRAPERROR_SOURCES

      wrap_SireError.cpp

      wrap_exceptions.cpp

    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireError ${WRAPERROR_SOURCES})

set_target_properties (PythonWrapping_SireError 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
		             SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Error"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
		          )
		       
target_link_libraries (PythonWrapping_SireError
                       SireError
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Error )

install( TARGETS PythonWrapping_SireError
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
