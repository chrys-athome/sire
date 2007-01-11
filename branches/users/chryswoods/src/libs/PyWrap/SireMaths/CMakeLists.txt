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

      rangenerator.py.cpp

      wrap_angle.cpp
      wrap_axisset.cpp
      wrap_complex.cpp
      wrap_geometry.cpp
      wrap_maths.cpp
      wrap_rational.cpp
      wrap_vector.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PyWrap_SireMaths ${WRAPMATHS_SOURCES})

set_target_properties (PyWrap_SireMaths PROPERTIES
                       VERSION ${SIRE_VERSION}
		       SOVERSION ${SIRE_VERSION_MAJOR}
		       )
		       
target_link_libraries (PyWrap_SireMaths
                       SireMaths
		       PyWrap_SireError
                       PyWrap_SireQt
		       ${BOOST_PYTHON_LIBRARY}
		      )

# Install the wrappers library with the other Sire libraries
install( TARGETS PyWrap_SireMaths
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

# Now create the thin-wrapper python module
add_library (PythonWrapping_SireMaths wrap_SireMaths.cpp)

target_link_libraries (PythonWrapping_SireMaths
                       PyWrap_SireMaths
                       )

# Use properties to change the filename of the python module
set_target_properties (PythonWrapping_SireMaths
                       PROPERTIES
                         OUTPUT_NAME "Maths"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Maths )

install( TARGETS PythonWrapping_SireMaths
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
