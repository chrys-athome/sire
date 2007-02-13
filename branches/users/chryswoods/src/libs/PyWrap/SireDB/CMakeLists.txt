########################################
#
# CMake file for Python wrapper:_DB
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

# Define the sources in SireDB
set ( WRAPDB_SOURCES

      _DB.main.cpp

      AssignBase.pypp.cpp
      AssignInstruction.pypp.cpp
      AtomDB.pypp.cpp
      AtomTable.pypp.cpp
      AtomType.pypp.cpp
      AtomTypeDB.pypp.cpp
      AtomTypeTable.pypp.cpp

      AtomTable_AtomType_.pypp.cpp

      DBBase.pypp.cpp

      FuncDB.pypp.cpp

      MatchAtom.pypp.cpp
      MatchAtomType.pypp.cpp
      MatchData.pypp.cpp
      MatchMR.pypp.cpp
      MatchMRA.pypp.cpp
      MatchMRData.pypp.cpp
      MatchMRAData.pypp.cpp
      MatchAtomTypeData.pypp.cpp
      MatchMol.pypp.cpp
      MatchRes.pypp.cpp

      NameDB.pypp.cpp

      OverWriteParams.pypp.cpp

      ParameterDB.pypp.cpp
      ParameterTable.pypp.cpp

      RelateAtomTypeDB.pypp.cpp
      RelateDB.pypp.cpp
      RelateMRADB.pypp.cpp
      RelateMRDB.pypp.cpp
      RelationshipDB.pypp.cpp

      Smarts.pypp.cpp

      TableBase.pypp.cpp
      Term12DB.pypp.cpp
      Term13DB.pypp.cpp
      Term14DB.pypp.cpp

      assign_atoms.pypp.cpp
      assign_parameters.pypp.cpp

      match_atom_element.pypp.cpp
      match_atom_name.pypp.cpp
      match_atom_num.pypp.cpp
      match_atom_type.pypp.cpp
      match_mol_group.pypp.cpp
      match_mol_name.pypp.cpp
      match_mol_state.pypp.cpp
      match_no_res_res_bond_via.pypp.cpp
      match_res_alias.pypp.cpp
      match_res_name.pypp.cpp
      match_res_num.pypp.cpp
      match_res_res_bond_via.pypp.cpp
      match_single_atom_element.pypp.cpp
      match_single_atom_name.pypp.cpp
      match_single_atom_num.pypp.cpp
      match_single_atom_type.pypp.cpp
      match_single_no_res_res_bond_via.pypp.cpp
      match_single_res_alias.pypp.cpp
      match_single_res_name.pypp.cpp
      match_single_res_num.pypp.cpp
      match_single_res_res_bond_via.pypp.cpp

      using_database.pypp.cpp
      using_parameters_base.pypp.cpp
      using_relationships_base.pypp.cpp
    )

# Create the library that holds all of the class wrappers
add_library (PythonWrapping_SireDB ${WRAPDB_SOURCES})

set_target_properties (PythonWrapping_SireDB 
                       PROPERTIES
                         VERSION ${SIRE_VERSION}
                         SOVERSION ${SIRE_VERSION_MAJOR}
                         OUTPUT_NAME "DB"
                         PREFIX "_"
                         SUFFIX ${PYTHON_MODULE_EXTENSION}
                      )
		       
target_link_libraries (PythonWrapping_SireDB
                       SireDB
                       SireMol
                       ${BOOST_PYTHON_LIBRARY}
                       ${PYTHON_LIBRARY}
		      )

# installation
set( INSTALLDIR ${SIRE_PYTHON}/Sire/DB )

install( TARGETS PythonWrapping_SireDB
         LIBRARY DESTINATION ${INSTALLDIR}
       )

install( FILES __init__.py
         DESTINATION ${INSTALLDIR} 
       )
