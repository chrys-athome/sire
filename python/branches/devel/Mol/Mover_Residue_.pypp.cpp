// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Mover_Residue_.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/errors.h"

#include "SireError/errors.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atom.h"

#include "atomidcombos.h"

#include "chain.h"

#include "evaluator.h"

#include "molecule.h"

#include "mover.hpp"

#include "mover_metaid.h"

#include "reseditor.h"

#include "residue.h"

#include "selector.hpp"

#include "residue.h"

#include "SireMaths/align.h"

#include "SireMaths/axisset.h"

#include "SireMaths/matrix.h"

#include "SireMaths/quaternion.h"

#include "SireMaths/rotate.h"

#include "SireMaths/vectorproperty.h"

#include "SireMol/errors.h"

#include "SireUnits/units.h"

#include "SireVol/coordgroup.h"

#include "SireVol/space.h"

#include "angleid.h"

#include "atomcoords.h"

#include "atommatcher.h"

#include "bondid.h"

#include "connectivity.h"

#include "dihedralid.h"

#include "improperid.h"

#include "mover.h"

#include "tostring.h"

#include "weightfunction.h"

#include "mover.h"

SireMol::Mover<SireMol::Residue> __copy__(const SireMol::Mover<SireMol::Residue> &other){ return SireMol::Mover<SireMol::Residue>(other); }

#include "Helpers/str.hpp"

void register_Mover_Residue__class(){

    { //::SireMol::Mover< SireMol::Residue >
        typedef bp::class_< SireMol::Mover< SireMol::Residue >, bp::bases< SireMol::MoverBase, SireMol::Residue, SireMol::MoleculeView, SireBase::Property > > Mover_Residue__exposer_t;
        Mover_Residue__exposer_t Mover_Residue__exposer = Mover_Residue__exposer_t( "Mover_Residue_", bp::init< >() );
        bp::scope Mover_Residue__scope( Mover_Residue__exposer );
        Mover_Residue__exposer.def( bp::init< SireMol::Residue const & >(( bp::arg("view") )) );
        Mover_Residue__exposer.def( bp::init< SireMol::Residue const &, SireMol::AtomSelection const & >(( bp::arg("view"), bp::arg("movable_atoms") )) );
        Mover_Residue__exposer.def( bp::init< SireMol::Mover< SireMol::Residue > const & >(( bp::arg("other") )) );
        { //::SireMol::Mover< SireMol::Residue >::align
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Residue >::align );
            
            Mover_Residue__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::align
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Residue >::align );
            
            Mover_Residue__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::align
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Residue >::align );
            
            Mover_Residue__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::align
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Residue >::align );
            
            Mover_Residue__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::alignTo
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Residue >::alignTo );
            
            Mover_Residue__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::alignTo
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Residue >::alignTo );
            
            Mover_Residue__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::alignTo
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Residue >::alignTo );
            
            Mover_Residue__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::alignTo
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Residue >::alignTo );
            
            Mover_Residue__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::change
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Residue >::change );
            
            Mover_Residue__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::change
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*change_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Residue >::change );
            
            Mover_Residue__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("angle"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::change
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*change_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Residue >::change );
            
            Mover_Residue__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::change
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Residue >::change );
            
            Mover_Residue__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::change
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*change_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Residue >::change );
            
            Mover_Residue__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("improper"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::changeFrame
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*changeFrame_function_type )( ::SireMaths::AxisSet const &,::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            changeFrame_function_type changeFrame_function_value( &::SireMol::Mover< SireMol::Residue >::changeFrame );
            
            Mover_Residue__exposer.def( 
                "changeFrame"
                , changeFrame_function_value
                , ( bp::arg("from_frame"), bp::arg("to_frame"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::commit
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Residue ( ::SireMol::Mover< SireMol::Residue >::*commit_function_type )(  ) const;
            commit_function_type commit_function_value( &::SireMol::Mover< SireMol::Residue >::commit );
            
            Mover_Residue__exposer.def( 
                "commit"
                , commit_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::mapInto
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*mapInto_function_type )( ::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            mapInto_function_type mapInto_function_value( &::SireMol::Mover< SireMol::Residue >::mapInto );
            
            Mover_Residue__exposer.def( 
                "mapInto"
                , mapInto_function_value
                , ( bp::arg("axes"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::operator=
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*assign_function_type )( ::SireMol::Mover< SireMol::Residue > const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Residue >::operator= );
            
            Mover_Residue__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::operator=
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*assign_function_type )( ::SireMol::Residue const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Residue >::operator= );
            
            Mover_Residue__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::rotate
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*rotate_function_type )( ::SireMaths::Quaternion const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Residue >::rotate );
            
            Mover_Residue__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("quat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::rotate
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*rotate_function_type )( ::SireMaths::Matrix const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Residue >::rotate );
            
            Mover_Residue__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("rotmat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::set
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*set_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Residue >::set );
            
            Mover_Residue__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("bond"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::set
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*set_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Residue >::set );
            
            Mover_Residue__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("angle"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::set
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*set_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Residue >::set );
            
            Mover_Residue__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::set
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*set_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Residue >::set );
            
            Mover_Residue__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("improper"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::setAll
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*setAll_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            setAll_function_type setAll_function_value( &::SireMol::Mover< SireMol::Residue >::setAll );
            
            Mover_Residue__exposer.def( 
                "setAll"
                , setAll_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::toString
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::QString ( ::SireMol::Mover< SireMol::Residue >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::Mover< SireMol::Residue >::toString );
            
            Mover_Residue__exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::transform
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*transform_function_type )( ::SireMaths::Transform const &,::SireBase::PropertyMap const & ) ;
            transform_function_type transform_function_value( &::SireMol::Mover< SireMol::Residue >::transform );
            
            Mover_Residue__exposer.def( 
                "transform"
                , transform_function_value
                , ( bp::arg("transform"), bp::arg("map")=SireBase::PropertyMap() )
                    /* undefined call policies */ );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::translate
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Residue > & ( ::SireMol::Mover< SireMol::Residue >::*translate_function_type )( ::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            translate_function_type translate_function_value( &::SireMol::Mover< SireMol::Residue >::translate );
            
            Mover_Residue__exposer.def( 
                "translate"
                , translate_function_value
                , ( bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Residue >::typeName
        
            typedef SireMol::Mover< SireMol::Residue > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::Mover< SireMol::Residue >::typeName );
            
            Mover_Residue__exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        Mover_Residue__exposer.staticmethod( "typeName" );
        Mover_Residue__exposer.def( "__copy__", &__copy__);
        Mover_Residue__exposer.def( "__deepcopy__", &__copy__);
        Mover_Residue__exposer.def( "clone", &__copy__);
        Mover_Residue__exposer.def( "__str__", &__str__< ::SireMol::Mover<SireMol::Residue> > );
        Mover_Residue__exposer.def( "__repr__", &__str__< ::SireMol::Mover<SireMol::Residue> > );
    }

}
