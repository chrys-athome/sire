// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Mover_Beads_.pypp.hpp"

namespace bp = boost::python;

#include "SireStream/datastream.h"

#include "beads.h"

#include "mover.hpp"

#include "beads.h"

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

SireMol::Mover<SireMol::Beads> __copy__(const SireMol::Mover<SireMol::Beads> &other){ return SireMol::Mover<SireMol::Beads>(other); }

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_Mover_Beads__class(){

    { //::SireMol::Mover< SireMol::Beads >
        typedef bp::class_< SireMol::Mover< SireMol::Beads >, bp::bases< SireMol::MoverBase, SireMol::Beads, SireMol::MoleculeView, SireBase::Property > > Mover_Beads__exposer_t;
        Mover_Beads__exposer_t Mover_Beads__exposer = Mover_Beads__exposer_t( "Mover_Beads_", bp::init< >() );
        bp::scope Mover_Beads__scope( Mover_Beads__exposer );
        Mover_Beads__exposer.def( bp::init< SireMol::Beads const & >(( bp::arg("view") )) );
        Mover_Beads__exposer.def( bp::init< SireMol::Beads const &, SireMol::AtomSelection const & >(( bp::arg("view"), bp::arg("movable_atoms") )) );
        Mover_Beads__exposer.def( bp::init< SireMol::Mover< SireMol::Beads > const & >(( bp::arg("other") )) );
        { //::SireMol::Mover< SireMol::Beads >::align
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Beads >::align );
            
            Mover_Beads__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map")=SireBase::PropertyMap() )
                    /* undefined call policies */ );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::align
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Beads >::align );
            
            Mover_Beads__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map0"), bp::arg("map1") )
                    /* undefined call policies */ );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::align
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Beads >::align );
            
            Mover_Beads__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                    /* undefined call policies */ );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::align
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Beads >::align );
            
            Mover_Beads__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                    /* undefined call policies */ );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::alignTo
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Beads >::alignTo );
            
            Mover_Beads__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::alignTo
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Beads >::alignTo );
            
            Mover_Beads__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::alignTo
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Beads >::alignTo );
            
            Mover_Beads__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::alignTo
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Beads >::alignTo );
            
            Mover_Beads__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::change
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Beads >::change );
            
            Mover_Beads__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::change
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*change_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Beads >::change );
            
            Mover_Beads__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("angle"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::change
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*change_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Beads >::change );
            
            Mover_Beads__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::change
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Beads >::change );
            
            Mover_Beads__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::change
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*change_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Beads >::change );
            
            Mover_Beads__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("improper"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::changeFrame
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*changeFrame_function_type )( ::SireMaths::AxisSet const &,::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            changeFrame_function_type changeFrame_function_value( &::SireMol::Mover< SireMol::Beads >::changeFrame );
            
            Mover_Beads__exposer.def( 
                "changeFrame"
                , changeFrame_function_value
                , ( bp::arg("from_frame"), bp::arg("to_frame"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::commit
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Beads ( ::SireMol::Mover< SireMol::Beads >::*commit_function_type )(  ) const;
            commit_function_type commit_function_value( &::SireMol::Mover< SireMol::Beads >::commit );
            
            Mover_Beads__exposer.def( 
                "commit"
                , commit_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::mapInto
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*mapInto_function_type )( ::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            mapInto_function_type mapInto_function_value( &::SireMol::Mover< SireMol::Beads >::mapInto );
            
            Mover_Beads__exposer.def( 
                "mapInto"
                , mapInto_function_value
                , ( bp::arg("axes"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::operator=
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*assign_function_type )( ::SireMol::Mover< SireMol::Beads > const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Beads >::operator= );
            
            Mover_Beads__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::operator=
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*assign_function_type )( ::SireMol::Beads const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Beads >::operator= );
            
            Mover_Beads__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::rotate
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*rotate_function_type )( ::SireMaths::Quaternion const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Beads >::rotate );
            
            Mover_Beads__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("quat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::rotate
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*rotate_function_type )( ::SireMaths::Matrix const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Beads >::rotate );
            
            Mover_Beads__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("rotmat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::set
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*set_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Beads >::set );
            
            Mover_Beads__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("bond"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::set
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*set_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Beads >::set );
            
            Mover_Beads__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("angle"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::set
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*set_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Beads >::set );
            
            Mover_Beads__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::set
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*set_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Beads >::set );
            
            Mover_Beads__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("improper"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::setAll
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*setAll_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            setAll_function_type setAll_function_value( &::SireMol::Mover< SireMol::Beads >::setAll );
            
            Mover_Beads__exposer.def( 
                "setAll"
                , setAll_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::toString
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::QString ( ::SireMol::Mover< SireMol::Beads >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::Mover< SireMol::Beads >::toString );
            
            Mover_Beads__exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::translate
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Beads > & ( ::SireMol::Mover< SireMol::Beads >::*translate_function_type )( ::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            translate_function_type translate_function_value( &::SireMol::Mover< SireMol::Beads >::translate );
            
            Mover_Beads__exposer.def( 
                "translate"
                , translate_function_value
                , ( bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Beads >::typeName
        
            typedef SireMol::Mover< SireMol::Beads > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::Mover< SireMol::Beads >::typeName );
            
            Mover_Beads__exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        Mover_Beads__exposer.staticmethod( "typeName" );
        Mover_Beads__exposer.def( "__copy__", &__copy__);
        Mover_Beads__exposer.def( "__deepcopy__", &__copy__);
        Mover_Beads__exposer.def( "clone", &__copy__);
        Mover_Beads__exposer.def( "__str__", &__str__< ::SireMol::Mover<SireMol::Beads> > );
        Mover_Beads__exposer.def( "__repr__", &__str__< ::SireMol::Mover<SireMol::Beads> > );
        Mover_Beads__exposer.def( "__len__", &__len_size< ::SireMol::Mover<SireMol::Beads> > );
    }

}
