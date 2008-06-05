
/**
  * This file contains the boost::python wrapping of ChargeTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireDB/atomtable.h"
#include "SireDB/wrapAtomTable.hpp"

#include "SireMM/chargetable.h"

#include "SireMol/molecule.h"

using namespace boost::python;

using namespace SireMol;
using namespace SireDB;

namespace SireMM
{

template<class Param>
uint wrap_chargetable_types_a_2( const ChargeTable &table, 
                                   QVector<Param> &param_vector, uint strt )
{
    return table.charges(param_vector,strt);
}

template<class Param>
uint wrap_chargetable_types_a_1( const ChargeTable &table, 
                                   QVector<Param> &param_vector )
{
    return table.charges(param_vector);
}

template<class IDX, class Param>
uint wrap_chargetable_types_b_3( const ChargeTable &table, IDX idx, 
                                   QVector<Param> &param_vector, uint strt )
{
    return table.charges( idx, param_vector, strt );
}

template<class IDX, class Param>
uint wrap_chargetable_types_b_2( const ChargeTable &table, IDX idx, 
                                   QVector<Param> &param_vector )
{
    return table.charges( idx, param_vector );
}

void export_ChargeTable()
{
    export_AtomTable<ChargeParameter>("AtomTableT_Charge_");

    class_< ChargeTable, 
    
            //dynamic casting in python from TableBase up to ChargeTable is 
            //broken by the templated AtomTableT class. I can fix this by 
            //declaring that both AtomTableT and AtomTable are bases of this
            //class. I need to spend some time working out why this is the case,
            //as it is probably due to either a bug in my code or a bug in 
            //boost::python (my money is on a bug in my code :-)
            bases< AtomTableT<ChargeParameter>, AtomTable >  
          
          >( "ChargeTable", init<>() )            
        .def( init<const Molecule&>() )
        .def( init<const ChargeTable&>() )
    
        .def( "typeName", &ChargeTable::typeName ).staticmethod("typeName")
    
        .def( "assignedCharge", (bool (ChargeTable::*)(const CGAtomID&) const)
                        &ChargeTable::assignedCharge )
        .def( "assignedCharge", (bool (ChargeTable::*)(const ResNumAtomID&) const)
                        &ChargeTable::assignedCharge )
        .def( "assignedCharge", (bool (ChargeTable::*)(const AtomIndex&) const)
                        &ChargeTable::assignedCharge )
    
        .def( "charges", (QVector<ChargeParameter> (ChargeTable::*)() const)
                        &ChargeTable::charges )
        .def( "charges", (QVector<ChargeParameter> (ChargeTable::*)(ResNum) const)
                        &ChargeTable::charges )
        .def( "charges", (QVector<ChargeParameter> (ChargeTable::*)(CutGroupID) const)
                        &ChargeTable::charges )
    
        .def( "charges", &wrap_chargetable_types_a_2<ChargeParameter> )
        .def( "charges", &wrap_chargetable_types_a_1<ChargeParameter> )
        .def( "charges", &wrap_chargetable_types_b_3<ResNum,ChargeParameter> )
        .def( "charges", &wrap_chargetable_types_b_2<ResNum,ChargeParameter> )
        .def( "charges", &wrap_chargetable_types_b_3<CutGroupID,ChargeParameter> )
        .def( "charges", &wrap_chargetable_types_b_2<CutGroupID,ChargeParameter> )
        
        //template<class C>
        //uint charges( QVector<C> &param_vector, uint strt=0 ) const;
        //template<class C>
        //uint charges( ResNum resnum, QVector<C> &param_vector, uint strt=0 ) const;
        //template<class C>
        //uint charges( CutGroupID cgid, QVector<C> &param_vector, uint strt=0 ) const;
    
        .def( "chargesByCutGroup", (GroupedVector<CGAtomID,ChargeParameter> 
                                  (ChargeTable::*)() const)
                        &ChargeTable::chargesByCutGroup )
        .def( "chargesByCutGroup", (GroupedVector<CGAtomID,ChargeParameter> 
                                  (ChargeTable::*)(const QSet<CutGroupID>&) const)
                        &ChargeTable::chargesByCutGroup )
    
        //template<class C>
        //void chargesByCutGroup( GroupedVector<C,CGAtomID> &param_vector ) const;
        //template<class C>
        //void chargesByCutGroup(const QSet<CutGroupID> &cgids, 
        //                         GroupedVector<C,CGAtomID> &param_vector) const;
    
        .def( "chargesByResidue", (GroupedVector<ResNumAtomID,ChargeParameter> 
                                  (ChargeTable::*)() const)
                        &ChargeTable::chargesByResidue )
        .def( "chargesByResidue", (GroupedVector<ResNumAtomID,ChargeParameter> 
                                  (ChargeTable::*)(const QSet<ResNum>&) const)
                        &ChargeTable::chargesByResidue )

        //template<class C>
        //void chargesByResidue( GroupedVector<C,ResNumAtomID> &param_vector ) const;
        //template<class C>
        //void chargesByResidue(const QSet<ResNum> &resnums,
        //                        GroupedVector<C,ResNumAtomID> &param_vector) const;
        
        .def( "clearCharge", (void (ChargeTable::*)(const CGAtomID&))
                        &ChargeTable::clearCharge )
        .def( "clearCharge", (void (ChargeTable::*)(const ResNumAtomID&))
                        &ChargeTable::clearCharge )
        .def( "clearCharge", (void (ChargeTable::*)(const AtomIndex&))
                        &ChargeTable::clearCharge )
    
        .def( "clearCharges", (void (ChargeTable::*)())
                        &ChargeTable::clearCharges )
        .def( "clearCharges", (void (ChargeTable::*)(ResNum))
                        &ChargeTable::clearCharges )
        .def( "clearCharges", (void (ChargeTable::*)(CutGroupID))
                        &ChargeTable::clearCharges )
    
        //template<class C>
        //void clearCharges(const C &lots);

        .def( "getCharge", (const ChargeParameter& (ChargeTable::*)(const CGAtomID&) const)
                        &ChargeTable::getCharge,
                            return_value_policy<copy_const_reference>() )
        .def( "getCharge", (const ChargeParameter& (ChargeTable::*)(const ResNumAtomID&) const)
                        &ChargeTable::getCharge,
                            return_value_policy<copy_const_reference>() )
        .def( "getCharge", (const ChargeParameter& (ChargeTable::*)(const AtomIndex&) const)
                        &ChargeTable::getCharge,
                            return_value_policy<copy_const_reference>() )
    
        .def( "hasMissingCharges", (bool (ChargeTable::*)() const)
                        &ChargeTable::hasMissingCharges )
        .def( "hasMissingCharges", (bool (ChargeTable::*)(ResNum) const)
                        &ChargeTable::hasMissingCharges )
        .def( "hasMissingCharges", (bool (ChargeTable::*)(CutGroupID) const)
                        &ChargeTable::hasMissingCharges )
    
        .def( "missingCharges", (QSet<AtomIndex> (ChargeTable::*)() const)
                        &ChargeTable::missingCharges )
        .def( "missingCharges", (QSet<AtomIndex> (ChargeTable::*)(ResNum) const)
                        &ChargeTable::missingCharges )
        .def( "missingCharges", (QSet<AtomIndex> (ChargeTable::*)(CutGroupID) const)
                        &ChargeTable::missingCharges )
    
        .def( "nCharges", (int (ChargeTable::*)() const) 
                        &ChargeTable::nCharges )
        .def( "nCharges", (int (ChargeTable::*)(ResNum) const) 
                        &ChargeTable::nCharges )
        .def( "nCharges", (int (ChargeTable::*)(CutGroupID) const) 
                        &ChargeTable::nCharges )
    
        .def( "setCharge", (void (ChargeTable::*)(const CGAtomID&, const ChargeParameter&))
                        &ChargeTable::setCharge )
        .def( "setCharge", (void (ChargeTable::*)(const ResNumAtomID&, const ChargeParameter&))
                        &ChargeTable::setCharge )
        .def( "setCharge", (void (ChargeTable::*)(const AtomIndex&, const ChargeParameter&))
                        &ChargeTable::setCharge )
                        
    ;
}

}
