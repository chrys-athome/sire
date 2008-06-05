
/**
  * This file contains the boost::python wrapping of MatchAtom
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

MatchMRA atom_and_res(const MatchAtom &atom, const MatchRes &res)
{
    return atom and res;
}

MatchMRA atom_and_mol(const MatchAtom &atom, const MatchMol &mol)
{
    return atom and mol;
}

MatchMRA atom_and_mr(const MatchAtom &atom, const MatchMR &mr)
{
    return atom and mr;
}

MatchMRAData atom_and_mrdata(const MatchAtom &atom, const MatchMRData &mrdata)
{
    return atom and mrdata;
}

MatchMRA res_and_atom(const MatchAtom &atom, const MatchRes &res)
{
    return res and atom;
}

MatchMRA mol_and_atom(const MatchAtom &atom, const MatchMol &mol)
{
    return mol and atom;
}

MatchMRA mr_and_atom(const MatchAtom &atom, const MatchMR &mr)
{
    return mr and atom;
}

MatchMRAData mrdata_and_atom(const MatchAtom &atom, const MatchMRData &mrdata)
{
    return mrdata and atom;
}

void export_MatchAtom()
{
    QString (MatchAtom::*wrap_MatchAtom_toString)(uint) const = &MatchAtom::toString;

    class_<MatchAtom, bases<MatchData> >("MatchAtom", init<>())
        .def( self == self )
        .def( self != self )
        .def( "__and__", &MatchAtom::operator&& )
        .def( "__and__", &atom_and_res )
        .def( "__and__", &atom_and_mol )
        .def( "__and__", &atom_and_mr )
        .def( "__and__", &atom_and_mrdata )
        .def( "__rand__", &res_and_atom )
        .def( "__rand__", &mol_and_atom )
        .def( "__rand__", &mr_and_atom )
        .def( "__rand__", &mrdata_and_atom )
        .def( "toString", wrap_MatchAtom_toString )
        .def( "hasNumberMatch", &MatchAtom::hasNumberMatch )
        .def( "hasNameMatch", &MatchAtom::hasNameMatch )
        .def( "setName", &MatchAtom::setName )
        .def( "setNumber", &MatchAtom::setNumber )
        .def( "name", &MatchAtom::name )
        .def( "number", &MatchAtom::number )
    ;
    
    implicitly_convertible<MatchAtom, MatchMRA>();
}

}
