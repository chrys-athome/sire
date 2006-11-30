#ifndef SQUIRE_MOLPROPROCESSOR_H
#define SQUIRE_MOLPROPROCESSOR_H

#include <memory>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceField;
}

namespace Squire
{

class MolproFF;

using SireFF::ForceField;

namespace detail
{
class MolproProcessorPvt;
}

class SQUIRE_EXPORT MolproProcessor
{

friend class MolproFF;  // so can call energy(const MolproFF &ff)

public:
    MolproProcessor();
    MolproProcessor(const ForceField &);

    ~MolproProcessor();

    const char* what();

    const QString& forcefieldName() const;
    const QString& name() const;

    double energy();

protected:
    double calculateEnergy(const MolproFF &ff);

private:

    /** Full path to the molpro executable */
    QString molpro_exe;

    /** Pointer to the class that implements this processor */
    std::auto_ptr<detail::MolproProcessorPvt> p;
};

}

SIRE_END_HEADER

#endif
