########################################
#
# CMake file for Python wrapper:_Py
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
set ( WRAPPY_SOURCES

      _Py.main.cpp

      sirepy_containers.cpp
    
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SirePy ${WRAPPY_SOURCES})

set_target_properties (PythonWrapping_SirePy
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Py"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
           )

target_link_libraries (PythonWrapping_SirePy
                       SireStream
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
          )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Py )

install( TARGETS PythonWrapping_SirePy
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
