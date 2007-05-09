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

      Celsius.pypp.cpp      
      Charge.pypp.cpp
      
      DerivedUnit.pypp.cpp
      
      Energy.pypp.cpp
      
      Fahrenheit.pypp.cpp
      
      Kelvin.pypp.cpp
      
      Length.pypp.cpp
      
      Mass.pypp.cpp
      
      Quantity.pypp.cpp
      
      Temperature.pypp.cpp
      Time.pypp.cpp
      
      Unit.pypp.cpp
      
      _Units_free_functions.pypp.cpp
      _Units_global_variables.pypp.cpp
     
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
