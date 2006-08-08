
/**
  * This file contains the boost::python wrapping of AtomTypeTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireDB/atomtable.h"
#include "SireDB/atomtypetable.h"
#include "SireDB/wrapAtomTable.hpp"

#include "SireMol/molecule.h"
#include "SireQt/qdatastream.hpp"

using namespace boost::python;

using namespace SireMol;
using namespace SireQt;

namespace SireDB
{

template<class Param>
uint wrap_atomtypetable_types_a_2( const AtomTypeTable &table, 
                                   QVector<Param> &param_vector, uint strt )
{
    return table.atomTypes(param_vector,strt);
}

template<class Param>
uint wrap_atomtypetable_types_a_1( const AtomTypeTable &table, 
                                   QVector<Param> &param_vector )
{
    return table.atomTypes(param_vector);
}

template<class IDX, class Param>
uint wrap_atomtypetable_types_b_3( const AtomTypeTable &table, IDX idx, 
                                   QVector<Param> &param_vector, uint strt )
{
    return table.atomTypes( idx, param_vector, strt );
}

template<class IDX, class Param>
uint wrap_atomtypetable_types_b_2( const AtomTypeTable &table, IDX idx, 
                                   QVector<Param> &param_vector )
{
    return table.atomTypes( idx, param_vector );
}

void export_AtomTypeTable()
{
    export_AtomTable<AtomType>("AtomTableT_AtomType_");

    class_< AtomTypeTable, 
    
            //dynamic casting in python from TableBase up to AtomTypeTable is 
            //broken by the templated AtomTableT class. I can fix this by 
            //declaring that both AtomTableT and AtomTable are bases of this
            //class. I need to spend some time working out why this is the case,
            //as it is probably due to either a bug in my code or a bug in 
            //boost::python (my money is on a bug in my code :-)
            bases< AtomTableT<AtomType>, AtomTable >  
          
          >( "AtomTypeTable", init<>() )            
        .def( init<const Molecule&>() )
        .def( init<const AtomTypeTable&>() )
    
        .def( "__rlshift__", &__rlshift__QDataStream<AtomTypeTable>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
        .def( "__rrshift__", &__rrshift__QDataStream<AtomTypeTable>,
                return_internal_reference< 1, with_custodian_and_ward<1,2> >() )
    
        .def( "typeName", &AtomTypeTable::typeName ).staticmethod("typeName")
    
        .def( "assignedAtomType", (bool (AtomTypeTable::*)(const CGAtomID&) const)
                        &AtomTypeTable::assignedAtomType )
        .def( "assignedAtomType", (bool (AtomTypeTable::*)(const ResNumAtomID&) const)
                        &AtomTypeTable::assignedAtomType )
        .def( "assignedAtomType", (bool (AtomTypeTable::*)(const AtomIndex&) const)
                        &AtomTypeTable::assignedAtomType )
    
        .def( "atomTypes", (QVector<AtomType> (AtomTypeTable::*)() const)
                        &AtomTypeTable::atomTypes )
        .def( "atomTypes", (QVector<AtomType> (AtomTypeTable::*)(ResNum) const)
                        &AtomTypeTable::atomTypes )
        .def( "atomTypes", (QVector<AtomType> (AtomTypeTable::*)(CutGroupID) const)
                        &AtomTypeTable::atomTypes )
    
        .def( "atomTypes", &wrap_atomtypetable_types_a_2<AtomType> )
        .def( "atomTypes", &wrap_atomtypetable_types_a_1<AtomType> )
        .def( "atomTypes", &wrap_atomtypetable_types_b_3<ResNum,AtomType> )
        .def( "atomTypes", &wrap_atomtypetable_types_b_2<ResNum,AtomType> )
        .def( "atomTypes", &wrap_atomtypetable_types_b_3<CutGroupID,AtomType> )
        .def( "atomTypes", &wrap_atomtypetable_types_b_2<CutGroupID,AtomType> )
        
        //template<class C>
        //uint atomTypes( QVector<C> &param_vector, uint strt=0 ) const;
        //template<class C>
        //uint atomTypes( ResNum resnum, QVector<C> &param_vector, uint strt=0 ) const;
        //template<class C>
        //uint atomTypes( CutGroupID cgid, QVector<C> &param_vector, uint strt=0 ) const;
    
        .def( "atomTypesByCutGroup", (GroupedVector<CGAtomID,AtomType> 
                                  (AtomTypeTable::*)() const)
                        &AtomTypeTable::atomTypesByCutGroup )
        .def( "atomTypesByCutGroup", (GroupedVector<CGAtomID,AtomType> 
                                  (AtomTypeTable::*)(const QSet<CutGroupID>&) const)
                        &AtomTypeTable::atomTypesByCutGroup )
    
        //template<class C>
        //void atomTypesByCutGroup( GroupedVector<C,CGAtomID> &param_vector ) const;
        //template<class C>
        //void atomTypesByCutGroup(const QSet<CutGroupID> &cgids, 
        //                         GroupedVector<C,CGAtomID> &param_vector) const;
    
        .def( "atomTypesByResidue", (GroupedVector<ResNumAtomID,AtomType> 
                                  (AtomTypeTable::*)() const)
                        &AtomTypeTable::atomTypesByResidue )
        .def( "atomTypesByResidue", (GroupedVector<ResNumAtomID,AtomType> 
                                  (AtomTypeTable::*)(const QSet<ResNum>&) const)
                        &AtomTypeTable::atomTypesByResidue )

        //template<class C>
        //void atomTypesByResidue( GroupedVector<C,ResNumAtomID> &param_vector ) const;
        //template<class C>
        //void atomTypesByResidue(const QSet<ResNum> &resnums,
        //                        GroupedVector<C,ResNumAtomID> &param_vector) const;
        
        .def( "clearAtomType", (void (AtomTypeTable::*)(const CGAtomID&))
                        &AtomTypeTable::clearAtomType )
        .def( "clearAtomType", (void (AtomTypeTable::*)(const ResNumAtomID&))
                        &AtomTypeTable::clearAtomType )
        .def( "clearAtomType", (void (AtomTypeTable::*)(const AtomIndex&))
                        &AtomTypeTable::clearAtomType )
    
        .def( "clearAtomTypes", (void (AtomTypeTable::*)())
                        &AtomTypeTable::clearAtomTypes )
        .def( "clearAtomTypes", (void (AtomTypeTable::*)(ResNum))
                        &AtomTypeTable::clearAtomTypes )
        .def( "clearAtomTypes", (void (AtomTypeTable::*)(CutGroupID))
                        &AtomTypeTable::clearAtomTypes )
    
        //template<class C>
        //void clearAtomTypes(const C &lots);

        .def( "getAtomType", (const AtomType& (AtomTypeTable::*)(const CGAtomID&) const)
                        &AtomTypeTable::getAtomType,
                            return_value_policy<copy_const_reference>() )
        .def( "getAtomType", (const AtomType& (AtomTypeTable::*)(const ResNumAtomID&) const)
                        &AtomTypeTable::getAtomType,
                            return_value_policy<copy_const_reference>() )
        .def( "getAtomType", (const AtomType& (AtomTypeTable::*)(const AtomIndex&) const)
                        &AtomTypeTable::getAtomType,
                            return_value_policy<copy_const_reference>() )
    
        .def( "hasMissingAtomTypes", (bool (AtomTypeTable::*)() const)
                        &AtomTypeTable::hasMissingAtomTypes )
        .def( "hasMissingAtomTypes", (bool (AtomTypeTable::*)(ResNum) const)
                        &AtomTypeTable::hasMissingAtomTypes )
        .def( "hasMissingAtomTypes", (bool (AtomTypeTable::*)(CutGroupID) const)
                        &AtomTypeTable::hasMissingAtomTypes )
    
        .def( "missingAtomTypes", (QSet<AtomIndex> (AtomTypeTable::*)() const)
                        &AtomTypeTable::missingAtomTypes )
        .def( "missingAtomTypes", (QSet<AtomIndex> (AtomTypeTable::*)(ResNum) const)
                        &AtomTypeTable::missingAtomTypes )
        .def( "missingAtomTypes", (QSet<AtomIndex> (AtomTypeTable::*)(CutGroupID) const)
                        &AtomTypeTable::missingAtomTypes )
    
        .def( "nAtomTypes", (int (AtomTypeTable::*)() const) 
                        &AtomTypeTable::nAtomTypes )
        .def( "nAtomTypes", (int (AtomTypeTable::*)(ResNum) const) 
                        &AtomTypeTable::nAtomTypes )
        .def( "nAtomTypes", (int (AtomTypeTable::*)(CutGroupID) const) 
                        &AtomTypeTable::nAtomTypes )
    
        .def( "setAtomType", (void (AtomTypeTable::*)(const CGAtomID&, const AtomType&))
                        &AtomTypeTable::setAtomType )
        .def( "setAtomType", (void (AtomTypeTable::*)(const ResNumAtomID&, const AtomType&))
                        &AtomTypeTable::setAtomType )
        .def( "setAtomType", (void (AtomTypeTable::*)(const AtomIndex&, const AtomType&))
                        &AtomTypeTable::setAtomType )
                        
    ;
}

}
