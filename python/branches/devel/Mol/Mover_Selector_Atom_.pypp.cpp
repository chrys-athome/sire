// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Mover_Selector_Atom_.pypp.hpp"

namespace bp = boost::python;

#include "mover_metaid.h"

#include "mover_metaid.h"

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

SireMol::Mover<SireMol::Selector<SireMol::Atom> > __copy__(const SireMol::Mover<SireMol::Selector<SireMol::Atom> > &other){ return SireMol::Mover<SireMol::Selector<SireMol::Atom> >(other); }

#include "Helpers/str.hpp"

#include "Helpers/len.hpp"

void register_Mover_Selector_Atom__class(){

    { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >
        typedef bp::class_< SireMol::Mover< SireMol::Selector< SireMol::Atom > >, bp::bases< SireMol::MoverBase, SireMol::Selector<SireMol::Atom>, SireMol::MoleculeView, SireBase::Property > > Mover_Selector_Atom__exposer_t;
        Mover_Selector_Atom__exposer_t Mover_Selector_Atom__exposer = Mover_Selector_Atom__exposer_t( "Mover_Selector_Atom_", bp::init< >() );
        bp::scope Mover_Selector_Atom__scope( Mover_Selector_Atom__exposer );
        Mover_Selector_Atom__exposer.def( bp::init< SireMol::Selector< SireMol::Atom > const & >(( bp::arg("view") )) );
        Mover_Selector_Atom__exposer.def( bp::init< SireMol::Selector< SireMol::Atom > const &, SireMol::AtomSelection const & >(( bp::arg("view"), bp::arg("movable_atoms") )) );
        Mover_Selector_Atom__exposer.def( bp::init< SireMol::Mover< SireMol::Selector< SireMol::Atom > > const & >(( bp::arg("other") )) );
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align );
            
            Mover_Selector_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align );
            
            Mover_Selector_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align );
            
            Mover_Selector_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::align );
            
            Mover_Selector_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo );
            
            Mover_Selector_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo );
            
            Mover_Selector_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo );
            
            Mover_Selector_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::alignTo );
            
            Mover_Selector_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change );
            
            Mover_Selector_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*change_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change );
            
            Mover_Selector_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("angle"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*change_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change );
            
            Mover_Selector_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change );
            
            Mover_Selector_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*change_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::change );
            
            Mover_Selector_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("improper"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::changeFrame
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*changeFrame_function_type )( ::SireMaths::AxisSet const &,::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            changeFrame_function_type changeFrame_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::changeFrame );
            
            Mover_Selector_Atom__exposer.def( 
                "changeFrame"
                , changeFrame_function_value
                , ( bp::arg("from_frame"), bp::arg("to_frame"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::commit
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Selector< SireMol::Atom > ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*commit_function_type )(  ) const;
            commit_function_type commit_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::commit );
            
            Mover_Selector_Atom__exposer.def( 
                "commit"
                , commit_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::mapInto
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*mapInto_function_type )( ::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            mapInto_function_type mapInto_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::mapInto );
            
            Mover_Selector_Atom__exposer.def( 
                "mapInto"
                , mapInto_function_value
                , ( bp::arg("axes"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::operator=
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*assign_function_type )( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::operator= );
            
            Mover_Selector_Atom__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::operator=
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*assign_function_type )( ::SireMol::Selector< SireMol::Atom > const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::operator= );
            
            Mover_Selector_Atom__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::rotate
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*rotate_function_type )( ::SireMaths::Quaternion const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::rotate );
            
            Mover_Selector_Atom__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("quat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::rotate
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*rotate_function_type )( ::SireMaths::Matrix const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::rotate );
            
            Mover_Selector_Atom__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("rotmat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*set_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set );
            
            Mover_Selector_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("bond"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*set_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set );
            
            Mover_Selector_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("angle"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*set_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set );
            
            Mover_Selector_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*set_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::set );
            
            Mover_Selector_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("improper"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::setAll
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*setAll_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            setAll_function_type setAll_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::setAll );
            
            Mover_Selector_Atom__exposer.def( 
                "setAll"
                , setAll_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::toString
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::QString ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::toString );
            
            Mover_Selector_Atom__exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::transform
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*transform_function_type )( ::SireMaths::Transform const &,::SireBase::PropertyMap const & ) ;
            transform_function_type transform_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::transform );
            
            Mover_Selector_Atom__exposer.def( 
                "transform"
                , transform_function_value
                , ( bp::arg("transform"), bp::arg("map")=SireBase::PropertyMap() )
                    /* undefined call policies */ );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::translate
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Selector< SireMol::Atom > > & ( ::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::*translate_function_type )( ::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            translate_function_type translate_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::translate );
            
            Mover_Selector_Atom__exposer.def( 
                "translate"
                , translate_function_value
                , ( bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::typeName
        
            typedef SireMol::Mover< SireMol::Selector< SireMol::Atom > > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::Mover< SireMol::Selector< SireMol::Atom > >::typeName );
            
            Mover_Selector_Atom__exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        Mover_Selector_Atom__exposer.staticmethod( "typeName" );
        Mover_Selector_Atom__exposer.def( "__copy__", &__copy__);
        Mover_Selector_Atom__exposer.def( "__deepcopy__", &__copy__);
        Mover_Selector_Atom__exposer.def( "clone", &__copy__);
        Mover_Selector_Atom__exposer.def( "__str__", &__str__< ::SireMol::Mover<SireMol::Selector<SireMol::Atom> > > );
        Mover_Selector_Atom__exposer.def( "__repr__", &__str__< ::SireMol::Mover<SireMol::Selector<SireMol::Atom> > > );
        Mover_Selector_Atom__exposer.def( "__len__", &__len_count< ::SireMol::Mover<SireMol::Selector<SireMol::Atom> > > );
    }

}
