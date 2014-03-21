// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "Mover_Atom_.pypp.hpp"

namespace bp = boost::python;

#include "SireBase/errors.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "atom.h"

#include "atomcharges.h"

#include "atomeditor.h"

#include "atomproperty.hpp"

#include "chain.h"

#include "cutgroup.h"

#include "evaluator.h"

#include "molecule.h"

#include "mover.hpp"

#include "mover_metaid.h"

#include "residue.h"

#include "segment.h"

#include "selector.hpp"

#include <QDebug>

#include "atom.h"

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

SireMol::Mover<SireMol::Atom> __copy__(const SireMol::Mover<SireMol::Atom> &other){ return SireMol::Mover<SireMol::Atom>(other); }

#include "Helpers/str.hpp"

void register_Mover_Atom__class(){

    { //::SireMol::Mover< SireMol::Atom >
        typedef bp::class_< SireMol::Mover< SireMol::Atom >, bp::bases< SireMol::MoverBase, SireMol::Atom, SireMol::MoleculeView, SireBase::Property > > Mover_Atom__exposer_t;
        Mover_Atom__exposer_t Mover_Atom__exposer = Mover_Atom__exposer_t( "Mover_Atom_", bp::init< >() );
        bp::scope Mover_Atom__scope( Mover_Atom__exposer );
        Mover_Atom__exposer.def( bp::init< SireMol::Atom const & >(( bp::arg("view") )) );
        Mover_Atom__exposer.def( bp::init< SireMol::Atom const &, SireMol::AtomSelection const & >(( bp::arg("view"), bp::arg("movable_atoms") )) );
        Mover_Atom__exposer.def( bp::init< SireMol::Mover< SireMol::Atom > const & >(( bp::arg("other") )) );
        { //::SireMol::Mover< SireMol::Atom >::align
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Atom >::align );
            
            Mover_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::align
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*align_function_type )( ::SireMol::MoleculeView const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Atom >::align );
            
            Mover_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::align
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Atom >::align );
            
            Mover_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::align
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*align_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            align_function_type align_function_value( &::SireMol::Mover< SireMol::Atom >::align );
            
            Mover_Atom__exposer.def( 
                "align"
                , align_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::alignTo
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Atom >::alignTo );
            
            Mover_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::alignTo
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Atom >::alignTo );
            
            Mover_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::alignTo
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Atom >::alignTo );
            
            Mover_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::alignTo
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*alignTo_function_type )( ::SireMol::MoleculeView const &,::SireMol::AtomSelection const &,::SireMol::AtomMatcher const &,::SireBase::PropertyMap const &,::SireBase::PropertyMap const & ) ;
            alignTo_function_type alignTo_function_value( &::SireMol::Mover< SireMol::Atom >::alignTo );
            
            Mover_Atom__exposer.def( 
                "alignTo"
                , alignTo_function_value
                , ( bp::arg("other"), bp::arg("aligning_atoms"), bp::arg("matcher"), bp::arg("map0"), bp::arg("map1") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::change
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Atom >::change );
            
            Mover_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::change
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*change_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Atom >::change );
            
            Mover_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("angle"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::change
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*change_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Atom >::change );
            
            Mover_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("dihedral"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::change
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*change_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Atom >::change );
            
            Mover_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("bond"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::change
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*change_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            change_function_type change_function_value( &::SireMol::Mover< SireMol::Atom >::change );
            
            Mover_Atom__exposer.def( 
                "change"
                , change_function_value
                , ( bp::arg("improper"), bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::changeFrame
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*changeFrame_function_type )( ::SireMaths::AxisSet const &,::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            changeFrame_function_type changeFrame_function_value( &::SireMol::Mover< SireMol::Atom >::changeFrame );
            
            Mover_Atom__exposer.def( 
                "changeFrame"
                , changeFrame_function_value
                , ( bp::arg("from_frame"), bp::arg("to_frame"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::commit
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Atom ( ::SireMol::Mover< SireMol::Atom >::*commit_function_type )(  ) const;
            commit_function_type commit_function_value( &::SireMol::Mover< SireMol::Atom >::commit );
            
            Mover_Atom__exposer.def( 
                "commit"
                , commit_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::mapInto
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*mapInto_function_type )( ::SireMaths::AxisSet const &,::SireBase::PropertyMap const & ) ;
            mapInto_function_type mapInto_function_value( &::SireMol::Mover< SireMol::Atom >::mapInto );
            
            Mover_Atom__exposer.def( 
                "mapInto"
                , mapInto_function_value
                , ( bp::arg("axes"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::operator=
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*assign_function_type )( ::SireMol::Mover< SireMol::Atom > const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Atom >::operator= );
            
            Mover_Atom__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::operator=
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*assign_function_type )( ::SireMol::Atom const & ) ;
            assign_function_type assign_function_value( &::SireMol::Mover< SireMol::Atom >::operator= );
            
            Mover_Atom__exposer.def( 
                "assign"
                , assign_function_value
                , ( bp::arg("other") )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::rotate
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*rotate_function_type )( ::SireMaths::Quaternion const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Atom >::rotate );
            
            Mover_Atom__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("quat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::rotate
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*rotate_function_type )( ::SireMaths::Matrix const &,::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            rotate_function_type rotate_function_value( &::SireMol::Mover< SireMol::Atom >::rotate );
            
            Mover_Atom__exposer.def( 
                "rotate"
                , rotate_function_value
                , ( bp::arg("rotmat"), bp::arg("point"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::set
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*set_function_type )( ::SireMol::BondID const &,::SireUnits::Dimension::PhysUnit< 0, 1, 0, 0, 0, 0, 0 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Atom >::set );
            
            Mover_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("bond"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::set
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*set_function_type )( ::SireMol::AngleID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Atom >::set );
            
            Mover_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("angle"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::set
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*set_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Atom >::set );
            
            Mover_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::set
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*set_function_type )( ::SireMol::ImproperID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            set_function_type set_function_value( &::SireMol::Mover< SireMol::Atom >::set );
            
            Mover_Atom__exposer.def( 
                "set"
                , set_function_value
                , ( bp::arg("improper"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::setAll
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*setAll_function_type )( ::SireMol::DihedralID const &,::SireUnits::Dimension::PhysUnit< 0, 0, 0, 0, 0, 0, 1 >,::SireBase::PropertyMap const & ) ;
            setAll_function_type setAll_function_value( &::SireMol::Mover< SireMol::Atom >::setAll );
            
            Mover_Atom__exposer.def( 
                "setAll"
                , setAll_function_value
                , ( bp::arg("dihedral"), bp::arg("value"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::toString
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::QString ( ::SireMol::Mover< SireMol::Atom >::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::Mover< SireMol::Atom >::toString );
            
            Mover_Atom__exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::translate
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef ::SireMol::Mover< SireMol::Atom > & ( ::SireMol::Mover< SireMol::Atom >::*translate_function_type )( ::SireMaths::Vector const &,::SireBase::PropertyMap const & ) ;
            translate_function_type translate_function_value( &::SireMol::Mover< SireMol::Atom >::translate );
            
            Mover_Atom__exposer.def( 
                "translate"
                , translate_function_value
                , ( bp::arg("delta"), bp::arg("map")=SireBase::PropertyMap() )
                , bp::return_self< >() );
        
        }
        { //::SireMol::Mover< SireMol::Atom >::typeName
        
            typedef SireMol::Mover< SireMol::Atom > exported_class_t;
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::Mover< SireMol::Atom >::typeName );
            
            Mover_Atom__exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        Mover_Atom__exposer.staticmethod( "typeName" );
        Mover_Atom__exposer.def( "__copy__", &__copy__);
        Mover_Atom__exposer.def( "__deepcopy__", &__copy__);
        Mover_Atom__exposer.def( "clone", &__copy__);
        Mover_Atom__exposer.def( "__str__", &__str__< ::SireMol::Mover<SireMol::Atom> > );
        Mover_Atom__exposer.def( "__repr__", &__str__< ::SireMol::Mover<SireMol::Atom> > );
    }

}
