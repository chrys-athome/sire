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
      atom.h
      atomaliases.h
      atomcutting.h
      atomicproperties.h
      atomid.h
      atomidcombo.hpp
      atomidcombos.h
      atomidentifier.h
      atomidx.h
      atominfo.h
      atominfogroup.h
      atomname.h
      atomnum.h
      atomselection.h
      atomselector.h
      calculateaxes.hpp
      cgatomidx.h
      cgid.h
      cgidentifier.h
      cgidx.h
      cgname.h
      cgnum.h
      cutgroup.h
      cuttingfunction.h
      element-data.h
      element.h
      elementdb.h
      errors.h
      molecule.h
      moleculebonds.h
      moleculecutting.h
      moleculedata.h
      moleculegroup.h
      moleculegroups.h
      moleculeinfo.h
      moleculeinfodata.h
      moleculeinfodataeditor.h
      moleculemover.h
      moleculeproperty.h
      molecules.h
      moleculeview.h
      moleculeview_inlines.h
      partialmolecule.h
      propertyextractor.h
      qhash_siremol.h
      resid.h
      residentifier.h
      residue.h
      residuebonds.h
      residuecutting.h
      residueinfo.h
      residx.h
      resname.h
      resnum.h
      segid.h
      segidentifier.h
      segidx.h
      segname.h
      segnum.h
      segment.h
      splitmolecule.h
      weightfunction.h
    )

# Define the sources in SireMol
set ( SIREMOL_SOURCES
      atom.cpp
      atomaliases.cpp
      atomicproperties.cpp
      atomidcombos.cpp
      atomidentifier.cpp
      atomcutting.cpp
      atominfo.cpp
      atominfogroup.cpp
      atomselection.cpp
      atomselector.cpp
      cgidentifier.cpp
      cutgroup.cpp
      cuttingfunction.cpp
      elementdb.cpp
      errors.cpp
      molecule.cpp
      moleculebonds.cpp
      moleculecutting.cpp
      moleculedata.cpp
      moleculegroup.cpp
      moleculegroups.cpp
      moleculeinfo.cpp
      moleculeinfodata.cpp
      moleculeinfodataeditor.cpp
      moleculemover.cpp
      moleculeproperty.cpp
      molecules.cpp
      moleculeview.cpp
      partialmolecule.cpp
      propertyextractor.cpp
      residentifier.cpp
      residue.cpp
      residuebonds.cpp
      residuecutting.cpp
      residueinfo.cpp
      segidentifier.cpp
      segment.cpp
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
