
#include <QDataStream>

#include "angle.h"

#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Angle> r_angle;

/** Serialise an angle to a binary datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const SireMol::Angle &ang)
{
    writeHeader(ds, r_angle, 1) << ang.atoms[0] << ang.atoms[1] << ang.atoms[2];

    return ds;
}

/** Deserialise an angle from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, SireMol::Angle &ang)
{
    VersionID v = readHeader(ds, r_angle);

    if (v == 1)
    {
        ds >> ang.atoms[0] >> ang.atoms[1] >> ang.atoms[2];
    }
    else
        throw version_error(v, "1", r_angle, CODELOC);

    return ds;
}

/** Provide a function to hash an angle (for use in a QSet or QHash) */
uint SIREMOL_EXPORT qHash(const SireMol::Angle &angle)
{
    //Assume 32bit uint
    // give first 16 bits to atom0, then 8 bits each for atom1 and atom2

    uint atm0 = qHash(angle.atom0());
    uint atm1 = qHash(angle.atom1());
    uint atm2 = qHash(angle.atom2());

    return (atm0 << 16) | ( (atm1<<8) & 0x0000FF00 ) | (atm2 & 0x000000FF);
}

/** Construct a null Angle */
Angle::Angle()
{}

/** Construct a Angle between these three AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Angle will sort the atoms
    so that the Angle between atom0-atom1-atom2 will be the same as the Angle
    between atom2-atom1-atom0.
*/
Angle::Angle(const AtomIndex &atom0, const AtomIndex &atom1,
             const AtomIndex &atom2)
{
    this->create(atom0,atom1,atom2);
}

/** Construct an angle from a tuple of three AtomIndex objects */
Angle::Angle(const tuple<AtomIndex,AtomIndex,AtomIndex> &tuple)
{
    this->create( tuple.get<0>(), tuple.get<1>(), tuple.get<2>() );
}

/** Construct an angle between atoms called 'atom0', 'atom1' and 'atom2' in residue with
    residue number 'resnum' */
Angle::Angle(const QString &atom0, const QString &atom1,
             const QString &atom2, ResNum resnum)
{
    this->create( AtomIndex(atom0, resnum), AtomIndex(atom1, resnum),
                  AtomIndex(atom2, resnum) );
}

/** Construct an angle between the atoms with the specified names in the specified
    residues */
Angle::Angle(const QString &atom0, ResNum res0, const QString &atom1, ResNum res1,
             const QString &atom2, ResNum res2)
{
    this->create( AtomIndex(atom0,res0), AtomIndex(atom1,res1),
                  AtomIndex(atom2,res2) );
}

/** Copy constructor */
Angle::Angle(const Angle &other)
{
    operator=(other);
}

/** Destructor */
Angle::~Angle()
{}

/** Construct the Angle between these two AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Angle will sort the atoms
    so that the Angle between atom0-atom1 will be the same as the Angle
    between atom1-atom0.

    \throw SireMol::duplicate_atom
*/
void Angle::create(const AtomIndex &atom0, const AtomIndex &atom1,
                   const AtomIndex &atom2)
{
    if (atom0 == atom1  or  atom0 == atom2)
    {
        throw SireMol::duplicate_atom(QObject::tr(
                    "Cannot create a Angle between the same atoms! %1-%2-%3")
                          .arg(atom0.toString(),atom1.toString())
                          .arg(atom2.toString()), CODELOC);
    }

    if (atom0 < atom2)
    {
        atoms[0] = atom0;
        atoms[1] = atom1;
        atoms[2] = atom2;
    }
    else
    {
        atoms[0] = atom2;
        atoms[1] = atom1;
        atoms[2] = atom0;
    }
}

/** Test for equality */
bool Angle::operator==(const Angle &other) const
{
    return atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1] and
           atoms[2] == other.atoms[2];
}

/** Test for inequality */
bool Angle::operator!=(const Angle &other) const
{
    return not operator==(other);
}

/** Assignment operator */
const Angle& Angle::operator=(const Angle &other)
{
    for (int i=0; i<3; ++i)
        atoms[i] = other.atoms[i];

    return *this;
}

/** A Angle is greater than another if atm0 of the angle is greater than
    atm0 of the other angle. If both atm0s are equal, then the test is on atm1,
    then on atm2 */
bool Angle::operator>(const Angle &other) const
{
    return (atoms[0] > other.atoms[0])
             or ( atoms[0] == other.atoms[0] and atoms[1] > other.atoms[1] )
              or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                  and atoms[2] > other.atoms[2]);
}

/** A Angle is greater than another if atm0 of the angle is greater than
    atm0 of the other angle. If both atm0s are equal, then the test is on atm1,
    then on atm2 */
bool Angle::operator>=(const Angle &other) const
{
    return (atoms[0] > other.atoms[0])
             or ( atoms[0] == other.atoms[0] and atoms[1] > other.atoms[1] )
              or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                  and atoms[2] >= other.atoms[2]);
}

/** A Angle is less than another if atm0 of the angle is less than
    atm0 of the other angle. If both atm0s are equal, then the test is on atm1,
    then on atm2 */
bool Angle::operator<(const Angle &other) const
{
    return not operator>=(other);
}

/** A Angle is less than another if atm0 of the angle is less than
    atm0 of the other angle. If both atm0s are equal, then the test is on atm1,
    then on atm2 */
bool Angle::operator<=(const Angle &other) const
{
    return not operator>(other);
}

/** Return a string representation of the Angle */
QString Angle::toString() const
{
    return QObject::tr("%1-%2-%3").arg(atoms[0].toString(),
                                atoms[1].toString(),atoms[2].toString());
}
