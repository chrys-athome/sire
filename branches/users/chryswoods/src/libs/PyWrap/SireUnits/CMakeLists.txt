########################################
#
# CMake file for Python wrapper:_Units
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

# Define the sources in SireUnits
set ( WRAPUNITS_SOURCES

      _Units.main.cpp

      Unit.pypp.cpp
      
      Celsius.pypp.cpp
      Fahrenheit.pypp.cpp
      TempBase.pypp.cpp
      
      Acceleration.pyman.cpp
      Area.pyman.cpp
      
      Charge.pyman.cpp
      
      Density.pyman.cpp
      Dimensionless.pyman.cpp
      
      ElecCurrent.pyman.cpp
      ElecPotential.pyman.cpp
      Energy.pyman.cpp
      
      Force.pyman.cpp
      
      Length.pyman.cpp
      
      Mass.pyman.cpp
      MolarCharge.pyman.cpp
      MolarDensity.pyman.cpp
      MolarEnergy.pyman.cpp
      MolarMass.pyman.cpp
      MolarPower.pyman.cpp
      MolarVolume.pyman.cpp
      
      Power.pyman.cpp
      Pressure.pyman.cpp
      
      Quantity.pyman.cpp
      
      Temperature.pyman.cpp
      Time.pyman.cpp
      
      Velocity.pyman.cpp
      Volume.pyman.cpp
      
      _Units_free_functions.pypp.cpp
      _Units_global_variables.pypp.cpp
      
      _Units_global_variables.pyman.cpp
     
     )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireUnits ${WRAPUNITS_SOURCES})

set_target_properties (PythonWrapping_SireUnits
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Units"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
           )

target_link_libraries (PythonWrapping_SireUnits
                       SireUnits
                       SireMaths
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
          )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Units )

install( TARGETS PythonWrapping_SireUnits
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
