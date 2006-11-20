
/**
  * This file contains the boost::python wrapping of MatchMRA
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireDB/matchdata.h"
#include "SireDB/matchatom.h"
#include "SireDB/matchres.h"
#include "SireDB/matchmol.h"
#include "SireDB/matchatomtype.h"
#include "SireDB/matchatomtypedata.h"
#include "SireDB/matchmr.h"
#include "SireDB/matchmrdata.h"
#include "SireDB/matchmra.h"
#include "SireDB/matchmradata.h"

using namespace boost::python;

namespace SireDB
{

MatchMRA mra_and_atom(const MatchMRA &mra, const MatchAtom &atom)
{
    return mra and atom;
}

MatchMRA mra_and_res(const MatchMRA &mra, const MatchRes &res)
{
    return mra and res;
}

MatchMRA mra_and_mol(const MatchMRA &mra, const MatchMol &mol)
{
    return mra and mol;
}

MatchMRA mra_and_mr(const MatchMRA &mra, const MatchMR &mr)
{
    return mra and mr;
}

MatchMRA atom_and_mra(const MatchMRA &mra, const MatchAtom &atom)
{
    return atom and mra;
}

MatchMRA res_and_mra(const MatchMRA &mra, const MatchRes &res)
{
    return res and mra;
}

MatchMRA mol_and_mra(const MatchMRA &mra, const MatchMol &mol)
{
    return mol and mra;
}

MatchMRA mr_and_mra(const MatchMRA &mra, const MatchMR &mr)
{
    return mr and mra;
}

MatchMRAData mra_and_mrdata(const MatchMRA &mra, const MatchMRData &mrdata)
{
    return mra and mrdata;
}

MatchMRAData mrdata_and_mra(const MatchMRA &mra, const MatchMRData &mrdata)
{
    return mrdata and mra;
}

MatchMRAData mradata_and_atom(const MatchMRAData &mradata, const MatchAtom &atom)
{
    return mradata and atom;
}

MatchMRAData mradata_and_res(const MatchMRAData &mradata, const MatchRes &res)
{
    return mradata and res;
}

MatchMRAData mradata_and_mol(const MatchMRAData &mradata, const MatchMol &mol)
{
    return mradata and mol;
}

MatchMRAData mradata_and_mr(const MatchMRAData &mradata, const MatchMR &mr)
{
    return mradata and mr;
}

MatchMRAData mradata_and_mrdata(const MatchMRAData &mradata, const MatchMRData &mrdata)
{
    return mradata and mrdata;
}

MatchMRAData mradata_and_mra(const MatchMRAData &mradata, const MatchMRA &mra)
{
    return mradata and mra;
}

MatchMRAData atom_and_mradata(const MatchMRAData &mradata, const MatchAtom &atom)
{
    return atom and mradata;
}

MatchMRAData res_and_mradata(const MatchMRAData &mradata, const MatchRes &res)
{
    return res and mradata;
}

MatchMRAData mol_and_mradata(const MatchMRAData &mradata, const MatchMol &mol)
{
    return mol and mradata;
}

MatchMRAData mr_and_mradata(const MatchMRAData &mradata, const MatchMR &mr)
{
    return mr and mradata;
}

MatchMRAData mrdata_and_mradata(const MatchMRAData &mradata, const MatchMRData &mrdata)
{
    return mrdata and mradata;
}

MatchMRAData mra_and_mradata(const MatchMRAData &mradata, const MatchMRA &mra)
{
    return mra and mradata;
}

void  
SIREDB_EXPORT
export_MatchMRA()
{
    class_<MatchMRA, bases<MatchData> >("MatchMRA", init<>())
        .def( init<const MatchMR&, const MatchAtom&>() )
        .def( init<const MatchMol&, const MatchRes&, const MatchAtom&>() )
        .def( init<const MatchMol&>() )
        .def( init<const MatchRes&>() )
        .def( init<const MatchMR&>() )
        .def( init<const MatchAtom&>() )
        .def( "__and__", &MatchMRA::operator&& )
        .def( "__and__", &mra_and_atom )
        .def( "__and__", &mra_and_res )
        .def( "__and__", &mra_and_mol )
        .def( "__and__", &mra_and_mr )
        .def( "__and__", &mra_and_mrdata )
        .def( "__rand__", &atom_and_mra )
        .def( "__rand__", &res_and_mra )
        .def( "__rand__", &mol_and_mra )
        .def( "__rand__", &mr_and_mra )
        .def( "__rand__", &mrdata_and_mra )
        .def( "molecule", &MatchMRA::molecule, return_value_policy<copy_const_reference>() )
        .def( "residue", &MatchMRA::residue, return_value_policy<copy_const_reference>() )
        .def( "atom", &MatchMRA::atom, return_value_policy<copy_const_reference>() )
        .def( "molAndRes", &MatchMRA::molAndRes, return_value_policy<copy_const_reference>() )
        .def( "hasMoleculeCriteria", &MatchMRA::hasMoleculeCriteria )
        .def( "hasResidueCriteria", &MatchMRA::hasResidueCriteria )
        .def( "hasAtomCriteria", &MatchMRA::hasAtomCriteria )
    ;
    
    implicitly_convertible<MatchMRA,MatchMRAData>();
    
    class_<MatchMRAData, bases<MatchData> >("MatchMRAData", init<>())
        .def( init<const MatchMol&>() )
        .def( init<const MatchRes&>() )
        .def( init<const MatchAtom&>() )
        .def( init<const MatchMRData&>() )
        .def( init<const MatchMRA&>() )
        .def( init<uint, const MatchRes&>() )
        .def( init<uint, const MatchAtom&>() )
        .def( "__and__", &MatchMRAData::operator&& )
        .def( "__and__", &mradata_and_atom )
        .def( "__and__", &mradata_and_res )
        .def( "__and__", &mradata_and_mol )
        .def( "__and__", &mradata_and_mr )
        .def( "__and__", &mradata_and_mrdata )
        .def( "__and__", &mradata_and_mra )
        .def( "__rand__", &atom_and_mradata )
        .def( "__rand__", &res_and_mradata )
        .def( "__rand__", &mol_and_mradata )
        .def( "__rand__", &mr_and_mradata )
        .def( "__rand__", &mrdata_and_mradata )
        .def( "__rand__", &mra_and_mradata )
        .def( "getMatch", &MatchMRAData::getMatch )
        .def( "hasMoleculeCriteria", &MatchMRAData::hasMoleculeCriteria )
        .def( "hasResidueCriteria", &MatchMRAData::hasResidueCriteria )
        .def( "hasAtomCriteria", &MatchMRAData::hasAtomCriteria )
        .def( "set", &MatchMRAData::set )
        .def( "simplify", &MatchMRAData::simplify )
    ;
}

}
