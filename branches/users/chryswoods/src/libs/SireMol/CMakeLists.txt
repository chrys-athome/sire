########################################
#
# CMake file for library:SireMol
#
########################################

# Third Party dependencies of this module

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs)

# Define the headers in SireMol
set ( SIREMOL_HEADERS
      angle.h
      angleset.h
      atom.h
      atomcutting.h
      atomicproperties.h
      atomid.h
      atomidgroup.h
      atomindex.h
      atominfo.h
      atominfogroup.h
      atomnum.h
      atomselection.h
      atomselector.h
      bond.h
      bondaddingfunction.h
      calculateaxes.hpp
      cgatomid.h
      cgnumatomid.h
      cutgroup.h
      cutgroupid.h
      cutgroupnum.h
      cuttingfunction.h
      dihedral.h
      editmol.h
      editmoldata.h
      editres.h
      element-data.h
      element.h
      elementdb.h
      errors.h
      groupedvector.hpp
      groupid.h
      groupindexid.h
      id.h
      idmolatom.h
      idtypes.h
      improper.h
      index.h
      indexinfo.h
      molcgatomid.h
      molcutgroupid.h
      molecule.h
      moleculebonds.h
      moleculecutting.h
      moleculedata.h
      moleculegroup.h
      moleculegroupid.h
      moleculegroups.h
      moleculeid.h
      moleculeinfo.h
      moleculemover.h
      moleculeproperty.h
      molecules.h
      moleculeview.h
      moleculeview_inlines.h
      newatom.h
      partialmolecule.h
      propertyextractor.h
      qhash_siremol.h
      resid.h
      residatomid.h
      residue.h
      residuebonds.h
      residuecutting.h
      residueinfo.h
      resnum.h
      resnumatomid.h
      resnumindexid.h
      selectionfrommol.h
      splitmolecule.h
      templatefunction.h
      weightfunction.h
    )

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
      atomselector.cpp
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
      moleculemover.cpp
      moleculeproperty.cpp
      molecules.cpp
      moleculeview.cpp
      partialmolecule.cpp
      propertyextractor.cpp
      residatomid.cpp
      residue.cpp
      residuebonds.cpp
      residuecutting.cpp
      residueinfo.cpp
      resnumatomid.cpp
      resnumindexid.cpp
      selectionfrommol.cpp
      splitmolecule.cpp
      weightfunction.cpp
    
      ${SIREMOL_HEADERS}
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

install( FILES ${SIREMOL_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMol )
