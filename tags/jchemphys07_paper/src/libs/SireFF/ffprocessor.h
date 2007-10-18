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

#ifndef SIREFF_FFPROCESSOR_H
#define SIREFF_FFPROCESSOR_H

#include "SireCluster/processor.h"

#include "forcefield.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Molecule;
class MoleculeID;
}

namespace SireCluster
{
class WorkerBase;
}

namespace SireFF
{

class FFWorkerBase;

using SireMol::Molecule;
using SireMol::MoleculeID;

using SireCluster::WorkerBase;

namespace detail
{

class SIREFF_EXPORT FFProcessorBasePvt : public SireCluster::detail::ProcessorPvt
{
public:
    FFProcessorBasePvt();
    FFProcessorBasePvt(const QString &name);

    ~FFProcessorBasePvt();

    virtual ForceField forcefield() const=0;

    virtual void setForceField(const ForceField &forcefield)=0;
};

}

/** This is the base class of all processors that are used to process
    forcefields

    @author Christopher Woods
*/
class SIREFF_EXPORT FFProcessorBase : public SireCluster::Processor
{
public:

    ~FFProcessorBase();

    ForceField forcefield() const;

    void setForceField(const ForceField &forcefield);

    boost::shared_ptr<FFWorkerBase> activate();

protected:
    FFProcessorBase(const boost::shared_ptr<detail::FFProcessorBasePvt> &data);
    FFProcessorBase(const FFProcessorBase &other);

    detail::FFProcessorBasePvt& data();
    const detail::FFProcessorBasePvt& data() const;

private:
    /** Pointer to the data of this object - this is to
        prevent excessive casting */
    detail::FFProcessorBasePvt *d;
};

namespace detail
{

/** Private data used by FFProcessor */
class SIREFF_EXPORT FFProcessorPvt : public FFProcessorBasePvt
{
public:
    FFProcessorPvt();
    FFProcessorPvt(const ForceField &ffield);

    ~FFProcessorPvt();

    ForceField forcefield() const;

    void setForceField(const ForceField &forcefield);

protected:
    boost::shared_ptr<WorkerBase> _pvt_activate();

private:
    /** The forcefield to be evaluated */
    ForceField ffield;
};

}

/** This is a basic FFProcessor that is a test processor that evaluates
    the energy and forces in the main thread

    @author Christopher Woods
*/
class SIREFF_EXPORT FFProcessor : public FFProcessorBase
{
public:
    FFProcessor();

    FFProcessor(const ForceField &ffield);

    FFProcessor(const FFProcessor &other);

    ~FFProcessor();

    static const char* typeName()
    {
        return "SireFF::FFProcessor";
    }

    const char* what() const
    {
        return FFProcessor::typeName();
    }
};

}

SIRE_END_HEADER

#endif

