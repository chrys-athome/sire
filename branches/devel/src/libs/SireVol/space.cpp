
#include "space.h"
#include "cartesian.h"

#include "SireStream/datastream.h"

using namespace SireVol;
using namespace SireBase;
using namespace SireStream;

///////////////
/////////////// Implementation of SpaceBase
///////////////

static const RegisterMetaType<SpaceBase> r_spacebase(MAGIC_ONLY, "SireVol::SpaceBase");

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const SpaceBase&)
{
    writeHeader(ds, r_spacebase, 0);
    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, SpaceBase&)
{
    VersionID v = readHeader(ds, r_spacebase);

    if (v != 0)
        throw version_error(v, "0", r_spacebase, CODELOC);

    return ds;
}

/** Construct a SpaceBase. */
SpaceBase::SpaceBase() : QSharedData()
{}

/** Copy constructor */
SpaceBase::SpaceBase(const SpaceBase&) : QSharedData()
{}

/** Destructor */
SpaceBase::~SpaceBase()
{}

//////////////
////////////// Implementation of Space
//////////////

static const RegisterMetaType<Space> r_space;

/** Serialise to a binary datastream */
QDataStream SIREVOL_EXPORT &operator<<(QDataStream &ds, const Space &space)
{
    writeHeader(ds, r_space, 1) << space.d;

    return ds;
}

/** Deserialise from a binary datastream */
QDataStream SIREVOL_EXPORT &operator>>(QDataStream &ds, Space &space)
{
    VersionID v = readHeader(ds, r_space);

    if (v == 1)
    {
        ds >> space.d;
    }
    else
        throw version_error(v, "1", r_space, CODELOC);

    return ds;
}

static SharedPolyPointer<SpaceBase> shared_null( new Cartesian() );

/** Null constructor - this is a infinite Cartesian volume */
Space::Space() : d( shared_null )
{}

/** Construct from the base class */
Space::Space(const SpaceBase &other)
      : d( other.clone() )
{}

/** Copy constructor - fast as this class is implicitly shared */
Space::Space(const Space &other)
      : d( other.d )
{}

/** Destructor */
Space::~Space()
{}

/** Assign from the base class */
Space& Space::operator=(const SpaceBase &other)
{
    d = other.clone();
    return *this;
}

/** Assignment operator */
Space& Space::operator=(const Space &other)
{
    d = other.d;
    return *this;
}

/** Return the type of Space */
const char* Space::what() const
{
    return d->what();
}
