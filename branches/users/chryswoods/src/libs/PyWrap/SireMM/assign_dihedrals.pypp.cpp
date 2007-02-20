// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "assign_dihedrals.pypp.hpp"
#include "boost/python.hpp"
#include "siremm_headers.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/newatom.h"
#include "SireMol/atom.h"

namespace bp = boost::python;

#include "SireDB/using_database.h"

using namespace SireMM;

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMM::assign_dihedrals&){ return "SireMM::assign_dihedrals";}

void register_assign_dihedrals_class(){

    bp::class_< SireMM::assign_dihedrals, bp::bases< SireMM::assign_internals<SireMM::MolDihedralInfo> > >( "assign_dihedrals", bp::init< bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("db7"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("db7"), bp::arg("db8"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("db7"), bp::arg("db8"), bp::arg("db9"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("db7"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("db7"), bp::arg("db8"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( bp::init< QSet<SireMol::Dihedral> const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, SireDB::using_database const &, bp::optional< SireMM::DihedralGeneratorBase const & > >(( bp::arg("dihedrals"), bp::arg("db0"), bp::arg("db1"), bp::arg("db2"), bp::arg("db3"), bp::arg("db4"), bp::arg("db5"), bp::arg("db6"), bp::arg("db7"), bp::arg("db8"), bp::arg("db9"), bp::arg("generator")=DihedralGenerator() )) )    
        .def( 
            "assignParameters"
            , (void ( ::SireMM::assign_dihedrals::* )( ::SireMol::Molecule const &,::SireDB::ParameterTable &,::SireDB::ParameterDB &,::SireDB::MatchMRData const & ) const)( &::SireMM::assign_dihedrals::assignParameters )
            , ( bp::arg("molecule"), bp::arg("param_table"), bp::arg("database"), bp::arg("matchmr")=::SireDB::MatchMRData( ) ) )    
        .def( 
            "typeName"
            , (char const * (*)(  ))( &::SireMM::assign_dihedrals::typeName ) )    
        .def( 
            "what"
            , (char const * ( ::SireMM::assign_dihedrals::* )(  ) const)( &::SireMM::assign_dihedrals::what ) )    
        .staticmethod( "typeName" )    
        .def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMM::assign_dihedrals >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMM::assign_dihedrals >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() )    
        .def( "__str__", &pvt_get_name);

}
