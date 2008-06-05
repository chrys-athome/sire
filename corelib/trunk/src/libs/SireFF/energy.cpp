
#include <QObject>

#include "energy.h"

#include "SireStream/datastream.h"

using namespace SireStream;
using namespace SireFF;

// Implementation of EnergyBase

QString EnergyBase::toString() const
{
    return QObject::tr("%1 kcal mol-1").arg(nrg);
}

// Now the implementation of Energy

static const RegisterMetaType<Energy> r_energy("SireFF::Energy");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const Energy &energy)
{
    writeHeader(ds, r_energy, 1) << energy.nrg;
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, Energy &energy)
{
    VersionID v = readHeader(ds, r_energy);
    
    if (v == 1)
    {
        ds >> energy.nrg;
    }
    else
        throw version_error(v, "1", r_energy, CODELOC);
    
    return ds;
}

Energy::Energy() : EnergyBase()
{}

Energy::Energy(const double &nrg) : EnergyBase(nrg)
{}

Energy::Energy(const EnergyBase &other) : EnergyBase(other)
{}

Energy::~Energy()
{}
