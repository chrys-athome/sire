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
      atomatomid.h
      atomcoords.h
      atomcutting.h
      atomelements.h
      atomid.h
      atomidcombos.h
      atomidentifier.h
      atomidx.h
      atommasses.h
      atomname.h
      atomnum.h
      atomproperty.hpp
      atomselection.h
      atomsin.hpp
      calculateaxes.hpp
      cgatomidx.h
      cgid.h
      cgidentifier.h
      cgidx.h
      cgname.h
      cgproperty.hpp
      chain.h
      chainchainid.h
      chainid.h
      chainidentifier.h
      chainidx.h
      chainname.h
      chainproperty.hpp
      chainresid.h
      connectivity.h
      cutgroup.h
      cuttingfunction.h
      editor.h
      editor.hpp
      element-data.h
      element.h
      elementdb.h
      errors.h
      evaluator.h
      groupatomids.h
      groupgroupids.h
      molecule.h
      moleculecutting.h
      moleculedata.h
      moleculegroup.h
      moleculegroups.h
      moleculeinfodata.h
      moleculeinfodataeditor.h
      moleculemover.h
      molecules.h
      moleculeview.h
      molid.h
      molidentifier.h
      molidx.h
      molname.h
      molnum.h
      molviewproperty.h
      mover.h
      mover.hpp
      partialmolecule.h
      qhash_siremol.h
      resid.h
      residentifier.h
      residue.h
      residuecutting.h
      residuesin.hpp
      residx.h
      resin.hpp
      resname.h
      resnum.h
      resproperty.hpp
      resresid.h
      segid.h
      segidentifier.h
      segidx.h
      segname.h
      segment.h
      segproperty.hpp
      selector.hpp
      specify.hpp
      splitmolecule.h
      viewsofmol.h
      weightfunction.h
    )

# Define the sources in SireMol
set ( SIREMOL_SOURCES
      atom.cpp
      atomaliases.cpp
      atomatomid.cpp
      atomcoords.cpp
      atomid.cpp
      atomidentifier.cpp
      atomcutting.cpp
      atomselection.cpp
      cgid.cpp
      cgidentifier.cpp
      chain.cpp
      chainchainid.cpp
      chainid.cpp
      chainidentifier.cpp
      chainresid.cpp
      connectivity.cpp
      cutgroup.cpp
      cuttingfunction.cpp
      editor.cpp
      elementdb.cpp
      errors.cpp
      evaluator.cpp
      groupatomids.cpp
      molecule.cpp
      moleculecutting.cpp
      moleculedata.cpp
      moleculegroup.cpp
      moleculegroups.cpp
      moleculeinfodata.cpp
      moleculeinfodataeditor.cpp
      moleculemover.cpp
      molecules.cpp
      moleculeview.cpp
      molid.cpp
      molidentifier.cpp
      molviewproperty.cpp
      mover.cpp
      partialmolecule.cpp
      resid.cpp
      residentifier.cpp
      residue.cpp
      residuecutting.cpp
      resresid.cpp
      segid.cpp
      segidentifier.cpp
      segment.cpp
      splitmolecule.cpp
      viewsofmol.cpp
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
                       SireID
                       )

# installation
install( TARGETS SireMol
         RUNTIME DESTINATION ${SIRE_BIN}
         LIBRARY DESTINATION ${SIRE_LIBS}
         ARCHIVE DESTINATION ${SIRE_ARCHIVES}
       )

install( FILES ${SIREMOL_HEADERS}
         DESTINATION ${SIRE_INCLUDES}/SireMol )
