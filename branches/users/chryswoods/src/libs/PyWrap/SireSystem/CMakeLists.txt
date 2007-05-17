########################################
#
# CMake file for Python wrapper:_System
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
set ( WRAPSYSTEM_SOURCES

      _System.main.cpp

      siresystem_containers.cpp

      CheckPoint.pypp.cpp

      LocalSimSystem.pypp.cpp

      MonitorEnergy.pypp.cpp
      MoveBase.pypp.cpp
      Move.pypp.cpp
      MovesBase.pypp.cpp
      Moves.pypp.cpp

      QuerySystem.pypp.cpp

      SameMoves.pypp.cpp
      SimSystem.pypp.cpp
      SystemData.pypp.cpp
      SystemID.pypp.cpp
      System.pypp.cpp
      SystemMonitorBase.pypp.cpp
      SystemMonitors.pypp.cpp
    
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireSystem ${WRAPSYSTEM_SOURCES})

set_target_properties (PythonWrapping_SireSystem 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "System"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
           )

target_link_libraries (PythonWrapping_SireSystem
                       SireSystem
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
          )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/System )

install( TARGETS PythonWrapping_SireSystem
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
