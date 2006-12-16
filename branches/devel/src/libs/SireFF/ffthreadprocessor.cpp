
#include "SireCAS/qhash_sirecas.h"

#include "ffthreadprocessor.h"
#include "ffcalculator.h"
#include "ffthreadworker.h"

using namespace SireFF;
using namespace SireFF::detail;

/////////
///////// Implementation of FFThreadProcessorPvt
/////////

/** Construct with a null forcefield */
FFThreadProcessorPvt::FFThreadProcessorPvt() : FFProcessorPvt()
{}

/** Construct with a specified forcefield */
FFThreadProcessorPvt::FFThreadProcessorPvt(const ForceField &forcefield)
                     : FFProcessorPvt(forcefield)
{}

/** Destructor */
FFThreadProcessorPvt::~FFThreadProcessorPvt()
{}

/** Activate this processor - return the active worker. */
boost::shared_ptr<WorkerBase> FFThreadProcessorPvt::_pvt_activate()
{
    return boost::shared_ptr<WorkerBase>(
                          new FFThreadWorker(
                               new FFCalculator( this->forcefield() )
                                            )
                                        );
}

/////////
///////// Implementation of FFThreadProcessor
/////////

/** Construct using a null forcefield */
FFThreadProcessor::FFThreadProcessor()
                  : FFProcessorBase( boost::shared_ptr<FFProcessorBasePvt>(
                                                    new FFThreadProcessorPvt()
                                                                          )
                                   )
{}

/** Construct using the passed forcefield */
FFThreadProcessor::FFThreadProcessor(const ForceField &forcefield)
                  : FFProcessorBase( boost::shared_ptr<FFProcessorBasePvt>(
                                                    new FFThreadProcessorPvt(forcefield)
                                                                          )
                                   )
{}

/** Construct from passed data */
FFThreadProcessor::FFThreadProcessor(const boost::shared_ptr<FFThreadProcessorPvt> &data)
                  : FFProcessorBase(data)
{}

/** Copy constructor */
FFThreadProcessor::FFThreadProcessor(const FFThreadProcessor &other)
                  : FFProcessorBase(other)
{}

/** Destructor */
FFThreadProcessor::~FFThreadProcessor()
{}
