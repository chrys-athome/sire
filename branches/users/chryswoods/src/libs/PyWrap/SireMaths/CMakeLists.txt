########################################
#
# CMake file for Python wrapper:_Maths
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

# Define the sources in SireMaths
set ( WRAPMATHS_SOURCES

      _Maths.main.cpp

      siremaths_containers.cpp
      
      AxisSet.pypp.cpp
      
      Complex.pypp.cpp
      
      Line.pypp.cpp
      
      Matrix4.pypp.cpp
      Matrix.pypp.cpp
      
      Plane.pypp.cpp
      
      Quaternion.pypp.cpp
      
      RanGenerator.pypp.cpp
      
      Sphere.pypp.cpp
      
      Torsion.pypp.cpp
      Triangle.pypp.cpp
      
      Vector.pypp.cpp
      
      _Maths_free_functions.pypp.cpp
      _Maths_global_variables.pypp.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireMaths ${WRAPMATHS_SOURCES})

set_target_properties (PythonWrapping_SireMaths 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Maths"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      ) 
		       
target_link_libraries (PythonWrapping_SireMaths
                       SireMaths
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Maths )

install( TARGETS PythonWrapping_SireMaths
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
