/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

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
