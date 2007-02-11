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

      QSet_AtomIndex_.py.cpp
      
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
      Molecule.pypp.cpp
      MoleculeBonds.pypp.cpp
      MoleculeCutting.pypp.cpp
      MoleculeGroup.pypp.cpp
      MoleculeGroupID.pypp.cpp
      MoleculeGroups.pypp.cpp
      MoleculeID.pypp.cpp
      MoleculeInfo.pypp.cpp
      MoleculeVersion.pypp.cpp

      NewAtom.pypp.cpp

      Property.pypp.cpp
      PropertyBase.pypp.cpp
      
      ResID.pypp.cpp
      ResIDAtomID.pypp.cpp
      ResNum.pypp.cpp
      ResNumAtomID.pypp.cpp
      ResNumIndexID.pypp.cpp
      Residue.pypp.cpp
      ResidueBonds.pypp.cpp
      ResidueCutting.pypp.cpp
      ResidueInfo.pypp.cpp

      VariantProperty.pypp.cpp
      WeightFunction.pypp.cpp

      _EditMol__classes.pypp.cpp
      _EditMol__enums.pypp.cpp
      _EditMol__memfuns0.pypp.cpp
      _EditMol__memfuns1.pypp.cpp
      _EditMol__memfuns10.pypp.cpp
      _EditMol__memfuns11.pypp.cpp
      _EditMol__memfuns12.pypp.cpp
      _EditMol__memfuns13.pypp.cpp
      _EditMol__memfuns14.pypp.cpp
      _EditMol__memfuns2.pypp.cpp
      _EditMol__memfuns3.pypp.cpp
      _EditMol__memfuns4.pypp.cpp
      _EditMol__memfuns5.pypp.cpp
      _EditMol__memfuns6.pypp.cpp
      _EditMol__memfuns7.pypp.cpp
      _EditMol__memfuns8.pypp.cpp
      _EditMol__memfuns9.pypp.cpp
      _EditMol__memfuns_pvirtual.pypp.cpp
      _EditMol__memfuns_virtual.pypp.cpp
      _EditMol__protected_memfuns.pypp.cpp
      _EditMol__unnamed_enums.pypp.cpp

      _EditRes__classes.pypp.cpp
      _EditRes__enums.pypp.cpp
      _EditRes__memfuns0.pypp.cpp
      _EditRes__memfuns1.pypp.cpp
      _EditRes__memfuns2.pypp.cpp
      _EditRes__memfuns3.pypp.cpp
      _EditRes__memfuns4.pypp.cpp
      _EditRes__memfuns5.pypp.cpp
      _EditRes__memfuns6.pypp.cpp
      _EditRes__memfuns_pvirtual.pypp.cpp
      _EditRes__memfuns_virtual.pypp.cpp
      _EditRes__protected_memfuns.pypp.cpp
      _EditRes__unnamed_enums.pypp.cpp

      _Molecule__classes.pypp.cpp
      _Molecule__enums.pypp.cpp
      _Molecule__memfuns0.pypp.cpp
      _Molecule__memfuns1.pypp.cpp
      _Molecule__memfuns2.pypp.cpp
      _Molecule__memfuns3.pypp.cpp
      _Molecule__memfuns4.pypp.cpp
      _Molecule__memfuns5.pypp.cpp
      _Molecule__memfuns6.pypp.cpp
      _Molecule__memfuns7.pypp.cpp
      _Molecule__memfuns8.pypp.cpp
      _Molecule__memfuns9.pypp.cpp
      _Molecule__memfuns_pvirtual.pypp.cpp
      _Molecule__memfuns_virtual.pypp.cpp
      _Molecule__protected_memfuns.pypp.cpp
      _Molecule__unnamed_enums.pypp.cpp

      _NewAtom__classes.pypp.cpp
      _NewAtom__enums.pypp.cpp
      _NewAtom__memfuns.pypp.cpp
      _NewAtom__memfuns_pvirtual.pypp.cpp
      _NewAtom__memfuns_virtual.pypp.cpp
      _NewAtom__protected_memfuns.pypp.cpp
      _NewAtom__unnamed_enums.pypp.cpp

      _Residue__classes.pypp.cpp
      _Residue__enums.pypp.cpp
      _Residue__memfuns0.pypp.cpp
      _Residue__memfuns1.pypp.cpp
      _Residue__memfuns2.pypp.cpp
      _Residue__memfuns3.pypp.cpp
      _Residue__memfuns4.pypp.cpp
      _Residue__memfuns5.pypp.cpp
      _Residue__memfuns_pvirtual.pypp.cpp
      _Residue__memfuns_virtual.pypp.cpp
      _Residue__protected_memfuns.pypp.cpp
      _Residue__unnamed_enums.pypp.cpp
     
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
