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
      
      _Vol.main.cpp
      
      AABox.pypp.cpp
      
      Cartesian.pypp.cpp
      CoordGroupBase.pypp.cpp
      CoordGroupEditor.pypp.cpp
      CoordGroup.pypp.cpp
      
      PeriodicBox.pypp.cpp
      SpaceBase.pypp.cpp
      Space.pypp.cpp
      
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireVol ${WRAPVOL_SOURCES})

set_target_properties (PythonWrapping_SireVol 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Vol"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )
		       
target_link_libraries (PythonWrapping_SireVol
                       SireVol
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Vol )

install( TARGETS PythonWrapping_SireVol
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
