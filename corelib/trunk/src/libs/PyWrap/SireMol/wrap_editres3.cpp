
/**
  * This file contains the boost::python wrapping of EditRes
  *
  * @author Christopher Woods
  *
  */

#include <Python.h>
#include <boost/python.hpp>

#include "SireMol/qhash_siremol.h"

#include "SireMol/editmol.h"
#include "SireMol/editres.h"
#include "SireMol/bond.h"
#include "SireMol/angle.h"
#include "SireMol/dihedral.h"
#include "SireMol/improper.h"
#include "SireMol/moleculebonds.h"
#include "SireMol/residuebonds.h"
#include "SireMol/atom.h"
#include "SireMol/atomvector.h"
#include "SireMol/weightfunction.h"
#include "SireMol/templatefunction.h"
#include "SireMol/atomidgroup.h"

#include "SireMaths/line.h"
#include "SireMaths/triangle.h"
#include "SireMaths/torsion.h"
#include "SireMaths/angle.h"
#include "SireMaths/vector.h"
#include "SireMaths/quaternion.h"


#include "SirePy/indexer.hpp"

using namespace boost::python;

namespace SireMol
{

/////////////
///////////// Function used to wrap the EditRes class
/////////////

void export_EditRes3(class_<EditRes> &wrapper)
{
    //exposing overloaded functions
    EditRes (EditRes::*wrap_extract1)() const = &EditRes::extract;
    EditRes (EditRes::*wrap_extract2)(ResNum) const = &EditRes::extract;
    
    const Atom& (EditRes::*wrap_atom1)(const QString&) const = &EditRes::atom;
    const Atom& (EditRes::*wrap_atom2)(int i) const = &EditRes::atom;

    double (EditRes::*wrap_measure1)(const QString&, const QString&) const = &EditRes::measure;
    SireMaths::Angle (EditRes::*wrap_measure2)(const QString&, const QString&,
                                               const QString&) const = &EditRes::measure;
    SireMaths::Angle (EditRes::*wrap_measure3)(const QString&, const QString&,
                                    const QString&, const QString&) const = &EditRes::measure;
    
    BondList (EditRes::*wrap_bonds1)() const = &EditRes::bonds;
    BondList (EditRes::*wrap_bonds2)(const QString&) const = &EditRes::bonds;
    
    //memory management
    wrapper.def("detach", &EditRes::detach);
    wrapper.def("shallowCopy", &EditRes::shallowCopy);
    wrapper.def("deepCopy", &EditRes::deepCopy);
    wrapper.def("extract", wrap_extract1);
    wrapper.def("extract", wrap_extract2);
               
    //query functions
    wrapper.def("name", &EditRes::name);
    wrapper.def("toString", &EditRes::toString);
    wrapper.def("number", &EditRes::number);
    wrapper.def("resNum", &EditRes::resNum);
    wrapper.def("molecule", &EditRes::molecule);
    wrapper.def("at", &EditRes::at, return_value_policy<copy_const_reference>());
    wrapper.def("count", &EditRes::count);
    wrapper.def("size", &EditRes::size);
    wrapper.def("nAtoms", &EditRes::nAtoms);
    wrapper.def("atomNames", &EditRes::atomNames);
    wrapper.def("nBonds", &EditRes::nBonds);
    wrapper.def("connectivity", &EditRes::connectivity);
    wrapper.def("contains", (bool (EditRes::*)(const QString&) const)&EditRes::contains);
    wrapper.def("contains", (bool (EditRes::*)(const AtomIndex&) const)&EditRes::contains);
    wrapper.def("atom", wrap_atom1, return_value_policy<copy_const_reference>());
    wrapper.def("atom", wrap_atom2, return_value_policy<copy_const_reference>());
    wrapper.def("bond", &EditRes::bond);
    wrapper.def("angle", &EditRes::angle);
    wrapper.def("dihedral", &EditRes::dihedral);
    wrapper.def("improper", &EditRes::improper);
    wrapper.def("measure", wrap_measure1);
    wrapper.def("measure", wrap_measure2);
    wrapper.def("measure", wrap_measure3);
    wrapper.def("measureImproper", &EditRes::measureImproper);
    wrapper.def("atoms", &EditRes::atoms);
    wrapper.def("atomVector", &EditRes::atomVector);
    wrapper.def("bonds", wrap_bonds1);
    wrapper.def("bonds", wrap_bonds2);
    wrapper.def("residuesBondedTo", &EditRes::residuesBondedTo);

}

}
