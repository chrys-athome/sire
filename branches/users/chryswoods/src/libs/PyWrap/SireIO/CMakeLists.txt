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

      _IO.main.cpp
      
      IOBase.pypp.cpp
      PDB.pypp.cpp
      ProtoMS.pypp.cpp
      
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireIO ${WRAPIO_SOURCES})

set_target_properties (PythonWrapping_SireIO 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "IO"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )
		       
target_link_libraries (PythonWrapping_SireIO
                       SireIO
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/IO )

install( TARGETS PythonWrapping_SireIO
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
