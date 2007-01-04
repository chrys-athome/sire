
#include "atomicproperties.h"

#include "cgatomid.h"

#include "SireError/errors.h"

using namespace SireMol;

/** Constructor */
AtomicProperties::AtomicProperties() : PropertyBase()
{}

/** Copy constructor */
AtomicProperties::AtomicProperties(const AtomicProperties &other)
                 : PropertyBase(other)
{}

/** Destructor */
AtomicProperties::~AtomicProperties()
{}

/** Throw an error caused by a missing CutGroup

    \throw SireError::invalid_index
*/
void AtomicProperties::throwMissingCutGroup(CutGroupID cgid, int ncg) const
{
    throw SireError::invalid_index( QObject::tr(
          "There is no CutGroup with ID == %1 in the AtomicCharges "
          "(Max ID == %2)")
              .arg(cgid).arg(ncg-1), CODELOC );
}

/** Throw an error caused by a missing atom

    \throw SireError::invalid_index
*/
void AtomicProperties::throwMissingAtom(const CGAtomID &cgatomid, int natms) const
{
    throw SireError::invalid_index( QObject::tr(
          "There is no Atom with ID == %1 in the CutGroup with "
          "ID == %2 in the AtomicCharges (Max ID == %2)")
              .arg(cgatomid.atomID()).arg(cgatomid.cutGroupID()).arg(natms-1),
                      CODELOC );
}
