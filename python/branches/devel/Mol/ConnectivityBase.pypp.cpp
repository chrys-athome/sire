// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include "ConnectivityBase.pypp.hpp"

namespace bp = boost::python;

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

#include "SireStream/shareddatastream.h"

#include "angleid.h"

#include "atommatcher.h"

#include "atomselection.h"

#include "bondid.h"

#include "connectivity.h"

#include "dihedralid.h"

#include "improperid.h"

#include "moleculedata.h"

#include "moleculeinfodata.h"

#include "moleculeview.h"

#include "tostring.h"

#include <QDataStream>

#include <QDebug>

#include <boost/assert.hpp>

#include "connectivity.h"

#include "Qt/qdatastream.hpp"

#include "Helpers/str.hpp"

void register_ConnectivityBase_class(){

    { //::SireMol::ConnectivityBase
        typedef bp::class_< SireMol::ConnectivityBase, bp::bases< SireMol::MolViewProperty, SireBase::Property >, boost::noncopyable > ConnectivityBase_exposer_t;
        ConnectivityBase_exposer_t ConnectivityBase_exposer = ConnectivityBase_exposer_t( "ConnectivityBase", bp::no_init );
        bp::scope ConnectivityBase_scope( ConnectivityBase_exposer );
        { //::SireMol::ConnectivityBase::areConnected
        
            typedef bool ( ::SireMol::ConnectivityBase::*areConnected_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            areConnected_function_type areConnected_function_value( &::SireMol::ConnectivityBase::areConnected );
            
            ConnectivityBase_exposer.def( 
                "areConnected"
                , areConnected_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::areConnected
        
            typedef bool ( ::SireMol::ConnectivityBase::*areConnected_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            areConnected_function_type areConnected_function_value( &::SireMol::ConnectivityBase::areConnected );
            
            ConnectivityBase_exposer.def( 
                "areConnected"
                , areConnected_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::areConnected
        
            typedef bool ( ::SireMol::ConnectivityBase::*areConnected_function_type )( ::SireMol::ResIdx,::SireMol::ResIdx ) const;
            areConnected_function_type areConnected_function_value( &::SireMol::ConnectivityBase::areConnected );
            
            ConnectivityBase_exposer.def( 
                "areConnected"
                , areConnected_function_value
                , ( bp::arg("res0"), bp::arg("res1") ) );
        
        }
        { //::SireMol::ConnectivityBase::areConnected
        
            typedef bool ( ::SireMol::ConnectivityBase::*areConnected_function_type )( ::SireMol::ResID const &,::SireMol::ResID const & ) const;
            areConnected_function_type areConnected_function_value( &::SireMol::ConnectivityBase::areConnected );
            
            ConnectivityBase_exposer.def( 
                "areConnected"
                , areConnected_function_value
                , ( bp::arg("res0"), bp::arg("res1") ) );
        
        }
        { //::SireMol::ConnectivityBase::connectionsTo
        
            typedef ::QSet< SireMol::AtomIdx > const & ( ::SireMol::ConnectivityBase::*connectionsTo_function_type )( ::SireMol::AtomIdx ) const;
            connectionsTo_function_type connectionsTo_function_value( &::SireMol::ConnectivityBase::connectionsTo );
            
            ConnectivityBase_exposer.def( 
                "connectionsTo"
                , connectionsTo_function_value
                , ( bp::arg("atomidx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::ConnectivityBase::connectionsTo
        
            typedef ::QSet< SireMol::AtomIdx > const & ( ::SireMol::ConnectivityBase::*connectionsTo_function_type )( ::SireMol::AtomID const & ) const;
            connectionsTo_function_type connectionsTo_function_value( &::SireMol::ConnectivityBase::connectionsTo );
            
            ConnectivityBase_exposer.def( 
                "connectionsTo"
                , connectionsTo_function_value
                , ( bp::arg("atomid") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::ConnectivityBase::connectionsTo
        
            typedef ::QSet< SireMol::ResIdx > const & ( ::SireMol::ConnectivityBase::*connectionsTo_function_type )( ::SireMol::ResIdx ) const;
            connectionsTo_function_type connectionsTo_function_value( &::SireMol::ConnectivityBase::connectionsTo );
            
            ConnectivityBase_exposer.def( 
                "connectionsTo"
                , connectionsTo_function_value
                , ( bp::arg("residx") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::ConnectivityBase::connectionsTo
        
            typedef ::QSet< SireMol::ResIdx > const & ( ::SireMol::ConnectivityBase::*connectionsTo_function_type )( ::SireMol::ResID const & ) const;
            connectionsTo_function_type connectionsTo_function_value( &::SireMol::ConnectivityBase::connectionsTo );
            
            ConnectivityBase_exposer.def( 
                "connectionsTo"
                , connectionsTo_function_value
                , ( bp::arg("resid") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::SireMol::ConnectivityBase::findPath
        
            typedef ::QList< SireMol::AtomIdx > ( ::SireMol::ConnectivityBase::*findPath_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            findPath_function_type findPath_function_value( &::SireMol::ConnectivityBase::findPath );
            
            ConnectivityBase_exposer.def( 
                "findPath"
                , findPath_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::findPath
        
            typedef ::QList< SireMol::AtomIdx > ( ::SireMol::ConnectivityBase::*findPath_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            findPath_function_type findPath_function_value( &::SireMol::ConnectivityBase::findPath );
            
            ConnectivityBase_exposer.def( 
                "findPath"
                , findPath_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::findPaths
        
            typedef ::QList< QList< SireMol::AtomIdx > > ( ::SireMol::ConnectivityBase::*findPaths_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            findPaths_function_type findPaths_function_value( &::SireMol::ConnectivityBase::findPaths );
            
            ConnectivityBase_exposer.def( 
                "findPaths"
                , findPaths_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::findPaths
        
            typedef ::QList< QList< SireMol::AtomIdx > > ( ::SireMol::ConnectivityBase::*findPaths_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            findPaths_function_type findPaths_function_value( &::SireMol::ConnectivityBase::findPaths );
            
            ConnectivityBase_exposer.def( 
                "findPaths"
                , findPaths_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::getAngles
        
            typedef ::QList< SireMol::AngleID > ( ::SireMol::ConnectivityBase::*getAngles_function_type )(  ) const;
            getAngles_function_type getAngles_function_value( &::SireMol::ConnectivityBase::getAngles );
            
            ConnectivityBase_exposer.def( 
                "getAngles"
                , getAngles_function_value );
        
        }
        { //::SireMol::ConnectivityBase::getAngles
        
            typedef ::QList< SireMol::AngleID > ( ::SireMol::ConnectivityBase::*getAngles_function_type )( ::SireMol::AtomID const & ) const;
            getAngles_function_type getAngles_function_value( &::SireMol::ConnectivityBase::getAngles );
            
            ConnectivityBase_exposer.def( 
                "getAngles"
                , getAngles_function_value
                , ( bp::arg("atom0") ) );
        
        }
        { //::SireMol::ConnectivityBase::getAngles
        
            typedef ::QList< SireMol::AngleID > ( ::SireMol::ConnectivityBase::*getAngles_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            getAngles_function_type getAngles_function_value( &::SireMol::ConnectivityBase::getAngles );
            
            ConnectivityBase_exposer.def( 
                "getAngles"
                , getAngles_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::getBondMatrix
        
            typedef ::QVector< QVector< bool > > ( ::SireMol::ConnectivityBase::*getBondMatrix_function_type )( int ) const;
            getBondMatrix_function_type getBondMatrix_function_value( &::SireMol::ConnectivityBase::getBondMatrix );
            
            ConnectivityBase_exposer.def( 
                "getBondMatrix"
                , getBondMatrix_function_value
                , ( bp::arg("order") ) );
        
        }
        { //::SireMol::ConnectivityBase::getBondMatrix
        
            typedef ::QVector< QVector< bool > > ( ::SireMol::ConnectivityBase::*getBondMatrix_function_type )( int,int ) const;
            getBondMatrix_function_type getBondMatrix_function_value( &::SireMol::ConnectivityBase::getBondMatrix );
            
            ConnectivityBase_exposer.def( 
                "getBondMatrix"
                , getBondMatrix_function_value
                , ( bp::arg("start"), bp::arg("end") ) );
        
        }
        { //::SireMol::ConnectivityBase::getBonds
        
            typedef ::QList< SireMol::BondID > ( ::SireMol::ConnectivityBase::*getBonds_function_type )(  ) const;
            getBonds_function_type getBonds_function_value( &::SireMol::ConnectivityBase::getBonds );
            
            ConnectivityBase_exposer.def( 
                "getBonds"
                , getBonds_function_value );
        
        }
        { //::SireMol::ConnectivityBase::getBonds
        
            typedef ::QList< SireMol::BondID > ( ::SireMol::ConnectivityBase::*getBonds_function_type )( ::SireMol::AtomID const & ) const;
            getBonds_function_type getBonds_function_value( &::SireMol::ConnectivityBase::getBonds );
            
            ConnectivityBase_exposer.def( 
                "getBonds"
                , getBonds_function_value
                , ( bp::arg("atom") ) );
        
        }
        { //::SireMol::ConnectivityBase::getDihedrals
        
            typedef ::QList< SireMol::DihedralID > ( ::SireMol::ConnectivityBase::*getDihedrals_function_type )(  ) const;
            getDihedrals_function_type getDihedrals_function_value( &::SireMol::ConnectivityBase::getDihedrals );
            
            ConnectivityBase_exposer.def( 
                "getDihedrals"
                , getDihedrals_function_value );
        
        }
        { //::SireMol::ConnectivityBase::getDihedrals
        
            typedef ::QList< SireMol::DihedralID > ( ::SireMol::ConnectivityBase::*getDihedrals_function_type )( ::SireMol::AtomID const & ) const;
            getDihedrals_function_type getDihedrals_function_value( &::SireMol::ConnectivityBase::getDihedrals );
            
            ConnectivityBase_exposer.def( 
                "getDihedrals"
                , getDihedrals_function_value
                , ( bp::arg("atom0") ) );
        
        }
        { //::SireMol::ConnectivityBase::getDihedrals
        
            typedef ::QList< SireMol::DihedralID > ( ::SireMol::ConnectivityBase::*getDihedrals_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            getDihedrals_function_type getDihedrals_function_value( &::SireMol::ConnectivityBase::getDihedrals );
            
            ConnectivityBase_exposer.def( 
                "getDihedrals"
                , getDihedrals_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::getDihedrals
        
            typedef ::QList< SireMol::DihedralID > ( ::SireMol::ConnectivityBase::*getDihedrals_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            getDihedrals_function_type getDihedrals_function_value( &::SireMol::ConnectivityBase::getDihedrals );
            
            ConnectivityBase_exposer.def( 
                "getDihedrals"
                , getDihedrals_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::BondID const & ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("bond") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::AngleID const & ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("angle") ) );
        
        }
        { //::SireMol::ConnectivityBase::inRing
        
            typedef bool ( ::SireMol::ConnectivityBase::*inRing_function_type )( ::SireMol::DihedralID const & ) const;
            inRing_function_type inRing_function_value( &::SireMol::ConnectivityBase::inRing );
            
            ConnectivityBase_exposer.def( 
                "inRing"
                , inRing_function_value
                , ( bp::arg("dihedral") ) );
        
        }
        { //::SireMol::ConnectivityBase::isCompatibleWith
        
            typedef bool ( ::SireMol::ConnectivityBase::*isCompatibleWith_function_type )( ::SireMol::MoleculeInfoData const & ) const;
            isCompatibleWith_function_type isCompatibleWith_function_value( &::SireMol::ConnectivityBase::isCompatibleWith );
            
            ConnectivityBase_exposer.def( 
                "isCompatibleWith"
                , isCompatibleWith_function_value
                , ( bp::arg("molinfo") ) );
        
        }
        { //::SireMol::ConnectivityBase::nConnections
        
            typedef int ( ::SireMol::ConnectivityBase::*nConnections_function_type )(  ) const;
            nConnections_function_type nConnections_function_value( &::SireMol::ConnectivityBase::nConnections );
            
            ConnectivityBase_exposer.def( 
                "nConnections"
                , nConnections_function_value );
        
        }
        { //::SireMol::ConnectivityBase::nConnections
        
            typedef int ( ::SireMol::ConnectivityBase::*nConnections_function_type )( ::SireMol::AtomIdx ) const;
            nConnections_function_type nConnections_function_value( &::SireMol::ConnectivityBase::nConnections );
            
            ConnectivityBase_exposer.def( 
                "nConnections"
                , nConnections_function_value
                , ( bp::arg("atomidx") ) );
        
        }
        { //::SireMol::ConnectivityBase::nConnections
        
            typedef int ( ::SireMol::ConnectivityBase::*nConnections_function_type )( ::SireMol::AtomID const & ) const;
            nConnections_function_type nConnections_function_value( &::SireMol::ConnectivityBase::nConnections );
            
            ConnectivityBase_exposer.def( 
                "nConnections"
                , nConnections_function_value
                , ( bp::arg("atomid") ) );
        
        }
        { //::SireMol::ConnectivityBase::nConnections
        
            typedef int ( ::SireMol::ConnectivityBase::*nConnections_function_type )( ::SireMol::ResIdx ) const;
            nConnections_function_type nConnections_function_value( &::SireMol::ConnectivityBase::nConnections );
            
            ConnectivityBase_exposer.def( 
                "nConnections"
                , nConnections_function_value
                , ( bp::arg("residx") ) );
        
        }
        { //::SireMol::ConnectivityBase::nConnections
        
            typedef int ( ::SireMol::ConnectivityBase::*nConnections_function_type )( ::SireMol::ResID const & ) const;
            nConnections_function_type nConnections_function_value( &::SireMol::ConnectivityBase::nConnections );
            
            ConnectivityBase_exposer.def( 
                "nConnections"
                , nConnections_function_value
                , ( bp::arg("resid") ) );
        
        }
        { //::SireMol::ConnectivityBase::nConnections
        
            typedef int ( ::SireMol::ConnectivityBase::*nConnections_function_type )( ::SireMol::ResIdx,::SireMol::ResIdx ) const;
            nConnections_function_type nConnections_function_value( &::SireMol::ConnectivityBase::nConnections );
            
            ConnectivityBase_exposer.def( 
                "nConnections"
                , nConnections_function_value
                , ( bp::arg("res0"), bp::arg("res1") ) );
        
        }
        { //::SireMol::ConnectivityBase::nConnections
        
            typedef int ( ::SireMol::ConnectivityBase::*nConnections_function_type )( ::SireMol::ResID const &,::SireMol::ResID const & ) const;
            nConnections_function_type nConnections_function_value( &::SireMol::ConnectivityBase::nConnections );
            
            ConnectivityBase_exposer.def( 
                "nConnections"
                , nConnections_function_value
                , ( bp::arg("res0"), bp::arg("res1") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::BondID const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("bond") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::BondID const &,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("bond"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AngleID const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("angle") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AngleID const &,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("angle"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::DihedralID const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("dihedral") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomIdx,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomID const &,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("atom0"), bp::arg("atom1"), bp::arg("atom2"), bp::arg("atom3"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::DihedralID const &,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("dihedral"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::ImproperID const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("improper") ) );
        
        }
        { //::SireMol::ConnectivityBase::split
        
            typedef ::boost::tuples::tuple< SireMol::AtomSelection, SireMol::AtomSelection, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type, boost::tuples::null_type > ( ::SireMol::ConnectivityBase::*split_function_type )( ::SireMol::ImproperID const &,::SireMol::AtomSelection const & ) const;
            split_function_type split_function_value( &::SireMol::ConnectivityBase::split );
            
            ConnectivityBase_exposer.def( 
                "split"
                , split_function_value
                , ( bp::arg("improper"), bp::arg("selected_atoms") ) );
        
        }
        { //::SireMol::ConnectivityBase::toString
        
            typedef ::QString ( ::SireMol::ConnectivityBase::*toString_function_type )(  ) const;
            toString_function_type toString_function_value( &::SireMol::ConnectivityBase::toString );
            
            ConnectivityBase_exposer.def( 
                "toString"
                , toString_function_value );
        
        }
        { //::SireMol::ConnectivityBase::typeName
        
            typedef char const * ( *typeName_function_type )(  );
            typeName_function_type typeName_function_value( &::SireMol::ConnectivityBase::typeName );
            
            ConnectivityBase_exposer.def( 
                "typeName"
                , typeName_function_value );
        
        }
        ConnectivityBase_exposer.staticmethod( "typeName" );
        ConnectivityBase_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::SireMol::ConnectivityBase >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ConnectivityBase_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::SireMol::ConnectivityBase >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        ConnectivityBase_exposer.def( "__str__", &__str__< ::SireMol::ConnectivityBase > );
        ConnectivityBase_exposer.def( "__repr__", &__str__< ::SireMol::ConnectivityBase > );
    }

}
