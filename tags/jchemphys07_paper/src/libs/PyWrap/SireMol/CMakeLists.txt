########################################
#
# CMake file for Python wrapper:_Mol
#
########################################

# Third Party dependencies of this module
include_directories( ${BOOST_INCLUDE_DIR}
                     ${PYTHON_INCLUDE_DIR} )

# Qt4
include(${QT_USE_FILE})

# Other Sire libraries
include_directories(${CMAKE_SOURCE_DIR}/src/libs
                    ${CMAKE_SOURCE_DIR}/src/libs/PyWrap)

# Define the sources in SireMol
set ( WRAPMOL_SOURCES

      _Mol.main.cpp

      siremol_containers.cpp
      
      Angle.pypp.cpp
      Atom.pypp.cpp
      AtomCutting.pypp.cpp
      AtomID.pypp.cpp
      AtomIDGroup.pypp.cpp
      AtomIndex.pypp.cpp
      AtomInfo.pypp.cpp
      AtomInfoGroup.pypp.cpp
      AtomNum.pypp.cpp
      AtomSelection.pypp.cpp
      AtomSelector.pypp.cpp
      AtomicProperties.pypp.cpp
      
      Bond.pypp.cpp
      BondAddingFunction.pypp.cpp

      CGAtomID.pypp.cpp
      CGNumAtomID.pypp.cpp
      CutGroup.pypp.cpp
      CutGroupID.pypp.cpp
      CutGroupNum.pypp.cpp
      CuttingFunction.pypp.cpp
      CuttingFunctionBase.pypp.cpp
      
      Dihedral.pypp.cpp
      
      EditMol.pypp.cpp
      EditRes.pypp.cpp
      Element.pypp.cpp
      
      GroupID.pypp.cpp
      GroupIndexID.pypp.cpp

      IDBase.pypp.cpp
      IDMolAtom.pypp.cpp
      Improper.pypp.cpp
      Index.pypp.cpp
      
      MolCGAtomID.pypp.cpp
      MolCutGroupID.pypp.cpp
      MolDataView.pypp.cpp
      Molecule.pypp.cpp
      MoleculeBonds.pypp.cpp
      MoleculeCutting.pypp.cpp
      MoleculeGroup.pypp.cpp
      MoleculeGroupID.pypp.cpp
      MoleculeGroups.pypp.cpp
      MoleculeID.pypp.cpp
      MoleculeInfo.pypp.cpp
      MoleculeProperty.pypp.cpp
      MoleculeMover.pypp.cpp
      MoleculeView.pypp.cpp

      NewAtom.pypp.cpp

      PartialMolecule.pypp.cpp
      PropertyExtractor.pypp.cpp
     
      ResID.pypp.cpp
      ResIDAtomID.pypp.cpp
      ResNum.pypp.cpp
      ResNumAtomID.pypp.cpp
      ResNumIndexID.pypp.cpp
      Residue.pypp.cpp
      ResidueBonds.pypp.cpp
      ResidueCutting.pypp.cpp
      ResidueInfo.pypp.cpp

      SelectionFromMol.pypp.cpp

      WeightFunction.pypp.cpp
     
     )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireMol ${WRAPMOL_SOURCES})

set_target_properties (PythonWrapping_SireMol 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "Mol"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
           )

target_link_libraries (PythonWrapping_SireMol
                       SireMol
                       SireMaths
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
          )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/Mol )

install( TARGETS PythonWrapping_SireMol
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR}
       )
