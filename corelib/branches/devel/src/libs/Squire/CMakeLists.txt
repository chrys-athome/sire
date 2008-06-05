########################################
#
# CMake file for library:Squire
#
########################################

# Third Party dependencies of this module
include_directories(${QT_QTNETWORK_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

set ( BUILD_SQUIRE "OFF" )

if (MOLPRO_FOUND)
 
  #we definitely want to build squire
  set ( BUILD_SQUIRE "ON" )

  #add the Molpro forcefield files
  set ( SQUIRE_SOURCES
  
        ${SQUIRE_SOURCES}
  
        molprocalculator.cpp
        molproff.cpp
        molproprocessor.cpp
        molprosession.cpp
      )

  set ( SQUIRE_INCLUDE_DIRS 
               ${SQUIRE_INCLUDE_DIRS} 
               ${MOLPRO_CLIENT_INCLUDE_DIR} 
      )

  set ( SQUIRE_LIBRARIES
               ${SQUIRE_LIBRARIES}
               ${MOLPRO_CLIENT_LIBRARIES}
      )
 
endif (MOLPRO_FOUND)

if (BUILD_SQUIRE)

  #add the additional include files needed by Squire
  include_directories( ${SQUIRE_INCLUDE_DIRS} )
  
  add_library (Squire ${SQUIRE_SOURCES})

  set_target_properties (Squire PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         LINK_FLAGS ${SIRE_LINK_FLAGS}
                        )

  # Link to other Sire libraries
  target_link_libraries (Squire
                         SireMM
                         ${SQUIRE_LIBRARIES}
                         ${QT_QTNETWORK_LIBRARY}
                        )

  # installation
  install( TARGETS Squire
           RUNTIME DESTINATION ${SIRE_BIN}
           LIBRARY DESTINATION ${SIRE_LIBS}
           ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

endif (BUILD_SQUIRE)

