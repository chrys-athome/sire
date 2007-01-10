#ifndef SIREMATHS_RANGENERATOR_H
#define SIREMATHS_RANGENERATOR_H

#include <boost/shared_ptr.hpp>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class RanGenerator;
}

QDataStream& operator<<(QDataStream&, const SireMaths::RanGenerator&);
QDataStream& operator>>(QDataStream&, SireMaths::RanGenerator&);

namespace SireMaths
{

namespace detail
{
class RanGeneratorPvt;
}

class SIREMATHS_EXPORT RanGenerator
{

friend QDataStream& ::operator<<(QDataStream&, const RanGenerator&);
friend QDataStream& ::operator>>(QDataStream&, RanGenerator&);

public:
    RanGenerator(uint32_t seed = 4738924321);

    RanGenerator(const RanGenerator &other);

    ~RanGenerator();

    double rand();

private:
    /** Shared pointer to the actual generator */
    boost::shared_ptr<detail::RanGeneratorPvt> d;
};

}

Q_DECLARE_METATYPE(SireMaths::RanGenerator);

SIRE_END_HEADER

#endif
