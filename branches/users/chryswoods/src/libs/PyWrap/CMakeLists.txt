###############################
#
# CMake file to compile the
# Sire Python wrappers
#
###############################

# Switch over to using size-optimised compilation
# as the python wrapper libraries are huge, and not
# time-critical

set( CMAKE_C_FLAGS ${SIRE_C_FLAGS_SMALL} )
set( CMAKE_CXX_FLAGS ${SIRE_CXX_FLAGS_SMALL} )

# Add each library
add_subdirectory (SireBase)
add_subdirectory (SireCAS)
add_subdirectory (SireCluster)
add_subdirectory (SireDB)
add_subdirectory (SireError)
#add_subdirectory (SireFF)
#add_subdirectory (SireIO)
add_subdirectory (SireMaths)
#add_subdirectory (SireMM)
add_subdirectory (SireMol)
#add_subdirectory (SireMove)
#add_subdirectory (SirePy)
add_subdirectory (SireQt)
#add_subdirectory (SireSim)
#add_subdirectory (SireStream)
add_subdirectory (SireSystem)
#add_subdirectory (SireTest)
#add_subdirectory (SireUnits)
#add_subdirectory (SireUnitTest)
#add_subdirectory (SireVol)
#add_subdirectory (Spier)
#add_subdirectory (Squire)
#add_subdirectory (ThirdParty)

# Install main Sire __init__.py file
set ( INSTALLDIR ${SIRE_PYTHON}/Sire )

install ( FILES __init__.py
          DESTINATION ${INSTALLDIR}
        )

