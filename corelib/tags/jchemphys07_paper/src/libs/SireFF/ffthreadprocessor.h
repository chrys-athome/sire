/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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
class SIREFF_EXPORT FFThreadProcessorPvt : public FFProcessorPvt
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
class SIREFF_EXPORT FFThreadProcessor : public FFProcessorBase
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

protected:
    FFThreadProcessor( const boost::shared_ptr<detail::FFThreadProcessorPvt> &data );
};

}


SIRE_END_HEADER

#endif
