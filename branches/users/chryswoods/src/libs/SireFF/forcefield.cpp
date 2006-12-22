
#include "SireCAS/qhash_sirecas.h"

#include "forcefield.h"
#include "ffbase.h"

#include "SireMol/molecule.h"
#include "SireMol/errors.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireBase;
using namespace SireFF;

static const RegisterMetaType<ForceField> r_forcefield("SireFF::ForceField", MAGIC_ONLY);

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const ForceField&)
{
    writeHeader(ds, r_forcefield, 0);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, ForceField&)
{
    VersionID v = readHeader(ds, r_forcefield);

    if (v != 0)
        throw version_error(v, "1", r_forcefield, CODELOC);

    return ds;
}

namespace SireFF
{
namespace detail
{
class NullFF;
}
}

QDataStream& operator<<(QDataStream&, const SireFF::detail::NullFF&);
QDataStream& operator>>(QDataStream&, SireFF::detail::NullFF&);

namespace SireFF
{
namespace detail
{

/** This is a null forcefield, which is used to provide a null object
    to prevent segfaults when using the null ForceField class.

    @author Christopher Woods
*/
class NullFF : public FFBase
{

friend QDataStream& ::operator<<(QDataStream&, const NullFF&);
friend QDataStream& ::operator>>(QDataStream&, NullFF&);

public:
    NullFF() : FFBase()
    {}

    NullFF(const NullFF &other) : FFBase(other)
    {}

    ~NullFF()
    {}

    class SIREFF_EXPORT Parameters : public FFBase::Parameters
    {
    public:
        Parameters()
        {}

        ~Parameters()
        {}

        static Parameters default_parameter;
    };

    const Parameters& parameters() const
    {
        return Parameters::default_parameter;
    }

    static const char* typeName()
    {
        return "SireFF::detail::NullFF";
    }

    const char* what() const
    {
        return NullFF::typeName();
    }

    NullFF* clone() const
    {
        return new NullFF(*this);
    }

    const Molecule& molecule(MoleculeID molid) const
    {
        throw SireMol::missing_molecule( QObject::tr(
               "By definition, the null forcefield contains nothing!, not even "
               "the molecule with ID == %1.").arg(molid), CODELOC );

        return null_molecule;
    }

    //virtual const Residue& residue(const MolResNumID &molresid) const=0;

    bool move(const Molecule&)
    {
        return false;
    }

    bool move(const Residue&)
    {
        return false;
    }

protected:
    void recalculateEnergy()
    {}

    /** Null molecule returned by the null forcefield */
    static Molecule null_molecule;
};

NullFF::Parameters NullFF::Parameters::default_parameter;

} // detail
} // SireFF

////////////
//////////// Implementation of NullFF
////////////

Q_DECLARE_METATYPE(SireFF::detail::NullFF)

using namespace SireFF::detail;

static const RegisterMetaType<NullFF> r_nullff("SireFF::detail::NullFF");

/** Serialise to a binary datastream */
QDataStream &operator<<(QDataStream &ds, const NullFF &nullff)
{
    writeHeader(ds, r_nullff, 1)
                  << static_cast<const FFBase&>(nullff);

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream &operator>>(QDataStream &ds, NullFF &nullff)
{
    VersionID v = readHeader(ds, r_nullff);

    if (v == 1)
    {
        ds >> static_cast<FFBase&>(nullff);
    }
    else
        throw version_error(v, "1", r_nullff, CODELOC);

    return ds;
}

Molecule NullFF::null_molecule;

////////////
//////////// Implementation of ForceField
////////////

static const SharedPolyPointer<FFBase> shared_null( new NullFF() );

/** Constructor */
ForceField::ForceField() : SharedPolyPointer<FFBase>(shared_null)
{}

/** Construct from the passed FFBase forcefield */
ForceField::ForceField(const FFBase &ffield)
           : SharedPolyPointer<FFBase>(ffield.clone())
{}

/** Construct from a shared pointer to a forcefield */
ForceField::ForceField(const SharedPolyPointer<FFBase> &ffptr)
           : SharedPolyPointer<FFBase>(ffptr)
{
    if (not constData())
        SharedPolyPointer<FFBase>::operator=(shared_null);
}

/** Copy constructor */
ForceField::ForceField(const ForceField &other)
           : SharedPolyPointer<FFBase>(other)
{}

/** Destructor */
ForceField::~ForceField()
{}

/** Assignment operator */
ForceField& ForceField::operator=(const ForceField &other)
{
    SharedPolyPointer<FFBase>::operator=(other);
    return *this;
}

/** Assignment operator */
ForceField& ForceField::operator=(const FFBase &other)
{
    SharedPolyPointer<FFBase>::operator=(other.clone());
    return *this;
}
