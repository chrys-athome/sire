########################################
#
# CMake file for library:SireMol
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the sources in SireMol
set ( SIREMOL_SOURCES
      angle.cpp
      atom.cpp
      newatom.cpp
      atomicproperties.cpp
      atomcutting.cpp
      atomidgroup.cpp
      atomindex.cpp
      atominfo.cpp
      atominfogroup.cpp
      atomselection.cpp
      bond.cpp
      cgatomid.cpp
      cgnumatomid.cpp
      cutgroup.cpp
      cuttingfunction.cpp
      dihedral.cpp
      editmol.cpp
      editmoldata.cpp
      editres.cpp
      elementdb.cpp
      errors.cpp
      groupindexid.cpp
      idmolatom.cpp
      improper.cpp
      indexinfo.cpp
      molcgatomid.cpp
      molcutgroupid.cpp
      molecule.cpp
      moleculebonds.cpp
      moleculecutting.cpp
      moleculedata.cpp
      moleculegroup.cpp
      moleculegroups.cpp
      moleculeinfo.cpp
      moleculeproperty.cpp
      moleculeversion.cpp
      partialmolecule.cpp
      residatomid.cpp
      residue.cpp
      residuebonds.cpp
      residuecutting.cpp
      residueinfo.cpp
      resnumatomid.cpp
      resnumindexid.cpp
      splitmolecule.cpp
      weightfunction.cpp
    )

add_library (SireMol ${SIREMOL_SOURCES})

set_target_properties (SireMol PROPERTIES
                       VERSION ${SIRE_VERSION}
                       SOVERSION ${SIRE_VERSION_MAJOR}
                       LINK_FLAGS ${SIRE_LINK_FLAGS}
                      )

# Link to other Sire libraries, and GSL
target_link_libraries (SireMol
                       SireVol
                       )

# installation
install( TARGETS SireMol
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )
