// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "boost/python.hpp"
#include "siremol_headers.h"
#include "SireMol/moleculedata.h"
#include "SireMaths/angle.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/triangle.h"
#include "SireMaths/line.h"
#include "SireMaths/torsion.h"
#include "SireVol/space.h"
#include "_Residue__enums.pypp.hpp"
#include "_Residue__unnamed_enums.pypp.hpp"
#include "_Residue__classes.pypp.hpp"
#include "_Residue__memfuns0.pypp.hpp"
#include "_Residue__memfuns1.pypp.hpp"
#include "_Residue__memfuns2.pypp.hpp"
#include "_Residue__memfuns3.pypp.hpp"
#include "_Residue__memfuns4.pypp.hpp"
#include "_Residue__memfuns5.pypp.hpp"
#include "_Residue__memfuns_virtual.pypp.hpp"
#include "_Residue__memfuns_pvirtual.pypp.hpp"
#include "_Residue__protected_memfuns.pypp.hpp"

namespace bp = boost::python;

SireMol::Residue __copy__(const SireMol::Residue &other){ return SireMol::Residue(other); }

#include "SireQt/qdatastream.hpp"

const char* pvt_get_name(const SireMol::Residue&){ return "SireMol::Residue";}

void register_Residue_class(){

{ //::SireMol::Residue
    typedef bp::class_< SireMol::Residue, bp::bases< SireMol::MoleculeView > > Residue_exposer_t;
    Residue_exposer_t Residue_exposer = Residue_exposer_t( "Residue" );
    bp::scope Residue_scope( Residue_exposer );
    Residue_exposer.def( bp::init< >() );
    Residue_exposer.def( bp::init< SireMol::Molecule const &, SireMol::ResNum >(( bp::arg("mol"), bp::arg("resnum") )) );
    Residue_exposer.def( bp::init< SireMol::Molecule const &, SireMol::ResID >(( bp::arg("mol"), bp::arg("resid") )) );
    Residue_exposer.def( bp::init< SireMol::Molecule const &, QString const & >(( bp::arg("mol"), bp::arg("resname") )) );
    Residue_exposer.def( bp::init< SireMol::NewAtom const & >(( bp::arg("atom") )) );
    register_Residue_memfuns0(Residue_exposer);
    register_Residue_memfuns1(Residue_exposer);
    register_Residue_memfuns2(Residue_exposer);
    register_Residue_memfuns3(Residue_exposer);
    Residue_exposer.def( bp::self != bp::self );
    Residue_exposer.def( bp::self == bp::self );
    register_Residue_memfuns4(Residue_exposer);
    register_Residue_memfuns5(Residue_exposer);
    Residue_exposer.def( "__copy__", &__copy__);
    Residue_exposer.def( "__rlshift__", &SireQt::__rlshift__QDataStream< ::SireMol::Residue >,
                        bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
    Residue_exposer.def( "__rrshift__", &SireQt::__rrshift__QDataStream< ::SireMol::Residue >,
                        bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
    Residue_exposer.def( "__str__", &pvt_get_name);
}

}
