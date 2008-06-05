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

#ifndef SQUIRE_MOLPROPROCESSOR_H
#define SQUIRE_MOLPROPROCESSOR_H

#ifdef _BUILD_MOLPRO_

#include "SireFF/ffthreadprocessor.h"

#include <QDir>

SIRE_BEGIN_HEADER

namespace Squire
{

using SireFF::ForceField;

namespace detail
{

/** Private implementation holding the data for MolproProcessor

    @author Christopher Woods
*/
class MolproProcessorPvt : public SireFF::detail::FFThreadProcessorPvt
{
public:
    MolproProcessorPvt();
    MolproProcessorPvt(const ForceField &forcefield);

    ~MolproProcessorPvt();

protected:
    boost::shared_ptr<SireCluster::WorkerBase> _pvt_activate();
};

}

/** This is the procesor that connects to an external Molpro process with
    which QM calculations may be performed. The connection is handled by
    a background thread, so will not block the main thread.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproProcessor : public SireFF::FFThreadProcessor
{
public:
    MolproProcessor();
    MolproProcessor(const ForceField &forcefield);

    MolproProcessor(const MolproProcessor &other);

    ~MolproProcessor();

    static const char* typeName()
    {
        return "Squire::MolproProcessor";
    }

    const char* what() const
    {
        return MolproProcessor::typeName();
    }

protected:
    MolproProcessor(const boost::shared_ptr<detail::MolproProcessorPvt> &data);

    detail::MolproProcessorPvt& data();
    const detail::MolproProcessorPvt& data() const;
};

}

SIRE_END_HEADER

#endif // end of '#ifdef _BUILD_MOLPRO_'

#endif
