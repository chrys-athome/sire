###############################
#
# CMake file to compile the
# Sire libraries
#
###############################

# List of each subdirectory
set ( SUBDIRS 
           #   SireBase
           #   SireCAS
           #   SireCluster
           #   SireDB
           #   SireFF
              SireID
           #   SireIO
           #   SireMaths
           #   SireMM
           #   SireMol
           #   SireMove
           #   SireSystem
              SireUnits
           #   SireVol
              Siren
           #   Spier
           #   Squire
            )

foreach (DIR ${SUBDIRS})
    add_subdirectory( ${DIR} )
endforeach (DIR)

set ( SIRE_HEADERS
      sireglobal.h
    )
    
install( FILES ${SIRE_HEADERS}
         DESTINATION ${SIRE_INCLUDES} )

install( EXPORT SireLibraries 
         NAMESPACE SIRE_
         DESTINATION ${SIRE_CMAKEFILES} )
