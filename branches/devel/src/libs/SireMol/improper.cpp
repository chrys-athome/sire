
#include <QDataStream>

#include "improper.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireMol;

static const RegisterMetaType<Improper> r_improper;

/** Serialise a improper to a datastream */
QDataStream SIREMOL_EXPORT &operator<<(QDataStream &ds, const Improper &improper)
{
    writeHeader(ds, r_improper, 1) << improper.atoms[0] << improper.atoms[1]
                                   << improper.atoms[2] << improper.atoms[3];
    return ds;
}

/** Deserialise a improper from a binary datastream */
QDataStream SIREMOL_EXPORT &operator>>(QDataStream &ds, Improper &improper)
{
    VersionID v = readHeader(ds, r_improper);

    if (v == 1)
    {
        ds >> improper.atoms[0] >> improper.atoms[1]
           >> improper.atoms[2] >> improper.atoms[3];
    }
    else
        throw version_error(v, "1", r_improper, CODELOC);

    return ds;
}

/** Hash a improper */
uint SIREMOL_EXPORT qHash(const Improper &imp)
{
    //hash each of the atoms in turn...
    uint atm0 = qHash(imp[0]);
    uint atm1 = qHash(imp[1]);
    uint atm2 = qHash(imp[2]);
    uint atm3 = qHash(imp[3]);

    //assume 32bit uint
    //give 8 bits to the hash of each atom...
    return ( atm0 << 24 ) | ( (atm1<<16) & 0x00FF0000 )
               | ( (atm2<<8) & 0x0000FF00 ) | ( atm3 & 0x000000FF );
}

/** Construct a null Improper */
Improper::Improper()
{}

/** Construct an Improper between these four AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Improper will sort the atoms
    so that the Improper between atom0-atom1-atom2-atom3 will be the same as the Improper
    between atom0-atom1-atom3-atom2 (note how the order, atom0-atom1 at the start is
    very different to atom1-atom0)
*/
Improper::Improper(const AtomIndex &atom0, const AtomIndex &atom1,
                   const AtomIndex &atom2, const AtomIndex &atom3)
{
    this->create(atom0,atom1,atom2,atom3);
}

/** Construct an Improper from a tuple of four AtomIndex objects */
Improper::Improper(const tuple<AtomIndex,AtomIndex,AtomIndex,AtomIndex> &tuple)
{
    this->create( tuple.get<0>(), tuple.get<1>(), tuple.get<2>(), tuple.get<3>() );
}

/** Construct an Improper between the four atoms called atom0-atom1-atom2-atom3 in the residue with
    residue number 'resnum' */
Improper::Improper(const QString &atom0, const QString &atom1,
                   const QString &atom2, const QString &atom3, ResNum resnum)
{
    this->create( AtomIndex(atom0, resnum), AtomIndex(atom1, resnum),
                  AtomIndex(atom2, resnum), AtomIndex(atom3, resnum) );
}

/** Construct an improper between the atoms with the specified names in the specified
    residues */
Improper::Improper(const QString &atom0, ResNum res0, const QString &atom1, ResNum res1,
                   const QString &atom2, ResNum res2, const QString &atom3, ResNum res3)
{
    this->create( AtomIndex(atom0,res0), AtomIndex(atom1,res1),
                  AtomIndex(atom2,res2), AtomIndex(atom3,res3) );
}

/** Copy constructor */
Improper::Improper(const Improper &other)
{
    operator=(other);
}

/** Destructor */
Improper::~Improper()
{}

/** Construct the Improper between these four AtomIndexes. The atoms must not be the same,
    or else an exception will be thrown. Note that the Improper will sort the atoms
    so that the Improper between atom0-atom1-atom2-atom3 will be the same as the Improper
    between atom3-atom2-atom1-atom0.

    \throw SireMol::duplicate_atoms
*/
void Improper::create(const AtomIndex &atom0, const AtomIndex &atom1,
                      const AtomIndex &atom2, const AtomIndex &atom3)
{
    if (atom0 == atom1  or  atom0 == atom2  or  atom0 == atom3)
    {
        throw SireMol::duplicate_atom(QObject::tr(
                    "Cannot create a Improper between the same atoms! %1-%2-%3-%4")
                          .arg(atom0.toString(),atom1.toString())
                          .arg(atom2.toString(),atom3.toString()), CODELOC);
    }

    if (atom2 < atom3)
    {
        atoms[0] = atom0;
        atoms[1] = atom1;
        atoms[2] = atom2;
        atoms[3] = atom3;
    }
    else
    {
        atoms[0] = atom0;
        atoms[1] = atom1;
        atoms[2] = atom3;
        atoms[3] = atom2;
    }
}

/** Test for equality */
bool Improper::operator==(const Improper &other) const
{
    return atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1] and
           atoms[2] == other.atoms[2] and atoms[3] == other.atoms[3];
}

/** Test for inequality */
bool Improper::operator!=(const Improper &other) const
{
    return not operator==(other);
}

/** Assignment operator */
const Improper& Improper::operator=(const Improper &other)
{
    for (int i=0; i<4; ++i)
        atoms[i] = other.atoms[i];

    return *this;
}

/** A Improper is greater than another if atm0 of the improper is greater than
    atm0 of the other improper. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3 */
bool Improper::operator>(const Improper &other) const
{
    return (atoms[0] > other.atoms[0])
             or ( atoms[0] == other.atoms[0] and atoms[1] > other.atoms[1] )
              or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                  and atoms[2] > other.atoms[2])

            or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                and atoms[2] == other.atoms[2]
                    and atoms[3] > other.atoms[3]);
}

/** A Improper is greater than another if atm0 of the improper is greater than
    atm0 of the other improper. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3 */
bool Improper::operator>=(const Improper &other) const
{
    return (atoms[0] > other.atoms[0])
             or ( atoms[0] == other.atoms[0] and atoms[1] > other.atoms[1] )
              or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                  and atoms[2] > other.atoms[2])

            or (atoms[0] == other.atoms[0] and atoms[1] == other.atoms[1]
                and atoms[2] == other.atoms[2]
                    and atoms[3] >= other.atoms[3]);
}

/** A Improper is less than another if atm0 of the improper is less than
    atm0 of the other improper. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3 */
bool Improper::operator<(const Improper &other) const
{
    return not operator>=(other);
}

/** A Improper is less than another if atm0 of the improper is less than
    atm0 of the other improper. If both atm0s are equal, then the test is on atm1,
    then on atm2, then on atm3s */
bool Improper::operator<=(const Improper &other) const
{
    return not operator>(other);
}

/** Return a string representation of the Improper */
QString Improper::toString() const
{
    return QObject::tr("%1-%2-%3-%4")
        .arg(atoms[0].toString(),atoms[1].toString(),
             atoms[2].toString(),atoms[3].toString());
}
