#ifndef SIREFF_FFTHREADPROCESSOR_H
#define SIREFF_FFTHREADPROCESSOR_H

#include "ffprocessor.h"

SIRE_BEGIN_HEADER

namespace SireFF
{

namespace detail
{

/** The private implementation class of FFProcessor

    @author Christopher Woods
*/
class FFThreadProcessorPvt : public FFProcessorPvt
{
public:
    FFThreadProcessorPvt();
    FFThreadProcessorPvt(const ForceField &forcefield);

    ~FFThreadProcessorPvt();

protected:
    boost::shared_ptr<WorkerBase> _pvt_activate();
};

}

/** This class represents a processor running in a separate thread.
    This allows a forcefield to be evaluated in a background thread.

    @author Christopher Woods
*/
class FFThreadProcessor : public FFProcessorBase
{
public:
    FFThreadProcessor();
    FFThreadProcessor(const ForceField &forcefield);

    FFThreadProcessor(const FFThreadProcessor &other);

    ~FFThreadProcessor();

    static const char* typeName()
    {
        return "SireFF::FFThreadProcessor";
    }

    const char* what() const
    {
        return FFThreadProcessor::typeName();
    }
};

}


SIRE_END_HEADER

#endif
