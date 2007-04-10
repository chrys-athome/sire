########################################
#
# CMake file for Python wrapper:_FF
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

# Define the sources in SireFF
set ( WRAPFF_SOURCES

      _FF.main.cpp

      sireff_containers.cpp

      FFBase.pypp.cpp
      FFCalculator.pypp.cpp
      FFCalculatorBase.pypp.cpp
      FFComponent.pypp.cpp
      FFExpression.pypp.cpp
      FFGroupID.pypp.cpp
      FFProcessor.pypp.cpp
      FFProcessorBase.pypp.cpp
      FFThreadProcessor.pypp.cpp
      FFThreadWorker.pypp.cpp
      FFWorker.pypp.cpp
      FFWorkerBase.pypp.cpp

      ForceField.pypp.cpp
      ForceFieldID.pypp.cpp
      ForceFields.pypp.cpp
      ForceFieldsBase.pypp.cpp
      ForceTable.pypp.cpp

      ParameterMap.pypp.cpp
      ParameterName.pypp.cpp
      ParameterSource.pypp.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireFF ${WRAPFF_SOURCES})

set_target_properties (PythonWrapping_SireFF 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "FF"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

target_link_libraries (PythonWrapping_SireFF
                       SireFF
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/FF )

install( TARGETS PythonWrapping_SireFF
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
