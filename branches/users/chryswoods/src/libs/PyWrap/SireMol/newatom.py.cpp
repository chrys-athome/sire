
#include <boost/python.hpp>

#include "SireMol/newatom.h"

#include "SireMol/moleculedata.h"
#include "SireMol/molecule.h"
#include "SireMol/residue.h"
#include "SireMol/cgatomid.h"
#include "SireMol/atominfo.h"
#include "SireMol/residueinfo.h"
#include "SireMol/moleculeinfo.h"
#include "SireMol/atomicproperties.h"

#include "SireMaths/rotate.h"
#include "SireMaths/quaternion.h"
#include "SireMaths/matrix.h"
#include "SireMaths/vector.h"

using namespace SireMaths;

using namespace boost::python;

namespace SireMol
{

bool withinRadii1(const NewAtom &atom, const NewAtom &other)
{
    return atom.withinBondRadii(other);
}

bool withinRadii2(const NewAtom &atom, const NewAtom &other, double err)
{
    return atom.withinBondRadii(other, err);
}

void SIREMOL_EXPORT export_NewAtom()
{
    class_<NewAtom>( "NewAtom", init<>() )

        .def( init<const CGAtomID&, const Molecule&>() )
        .def( init<const IDMolAtom&, const Molecule&>() )

        .def( init<const QString&, const Residue&>() )
        .def( init<AtomID, const Residue&>() )

        .def( init<const NewAtom&>() )

        .def( self == self )
        .def( self != self )

        .def( "name", &NewAtom::name )
        .def( "info", &NewAtom::info, return_value_policy<copy_const_reference>() )
        .def( "element", &NewAtom::element )
        .def( "coordinates", &NewAtom::coordinates )

        .def( "property", (QVariant (NewAtom::*)(const QString&) const)
                            &NewAtom::property )

        .def( "toString", &NewAtom::toString )

        .def( "molecule", &NewAtom::molecule )
        .def( "residue", &NewAtom::residue )

        .def( "bondedAtoms", &NewAtom::bondedAtoms )
        .def( "bondedResidues", &NewAtom::bondedResidues )

        .def( "setCoordinates", (void (NewAtom::*)(double, double, double))
                    &NewAtom::setCoordinates )

        .def( "setCoordinates", (void (NewAtom::*)(const Vector&))
                    &NewAtom::setCoordinates )

        .def( "translate", (void (NewAtom::*)(const Vector&))
                    &NewAtom::translate )

        .def( "rotate", (void (NewAtom::*)(const Quaternion&, const Vector&))
                    &NewAtom::rotate )
        .def( "rotate", (void (NewAtom::*)(const Matrix&, const Vector&))
                    &NewAtom::rotate )

        .def( "withinBondRadii", &withinRadii1 )
        .def( "withinBondRadii", &withinRadii2 )
    ;

    implicitly_convertible<NewAtom,Vector>();
    implicitly_convertible<NewAtom,Element>();
    implicitly_convertible<NewAtom,AtomInfo>();
}

}
