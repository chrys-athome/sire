########################################
#
# CMake file for Python wrapper:_MM
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

# Define the sources in SireMM
set ( WRAPMM_SOURCES

      _MM.main.cpp

      siremm_containers.cpp

      AngleTable_Expression_.pypp.cpp
      BondTable_Expression_.pypp.cpp
      DihedralTable_Expression_.pypp.cpp

      assign_angles.pypp.cpp
      assign_bonds.pypp.cpp
      assign_dihedrals.pypp.cpp
      
      assign_internals_MolAngleInfo_.pypp.cpp
      assign_internals_MolBondInfo_.pypp.cpp
      assign_internals_MolDihedralInfo_.pypp.cpp

      AngleAtomID.pypp.cpp
      AngleDB.pypp.cpp
      AngleGenerator.pypp.cpp
      AngleResID.pypp.cpp
      AngleTable.pypp.cpp
      ArithmeticCombiningRules.pypp.cpp
      AtomicCharges.pypp.cpp
      AtomicLJs.pypp.cpp

      AtomTable_ChargeParameter_.pypp.cpp
      AtomTable_LJParameter_.pypp.cpp

      BondAtomID.pypp.cpp
      BondDB.pypp.cpp
      BondGenerator.pypp.cpp
      BondResID.pypp.cpp
      BondTable.pypp.cpp

      CLJFF.pypp.cpp
      CLJPair.pypp.cpp
      CLJParameter.pypp.cpp
      ChargeDB.pypp.cpp
      ChargeParameter.pypp.cpp
      ChargeTable.pypp.cpp
      CombiningRuleBase.pypp.cpp
      CombiningRules.pypp.cpp
      CoulombFF.pypp.cpp

      DihedralAtomID.pypp.cpp
      DihedralDB.pypp.cpp
      DihedralGenerator.pypp.cpp
      DihedralResID.pypp.cpp
      DihedralTable.pypp.cpp

      GeometricCombiningRules.pypp.cpp

      HarmonicSwitchingFunction.pypp.cpp

      InterCLJFF.pypp.cpp
      InterCoulombFF.pypp.cpp
      InterGroupCLJFF.pypp.cpp
      InterGroupCoulombFF.pypp.cpp
      InterGroupLJFF.pypp.cpp
      InterLJFF.pypp.cpp

      InternalDB_MolAngleInfo_.pypp.cpp
      InternalDB_MolBondInfo_.pypp.cpp
      InternalDB_MolDihedralInfo_.pypp.cpp
      InternalGenerator_MolAngleInfo_.pypp.cpp
      InternalGenerator_MolBondInfo_.pypp.cpp
      InternalGenerator_MolDihedralInfo_.pypp.cpp
      
      InternalInfo_Angle_.pypp.cpp
      InternalInfo_Bond_.pypp.cpp
      InternalInfo_Dihedral_.pypp.cpp
      
      InternalTableBase_MolAngleInfo_.pypp.cpp
      InternalTableBase_MolBondInfo_.pypp.cpp
      InternalTableBase_MolDihedralInfo_.pypp.cpp
      InternalTable_MolAngleInfo_Expression_.pypp.cpp
      InternalTable_MolBondInfo_Expression_.pypp.cpp
      InternalTable_MolDihedralInfo_Expression_.pypp.cpp
            
      LJDB.pypp.cpp
      LJFF.pypp.cpp
      LJPair.pypp.cpp
      LJParameter.pypp.cpp
      LJTable.pypp.cpp

      MolAngleInfo.pypp.cpp
      MolBondInfo.pypp.cpp
      MolDihedralInfo.pypp.cpp
      
      MolInternalInfo_Angle_.pypp.cpp
      MolInternalInfo_Bond_.pypp.cpp
      MolInternalInfo_Dihedral_.pypp.cpp

      NoCutoff.pypp.cpp

      ResAngleInfo.pypp.cpp
      ResBondInfo.pypp.cpp
      ResDihedralInfo.pypp.cpp

      ResInternalInfo_Angle_.pypp.cpp
      ResInternalInfo_Bond_.pypp.cpp
      ResInternalInfo_Dihedral_.pypp.cpp

      SwitchFuncBase.pypp.cpp
      SwitchingFunction.pypp.cpp

      UreyBradleyDB.pypp.cpp

      UsePassedInternals_MolAngleInfo_.pypp.cpp
      UsePassedInternals_MolBondInfo_.pypp.cpp
      UsePassedInternals_MolDihedralInfo_.pypp.cpp

      UsePassedAngles.pypp.cpp
      UsePassedBonds.pypp.cpp
      UsePassedDihedrals.pypp.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireMM ${WRAPMM_SOURCES})

set_target_properties (PythonWrapping_SireMM 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "MM"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )
		       
target_link_libraries (PythonWrapping_SireMM
                       SireMM
                       SireDB
                       SireFF
                       SireMol
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
                      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/MM )

install( TARGETS PythonWrapping_SireMM
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
