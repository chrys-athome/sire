########################################
#
# CMake file for Python wrapper:_Move
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

# Define the sources in SireMove
set ( WRAPMOVE_SOURCES

      _Move.main.cpp

      siremove_containers.cpp

      MapAsCutGroups.pypp.cpp
      MapAsMolecules.pypp.cpp
      MolMappingFunctionBase.pypp.cpp      
      MonteCarlo.pypp.cpp
      MTSMC.pypp.cpp

      PrefSampler.pypp.cpp
      
      RigidBodyMC.pypp.cpp
      
      SamplerBase.pypp.cpp
      Sampler.pypp.cpp
      
      UniformSampler.pypp.cpp
      UniformVolumeChange.pypp.cpp

      VolChangingFunctionBase.pypp.cpp
      VolumeMove.pypp.cpp

      WeightedMoves.pypp.cpp

     )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireMove ${WRAPMOVE_SOURCES})

set_target_properties (PythonWrapping_SireMove 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Move"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

target_link_libraries (PythonWrapping_SireMove
                       SireMove
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Move )

install( TARGETS PythonWrapping_SireMove
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )

