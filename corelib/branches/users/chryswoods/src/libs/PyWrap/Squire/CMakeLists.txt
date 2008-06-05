########################################
#
# CMake file for Python wrapper:_Squire
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

set ( BUILD_SQUIRE_PYWRAPPERS "OFF" )

if (MOLPRO_FOUND)

  # we definitly want to build the wrappers
  set ( BUILD_SQUIRE_PYWRAPPERS "ON" )

  # Define the sources in Squire
  set ( WRAPSQUIRE_SOURCES

        ${WRAPSQUIRE_SOURCES}

        MolproCalculator.pypp.cpp
        MolproFF.pypp.cpp
        MolproProcessor.pypp.cpp
        MolproSession.pypp.cpp
      )
      
   set ( SQUIRE_PYWRAP_INCLUDE_DIRS 
               ${SQUIRE_PYWRAP_INCLUDE_DIRS}
               ${MOLPRO_CLIENT_INCLUDE_DIR}
       )

   set ( SQUIRE_PYWRAP_LIBRARIES
               ${SQUIRE_PYWRAP_LIBRARIES}
               ${MOLPRO_CLIENT_LIBRARIES}
       )

   add_definitions( ${MOLPRO_DEFINITIONS} )

endif (MOLPRO_FOUND)


if (BUILD_SQUIRE_PYWRAPPERS)

  include_directories( ${SQUIRE_PYWRAP_INCLUDE_DIRS} )

  # Define the sources in Squire
  set ( WRAPSQUIRE_SOURCES

        _Squire.main.cpp
        squire_containers.cpp
        
        ${WRAPSQUIRE_SOURCES}
      )

  # Create the library that holds all of the class wrappers
  add_library (PythonWrapping_Squire ${WRAPSQUIRE_SOURCES})

  set_target_properties (PythonWrapping_Squire
                         PROPERTIES
                           VERSION ${SIRE_VERSION}
                           SOVERSION ${SIRE_VERSION_MAJOR}
                           OUTPUT_NAME "Squire"
                           PREFIX "_"
                           SUFFIX ${PYTHON_MODULE_EXTENSION}
                        )

  target_link_libraries (PythonWrapping_Squire
                         Squire
                         ${SQUIRE_PYWRAP_LIBRARIES}
                         ${BOOST_PYTHON_LIBRARY}
                         ${PYTHON_LIBRARY}
                        )

  # installation
  set( INSTALLDIR ${SIRE_PYTHON}/Sire/Squire )

  install( TARGETS PythonWrapping_Squire
           LIBRARY DESTINATION ${INSTALLDIR}
         )

  install( FILES __init__.py
           DESTINATION ${INSTALLDIR}
         )

endif (BUILD_SQUIRE_PYWRAPPERS)
