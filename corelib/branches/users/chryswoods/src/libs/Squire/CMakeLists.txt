########################################
#
# CMake file for library:Squire
#
########################################

# Third Party dependencies of this module
#include_directories(${QT_QTNETWORK_INCLUDE_DIR})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

set ( SQUIRE_HEADERS

      eff.h
      effcomponents.h
    )

set ( SQUIRE_SOURCES

      register_squire.cpp

      eff.cpp
      effcomponents.cpp

      ${SQUIRE_HEADERS}
    )

# Add option to disable compilation of the molpro forcefield
option ( SIRE_BUILD_MOLPRO
         "Build the Molpro forcefield (requires MolproClient)" OFF )

if (SIRE_BUILD_MOLPRO)

  #try to find the directory containing the 
  #molproclient.h header file
  FIND_PATH( MOLPRO_CLIENT_INCLUDE_DIR
                MolproClient/molproclient.h
             PATHS
                $ENV{MOLPROCLIENTDIR}/include
                $ENV{MOLPRODIR}/include
             DOC
                Path to the molproclient.h and molproclient_cpp.h include files.
           )
  
  if ( MOLPRO_CLIENT_INCLUDE_DIR )

    #try to find the MolproClient library...
    FIND_LIBRARY ( MOLPRO_CLIENT_C_LIBRARY
                     MolproClient
                   PATHS
                     ENV LD_LIBRARY_PATH
                 )
  
    if ( MOLPRO_CLIENT_C_LIBRARY )

      get_filename_component( MOLPRO_CLIENT_LIBRARY_PATH
                              ${MOLPRO_CLIENT_C_LIBRARY}
                              PATH CACHE )
    
      set ( MOLPRO_CLIENT_LIBRARIES 
            "-L${MOLPRO_CLIENT_LIBRARY_PATH} -lMolproClient -lMolproClient_cpp"
          )
                             
      set ( MOLPRO_FOUND "ON" )

      set ( MOLPRO_DEFINITIONS "-D_BUILD_MOLPRO_" )
 
    endif()

  endif()

  if (NOT MOLPRO_FOUND)
  
    message( STATUS "The MolproClient library was not found! "
                    "Compilation of the Molpro-based forcefields "
                    "has been disabled." )

  endif()

endif()

if (MOLPRO_FOUND)

  #add the Molpro forcefield include files
  set ( SQUIRE_HEADERS
   
        ${SQUIRE_HEADERS}

        molprocalculator.h
        molproff.h
        molproprocessor.h
        molprosession.h
      )

  #add the Molpro forcefield files
  set ( SQUIRE_SOURCES
  
        ${SQUIRE_SOURCES}
  
        molprocalculator.cpp
        molproff.cpp
        molproprocessor.cpp
        molprosession.cpp
      
        ${SQUIRE_HEADERS}
      )

  set ( SQUIRE_INCLUDE_DIRS 
             ${SQUIRE_INCLUDE_DIRS} 
             ${MOLPRO_CLIENT_INCLUDE_DIR} 
       )

  set ( SQUIRE_LIBRARIES
               ${SQUIRE_LIBRARIES}
               ${MOLPRO_CLIENT_LIBRARIES}
      )

  add_definitions( ${MOLPRO_DEFINITIONS} )
 
endif()

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
                       SireFF
                       SireMol
                       SireCAS
                       SireBase
                       SireStream
                       SireError
                       ${SQUIRE_LIBRARIES}
                      )

# installation
install( TARGETS Squire
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SQUIRE_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/Squire )

