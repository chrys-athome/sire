
/**
  * This file contains the boost::python wrapping of LJTable
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireDB/atomtable.h"
#include "SireDB/wrapAtomTable.hpp"

#include "SireMM/ljtable.h"

#include "SireMol/molecule.h"

using namespace boost::python;

using namespace SireMol;
using namespace SireDB;

namespace SireMM
{

template<class Param>
uint wrap_ljtable_types_a_2( const LJTable &table, 
                                   QVector<Param> &param_vector, uint strt )
{
    return table.ljs(param_vector,strt);
}

template<class Param>
uint wrap_ljtable_types_a_1( const LJTable &table, 
                                   QVector<Param> &param_vector )
{
    return table.ljs(param_vector);
}

template<class IDX, class Param>
uint wrap_ljtable_types_b_3( const LJTable &table, IDX idx, 
                                   QVector<Param> &param_vector, uint strt )
{
    return table.ljs( idx, param_vector, strt );
}

template<class IDX, class Param>
uint wrap_ljtable_types_b_2( const LJTable &table, IDX idx, 
                                   QVector<Param> &param_vector )
{
    return table.ljs( idx, param_vector );
}

void export_LJTable()
{
    export_AtomTable<LJParameter>("AtomTableT_LJ_");

    class_< LJTable, 
    
            //dynamic casting in python from TableBase up to LJTable is 
            //broken by the templated AtomTableT class. I can fix this by 
            //declaring that both AtomTableT and AtomTable are bases of this
            //class. I need to spend some time working out why this is the case,
            //as it is probably due to either a bug in my code or a bug in 
            //boost::python (my money is on a bug in my code :-)
            bases< AtomTableT<LJParameter>, AtomTable >  
          
          >( "LJTable", init<>() )            
        .def( init<const Molecule&>() )
        .def( init<const LJTable&>() )
    
        .def( "typeName", &LJTable::typeName ).staticmethod("typeName")
    
        .def( "assignedLJ", (bool (LJTable::*)(const CGAtomID&) const)
                        &LJTable::assignedLJ )
        .def( "assignedLJ", (bool (LJTable::*)(const ResNumAtomID&) const)
                        &LJTable::assignedLJ )
        .def( "assignedLJ", (bool (LJTable::*)(const AtomIndex&) const)
                        &LJTable::assignedLJ )
    
        .def( "ljs", (QVector<LJParameter> (LJTable::*)() const)
                        &LJTable::ljs )
        .def( "ljs", (QVector<LJParameter> (LJTable::*)(ResNum) const)
                        &LJTable::ljs )
        .def( "ljs", (QVector<LJParameter> (LJTable::*)(CutGroupID) const)
                        &LJTable::ljs )
    
        .def( "ljs", &wrap_ljtable_types_a_2<LJParameter> )
        .def( "ljs", &wrap_ljtable_types_a_1<LJParameter> )
        .def( "ljs", &wrap_ljtable_types_b_3<ResNum,LJParameter> )
        .def( "ljs", &wrap_ljtable_types_b_2<ResNum,LJParameter> )
        .def( "ljs", &wrap_ljtable_types_b_3<CutGroupID,LJParameter> )
        .def( "ljs", &wrap_ljtable_types_b_2<CutGroupID,LJParameter> )
        
        //template<class C>
        //uint ljs( QVector<C> &param_vector, uint strt=0 ) const;
        //template<class C>
        //uint ljs( ResNum resnum, QVector<C> &param_vector, uint strt=0 ) const;
        //template<class C>
        //uint ljs( CutGroupID cgid, QVector<C> &param_vector, uint strt=0 ) const;
    
        .def( "ljsByCutGroup", (GroupedVector<CGAtomID,LJParameter> 
                                  (LJTable::*)() const)
                        &LJTable::ljsByCutGroup )
        .def( "ljsByCutGroup", (GroupedVector<CGAtomID,LJParameter> 
                                  (LJTable::*)(const QSet<CutGroupID>&) const)
                        &LJTable::ljsByCutGroup )
    
        //template<class C>
        //void ljsByCutGroup( GroupedVector<C,CGAtomID> &param_vector ) const;
        //template<class C>
        //void ljsByCutGroup(const QSet<CutGroupID> &cgids, 
        //                         GroupedVector<C,CGAtomID> &param_vector) const;
    
        .def( "ljsByResidue", (GroupedVector<ResNumAtomID,LJParameter> 
                                  (LJTable::*)() const)
                        &LJTable::ljsByResidue )
        .def( "ljsByResidue", (GroupedVector<ResNumAtomID,LJParameter> 
                                  (LJTable::*)(const QSet<ResNum>&) const)
                        &LJTable::ljsByResidue )

        //template<class C>
        //void ljsByResidue( GroupedVector<C,ResNumAtomID> &param_vector ) const;
        //template<class C>
        //void ljsByResidue(const QSet<ResNum> &resnums,
        //                        GroupedVector<C,ResNumAtomID> &param_vector) const;
        
        .def( "clearLJ", (void (LJTable::*)(const CGAtomID&))
                        &LJTable::clearLJ )
        .def( "clearLJ", (void (LJTable::*)(const ResNumAtomID&))
                        &LJTable::clearLJ )
        .def( "clearLJ", (void (LJTable::*)(const AtomIndex&))
                        &LJTable::clearLJ )
    
        .def( "clearLJs", (void (LJTable::*)())
                        &LJTable::clearLJs )
        .def( "clearLJs", (void (LJTable::*)(ResNum))
                        &LJTable::clearLJs )
        .def( "clearLJs", (void (LJTable::*)(CutGroupID))
                        &LJTable::clearLJs )
    
        //template<class C>
        //void clearLJs(const C &lots);

        .def( "getLJ", (const LJParameter& (LJTable::*)(const CGAtomID&) const)
                        &LJTable::getLJ,
                            return_value_policy<copy_const_reference>() )
        .def( "getLJ", (const LJParameter& (LJTable::*)(const ResNumAtomID&) const)
                        &LJTable::getLJ,
                            return_value_policy<copy_const_reference>() )
        .def( "getLJ", (const LJParameter& (LJTable::*)(const AtomIndex&) const)
                        &LJTable::getLJ,
                            return_value_policy<copy_const_reference>() )
    
        .def( "hasMissingLJs", (bool (LJTable::*)() const)
                        &LJTable::hasMissingLJs )
        .def( "hasMissingLJs", (bool (LJTable::*)(ResNum) const)
                        &LJTable::hasMissingLJs )
        .def( "hasMissingLJs", (bool (LJTable::*)(CutGroupID) const)
                        &LJTable::hasMissingLJs )
    
        .def( "missingLJs", (QSet<AtomIndex> (LJTable::*)() const)
                        &LJTable::missingLJs )
        .def( "missingLJs", (QSet<AtomIndex> (LJTable::*)(ResNum) const)
                        &LJTable::missingLJs )
        .def( "missingLJs", (QSet<AtomIndex> (LJTable::*)(CutGroupID) const)
                        &LJTable::missingLJs )
    
        .def( "nLJs", (int (LJTable::*)() const) 
                        &LJTable::nLJs )
        .def( "nLJs", (int (LJTable::*)(ResNum) const) 
                        &LJTable::nLJs )
        .def( "nLJs", (int (LJTable::*)(CutGroupID) const) 
                        &LJTable::nLJs )
    
        .def( "setLJ", (void (LJTable::*)(const CGAtomID&, const LJParameter&))
                        &LJTable::setLJ )
        .def( "setLJ", (void (LJTable::*)(const ResNumAtomID&, const LJParameter&))
                        &LJTable::setLJ )
        .def( "setLJ", (void (LJTable::*)(const AtomIndex&, const LJParameter&))
                        &LJTable::setLJ )
                        
    ;
}

}
