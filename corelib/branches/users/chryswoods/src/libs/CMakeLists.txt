###############################
#
# CMake file to compile the
# Sire libraries
#
###############################

# Add each library
add_subdirectory (SireBase)
add_subdirectory (SireCAS)
add_subdirectory (SireDB)
add_subdirectory (SireError)
add_subdirectory (SireFF)
add_subdirectory (SireID)
add_subdirectory (SireIO)
add_subdirectory (SireMaths)
add_subdirectory (SireMM)
#add_subdirectory (SireMMDB)
add_subdirectory (SireMol)
add_subdirectory (SireMove)
add_subdirectory (SireMPI)
add_subdirectory (SireStream)
add_subdirectory (SireSystem)
#add_subdirectory (SireTest)
add_subdirectory (SireUnits)
#add_subdirectory (SireUnitTest)
add_subdirectory (SireVol)
add_subdirectory (SqliteDriver)
#add_subdirectory (Squire)
add_subdirectory (ThirdParty)
add_subdirectory (ThirdPartyGUI)

set ( SIRE_HEADERS
      sireglobal.h
      tostring.h
    )
    
install( FILES ${SIRE_HEADERS}
         DESTINATION ${SIRE_INCLUDES} )

