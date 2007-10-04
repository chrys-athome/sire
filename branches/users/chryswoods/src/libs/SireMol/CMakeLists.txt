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
      atomcutting.h
      atomicproperties.h
      atomid.h
      atomidcombos.h
      atomidentifier.h
      atomidx.h
      atominfo.h
      atominfogroup.h
      atomname.h
      atomnum.h
      atomselection.h
      atomselector.h
      atomsin.hpp
      atomsinmol.h
      calculateaxes.hpp
      cgatomidx.h
      cgid.h
      cgidentifier.h
      cgidx.h
      cginfo.h
      cginmol.h
      cgname.h
      cginmol.h
      chain.h
      chainchainid.h
      chainid.h
      chainidentifier.h
      chainidx.h
      chaininfo.h
      chainname.h
      chainresid.h
      chainsinmol.h
      connectivity.h
      cutgroup.h
      cuttingfunction.h
      editor.h
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
      moleculeinfo.h
      moleculeinfodata.h
      moleculeinfodataeditor.h
      moleculemover.h
      moleculeproperty.h
      molecules.h
      moleculeview.h
      moleculeview_inlines.h
      molid.h
      molidentifier.h
      molidx.h
      molname.h
      molnum.h
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
      resinmol.h
      resinfo.h
      resname.h
      resnum.h
      resresid.h
      segid.h
      segidentifier.h
      segidx.h
      seginfo.h
      seginmol.h
      segname.h
      segment.h
      specify.hpp
      splitmolecule.h
      weightfunction.h
    )

# Define the sources in SireMol
set ( SIREMOL_SOURCES
      atom.cpp
      atomaliases.cpp
      atomatomid.cpp
      atomicproperties.cpp
      atomid.cpp
      atomidentifier.cpp
      atomcutting.cpp
      atominfo.cpp
      atomselection.cpp
      atomselector.cpp
      atomsinmol.cpp
      cgidentifier.cpp
      cginfo.cpp
      cginmol.cpp 
      chain.cpp
      chainchainid.cpp
      chaininfo.cpp
      chainresid.cpp
      chainsinmol.cpp
      connectivity.cpp
      cutgroup.cpp
      cuttingfunction.cpp
      editor.cpp
      elementdb.cpp
      errors.cpp
      evaluator.cpp
      molecule.cpp
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
      molidentifier.cpp
      mover.cpp
      partialmolecule.cpp
      residentifier.cpp
      residue.cpp
      residuecutting.cpp
      resinfo.cpp
      resinmol.cpp
      resresid.cpp
      segidentifier.cpp
      seginfo.cpp
      seginmol.cpp
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
